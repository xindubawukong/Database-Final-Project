#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <vector>
#include <string>
#include <assert.h>
#include <cstring>
#include "../global.h"
#include "../systemmanager/sm.h"
#include "../recordmanager/rm.h"
#include "../indexing/ix.h"
#include "../querylanguage/ql.h"

namespace parser {


enum AlterObject {
  PRIMARY,
  FOREIGN,
  COLUMN
};

enum AlterType {
  ADD,
  DROP,
  CHANGE
};

class Tree;

// Sys
class ShowDatabase;

// DataBases
class CreateDatabase;

class DropDatabase;

class UseDatabase;

// Tables
class CreateTable;

class DropTable;

class ShowTable;

class AlterTable;

// Indexes
class CreateIndex;

class DropIndex;

// Data types
class ColumnList;

class Field;

class FieldList;

class KeyConstraint;

class KeyConstraintList;

class Tree {
  public:
    virtual ~Tree() = default;
    static void setInstance(Tree* t) {
      delete tree;
      tree = t;
    } 

    virtual void visit() {
      assert(false);
    }

    static void run() {
      if(tree != nullptr) {
        tree->visit();
      }
    }

    static Tree* tree;
};

class ShowDatabase: public Tree {
  public:
    ShowDatabase(std::string dbName);

    void visit() override;

    std::string dbName;
};

class CreateDatabase: public Tree {
  public:
    CreateDatabase(std::string dbName);

    void visit() override;

    std::string dbName;
};

class DropDatabase: public Tree {
  public:
    DropDatabase(std::string dbName);
    void visit() override;

    std::string dbName;
};

class UseDatabase: public Tree {
  public:
    UseDatabase(std::string dbName);
    void visit() override;

    std::string dbName;
};

class ShowTable: public Tree {
  public:
    ShowTable(std::string tbName);
    void visit() override;

    std::string tbName;
};

class CreateTable: public Tree {
  public:
    CreateTable(std::string tbName, FieldList* fieldList, KeyConstraintList* constraints = nullptr);
    void visit() override;
    ~CreateTable() override;

    std::string tbName;
    FieldList* fieldList;
    KeyConstraintList* constraintts = nullptr;
};

class DropTable: public Tree {
  public:
    DropTable(std::string tbName);
    void visit() override;

    std::string tbName;
};

class AlterTable: public Tree {
  public:
    AlterTable(std::string tbName, AlterObject alterObject, AlterType alterType, ColumnList* c1 = nullptr, ColumnList* c2 = nullptr, std::string columnName = nullptr, Field* field = nullptr);
    void visit() override;
    ~AlterTable() override;

    std::string tbName;
    AlterObject alterObject;
    AlterType alterType;
    ColumnList* c1 = nullptr, *c2 = nullptr;
    std::string columnName;
    Field* field = nullptr;
};

class CreateIndex: public Tree {
  public:
    CreateIndex(std::string tbname, std::string columnName);
    void visit() override;

    std::string tbName;
    std::string columnName;
};

class DropIndex: public Tree {
  public:
    DropIndex(std::string tbName, std::string columnName);
    void visit() override;

    std::string tbName;
    std::string coloumnName;
};

class ColumnList: public Tree {
  public:
    ColumnList() = default;
    void visit() override;
    void add(std::string attrName);

    std::vector<std::string> attrNameList;
};

class Field: public Tree {
  public:
    Field(std::string attrName, AttrType attrType, int attrLength, bool notNull = false, void* defaltValue = nullptr);
    void visit() override;
    ~Field() override;

    std::string attrName;
    AttrType attrType;
    int attrLength;
    bool notNull;
    void* defaultValue;
};

class FieldList: public Tree {
  public:
    FieldList() = default;
    void visit() override;
    void add(Field* field);
    std::vector<Field*> fieldList;
};

class KeyConstraint: public Tree {
  public:
    KeyConstraint(bool isPrimary, ColumnList* columnList, std::string tbName, ColumnList* foreignList = nullptr);
    void visit() override;
    ~KeyConstraint() override;

    std::string tbName;
    ColumnList* columnList;
    ColumnList* foreignList;
    bool isPrimary;

};

class KeyConstraintList: public Tree {
  public:
    KeyConstraintList();
    void visit() override;
    void add(KeyConstraint* constraint);
    std::vector<KeyConstraint*> constraintList;
};

class ValueList {
 public:
  std::vector<querylanguage::Value*> values;

  ValueList(querylanguage::Value* value) {
    values.clear();
    values.push_back(value);
  }

  void AddValue(querylanguage::Value* value) {
    values.push_back(value);
  }
};

class ValueLists {
 public:
  std::vector<ValueList*> valuelists;

  ValueLists(ValueList* valuelist) {
    valuelists.clear();
    valuelists.push_back(valuelist);
  }

  void AddValueList(ValueList* valuelist) {
    valuelists.push_back(valuelist);
  }
};

class Insert: public Tree {
 public:
  std::string tbname;
  ValueLists* valuelists;

  Insert(char* tbname, ValueLists* valuelists) {
    this->tbname = tbname;
    this->valuelists = valuelists;
  }

  void visit();
};

class Col {
 public:
  char* tbname;
  char* colname;

  Col(char* tbname = NULL, char* colname = NULL) {
    this->tbname = tbname;
    this->colname = colname;
  }
};

class Expr {
 public:
  Col* col;
  querylanguage::Value* value;
  bool is_value;
};

class WhereClause: public Tree {
 public:
  std::vector<querylanguage::Condition*> conditions;

  WhereClause(Col* col, CompOp op, Expr* expr) {
    auto condition = new querylanguage::Condition();
    condition->lhsAttr.relName = col->tbname;
    condition->lhsAttr.attrName = col->colname;
    condition->op = op;
    if (expr->is_value) {
      condition->bRhsIsAttr = false;
      condition->rhsValue = *expr->value;
    }
    else {
      condition->bRhsIsAttr = true;
      condition->rhsAttr.relName = expr->col->tbname;
      condition->rhsAttr.attrName = expr->col->colname;
    }
    conditions.clear();
    conditions.push_back(condition);
  }

  WhereClause(WhereClause* w1, WhereClause* w2) {
    conditions.clear();
    for (auto t : w1->conditions) conditions.push_back(t);
    for (auto t : w2->conditions) conditions.push_back(t);
  }
};

class Delete : public Tree {
 public:
  std::string tbname;
  WhereClause* whereclause;

  Delete(char* tbname, WhereClause* whereclause) {
    this->tbname = tbname;
    this->whereclause = whereclause;
  }

  void visit();
};

}


#endif