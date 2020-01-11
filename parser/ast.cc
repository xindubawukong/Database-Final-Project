#include "ast.h"
#include "systemmanager/sm.h"

namespace parser {
  using std::string;
  using std::move;
  using systemmanager::SM_Manager;

  Tree *Tree::tree = nullptr;

  ShowDatabase::ShowDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void ShowDatabase::visit() {
    systemmanager::sm->ShowDb(dbName.c_str());
  }

  CreateDatabase::CreateDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void CreateDatabase::visit() {
  }

  DropDatabase::DropDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void DropDatabase::visit() {
    systemmanager::sm->DropDb(dbName.c_str());
  }

  UseDatabase::UseDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void UseDatabase::visit() {
    systemmanager::sm->OpenDb(dbName.c_str());
  }



};