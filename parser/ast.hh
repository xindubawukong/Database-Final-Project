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

class Tree;

// Sys
class ShowDatabase;

// DataBases
class CreateDatabase;

class DropDatabase;

class UseDatabase;

class CloseDatabase;

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

class Constraint;

class ConstraintList;

class AlterPrimaryKey;

class AlterForeignKey;

class AlterColumn;

struct Type {
  AttrType attrType;
  int attrLength;
};

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
    ShowDatabase();

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

class CloseDatabase: public Tree {
  public:
  CloseDatabase();
  void visit() override;
};

class ShowTable: public Tree {
  public:
    ShowTable(std::string tbName);
    ShowTable();
    void visit() override;

    std::string tbName;
};

class CreateTable: public Tree {
  public:
    CreateTable(std::string tbName, FieldList* fieldList, ConstraintList* constraints = nullptr);
    void visit() override;
    ~CreateTable() override;

    std::string tbName;
    FieldList* fieldList;
    ConstraintList* constraints = nullptr;
};

class DropTable: public Tree {
  public:
    DropTable(std::string tbName);
    void visit() override;

    std::string tbName;
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
    void visit() {};
    void add(std::string attrName);

    std::vector<std::string> attrNameList;
};

class Field: public Tree {
  public:
    Field(std::string attrName, AttrType attrType, int attrLength, bool notNull = false, void* defaltValue = nullptr);
    void visit() {};
    systemmanager::AttrInfo info;
};

class FieldList: public Tree {
  public:
    FieldList() {};
    void visit() {};
    void add(Field* field);
    std::vector<Field*> fieldList;
};

class Constraint: public Tree {
  public:
    Constraint(bool isPrimary, ColumnList* columnList, std::string fkName, std::string tbName, ColumnList* foreignList = nullptr);
    void visit() {};

    systemmanager::Constraint cons;

};

class ConstraintList: public Tree {
  public:
    ConstraintList() {};
    void visit() {};
    void add(Constraint* constraint);
    std::vector<Constraint*> constraintList;
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

class SetClause {
 public:
  std::vector<std::pair<char*, querylanguage::Value*>> list;

  SetClause(char* colname, querylanguage::Value* value) {
    list.clear();
    list.push_back(std::make_pair(colname, value));
  }

  void Add(char* colname, querylanguage::Value* value) {
    list.push_back(std::make_pair(colname, value));
  }
};

class Update: public Tree {
 public:
  char* tbname;
  SetClause* setclause;

  Update(char* tbname, SetClause* setclause) {
    this->tbname = tbname;
    this->setclause = setclause;
  }

  void visit();
};

class Selector {
 public:
  bool is_xing;
  std::vector<Col*>* cols;
};

class Select : public Tree {
 public:
  Selector* selector;
  std::vector<char*>* tablelist;
  WhereClause* whereclause;
  Select(Selector* selector, std::vector<char*>* tablelist, WhereClause* whereclause) {
    this->selector = selector;
    this->tablelist = tablelist;
    this->whereclause = whereclause;
  }

  void visit();
};

}


#endif