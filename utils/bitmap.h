#ifndef UTILS_BITMAP_H
#define UTILS_BITMAP_H

namespace utils {

#define BITMAP_MAX_LENGTH 32768

class BitMap {
 public:
  BitMap(unsigned int* addr, int length);

  int SetZero(int pos);

  int SetOne(int pos);

  int Get(int pos, int& x);

  int FindFirstZeroPosition();

  bool IsFull();

 private:
  unsigned int* addr_;
  int length_;
  const unsigned int all_one_ = 4294967295;
};

}  // namespace utils

#endif  // UTILS_BITMAP_H