#include "ast.hh"
#include "systemmanager/sm.h"

namespace parser {
  using std::string;
  using std::move;

  Tree *Tree::tree = nullptr;

  filesystem::FileManager* fm = new filesystem::FileManager();
  filesystem::BufPageManager* bpm = new filesystem::BufPageManager(fm);
  recordmanager::RM_Manager* rmm = new recordmanager::RM_Manager(fm, bpm);
  indexing::IX_Manager* ixm = new indexing::IX_Manager(fm, bpm);
  systemmanager::SM_Manager* sm = new systemmanager::SM_Manager(fm, bpm, ixm, rmm);
  querylanguage::QL_Manager* qlm = new querylanguage::QL_Manager(rmm, ixm, sm, fm, bpm);

  ShowDatabase::ShowDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  ShowDatabase::ShowDatabase() {
    this->dbName = "";
  }

  void ShowDatabase::visit() {
    int rc = 0;
    if(this->dbName != "")
      rc = sm->ShowDb(dbName.c_str());
    else {
      rc = sm->ShowAllDb();
    }
    if(rc != 0) {
      std::cout << "Show Database " + dbName + " failed." << std::endl;
    }
  }

  CreateDatabase::CreateDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void CreateDatabase::visit() {

    int rc  = sm->CreateDb(dbName.c_str());
    if(rc != 0) {
      std::cout << "Create database " << dbName << " failed." << std::endl;
    }
  }

  DropDatabase::DropDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void DropDatabase::visit() {
    int rc = sm->DropDb(dbName.c_str());
    if(rc != 0) {
      std::cout << "Drop database " << dbName << "failed." << std::endl;
    }
  }

  UseDatabase::UseDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void UseDatabase::visit() {
    int rc = sm->OpenDb(dbName.c_str());
    if(rc != 0) {
      std::cout << "Open database " << dbName << " failed." << std::endl;
    }
  }

  CloseDatabase::CloseDatabase() {

  }

  void CloseDatabase::visit() {
    int rc = sm->CloseDb();
    if(rc != 0) {
      std::cout << "Close database failed." << std::endl; 
    }
  }

  ShowTable::ShowTable(string tbName) {
    this->tbName = move(tbName);
  }

  ShowTable::ShowTable() {
    this->tbName = "";
  }

  void ShowTable::visit() {
    int rc = 0;
    if(this->tbName == "") {
      rc = sm->ShowAllTable();
    } else {
      rc = sm->ShowTable(tbName.c_str());
    }
    if(rc != 0) {
      std::cout << "Show table " << tbName << " failed." << std::endl;
    }
  }

  void ColumnList::add(std::string name) {
    this->attrNameList.push_back(name);
  }

  Field::Field(std::string attrName, AttrType attrType, int attrLength, bool notNull, void* defaultValue) {
    systemmanager::CopyStr(info.attrName, attrName.c_str(), MAX_LENGTH);
    info.attrType = attrType;
    info.attrLength = attrLength;
    info.notNullFlag = notNull;
    if(defaultValue != nullptr)
      systemmanager::CopyStr(info.defaultValue, (char*)defaultValue, attrLength);
  }

  void FieldList::add(Field* field) {
    this->fieldList.push_back(field);
  }

  Constraint::Constraint(bool isPrimary, ColumnList* thisNameList, std::string fkName, std::string tbName, ColumnList* ref) {
    cons.isPrimary = isPrimary;
    systemmanager::CopyStr(cons.constraintName, fkName.c_str(), MAX_LENGTH);
    if(tbName != "")
      systemmanager::CopyStr(cons.foreignTableName, tbName.c_str(), MAX_LENGTH);
    for(int i = 0; i < (int)thisNameList->attrNameList.size(); ++i) {
      systemmanager::CopyStr(cons.thisNameList.names[i], thisNameList->attrNameList[i].c_str(), MAX_LENGTH);
    }
    cons.thisNameList.attrCount = thisNameList->attrNameList.size();
    if(ref != nullptr) {
      for(int i = 0; i < (int)ref->attrNameList.size(); ++i) {
        systemmanager::CopyStr(cons.referencesNameList.names[i], ref->attrNameList[i].c_str(), MAX_LENGTH);
      }
      cons.referencesNameList.attrCount = ref->attrNameList.size();
    }
  }

  void ConstraintList::add(Constraint* cons) {
    constraintList.push_back(cons);
  }

  CreateTable::CreateTable(std::string tbName, FieldList* fieldList, ConstraintList* cons) {
    this->tbName = move(tbName);
    this->fieldList = fieldList;
    this->constraints = cons;
  }

  void CreateTable::visit() {
    int attrCount = fieldList->fieldList.size();
    int consCount = constraints == nullptr? 0 : constraints->constraintList.size();
    systemmanager::AttrInfo *infos = new systemmanager::AttrInfo[attrCount];
    for(int i = 0; i < attrCount; ++i) {
      infos[i] = fieldList->fieldList[i]->info;
    }
    systemmanager::Constraint *cons = new systemmanager::Constraint[consCount];
    for(int i = 0; i < consCount; ++i) {
      cons[i] = constraints->constraintList[i]->cons;
    }
    int rc = sm->CreateTable(tbName.c_str(), attrCount, infos, consCount, cons);
    if(rc != 0) {
      std::cout << "Create table " << tbName << " failed." << std::endl;
    }

  }

  CreateTable::~CreateTable() {
    if(fieldList != nullptr) {
      delete fieldList;
    }
    if(constraints != nullptr) {
      delete constraints;
    }
  }

  DropTable::DropTable(std::string tbName) {
    this->tbName = move(tbName);
  }

  void DropTable::visit() {
    int rc = sm->DropTable(tbName.c_str());
    if(rc != 0) {
      std::cout << "Drop table " << tbName << " failed." << std::endl;
    }
  }



void Insert::visit() {
  std::cout << "Insert visit" << std::endl;
  for (ValueList* valuelist : this->valuelists->valuelists) {
    int nValues = valuelist->values.size();
    querylanguage::Value values[nValues];
    for (int i = 0; i < nValues; i++) {
      querylanguage::Value* tmp = valuelist->values[i];
      values[i].type = tmp->type;
      values[i].data = tmp->data;
    }
    qlm->Insert(this->tbname.c_str(), nValues, values);
  }
}

void Delete::visit() {
  std::cout << "Delete visit" << std::endl;
  int n = this->whereclause->conditions.size();
  querylanguage::Condition conditions[n];
  for (int i = 0; i < n; i++) {
    conditions[i] = *this->whereclause->conditions[i];
  }
  qlm->Delete(this->tbname.c_str(), n, conditions);
}

void Update::visit() {
  for (auto& tmp : this->setclause->list) {
    char* colname = tmp.first;
    querylanguage::Value value = *tmp.second;
    querylanguage::RelAttr updAttr;
    updAttr.relName = NULL;
    updAttr.attrName = colname;
    querylanguage::RelAttr rhsRelAttr;
    querylanguage::Condition conditions[1];
    qlm->Update(this->tbname, updAttr, 1, rhsRelAttr, value, 0, conditions);
  }
}


};