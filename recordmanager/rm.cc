#include "rm.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"
#include "utils/bitmap.h"

namespace recordmanager {

void RM_PrintError(int rc) { fprintf(stderr, "RM error %d.\n", rc); }

const int RM_FileHandle::kPageSize = 8192;
const int RM_FileHandle::kRecordSizePosition = 0;
const int RM_FileHandle::kPageBitMapStartPosition = 4096;
const int RM_FileHandle::kMaxPageNum = 32768;
const int RM_FileHandle::kBitMapStartPosition = 0;
const int RM_FileHandle::kBitMapLength = 4;
const int RM_FileHandle::kRecordStartPosition = 192;
const int RM_FileHandle::kRecordNumEachPage = 32;
const int RM_FileHandle::kRecordMaxLength = 250;

RM_FileHandle::RM_FileHandle() : fm_(nullptr), bpm_(nullptr) {}

RM_FileHandle::~RM_FileHandle() {
  ForcePages();
}

void RM_FileHandle::init(const std::string& file_name, int record_size, filesystem::FileManager* fm, filesystem::BufPageManager* bpm, int file_id) {
  file_name_ = file_name;
  record_size_ = record_size;
  fm_ = fm;
  bpm_ = bpm;
  file_id_ = file_id;
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
  int page = page_bitmap.FindFirstZeroPosition();

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
    if ((rc = page_bitmap.SetOne(page))) return rc;
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
    if ((rc = page_bitmap.SetZero(page_num))) return rc;
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
  int record_offset = kRecordStartPosition / 4 + slot_num * kRecordMaxLength / 4;
  std::memcpy(addr + record_offset, data, record_size_);
  bpm_->markDirty(index);
  return NO_ERROR;
}

int RM_FileHandle::ForcePages() {
  bpm_->close();
  return NO_ERROR;
}

RM_Manager::RM_Manager(filesystem::FileManager* fm,
                       filesystem::BufPageManager* bpm)
    : fm_(fm), bpm_(bpm) {}

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

}  // namespace recordmanager