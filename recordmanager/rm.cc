#include "rm.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"
#include "utils/bitmap.h"

namespace recordmanager {

void RM_PrintError(int rc) { fprintf(stderr, "RM error %d.\n", rc); }

RM_Manager::RM_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm) {
  this->fm_ = fm;
  this->bpm_ = bpm;
}

const int RM_FileHandle::kPageSize = 8192;
const int RM_FileHandle::kRecordSizePosition = 0;
const int RM_FileHandle::kPageBitMapStartPosition = 4096;
const int RM_FileHandle::kMaxPageNum = 32768;
const int RM_FileHandle::kBitMapStartPosition = 0;
const int RM_FileHandle::kBitMapLength = 4;
const int RM_FileHandle::kRecordStartPosition = 192;
const int RM_FileHandle::kRecordNumEachPage = 32;
const int RM_FileHandle::kRecordMaxLength = 250;

int RM_FileHandle::GetRecord(const RID& rid, RM_Record& rec) const {
  int rc;
  int page_num, slot_num;
  if ((rc = rid.GetPageNum(page_num))) return rc;
  if ((rc = rid.GetSlotNum(slot_num))) return rc;
  int index;
  auto b = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  int bitmap_offset = kBitMapStartPosition / 4;
  int bitmap_length = kBitMapLength * 8;
  utils::BitMap bitmap(b + bitmap_offset, bitmap_length);
  int t;
  if ((rc = bitmap.Get(slot_num, t))) return rc;
  if (t == 0) return RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR;
  int record_offset = kRecordStartPosition / 4 + slot_num * kRecordMaxLength / 4;
  if ((rc = rec.Set(record_size_, (char*)(b + record_offset), rid))) return rc;
  return NO_ERROR;
}

int RM_FileHandle::InsertRecord(const char* pdata, RID& rid) {
  int rc;
}

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