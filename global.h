#ifndef GLOBAL_H
#define GLOBAL_H

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

#endif  // GLOBAL_H