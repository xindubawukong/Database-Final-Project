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

  void ShowDatabase::visit() {
    sm->ShowDb(dbName.c_str());
  }

  CreateDatabase::CreateDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void CreateDatabase::visit() {
    sm->CreateDb(dbName.c_str());
  }

  DropDatabase::DropDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void DropDatabase::visit() {
    sm->DropDb(dbName.c_str());
  }

  UseDatabase::UseDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void UseDatabase::visit() {
    sm->OpenDb(dbName.c_str());
  }


void Insert::visit() {
  std::cout << "Insert visit" << std::endl;
  for (ValueList* valuelist : this->valuelists->valuelists) {
    int nValues = valuelist->values.size();
    querylanguage::Value values[nValues];
    for (int i = 0; i < nValues; i++) {
      auto& tmp = valuelist->values[i];
      values[i].type = tmp.type;
      if (tmp.is_null_val) values[i].data = NULL;
      else if (tmp.type == AttrType::INT) values[i].data = &tmp.int_val;
      else if (tmp.type == AttrType::FLOAT) values[i].data = &tmp.float_val;
      else values[i].data = (void*)tmp.string_val.c_str();
    }
    qlm->Insert(this->tbname.c_str(), nValues, values);
  }
}


};