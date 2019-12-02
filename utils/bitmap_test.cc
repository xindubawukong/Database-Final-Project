#include "bitmap.h"
#include "gtest/gtest.h"
#include "return_code.h"

namespace utils {

TEST(TestBitMap, SimpleTest) {
  char a[100];
  BitMap* bitmap = new BitMap(&a, 100 * 8);
  int x;
  EXPECT_EQ(bitmap->Get(23, x), NO_ERROR);
  EXPECT_EQ(x, 0);
  bitmap->SetOne(23);
  EXPECT_EQ(bitmap->Get(23, x), NO_ERROR);
  EXPECT_EQ(x, 1);
  bitmap->SetZero(23);
  EXPECT_EQ(bitmap->Get(23, x), NO_ERROR);
  EXPECT_EQ(x, 0);
  EXPECT_FALSE(bitmap->IsFull());
  delete bitmap;
}

}  // namespace utils