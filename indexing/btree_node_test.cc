#include <gtest/gtest.h>
#include <cstring>
#include "btree_node.h"

namespace indexing {

using filesystem::BufPageManager;
using filesystem::FileManager;
using recordmanager::RID;
using std::string;

TEST(TestBTreeNode, Construct) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode intNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0, true);
  EXPECT_EQ(intNode.GetMaxKeys(), 681);
  intNode.Destroy();

  BTreeNode floatNode = BTreeNode(FLOAT, sizeof(float), bpm, fileID, 1, true);
  EXPECT_EQ(floatNode.GetMaxKeys(), 681);
  floatNode.Destroy();

  BTreeNode stringNode = BTreeNode(STRING, sizeof(char[10]), bpm, fileID, 2, true);
  EXPECT_EQ(stringNode.GetMaxKeys(), 454);
  stringNode.Destroy();

  bpm->close();
}

TEST(TestBTreeNode, KeyOperation) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode testNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0);
  for(int i = 0; i < 10; ++i) {
    testNode.Insert(&i, RID());
    void* key = nullptr;
    testNode.GetKey(i, key);
    EXPECT_EQ(i, *(int*)key);
  }

  for(int i = 0; i < 10; ++i) {
    int new_key = i + 1;
    testNode.SetKey(i, (const void*)&new_key);
    void* key = nullptr;
    testNode.GetKey(i, key);
    EXPECT_EQ(i + 1, *(int*)key);
  }

  BTreeNode testStringNode = BTreeNode(STRING, sizeof(char[10]), bpm, fileID, 1);
  for (int i = 0; i < 10; i++) {
    char buf[10];
    sprintf(buf, "test %d", i);
    testStringNode.Insert(buf, RID());
    const char * pbuf = nullptr;
    testStringNode.GetKey(i, (void*&)pbuf);
    ASSERT_STREQ(buf, pbuf);
  }

  bpm->close();
}

TEST(TestBTreeNode, InsertAndRemove) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode testNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0);
  for(int i = 0; i < 681; ++i) {
    testNode.Insert(&i, RID());
    void* key = nullptr;
    testNode.GetKey(i, key);
    EXPECT_EQ(i, *(int*)key);
  }
  int max_index = 681;
  int ret = testNode.Insert(&max_index, RID());
  EXPECT_EQ(ret, -1);

  EXPECT_EQ(testNode.GetNumKeys(), 681);

  int deleted_key = 89; 
  testNode.Remove((const void*)&deleted_key);
  EXPECT_EQ(testNode.GetNumKeys(), 680);
  testNode.Insert(&deleted_key, RID());

  deleted_key = 0; 
  testNode.Remove((const void*)&deleted_key);
  EXPECT_EQ(testNode.GetNumKeys(), 680);
  testNode.Insert(&deleted_key, RID());

  deleted_key = 680; 
  testNode.Remove((const void*)&deleted_key);
  EXPECT_EQ(testNode.GetNumKeys(), 680);
  testNode.Insert(&deleted_key, RID());

  deleted_key = 1000; 
  ret = testNode.Remove((const void*)&deleted_key);
  EXPECT_EQ(ret, -1);

  testNode.RangeRemove(10, 20);
  EXPECT_EQ(testNode.GetNumKeys(), 670);

  bpm->close();
}

TEST(TestBTreeNode, Find) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode testNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0);
  for(int i = 0; i < 100; ++i) {
    testNode.Insert(&i, RID(0, i));
  }
  int dupKey = 99;
  for(int i = 100; i < 200; ++i) {
    testNode.Insert(&dupKey, RID(0, i));
  }

  int* queryKey = new int(-1);
  int ret = testNode.FindKey((const void* &)queryKey);
  EXPECT_EQ(ret, -1);

  *queryKey = 8;
  ret = testNode.FindKey((const void* &)queryKey);
  EXPECT_EQ(ret, 8);
  ret = testNode.FindKey((const void* &)queryKey, RID(0, 10));
  EXPECT_EQ(ret, -1);
  ret = testNode.FindKey((const void* &)queryKey, RID(0, 8));
  EXPECT_EQ(ret, 8);

  *queryKey = 99;
  ret = testNode.FindKey((const void* &)queryKey);
  EXPECT_EQ(ret, 199);
  ret = testNode.FindKey((const void* &)queryKey, RID(0, 100));
  EXPECT_EQ(ret, 100);
  ret = testNode.FindKey((const void* &)queryKey, RID(0, 8));
  EXPECT_EQ(ret, -1);
  
  RID r;
  testNode.GetAddrByKey((const void* &)queryKey, r);
  EXPECT_EQ(r, RID(0, 199));
  testNode.GetAddrByPosition(8, r);
  EXPECT_EQ(r, RID(0, 8));

  int pos = testNode.FindPositionByKey((const void* &)queryKey);
  testNode.FindAddrByKey((const void* &)queryKey, r);
  EXPECT_EQ(pos, 199);
  EXPECT_EQ(r, RID(0, 199));
  *queryKey = -1;
  pos = testNode.FindPositionByKey((const void* &)queryKey);
  testNode.FindAddrByKey((const void* &)queryKey, r);
  EXPECT_EQ(pos, 0);
  EXPECT_EQ(r, RID(0, 0));
  bpm->close();
}

TEST(TestBTreeNode, SetGet) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode testNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0);
  EXPECT_EQ(testNode.GetLeft(), -1);
  EXPECT_EQ(testNode.GetRight(), -1);
  EXPECT_EQ(testNode.GetNumKeys(), 0);
  testNode.SetLeft(10);
  testNode.SetRight(11);
  testNode.SetNumKeys(100);
  EXPECT_EQ(testNode.GetLeft(), 10);
  EXPECT_EQ(testNode.GetRight(), 11);
  EXPECT_EQ(testNode.GetNumKeys(), 100);
  EXPECT_EQ(testNode.GetPageNum(), 0);

  BTreeNode copyNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0, false);
  EXPECT_EQ(testNode.GetLeft(), 10);
  EXPECT_EQ(testNode.GetRight(), 11);
  EXPECT_EQ(testNode.GetNumKeys(), 100);
}

TEST(TestBTreeNode, SplitAndMerge) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);

  string name = "testfile";
  fm->createFile(name.c_str());
  int fileID;
  fm->openFile(name.c_str(), fileID);

  BTreeNode testNode = BTreeNode(INT, sizeof(int), bpm, fileID, 0);
  for(int i = 0; i < 20; ++i) {
    testNode.Insert(&i, RID());
  }
  BTreeNode otherNode(INT, sizeof(int), bpm, fileID, 1);
  int ret = testNode.Split(&otherNode);
  EXPECT_EQ(ret, 0);
  EXPECT_EQ(testNode.GetNumKeys(), 10);
  EXPECT_EQ(otherNode.GetNumKeys(), 10);
  EXPECT_EQ(testNode.GetRight(), 1);
  EXPECT_EQ(otherNode.GetLeft(), 0);

  for(int i = 0; i < 10; ++i) {
    int* key = nullptr;
    otherNode.GetKey(i, (void* &)key);
    EXPECT_EQ(*key, i + 10);
  }

  testNode.Merge(&otherNode);
  EXPECT_EQ(testNode.GetNumKeys(), 20);
  EXPECT_EQ(otherNode.GetNumKeys(), 0);
  EXPECT_EQ(testNode.GetRight(), -1);

  for(int i = 0; i < 20; ++i) {
    int* key = nullptr;
    testNode.GetKey(i, (void* &)key);
    EXPECT_EQ(*key, i);
  }
}

}