#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include <vector>
#include <string>
#include <assert.h>
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

class AlterPrimaryKey: public 


}


#endif