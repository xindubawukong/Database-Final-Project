#include "rm_record.h"

#include <cstring>

#include "../return_code.h"

namespace recordmanager {

RM_Record::RM_Record() {
  initialized_ = false;
  length_ = 0;
  data_ = nullptr;
}

RM_Record::~RM_Record() {
  delete[] data_;
}

int RM_Record::Set(int length, char* data, RID rid) {
  if (length <= 0) return RM_RECORD_INVALID_LENGTH_ERROR;
  if (initialized_) return RM_RECORD_MULTIPLE_SET_ERROR;
  initialized_ = true;
  length_ = length;
  data_ = new char[length]();
  std::memcpy(data_, data, length);
  rid_ = rid;
  return NO_ERROR;
}

int RM_Record::GetData(char*& p) const {
  if (data_ == nullptr) return RM_RECORD_NOT_INITIALIZED_ERROR;
  p = data_;
  return NO_ERROR;
}

int RM_Record::GetRid(RID& rid) const {
  if (data_ == nullptr) return RM_RECORD_NOT_INITIALIZED_ERROR;
  rid = rid_;
  return NO_ERROR;
}

}  // namespace recordmanager