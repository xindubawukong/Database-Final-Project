#include "global.h"
#include <iostream>
#include <cstring>

const int kPageSize = 8192;

std::function<bool(void*, void*, int)> GetCheckFunction(AttrType attr_type,
                                                        CompOp comp_op) {
  if (attr_type == AttrType::INT) {
    if (comp_op == CompOp::EQ_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((int*)a) == *((int*)b);
      };
    } else if (comp_op == CompOp::LT_OP) {
      return
          [](void* a, void* b, int) -> bool { return *((int*)a) < *((int*)b); };
    } else if (comp_op == CompOp::GT_OP) {
      return
          [](void* a, void* b, int) -> bool { return *((int*)a) > *((int*)b); };
    } else if (comp_op == CompOp::LE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((int*)a) <= *((int*)b);
      };
    } else if (comp_op == CompOp::GE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((int*)a) >= *((int*)b);
      };
    } else if (comp_op == CompOp::NE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((int*)a) != *((int*)b);
      };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  } else if (attr_type == AttrType::FLOAT) {
    if (comp_op == CompOp::EQ_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) == *((float*)b);
      };
    } else if (comp_op == CompOp::LT_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) < *((float*)b);
      };
    } else if (comp_op == CompOp::GT_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) > *((float*)b);
      };
    } else if (comp_op == CompOp::LE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) <= *((float*)b);
      };
    } else if (comp_op == CompOp::GE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) >= *((float*)b);
      };
    } else if (comp_op == CompOp::NE_OP) {
      return [](void* a, void* b, int) -> bool {
        return *((float*)a) != *((float*)b);
      };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  } else {  // attr_type == AttrType::STRING
    if (comp_op == CompOp::EQ_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) == 0;
      };
    } else if (comp_op == CompOp::LT_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) < 0;
      };
    } else if (comp_op == CompOp::GT_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) > 0;
      };
    } else if (comp_op == CompOp::LE_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) <= 0;
      };
    } else if (comp_op == CompOp::GE_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) >= 0;
      };
    } else if (comp_op == CompOp::NE_OP) {
      return [](void* a, void* b, int length) -> bool {
        return std::strcmp((char*)a, (char*)b) != 0;
      };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  }
}