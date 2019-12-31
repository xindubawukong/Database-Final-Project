#include "rm_record.h"

#include <cstring>

#include "../return_code.h"

namespace recordmanager {

RM_Record::RM_Record() {
  length_ = -1;
  data_ = nullptr;
}

RM_Record::~RM_Record() {
  delete[] data_;
}

int RM_Record::Set(int length, char* data, RID rid) {
  if (length <= 0) return RM_RECORD_INVALID_LENGTH_ERROR;
  length_ = length;
  data_ = new char[length]();
  std::memcpy(data_, data, length);
  rid_ = rid;
  return NO_ERROR;
}

int RM_Record::GetData(char*& p) const {
  if (length_ == -1 && data_ == nullptr) return RM_RECORD_NOT_INITIALIZED_ERROR;
  p = data_;
  return NO_ERROR;
}

int RM_Record::GetRid(RID& rid) const {
  if (length_ == -1 && data_ == nullptr) return RM_RECORD_NOT_INITIALIZED_ERROR;
  rid = rid_;
  return NO_ERROR;
}

void RM_Record::operator =(const RM_Record& record) {
  rid_ = record.rid_;
  length_ = record.length_;
  delete[] data_;
  data_ = new char[length_]();
  std::memcpy(data_, record.data_, length_);
}

}  // namespace recordmanager