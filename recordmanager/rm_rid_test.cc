#include "gtest/gtest.h"
#include "rm_rid.h"
#include "return_code.h"

using recordmanager::RID;

TEST(TestRid, SimpleTest) {
  RID rid1;
  int x;
  EXPECT_EQ(rid1.GetPageNum(x), RM_RID_NOT_INITIALIZED_ERROR);
  EXPECT_EQ(rid1.GetSlotNum(x), RM_RID_NOT_INITIALIZED_ERROR);

  EXPECT_EQ(rid1.Set(3, 4), NO_ERROR);
  rid1.GetPageNum(x);
  EXPECT_EQ(x, 3);
  rid1.GetSlotNum(x);
  EXPECT_EQ(x, 4);

  EXPECT_EQ(rid1.Set(5, 6), RM_RID_MULTIPLE_SET_ERROR);

  RID rid2(1, 2);
  rid2.GetPageNum(x);
  EXPECT_EQ(x, 1);
  rid2.GetSlotNum(x);
  EXPECT_EQ(x, 2);

  RID rid3;
  rid3.Set(1, 2);
  EXPECT_TRUE(rid2 == rid3);
}