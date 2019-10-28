#include <cstring>
#include "gtest/gtest.h"
#include "rm_record.h"
#include "return_code.h"

namespace recordmanager {

TEST(TestRMRecord, SimpleTest) {
  RM_Record rm_record;
  char* a;
  RID rid;
  EXPECT_EQ(rm_record.GetData(a), RM_RECORD_NOT_INITIALIZED_ERROR);
  EXPECT_EQ(rm_record.GetRid(rid), RM_RECORD_NOT_INITIALIZED_ERROR);
  rid = RID(1, 3);
  char b[10] = "ilovewjq";
  rm_record.Set(10, b, rid);
  EXPECT_EQ(rm_record.GetData(a), 0);
  EXPECT_EQ(std::strcmp(a, "ilovewjq"), 0);
  RID rid2;
  EXPECT_EQ(rm_record.GetRid(rid2), 0);
  EXPECT_EQ(rid2, rid);
  EXPECT_EQ(rm_record.Set(10, a, rid), RM_RECORD_MULTIPLE_SET_ERROR);
}

}  // namespace recordmanager