#include "ast.h"
#include "systemmanager/sm.h"

namespace parser {
  using std::string;
  using std::move;
  using systemmanager::SM_Manager;

  ShowDatabase::ShowDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void ShowDatabase::visit() {
    // TODO
  }

  CreateDatabase::CreateDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void CreateDatabase::visit() {
    SM_Manager::getInstance()->CreateDb(dbName.c_str());
  }

  DropDatabase::DropDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void DropDatabase::visit() {
    SM_Manager::getInstance()->DropDb(dbName.c_str());
  }

  UseDatabase::UseDatabase(string dbName) {
    this->dbName = move(dbName);
  }

  void UseDatabase::visit() {
    SM_Manager::getInstance()->OpenDb(dbName.c_str());
  }



};