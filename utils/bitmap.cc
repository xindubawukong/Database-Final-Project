#include "bitmap.h"

#include <cstdio>

#include "return_code.h"

namespace utils {

BitMap::BitMap(void* addr, int length) {
  addr_ = (unsigned int*)addr;
  length_ = length;
  if (length % 32 > 0) {
    fprintf(stderr, "BitMap length must be a multiple of 32.");
  }
}

int BitMap::SetZero(int pos) {
  if (pos < 0 || pos >= length_) {
    return UTILS_BITMAP_POSITION_OUT_OF_BOUNDS_ERROR;
  }
  unsigned int* p = addr_ + (pos / 32);
  unsigned int t = 1 << (pos % 32);
  (*p) &= t ^ all_one_;
  return NO_ERROR;
}

int BitMap::SetOne(int pos) {
  if (pos < 0 || pos >= length_) {
    return UTILS_BITMAP_POSITION_OUT_OF_BOUNDS_ERROR;
  }
  unsigned int* p = addr_ + (pos / 32);
  unsigned int t = 1 << (pos % 32);
  (*p) |= t;
  return NO_ERROR;
}

int BitMap::Get(int pos, int& x) {
  if (pos < 0 || pos >= length_) {
    return UTILS_BITMAP_POSITION_OUT_OF_BOUNDS_ERROR;
  }
  unsigned int* p = addr_ + (pos / 32);
  x = ((*p) >> (pos % 32)) & 1;
  return NO_ERROR;
}

int BitMap::FindFirstZeroPosition() {
  unsigned int* p = addr_;
  int num = 0;
  while (num < length_) {
    int x = *p;
    if (x != all_one_) {
      for (int i = 0; i < 32; i++) {
        if ((x & (1 << i)) == 0) {
          return num + i;
        }
      }
    }
    p++;
    num += 32;
  }
  return length_;
}

bool BitMap::IsFull() {
  unsigned int* p = addr_;
  int num = 0;
  while (num < length_) {
    if ((*p) != all_one_) return false;
    p++;
    num += 32;
  }
  return true;
}

}  // namespace utils