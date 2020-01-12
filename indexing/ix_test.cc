#include <gtest/gtest.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include "ix.h"

namespace indexing {
  using filesystem::BufPageManager;
  using filesystem::FileManager;
  using recordmanager::RID;
  using std::string;

TEST(TestManager, Manager) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  IX_Manager manager(fm, bpm);
  int ret = manager.CreateIndex(name.c_str(), 0, INT, sizeof(int));
  EXPECT_EQ(ret, 0);

  IX_IndexHandle handle;
  ret = manager.OpendIndex(name.c_str(), 0, handle);
  EXPECT_EQ(ret, 0);
  EXPECT_EQ(handle.GetAttrType(), INT);
  EXPECT_EQ(handle.GetAttrLength(), sizeof(int));
  EXPECT_EQ(handle.GetHeight(), 1);
  EXPECT_EQ(handle.GetNumPages(), 2);

  ret = manager.CloseIndex(handle);
  EXPECT_EQ(ret, 0);
  ret = manager.OpendIndex(name.c_str(), 0, handle);
  EXPECT_EQ(ret, 0);
  EXPECT_EQ(handle.GetAttrType(), INT);
  EXPECT_EQ(handle.GetAttrLength(), sizeof(int));
  EXPECT_EQ(handle.GetHeight(), 1);
  EXPECT_EQ(handle.GetNumPages(), 2);

  ret = manager.DestroyIndex(name.c_str(), 0);
  
  EXPECT_EQ(ret, 0);
  std::stringstream s;
  s << name << "." << 0;
  std::ifstream file;
  file.open(s.str().c_str());
  EXPECT_EQ(file.good(), false);
  std::cout << "index test" <<std::endl;
} 

TEST(TestHandle, Handle) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  IX_Manager manager(fm, bpm);
  int ret = manager.CreateIndex(name.c_str(), 0, INT, sizeof(int));
  EXPECT_EQ(ret, 0);

  IX_IndexHandle handle;
  ret = manager.OpendIndex(name.c_str(), 0, handle);

  for(int i = 0; i < 682; ++i) {
    RID r = RID(i, i);
    handle.InsertEntry(&i, r);
  }
  EXPECT_EQ(handle.GetHeight(), 2);
  EXPECT_EQ(handle.GetNumPages(), 4);

  for(int i = 10000; i > 681; --i) {
    RID r = RID(i, i);
    handle.InsertEntry(&i, r);
  }

  EXPECT_EQ(handle.GetHeight(), 2);
  EXPECT_EQ(handle.GetNumPages(), 31);

  for(int i = 0; i < 10001; ++i) {
    RID r = RID(i, i);
    handle.DeleteEnrty(&i, r);
  }

  EXPECT_EQ(handle.GetHeight(), 1);
  EXPECT_EQ(handle.GetNumPages(), 2);

  int duplicateKey = 0;
  for(int i = 0; i < 10001; ++i) {
    RID r(i, i);
    handle.InsertEntry(&duplicateKey, r);
  }
  EXPECT_EQ(handle.GetHeight(), 2);
  EXPECT_EQ(handle.GetNumPages(), 31);

  // for(int i = 0; i < 10001; ++i) {
  //   RID r = RID(i, i);
  //   handle.DeleteEnrty(&duplicateKey, r);
  // }

  // EXPECT_EQ(handle.GetHeight(), 2);
  // EXPECT_EQ(handle.GetNumPages(), 30); // duplicate keys underflow not dealed
}

TEST(TestScan, Scan) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  IX_Manager manager(fm, bpm);
  int ret = manager.CreateIndex(name.c_str(), 0, INT, sizeof(int));
  EXPECT_EQ(ret, 0);

  IX_IndexHandle handle;
  ret = manager.OpendIndex(name.c_str(), 0, handle);
  for(int i = 0; i < 10001; ++i) {
    RID r(i, i);
    handle.InsertEntry(&i, r);
  }

  IX_IndexScan scan;
  RID r;
  std::function<bool(void*, void*)> checkFunc = scan.GetCheckFunction(STRING, 10, LT_OP);
  string test1 = "hello";
  string test2 = "hehe";
  bool res = checkFunc((void*)test1.c_str(), (void*)test2.c_str());
  EXPECT_EQ(res, false);

  int* queryKey = new int(1000);

  scan.OpenScan(handle, LT_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  int i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1000);
  scan.CloseScan();

  scan.OpenScan(handle, GE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(i + 1000, i + 1000));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9001);
  scan.CloseScan();

  scan.OpenScan(handle, GT_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(i + 1001, i + 1001));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9000);
  scan.CloseScan();

  scan.OpenScan(handle, LE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1001);
  scan.CloseScan();

  scan.OpenScan(handle, EQ_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(*queryKey, *queryKey));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1);
  scan.CloseScan();

  scan.OpenScan(handle, NE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ((r == RID(*queryKey, *queryKey)), false);
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10000);
  scan.CloseScan();

  scan.OpenScan(handle, LT_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    // EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1000);
  scan.CloseScan();

  scan.OpenScan(handle, GE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1000, i + 1000));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9001);
  scan.CloseScan();

  scan.OpenScan(handle, GT_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1001, i + 1001));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9000);
  scan.CloseScan();

  scan.OpenScan(handle, LE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1001);
  scan.CloseScan();

  scan.OpenScan(handle, EQ_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ(r, RID(*queryKey, *queryKey));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1);
  scan.CloseScan();

  scan.OpenScan(handle, NE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    EXPECT_EQ((r == RID(*queryKey, *queryKey)), false);
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10000);
  scan.CloseScan();

  
  for(int i = 0; i < 1000; ++i) {
    RID insertRid(i + 10001, i + 10001);
    handle.InsertEntry(queryKey, insertRid);
  }

  scan.OpenScan(handle, LT_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    // EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1000);
  scan.CloseScan();

  scan.OpenScan(handle, GE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1000, i + 1000));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10001);
  scan.CloseScan();

  scan.OpenScan(handle, GT_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1001, i + 1001));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9000);
  scan.CloseScan();

  scan.OpenScan(handle, LE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 2001);
  scan.CloseScan();

  scan.OpenScan(handle, EQ_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(*queryKey, *queryKey));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1001);
  scan.CloseScan();

  scan.OpenScan(handle, NE_OP, queryKey, false);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ((r == RID(*queryKey, *queryKey)), false);
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10000);
  scan.CloseScan();

  scan.OpenScan(handle, LT_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    // EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1000);
  scan.CloseScan();

  scan.OpenScan(handle, GE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1000, i + 1000));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10001);
  scan.CloseScan();

  scan.OpenScan(handle, GT_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i + 1001, i + 1001));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 9000);
  scan.CloseScan();

  scan.OpenScan(handle, LE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(i, i));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 2001);
  scan.CloseScan();

  scan.OpenScan(handle, EQ_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ(r, RID(*queryKey, *queryKey));
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 1001);
  scan.CloseScan();

  scan.OpenScan(handle, NE_OP, queryKey, true);
  ret = scan.GetNextEntry(r);
  i = 0;
  while(ret == 0) {
    //EXPECT_EQ((r == RID(*queryKey, *queryKey)), false);
    ++i;
    ret = scan.GetNextEntry(r);
  }
  EXPECT_EQ(i, 10000);
  scan.CloseScan();
  
  
}

}