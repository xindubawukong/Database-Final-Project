#include <cstring>
#include <fstream>
#include <assert.h>
#include "sm.h"

using filesystem::BufPageManager;
using filesystem::FileManager;
using indexing::IX_Manager;
using recordmanager::RM_Manager;
using std::string;
using namespace systemmanager;
using namespace filesystem;

  


int main() {
 // chdir("/mnt/d/learn/senior/database/Test");
  MyBitMap::initConst();  //新加的初始化
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  RM_Manager* rm = new RM_Manager(fm, bpm);
  IX_Manager* ixm = new IX_Manager(fm, bpm);
  SM_Manager* sm = new SM_Manager(fm, bpm, ixm, rm);

  string dbName("NewTest");
  int rc = sm->CreateDb(dbName.c_str());
  sm->OpenDb(dbName.c_str());

  AttrInfo infos[10]; //= new AttrInfo[10];
  AttrInfo attr1;
  CopyStr(attr1.attrName, "name", MAX_LENGTH);
  attr1.attrType = AttrType::STRING;
  attr1.attrLength = 10;
  attr1.notNullFlag = true;
  CopyStr(attr1.defaultValue, "NO_NAME", MAX_LENGTH);
  infos[0] = attr1;

  CopyStr(infos[1].attrName, "age", MAX_LENGTH);
  infos[1].attrType = AttrType::INT;
  infos[1].attrLength = 4;
  infos[1].notNullFlag = true;
  int value = 18;
  memcpy(infos[1].defaultValue, &value, sizeof(int));

  CopyStr(infos[2].attrName, "salary", MAX_LENGTH);
  infos[2].attrType = AttrType::FLOAT;
  infos[2].attrLength = 4;
  infos[2].notNullFlag = false;
  float fvalue = 20000.123;
  memcpy(infos[2].defaultValue, &fvalue, sizeof(float));

  AttrList *attrs = new AttrList[10];
  attrs[0].attrCount = 1;
  CopyStr(attrs[0].names[0], "name", MAX_LENGTH);

  attrs[1].attrCount = 1;
  CopyStr(attrs[1].names[0], "age", MAX_LENGTH);

  attrs[2].attrCount = 1;
  CopyStr(attrs[2].names[0], "salary", MAX_LENGTH);

  attrs[3].attrCount = 2;
  CopyStr(attrs[3].names[0], "name", MAX_LENGTH);
  CopyStr(attrs[3].names[1], "age", MAX_LENGTH);

  attrs[4].attrCount = 2;
  CopyStr(attrs[4].names[0], "age", MAX_LENGTH);
  CopyStr(attrs[4].names[1], "salary", MAX_LENGTH);

  attrs[5].attrCount = 2;
  CopyStr(attrs[5].names[0], "salary", MAX_LENGTH);
  CopyStr(attrs[5].names[1], "name", MAX_LENGTH);

  attrs[6].attrCount = 3;
  CopyStr(attrs[6].names[0], "name", MAX_LENGTH);
  CopyStr(attrs[6].names[1], "age", MAX_LENGTH);
  CopyStr(attrs[6].names[2], "salary", MAX_LENGTH);

  Constraint *cons = new Constraint[10];
  CopyStr(cons[0].constraintName, "PrimaryKey", MAX_LENGTH);
  cons[0].isPrimary = true;
  cons[0].thisNameList = attrs[0];

  CopyStr(cons[1].constraintName, "fk1", MAX_LENGTH);
  cons[1].isPrimary = false;
  cons[1].thisNameList = attrs[0];
  CopyStr(cons[1].foreignTableName, "ATest", MAX_LENGTH);
  cons[1].referencesNameList = attrs[0];


  rc = sm->CreateTable("ATest", 1, infos, 1, cons);
  assert(rc == 0);
  rc = sm->AlterPrimaryKey("ATest", true, &attrs[1]);
  assert(rc == -1);
  rc = sm->AlterPrimaryKey("ATest", true, &attrs[0]);
  assert(rc == -1);
  rc = sm->ShowTable("ATest");

  
  rc = sm->CreateTable("BTest", 2, infos, 1, &cons[1]);
  assert(rc == 0);
  rc = sm->AlterPrimaryKey("ATest");
  assert(rc == 0);
  rc = sm->CreateTable("CTest", 3, infos, 1, &cons[1]);
  assert(rc == -1);
  rc = sm->CreateTable("ATest", 1, infos, 0, nullptr);


  // sm->CloseDb();
  // sm->ShowDb("NewTest");
  // sm->ShowAllDb();
  // sm->OpenDb("NewTest");
  // bpm->close();
  sm->ShowAllTable();
  // rc = sm->DropTable("ATest");
  // std::cout << "Drop Table: " << rc << std::endl;
  sm->CloseDb();
  sm->ShowAllDb();
  

  return 0;
}