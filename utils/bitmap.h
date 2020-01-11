#ifndef UTILS_BITMAP_H
#define UTILS_BITMAP_H

namespace utils {

#define BITMAP_MAX_LENGTH 32768

class BitMap {
 public:
  //
  // The BitMap starts from memory address 'addr', with 'length' bits.
  //
  // Notice that the BitMap may use a little more memory than 'length' in order
  // to make it a multiply of 32. For example, if you create a BitMap with
  // length=33, it may use 64 bits(2 unsigned int) from addr. But its
  // correctness is not influenced by this feature.
  //
  explicit BitMap(void* addr, int length);

  int SetZero(int pos);

  int SetOne(int pos);

  int Get(int pos, int& x);

  int FindFirstZeroPosition();

  int FindLastOnePosition();

  bool IsFull();

  bool IsEmpty();

 private:
  unsigned int* addr_;
  int length_;

  static const unsigned int all_one_;
};

}  // namespace utils

#endif  // UTILS_BITMAP_H