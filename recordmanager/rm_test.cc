#include <cstring>

#include "../return_code.h"
#include "gtest/gtest.h"
#include "rm.h"

namespace recordmanager {

using filesystem::BufPageManager;
using filesystem::FileManager;

TEST(TestPrintError, SimpleTest) {
  std::cout << "The following error is a test:" << std::endl;
  RM_PrintError(1);
}

TEST(TestRMFileHandle, SimpleTest) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  std::string file_name = "test.txt";
  fm->createFile(file_name.c_str());
  int file_id;
  fm->openFile(file_name.c_str(), file_id);

  RM_FileHandle file_handle;
  file_handle.Init(file_name, 10, fm, bpm, file_id);
  char a[] = "abcdefghij";

  RID rid;
  EXPECT_EQ(file_handle.InsertRecord(a, rid), NO_ERROR);
  int page_num, slot_num;
  EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
  EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
  EXPECT_EQ(page_num, 1);
  EXPECT_EQ(slot_num, 0);

  RM_Record record;
  EXPECT_EQ(file_handle.GetRecord(rid, record), NO_ERROR);
  char* data;
  EXPECT_EQ(record.GetData(data), NO_ERROR);
  EXPECT_EQ(std::memcmp(a, data, 10), 0);

  for (int i = 0; i < 31; i++) {
    EXPECT_EQ(file_handle.InsertRecord(a, rid), NO_ERROR);
    EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
    EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
  }

  EXPECT_EQ(file_handle.InsertRecord(a, rid), NO_ERROR);
  EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
  EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
  EXPECT_EQ(page_num, 2);
  EXPECT_EQ(slot_num, 0);

  EXPECT_EQ(file_handle.GetRecord(rid, record), NO_ERROR);
  EXPECT_EQ(record.GetData(data), NO_ERROR);
  EXPECT_EQ(std::memcmp(a, data, 10), 0);

  char b[] = "1234567890";
  rid.Set(1, 17);
  record.Set(10, b, rid);
  file_handle.UpdateRecord(record);
  RID rid2(1, 17);
  RM_Record record2;
  EXPECT_EQ(file_handle.GetRecord(rid2, record2), NO_ERROR);
  EXPECT_EQ(record2.GetData(data), NO_ERROR);
  EXPECT_EQ(std::memcmp(b, data, 10), 0);

  rid.Set(1, 20);
  file_handle.DeleteRecord(rid);
  EXPECT_EQ(file_handle.GetRecord(rid, record),
            RM_FILEHANDLE_RECORD_NOT_FOUND_ERROR);

  EXPECT_EQ(file_handle.InsertRecord(b, rid2), NO_ERROR);
  EXPECT_EQ(rid2.GetPageNum(page_num), NO_ERROR);
  EXPECT_EQ(rid2.GetSlotNum(slot_num), NO_ERROR);
  EXPECT_EQ(page_num, 1);
  EXPECT_EQ(slot_num, 20);
  EXPECT_EQ(file_handle.GetRecord(rid, record2), NO_ERROR);
  EXPECT_EQ(record2.GetData(data), NO_ERROR);
  EXPECT_EQ(std::memcmp(b, data, 10), 0);

  bpm->close();
}

TEST(TestRMFileHandle, TestMoreData) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  std::string file_name = "test.txt";
  fm->createFile(file_name.c_str());
  int file_id;
  fm->openFile(file_name.c_str(), file_id);

  RM_FileHandle file_handle;
  file_handle.Init(file_name, 200, fm, bpm, file_id);

  for (int i = 0; i < 1000; i++) {
    int a[50];
    for (int j = 0; j < 50; j++) a[j] = i;
    RID rid;
    EXPECT_EQ(file_handle.InsertRecord((char*)a, rid), 0);
    int page_num, slot_num;
    EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
    EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
    // std::cout << page_num << ' ' << slot_num << std::endl;
  }
  EXPECT_EQ(file_handle.GetMaxPageNum(), 32);
  RID rid(5, 10);  // 4 * 32 + 10 = 138
  RM_Record record;
  EXPECT_EQ(file_handle.GetRecord(rid, record), NO_ERROR);
  char* data;
  EXPECT_EQ(record.GetData(data), NO_ERROR);
  EXPECT_EQ(*((int*)data), 138);

  EXPECT_EQ(file_handle.GetNextNotEmptySlot(1, -1), 0);
  for (int i = 0; i < 10; i++) {
    RID rid(1, i);
    file_handle.DeleteRecord(rid);
  }
  EXPECT_EQ(file_handle.GetNextNotEmptySlot(1, -1), 10);
  for (int i = 10; i < 32; i++) {
    RID rid(1, i);
    file_handle.DeleteRecord(rid);
  }
  EXPECT_EQ(file_handle.GetNextNotEmptySlot(1, -1), 32);

  EXPECT_EQ(file_handle.GetFileID(), file_id);
}

TEST(TestRMManager, SimpleTest) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  RM_Manager rm_manager(fm, bpm);
  RM_FileHandle file_handle;
  std::string file_name = "test.txt";
  EXPECT_EQ(rm_manager.OpenFile(file_name, file_handle),
            RM_MANAGER_FILE_NOT_FOUND_ERROR);
  EXPECT_EQ(rm_manager.CreateFile(file_name, 100), NO_ERROR);
  EXPECT_EQ(rm_manager.OpenFile(file_name, file_handle), NO_ERROR);
  EXPECT_EQ(rm_manager.CloseFile(file_handle), NO_ERROR);
  EXPECT_EQ(rm_manager.DestroyFile(file_name), NO_ERROR);
}

TEST(TestRMFileScan, SimpleTest) {
  FileManager* fm = new FileManager();
  BufPageManager* bpm = new BufPageManager(fm);
  RM_Manager manager(fm, bpm);
  RM_FileHandle file_handle;
  std::string file_name = "test.txt";
  EXPECT_EQ(manager.CreateFile(file_name, 100), NO_ERROR);
  EXPECT_EQ(manager.OpenFile(file_name, file_handle), NO_ERROR);
  for (int i = 0; i < 1000; i++) {
    int a[25];
    for (int j = 0; j < 25; j++) {
      if (i % 2 == 0)
        a[j] = i;
      else
        a[j] = -i;
    }
    RID rid;
    EXPECT_EQ(file_handle.InsertRecord((char*)a, rid), NO_ERROR);
    int page_num, slot_num;
    EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
    EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
    // std::cout << page_num << ' ' << slot_num << std::endl;
  }
  RM_FileScan file_scan;
  int value = 0;
  EXPECT_EQ(file_scan.OpenScan(&file_handle, AttrType::INT, 4, 24,
                               CompOp::GT_OP, &value),
            NO_ERROR);
  RM_Record record;
  int x = 2;
  while (file_scan.GetNextRecord(record) != EOF) {
    char* data;
    EXPECT_EQ(record.GetData(data), NO_ERROR);
    EXPECT_EQ(*((int*)data), x);
    x += 2;
  }
  EXPECT_EQ(file_scan.CloseScan(), NO_ERROR);
  EXPECT_EQ(file_scan.OpenScan(&file_handle, AttrType::INT, 4, 96,
                               CompOp::LE_OP, &value),
            NO_ERROR);
  x = 0;
  while (file_scan.GetNextRecord(record) != EOF) {
    char* data;
    EXPECT_EQ(record.GetData(data), NO_ERROR);
    EXPECT_EQ(*((int*)data), x);
    if (x == 0)
      x = -1;
    else
      x -= 2;
  }
}

}  // namespace recordmanager