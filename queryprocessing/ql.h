#ifndef QUERYPROCESSING_QL_H
#define QUERYPROCESSING_QL_H

#include <iostream>

#include "global.h"

struct RelAttr {
  char *relName;     // relation name (may be NULL) 
  char *attrName;    // attribute name              
  friend std::ostream& operator <<(std::ostream &s, const RelAttr &ra);
};

struct Value {
  AttrType type;     // type of value               
  void     *data;    // value                       
  friend std::ostream& operator <<(std::ostream &s, const Value &v);
};

struct Condition {
  RelAttr lhsAttr;      // left-hand side attribute                     
  CompOp  op;           // comparison operator                          
  int     bRhsIsAttr;   // TRUE if right-hand side is an attribute
                        // and not a value
  RelAttr rhsAttr;      // right-hand side attribute if bRhsIsAttr = TRUE
  Value   rhsValue;     // right-hand side value if bRhsIsAttr = FALSE
  friend std::ostream& operator <<(std::ostream &s, const Condition &c);
};

#endif  // QUERYPROCESSING_QL_H