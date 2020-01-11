#include "bitmap.h"

#include <cstdio>
#include <algorithm>

#include "return_code.h"

namespace utils {

const unsigned int BitMap::all_one_ = 4294967295;

BitMap::BitMap(void* addr, int length) {
  addr_ = (unsigned int*)addr;
  length_ = length;
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
      int up = std::min(length_ - num, 32);
      for (int i = 0; i < up; i++) {
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

int BitMap::FindLastOnePosition() {
  unsigned int* p = addr_;
  int last = 0;
  int num = 0;
  while (num < length_) {
    int x = *p;
    if (x != 0) {
      int up = std::min(length_ - num, 32);
      for (int i = 0; i < up; i++) {
        if ((x & (1 << i)) != 0) {
          last = num + i;
        }
      }
    }
    p++;
    num += 32;
  }
  return last;
}

bool BitMap::IsFull() {
  unsigned int* p = addr_;
  int num = 0;
  while (num < length_) {
    if (num + 32 > length_) {
      int n = length_ - num;
      unsigned int tmp = (1 << n) - 1;
      if (((*p) & tmp) != tmp) return false;
    }
    else {
      if ((*p) != all_one_) return false;
    }
    p++;
    num += 32;
  }
  return true;
}

bool BitMap::IsEmpty() {
  unsigned int* p = addr_;
  int num = 0;
  while (num < length_) {
    if (num + 32 > length_) {
      int n = length_ - num;
      unsigned int tmp = (1 << n) - 1;
      if (((*p) & tmp) != 0) return false;
    }
    else {
      if ((*p) != 0) return false;
    }
    p++;
    num += 32;
  }
  return true;
}

}  // namespace utils