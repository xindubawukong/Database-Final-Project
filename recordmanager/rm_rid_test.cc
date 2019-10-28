#include "gtest/gtest.h"
#include "rm_rid.h"
#include "return_code.h"

using recordmanager::RID;

TEST(TestRid, BasicTest) {
  RID rid1;
  int x;
  EXPECT_EQ(rid1.GetPageNum(x), RM_RID_NOT_INITIALIZED_ERROR);
  EXPECT_EQ(rid1.GetSlotNum(x), RM_RID_NOT_INITIALIZED_ERROR);

  rid1.Set(3, 4);
  rid1.GetPageNum(x);
  EXPECT_EQ(x, 3);
  rid1.GetSlotNum(x);
  EXPECT_EQ(x, 4);

  RID rid2(1, 2);
  rid2.GetPageNum(x);
  EXPECT_EQ(x, 1);
  rid2.GetSlotNum(x);
  EXPECT_EQ(x, 2);
}