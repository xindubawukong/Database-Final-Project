#include <iostream>
#include <vector>
#include <string>

#include "ql.h"
#include "recordmanager/rm.h"
#include "indexing/ix.h"
#include "systemmanager/sm.h"

using std::cout;
using std::endl;
using std::string;
using systemmanager::CopyStr;

int main() {
  
  filesystem::MyBitMap::initConst();

  // chdir("/Users/xdbwk/Desktop/thu41/database/test_dir");

  auto fm = new filesystem::FileManager();
  auto bpm = new filesystem::BufPageManager(fm);

  auto rmm = new recordmanager::RM_Manager(fm, bpm);
  auto ixm = new indexing::IX_Manager(fm, bpm);
  auto smm = new systemmanager::SM_Manager(fm, bpm, ixm, rmm);
  auto qlm = new querylanguage::QL_Manager(rmm, ixm, smm, fm, bpm);

  int rc;

  string db1 = "database_1";
  rc = smm->CreateDb(db1.c_str());
  cout << "CreateDb(db1): " << rc << endl;
  rc = smm->OpenDb(db1.c_str());
  cout << "OpenDb(db1): " << rc << endl;

  string tb1 = "table_1";

  systemmanager::AttrInfo attr_infos[3];

  // systemmanager::AttrInfo* attr_infos = new systemmanager::AttrInfo[3];

  CopyStr(attr_infos[0].attrName, "name", MAX_LENGTH);
  attr_infos[0].attrType = AttrType::STRING;
  attr_infos[0].attrLength = 10;
  // attr_infos[0].notNullFlag = true;
  // std::memset(attr_infos[0].defaultValue, 0, MAX_LENGTH);

  CopyStr(attr_infos[1].attrName, "age", MAX_LENGTH);
  attr_infos[1].attrType = AttrType::INT;
  attr_infos[1].attrLength = 4;
  // attr_infos[1].notNullFlag = true;
  // std::memset(attr_infos[1].defaultValue, 0, MAX_LENGTH);

  CopyStr(attr_infos[2].attrName, "score", MAX_LENGTH);
  attr_infos[2].attrType = AttrType::FLOAT;
  attr_infos[2].attrLength = 4;
  // attr_infos[2].notNullFlag = true;
  // std::memset(attr_infos[2].defaultValue, 0, MAX_LENGTH);

  for (int i = 0; i < 3; i++) systemmanager::Print(attr_infos[i]);

  rc = smm->CreateTable(tb1.c_str(), 3, attr_infos, 0, NULL);
  cout << "CreateTable(tb1): " << rc << endl;

  querylanguage::Value values[3];
  values[0].type = AttrType::STRING;
  values[0].data = new char[11];
  CopyStr((char*)values[0].data, "dxy", 10);
  values[1].type = AttrType::INT;
  values[1].data = new int(21);
  values[2].type = AttrType::FLOAT;
  values[2].data = new float(100);

  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  rc = qlm->Delete(tb1.c_str(), 0, NULL);
  cout << "Delete(tb1): " << rc << endl;

  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  values[0].data = new char[11];
  CopyStr((char*)values[0].data, "yyf", 10);
  values[1].data = new int(21);
  values[2].data = new float(90);
  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  querylanguage::Condition conditions[1];
  conditions[0].bRhsIsAttr = false;
  conditions[0].lhsAttr.relName = NULL;
  conditions[0].lhsAttr.attrName = "age";
  conditions[0].op = CompOp::GT_OP;
  conditions[0].rhsValue.type = AttrType::INT;
  conditions[0].rhsValue.data = new int(30);
  rc = qlm->Delete(tb1.c_str(), 1, conditions);
  cout << "Delete(tb1 >= 30): " << rc << endl;

  conditions[0].bRhsIsAttr = false;
  conditions[0].lhsAttr.relName = NULL;
  conditions[0].lhsAttr.attrName = "age";
  conditions[0].op = CompOp::GT_OP;
  conditions[0].rhsValue.type = AttrType::INT;
  conditions[0].rhsValue.data = new int(10);
  rc = qlm->Delete(tb1.c_str(), 1, conditions);
  cout << "Delete(tb1 >= 10): " << rc << endl;

  values[0].data = new char[11];
  CopyStr((char*)values[0].data, "yyf", 10);
  values[1].data = new int(21);
  values[2].data = new float(90);
  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  values[0].data = new char[11];
  CopyStr((char*)values[0].data, "dxy", 10);
  values[1].data = new int(21);
  values[2].data = new float(100);
  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  values[0].data = new char[11];
  CopyStr((char*)values[0].data, "zxh", 10);
  values[1].data = new int(99);
  values[2].data = new float(999);
  rc = qlm->Insert(tb1.c_str(), 3, values);
  cout << "Insert(tb1): " << rc << endl;

  querylanguage::RelAttr selAttrs[1];
  selAttrs[0].relName = NULL;
  selAttrs[0].attrName = "name";
  char* relations[1] = {"table_1"};

  rc = qlm->Select(1, selAttrs, 1, relations, 0, conditions);
  cout << "Select name from tb1: " << rc << endl;

  conditions[0].bRhsIsAttr = false;
  conditions[0].lhsAttr.relName = NULL;
  conditions[0].lhsAttr.attrName = "score";
  conditions[0].op = CompOp::GE_OP;
  conditions[0].rhsValue.type = AttrType::FLOAT;
  conditions[0].rhsValue.data = new float(95);
  rc = qlm->Select(1, selAttrs, 1, relations, 1, conditions);
  cout << "Select name from tb1 where score>=95: " << rc << endl;

  return 0;
}