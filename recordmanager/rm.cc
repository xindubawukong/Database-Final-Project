#include "rm.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"
#include "utils/bitmap.h"

namespace recordmanager {

void RM_PrintError(int rc) { fprintf(stderr, "RM error %d.\n", rc); }

const int RM_FileHandle::kRecordSizePosition = 0;
const int RM_FileHandle::kPageBitMapStartPosition = 4096;
const int RM_FileHandle::kMaxPageNum = 32769;
const int RM_FileHandle::kBitMapStartPosition = 0;
const int RM_FileHandle::kBitMapLength = 4;
const int RM_FileHandle::kRecordStartPosition = 192;
const int RM_FileHandle::kRecordNumEachPage = 32;
const int RM_FileHandle::kRecordMaxLength = 250;

RM_FileHandle::RM_FileHandle() : fm_(nullptr), bpm_(nullptr) {}

RM_FileHandle::~RM_FileHandle() { ForcePages(); }

void RM_FileHandle::Init(const std::string& file_name, int record_size,
                         filesystem::FileManager* fm,
                         filesystem::BufPageManager* bpm, int file_id) {
  file_name_ = file_name;
  record_size_ = record_size;
  fm_ = fm;
  bpm_ = bpm;
  file_id_ = file_id;
  max_page_num_ = 0;
}

int RM_FileHandle::GetRecord(const RID& rid, RM_Record& rec) const {
  if (bpm_ == nullptr) return RM_FILEHANDLE_NOT_INITIALIZED_ERROR;
  int rc;
  int page_num, slot_num;
  if ((rc = rid.GetPageNum(page_num))) return rc;
  if ((rc = rid.GetSlotNum(slot_num))) return rc;
  int index;
  auto addr = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  utils::BitMap bitmap(addr + kBitMapStartPosition / 4, kBitMapLength * 8);
  int exist;
  if ((rc = bitmap.Get(slot_num, exist))) return rc;
  if (exist == 0) return RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR;
  int record_offset =
      kRecordStartPosition / 4 + slot_num * kRecordMaxLength / 4;
  if ((rc = rec.Set(record_size_, (char*)(addr + record_offset), rid)))
    return rc;
  return NO_ERROR;
}

int RM_FileHandle::InsertRecord(const char* data, RID& rid) {
  if (bpm_ == nullptr) return RM_FILEHANDLE_NOT_INITIALIZED_ERROR;

  int rc;

  int index;
  auto addr = bpm_->getPage(file_id_, /*page_num=*/0, index);
  bpm_->access(index);
  utils::BitMap page_bitmap(addr + kPageBitMapStartPosition / 4, kMaxPageNum);
  int page = page_bitmap.FindFirstZeroPosition() + 1;
  if (page > kMaxPageNum) return RM_FILEHANDLE_NO_EMPTY_POSITION_ERROR;
  max_page_num_ = std::max(max_page_num_, page);

  addr = bpm_->getPage(file_id_, page, index);
  bpm_->access(index);
  utils::BitMap bitmap(addr + kBitMapStartPosition / 4, kBitMapLength * 8);
  int slot = bitmap.FindFirstZeroPosition();
  if ((rc = rid.Set(page, slot))) return rc;
  int record_offset = kRecordStartPosition / 4 + slot * kRecordMaxLength / 4;
  std::memcpy(addr + record_offset, data, record_size_);
  bpm_->markDirty(index);
  if ((rc = bitmap.SetOne(slot))) return rc;

  if (bitmap.IsFull()) {
    addr = bpm_->getPage(file_id_, /*page_num=*/0, index);
    bpm_->access(index);
    utils::BitMap page_bitmap(addr + kPageBitMapStartPosition / 4, kMaxPageNum);
    if ((rc = page_bitmap.SetOne(page - 1))) return rc;
    bpm_->markDirty(index);
  }
  return NO_ERROR;
}

int RM_FileHandle::DeleteRecord(const RID& rid) {
  if (bpm_ == nullptr) return RM_FILEHANDLE_NOT_INITIALIZED_ERROR;

  int rc;

  int page_num, slot_num;
  if ((rc = rid.GetPageNum(page_num))) return rc;
  if ((rc = rid.GetSlotNum(slot_num))) return rc;

  int index;
  auto addr = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  utils::BitMap bitmap(addr + kBitMapStartPosition / 4, kBitMapLength * 8);
  bool is_full_before_destroy = bitmap.IsFull();
  int exist;
  if ((rc = bitmap.Get(slot_num, exist))) return rc;
  if (exist == 0) return RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR;
  if ((rc = bitmap.SetZero(slot_num))) return rc;
  bpm_->markDirty(index);

  if (is_full_before_destroy) {
    addr = bpm_->getPage(file_id_, /*page_num=*/0, index);
    bpm_->access(index);
    utils::BitMap page_bitmap(addr + kPageBitMapStartPosition / 4, kMaxPageNum);
    if ((rc = page_bitmap.SetZero(page_num - 1))) return rc;
    bpm_->markDirty(index);
  }
  return NO_ERROR;
}

int RM_FileHandle::UpdateRecord(const RM_Record& rec) {
  if (bpm_ == nullptr) return RM_FILEHANDLE_NOT_INITIALIZED_ERROR;

  int rc;

  char* data;
  RID rid;
  if ((rc = rec.GetData(data))) return rc;
  if ((rc = rec.GetRid(rid))) return rc;
  int page_num, slot_num;
  if ((rc = rid.GetPageNum(page_num))) return rc;
  if ((rc = rid.GetSlotNum(slot_num))) return rc;

  int index;
  auto addr = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  utils::BitMap bitmap(addr + kBitMapStartPosition / 4, kBitMapLength * 8);
  int exist;
  if ((rc = bitmap.Get(slot_num, exist))) return rc;
  if (exist == 0) return RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR;
  int record_offset =
      kRecordStartPosition / 4 + slot_num * kRecordMaxLength / 4;
  std::memcpy(addr + record_offset, data, record_size_);
  bpm_->markDirty(index);
  return NO_ERROR;
}

int RM_FileHandle::ForcePages() {
  if (bpm_ != nullptr) bpm_->close();
  return NO_ERROR;
}

int RM_FileHandle::GetFileID() { return file_id_; }

int RM_FileHandle::GetRecordSize() { return record_size_; }

int RM_FileHandle::GetMaxPageNum() { return max_page_num_; }

int RM_FileHandle::GetNextNotEmptySlot(int page_num, int slot_num) {
  int index;
  auto addr = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  utils::BitMap bitmap(addr + kBitMapStartPosition / 4, kBitMapLength * 8);
  for (int i = slot_num + 1; i < kRecordNumEachPage; i++) {
    int x;
    bitmap.Get(i, x);
    if (x == 1) return i;
  }
  return kRecordNumEachPage;
}

RM_Manager::RM_Manager(filesystem::FileManager* fm,
                       filesystem::BufPageManager* bpm)
    : fm_(fm), bpm_(bpm) {}

RM_Manager::~RM_Manager() {}

int RM_Manager::CreateFile(const std::string& file_name, int record_size) {
  if (fn_to_rs_.find(file_name) != fn_to_rs_.end()) {
    return RM_MANAGER_DUPLICATE_FILE_NAME_ERROR;
  }
  fm_->createFile(file_name.c_str());
  fn_to_rs_[file_name] = record_size;
  return NO_ERROR;
}

int RM_Manager::DestroyFile(const std::string& file_name) {
  auto it = fn_to_rs_.find(file_name);
  if (it == fn_to_rs_.end()) {
    return RM_MANAGER_FILE_NOT_FOUND_ERROR;
  }
  fn_to_rs_.erase(file_name);
  return NO_ERROR;
}

int RM_Manager::OpenFile(const std::string& file_name,
                         RM_FileHandle& file_handle) {
  auto it = fn_to_rs_.find(file_name);
  if (it == fn_to_rs_.end()) {
    return RM_MANAGER_FILE_NOT_FOUND_ERROR;
  }
  int file_id;
  if (!fm_->openFile(file_name.c_str(), file_id)) {
    return RM_MANAGER_FILE_CANNOT_OPEN_ERROR;
  }
  file_handle.Init(file_name, it->second, fm_, bpm_, file_id);
  return NO_ERROR;
}

int RM_Manager::CloseFile(RM_FileHandle& file_handle) {
  int rc;
  if ((rc = fm_->closeFile(file_handle.GetFileID()))) return rc;
  return NO_ERROR;
}

RM_FileScan::RM_FileScan() {
  file_handle_ = nullptr;
  attr_length_ = 0;
  attr_offset_ = 0;
  value_ = nullptr;
}

RM_FileScan::~RM_FileScan() {}

int RM_FileScan::OpenScan(RM_FileHandle* file_handle, AttrType attr_type,
                          int attr_length, int attr_offset, CompOp comp_op,
                          void* value) {
  if ((attr_type == AttrType::INT || attr_type == AttrType::FLOAT) &&
      attr_length != 4) {
    return RM_FILESCAN_INVALID_ATTR_LENGTH_ERROR;
  }
  file_handle_ = file_handle;
  attr_length_ = attr_length;
  attr_offset_ = attr_offset;
  value_ = value;
  check_ = GetCheckFunction(attr_type, comp_op);
  current_.Set(0, RM_FileHandle::kRecordNumEachPage - 1);
  return NO_ERROR;
}

int RM_FileScan::GetNextRecord(RM_Record& record) {
  int rc;
  int page_num, slot_num;
  if ((rc = current_.GetPageNum(page_num))) return rc;
  if ((rc = current_.GetSlotNum(slot_num))) return rc;
  for (int i = page_num; i <= file_handle_->GetMaxPageNum(); i++) {
    int j = -1;
    if (i == page_num) j = slot_num;
    for (j = file_handle_->GetNextNotEmptySlot(i, j);
         j < RM_FileHandle::kRecordNumEachPage;
         j = file_handle_->GetNextNotEmptySlot(i, j)) {
      RID rid(i, j);
      RM_Record tmp;
      if ((rc = file_handle_->GetRecord(rid, tmp))) return rc;
      char* data;
      if ((rc = tmp.GetData(data))) return rc;
      if (check_(data + attr_offset_, value_, attr_length_)) {
        current_.Set(i, j);
        record = tmp;
        return NO_ERROR;
      }
    }
  }
  return EOF;
}

int RM_FileScan::CloseScan() {
  file_handle_ = nullptr;
  attr_length_ = 0;
  attr_offset_ = 0;
  value_ = nullptr;
  current_.Set(0, RM_FileHandle::kRecordNumEachPage - 1);
  return NO_ERROR;
}

}  // namespace recordmanager