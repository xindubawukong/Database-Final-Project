#include "global.h"
#include "gtest/gtest.h"

TEST(TestGetCheckFunction, TestInt) {
  auto f = GetCheckFunction(AttrType::INT, CompOp::GE_OP);
  int x = 1, y = 2;
  EXPECT_FALSE(f(&x, &y, 4));
  x = 3;
  EXPECT_TRUE(f(&x, &y, 4));
}

TEST(TestGetCheckFunction, TestFloat) {
  auto f = GetCheckFunction(AttrType::FLOAT, CompOp::LT_OP);
  float x = 1.5, y = 2.2;
  EXPECT_TRUE(f(&x, &y, 4));
  x = 2.3;
  EXPECT_FALSE(f(&x, &y, 4));
}

TEST(TestGetCheckFunction, TestString) {
  auto f = GetCheckFunction(AttrType::STRING, CompOp::EQ_OP);
  char a[] = "abcdefghij";
  char b[] = "abcdefgaaa";
  auto g = GetCheckFunction(AttrType::STRING, CompOp::GT_OP);
  EXPECT_TRUE(g(a, b, 10));
}