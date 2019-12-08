#ifndef RECORDMANAGER_RM_RECORD_H
#define RECORDMANAGER_RM_RECORD_H

#include "rm_rid.h"

namespace recordmanager {

class RM_Record {
 public:
  RM_Record();
  ~RM_Record();

  int Set(int length, char* data, RID rid);

  int GetData(char*& p) const;
  int GetRid(RID& rid) const;

  void operator =(const RM_Record& record);

 private:
  RID rid_;
  int length_;
  char* data_;
};

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_RECORD_H