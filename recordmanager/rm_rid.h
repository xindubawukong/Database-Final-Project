#ifndef RECORDMANAGER_RM_RID_H
#define RECORDMANAGER_RM_RID_H

namespace recordmanager {

class RID {
 public:
  RID();
  ~RID();
  RID(int page_num, int slotNum);

  void Set(int page_num, int slot_num);

  int GetPageNum(int& page_num) const;
  int GetSlotNum(int& slot_num) const;

 private:
  bool initialized_;
  int page_num_;
  int slot_num_;
};

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_RID_H
