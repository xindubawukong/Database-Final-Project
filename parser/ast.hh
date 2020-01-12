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

extern filesystem::FileManager *fm;
extern filesystem::BufPageManager* bpm;
extern recordmanager::RM_Manager* rmm;
extern indexing::IX_Manager* ixm;
extern systemmanager::SM_Manager* sm;


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
    systemmanager::AttrList* toAttrList() {
      systemmanager::AttrList *attrs = new systemmanager::AttrList;
      for(int i = 0; i < (int)attrNameList.size(); ++i) {
        systemmanager::CopyStr(attrs->names[i], attrNameList[i].c_str(), MAX_LENGTH);
      }
      attrs->attrCount = (int)attrNameList.size();
      return attrs;
    }

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

class AlterPrimaryKey: public Tree {
  public:
    AlterPrimaryKey(std::string tbName, bool add = false, ColumnList* attrList = nullptr) {
      this->tbName = move(tbName);
      this->add = add;
      this->attrList = attrList;
    }

    void visit() {
      int rc = 0;
      if(!add) {
        rc = sm->AlterPrimaryKey(tbName.c_str());
      } else {
        systemmanager::AttrList *attrs = new systemmanager::AttrList();
        for(int i = 0; i < (int)attrList->attrNameList.size(); ++i) {
          systemmanager::CopyStr(attrs->names[i], attrList->attrNameList[i].c_str(), MAX_LENGTH);
        }
        attrs->attrCount = (int)attrList->attrNameList.size();
        rc = sm->AlterPrimaryKey(tbName.c_str(), true, attrs);
        delete attrs;
      }
      if(rc != 0) {
        std::cout << "Alter PrimaryKey for " << tbName << " failed." << std::endl;
      }
    }

    ~AlterPrimaryKey() {
      if(attrList != nullptr) {
        delete attrList;
      }
    }

    std::string tbName;
    bool add;
    ColumnList* attrList;
};

class AlterForeignKey: public Tree {
  public:
  AlterForeignKey(std::string relName, std::string fkName, std::string tbName = "", ColumnList* thisName = nullptr, ColumnList* otherName = nullptr) {
    this->relName = move(relName);
    this->fkName = move(fkName);
    this->tbName = move(tbName);
    this->thisList = thisName;
    this->refList = otherName;
  }

  void visit() {
    int rc = 0;
    if(tbName == "") {
      rc = sm->AlterForeignKey(relName.c_str(), fkName.c_str());
    } else {
      rc = sm->AlterForeignKey(relName.c_str(), fkName.c_str(), tbName.c_str(), thisList->toAttrList(), refList->toAttrList());
    }
    if(rc != 0) {
      std::cout << "Alter Foreign Key failed." << std::endl; 
    }
     
  }
  ~AlterForeignKey() {
    // if(thisList != nullptr) {
    //   delete thisList;
    // }
    // if(refList != nullptr) {
    //   delete refList;
    // }
  }
  std::string relName, fkName, tbName;
  ColumnList* thisList, *refList;
};

}

#endif