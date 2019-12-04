#include "rm_rid.h"
#include "../return_code.h"

namespace recordmanager {

RID::RID(): page_num_(-1), slot_num_(-1) {}

RID::~RID() {}

RID::RID(int page_num, int slot_num) {
  page_num_ = page_num;
  slot_num_ = slot_num;
}

int RID::Set(int page_num, int slot_num) {
  page_num_ = page_num;
  slot_num_ = slot_num;
  return NO_ERROR;
}

int RID::GetPageNum(int& page_num) const {
  if (page_num_ == -1 && slot_num_ == -1) return RM_RID_NOT_INITIALIZED_ERROR;
  page_num = page_num_;
  return NO_ERROR;
}

int RID::GetSlotNum(int& slot_num) const {
  if (page_num_ == -1 && slot_num_ == -1) return RM_RID_NOT_INITIALIZED_ERROR;
  slot_num = slot_num_;
  return NO_ERROR;
}

bool operator ==(const RID& a, const RID& b)  {
  return a.page_num_ == b.page_num_ && a.slot_num_ == b.slot_num_;
}

}  // namespace recordmanager