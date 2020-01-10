#include <gtest/gtest.h>
#include <cstring>
#include <fstream>
#include "sm.h"

namespace systemmanager {
  using filesystem::BufPageManager;
  using filesystem::FileManager;
  using indexing::IX_Manager;
  using recordmanager::RM_Manager;
  using std::string;

TEST(TestDatabases, DatabasesOperation) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  RM_Manager* rm = new RM_Manager(fm, bpm);
  IX_Manager* ixm = new IX_Manager(fm, bpm);
  SM_Manager* sm = new SM_Manager(fm, bpm, ixm, rm);

  EXPECT_NE(sm, nullptr);

  string dbName("test");
  sm->CreateDb(dbName.c_str());
  std::ifstream file;
  file.open(dbName + "/TableList");
  EXPECT_EQ(file.good(), true);

  sm->OpenDb(dbName.c_str());
  sm->CloseDb();

  sm->ShowDb(dbName.c_str());
  sm->DropDb(dbName.c_str());
  file.open(dbName + "/TableList");
  EXPECT_EQ(file.good(), false);
}

TEST(TestTables, TableOperations) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  RM_Manager* rm = new RM_Manager(fm, bpm);
  IX_Manager* ixm = new IX_Manager(fm, bpm);
  SM_Manager* sm = new SM_Manager(fm, bpm, ixm, rm);

  string dbName("test");
  sm->CreateDb(dbName.c_str());
  sm->OpenDb(dbName.c_str());

  AttrInfo attr1;
  memcpy(attr1.attrName, "name", std::string("name").size());
  attr1.attrType = STRING;
  attr1.attrLength = 10;
  attr1.notNullFlag = true;
  memcpy(attr1.defaultValue, "NO_NAME", std::string("NO_NAME").size());

  sm->CreateTable("Test", 1, &attr1, 0, nullptr);
  
  

}

} 