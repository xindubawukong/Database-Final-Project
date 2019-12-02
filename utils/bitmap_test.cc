#include "bitmap.h"
#include "gtest/gtest.h"
#include "return_code.h"

namespace utils {

TEST(TestBitMap, SimpleTest) {
  int a[100];
  for (int i = 0; i < 100; i++) a[i] = 0;
  BitMap* bitmap = new BitMap(&a, 100 * 32);
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
  EXPECT_EQ(bitmap->FindFirstZeroPosition(), 0);
  delete bitmap;
}

TEST(TestBitMap, TestStrangeLength) {
  int a[100];
  for (int i = 0; i < 100; i++) a[i] = 0;
  BitMap* bitmap = new BitMap(&a, 100);
  for (int i = 0; i < 100; i++) {
    EXPECT_EQ(bitmap->SetOne(i), NO_ERROR);
  }
  EXPECT_EQ(bitmap->FindFirstZeroPosition(), 100);
  EXPECT_TRUE(bitmap->IsFull());
  EXPECT_EQ(bitmap->SetZero(33), NO_ERROR);
  EXPECT_EQ(bitmap->FindFirstZeroPosition(), 33);
}

}  // namespace utils