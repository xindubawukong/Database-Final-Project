#include "global.h"

const int kPageSize = 8192;

std::function<bool(void*, void*, int)> GetCheckFunction(
    AttrType attr_type, CompOp comp_op) {
  if (attr_type == AttrType::INT) {
    std::function<int(void*)> f = [](void* x) -> int { return *((int*)x); };
    if (comp_op == CompOp::EQ_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) == f(b); };
    } else if (comp_op == CompOp::LT_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) < f(b); };
    } else if (comp_op == CompOp::GT_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) > f(b); };
    } else if (comp_op == CompOp::LE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) <= f(b); };
    } else if (comp_op == CompOp::GE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) >= f(b); };
    } else if (comp_op == CompOp::NE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) != f(b); };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  } else if (attr_type == AttrType::FLOAT) {
    std::function<float(void*)> f = [](void* x) -> float {
      return *((float*)x);
    };
    if (comp_op == CompOp::EQ_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) == f(b); };
    } else if (comp_op == CompOp::LT_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) < f(b); };
    } else if (comp_op == CompOp::GT_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) > f(b); };
    } else if (comp_op == CompOp::LE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) <= f(b); };
    } else if (comp_op == CompOp::GE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) >= f(b); };
    } else if (comp_op == CompOp::NE_OP) {
      return [&f](void* a, void* b, int) -> bool { return f(a) != f(b); };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  } else {  // attr_type == AttrType::STRING
    if (comp_op == CompOp::EQ_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) != (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::LT_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) < (*y)) return true;
          if ((*x) > (*y)) return false;
          x++;
          y++;
        }
        return false;
      };
    } else if (comp_op == CompOp::GT_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) > (*y)) return true;
          if ((*x) < (*y)) return false;
          x++;
          y++;
        }
        return false;
      };
    } else if (comp_op == CompOp::LE_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) < (*y)) return true;
          if ((*x) > (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::GE_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) > (*y)) return true;
          if ((*x) < (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::NE_OP) {
      return [](void* a, void* b, int length) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < length; i++) {
          if ((*x) != (*y)) return true;
          x++;
          y++;
        }
        return false;
      };
    } else {
      return [](void*, void*, int) -> bool { return true; };
    }
  }
}