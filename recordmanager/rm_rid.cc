#include "rm_rid.h"
#include "../return_code.h"

namespace recordmanager {

RID::RID() {
  initialized_ = false;
}

RID::~RID() {}

RID::RID(int page_num, int slot_num) {
  initialized_ = true;
  page_num_ = page_num;
  slot_num_ = slot_num;
}

int RID::Set(int page_num, int slot_num) {
  if (initialized_) return RM_RID_MULTIPLE_SET_ERROR;
  initialized_ = true;
  page_num_ = page_num;
  slot_num_ = slot_num;
  return 0;
}

int RID::GetPageNum(int& page_num) const {
  if (!initialized_) return RM_RID_NOT_INITIALIZED_ERROR;
  page_num = page_num_;
  return 0;
}

int RID::GetSlotNum(int& slot_num) const {
  if (!initialized_) return RM_RID_NOT_INITIALIZED_ERROR;
  slot_num = slot_num_;
  return 0;
}

bool operator ==(const RID& a, const RID& b)  {
  return a.initialized_ == b.initialized_ && a.page_num_ == b.page_num_ && a.slot_num_ == b.slot_num_;
}

}  // namespace recordmanager