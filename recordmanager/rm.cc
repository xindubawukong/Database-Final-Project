#include "rm.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"

namespace recordmanager {

void RM_PrintError(int rc) { fprintf(stderr, "RM error %d.\n", rc); }

RM_Manager::RM_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm) {
  this->fm_ = fm;
  this->bpm_ = bpm;
}

const int RM_FileHandle::kBitMapStartPosition = 0;
const int RM_FileHandle::kBitMapLength = 32;
const int RM_FileHandle::kRecordStartPosition = 192;
const int RM_FileHandle::kRecordNumEachPage = 32;
const int RM_FileHandle::kRecordLength = 250;

int RM_FileHandle::GetRecord(const RID& rid, RM_Record& rec) const {
  int rc;
  int page_num, slot_num;
  if ((rc = rid.GetPageNum(page_num))) return rc;
  if ((rc = rid.GetSlotNum(slot_num))) return rc;
  int index;
  auto b = bpm_->getPage(file_id_, page_num, index);
  bpm_->access(index);
  int pos = kRecordStartPosition / 4 + slot_num * kRecordLength / 4;
  if ((rc = rec.Set(kRecordLength, (char*)(b + pos), rid))) return rc;
  return NO_ERROR;
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