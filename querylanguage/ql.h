#ifndef QUERYPROCESSING_QL_H
#define QUERYPROCESSING_QL_H

#include <iostream>

#include "global.h"
#include "indexing/ix.h"
#include "recordmanager/rm.h"
#include "systemmanager/sm.h"

namespace querylanguage {

struct RelAttr {
  char *relName;   // relation name (may be NULL)
  char *attrName;  // attribute name
  friend std::ostream& operator <<(std::ostream &s, const RelAttr &ra);
};

struct Value {
  AttrType type;  // type of value
  void *data;     // value
  friend std::ostream& operator <<(std::ostream &s, const Value &v);
};

struct Condition {
  RelAttr lhsAttr;  // left-hand side attribute
  CompOp op;        // comparison operator
  int bRhsIsAttr;   // TRUE if right-hand side is an attribute
                    // and not a value
  RelAttr rhsAttr;  // right-hand side attribute if bRhsIsAttr = TRUE
  Value rhsValue;   // right-hand side value if bRhsIsAttr = FALSE
  friend std::ostream& operator <<(std::ostream &s, const Condition &c);
};

class QL_Manager {
 public:
  QL_Manager(recordmanager::RM_Manager *rmm, indexing::IX_Manager *ixm,
             systemmanager::SM_Manager *smm, filesystem::FileManager* fm,
             filesystem::BufPageManager* bpm);

  ~QL_Manager();

  int Select(int nSelAttrs,                  // # attrs in Select clause
             const RelAttr selAttrs[],       // attrs in Select clause
             int nRelations,                 // # relations in From clause
             const char *const relations[],  // relations in From clause
             int nConditions,                // # conditions in Where clause
             const Condition conditions[]);  // conditions in Where clause

  int Insert(const char *relName,    // relation to insert into
             int nValues,            // # values to insert
             const Value values[]);  // values to insert

  int Delete(const char *relName,            // relation to delete from
             int nConditions,                // # conditions in Where clause
             const Condition conditions[]);  // conditions in Where clause

  int Update(const char *relName,        // relation to update
             const RelAttr &updAttr,     // attribute to update
             const int bIsValue,         // 0/1 if RHS of = is attribute/value
             const RelAttr &rhsRelAttr,  // attr on RHS of =
             const Value &rhsValue,      // value on RHS of =
             int nConditions,            // # conditions in Where clause
             const Condition conditions[]);  // conditions in Where clause

 private:
  filesystem::FileManager* fm_;
  filesystem::BufPageManager* bpm_;
  recordmanager::RM_Manager *rmm_;
  indexing::IX_Manager *ixm_;
  systemmanager::SM_Manager *smm_;
};

}  // namespace querylanguage

#endif  // QUERYPROCESSING_QL_H