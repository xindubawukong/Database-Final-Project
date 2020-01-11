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



};