#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>

extern const int kPageSize;

enum AttrType {
  INT,
  FLOAT,
  STRING,
};

enum CompOp {
  EQ_OP,
  LT_OP,
  GT_OP,
  LE_OP,
  GE_OP,
  NE_OP,
  NO_OP,
};

std::function<bool(void*, void*, int)> GetCheckFunction(
      AttrType attr_type, CompOp comp_op);

#endif  // GLOBAL_H