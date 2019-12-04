#ifndef RECORDMANAGER_RM_RID_H
#define RECORDMANAGER_RM_RID_H

namespace recordmanager {

class RID {
 public:
  RID();
  ~RID();
  RID(int page_num, int slotNum);

  int Set(int page_num, int slot_num);

  int GetPageNum(int& page_num) const;
  int GetSlotNum(int& slot_num) const;

  friend bool operator ==(const RID& a, const RID& b);

 private:
  int page_num_;
  int slot_num_;
};

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_RID_H
