#include <cstring>

#include "rm.h"
#include "gtest/gtest.h"
#include "../return_code.h"

namespace recordmanager {

using filesystem::FileManager;
using filesystem::BufPageManager;

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
  int record_size = 10;
  file_handle.Init(file_name, record_size, fm, bpm, file_id);
  char a[] = "abcdefghij";

  RID rid;
  EXPECT_EQ(file_handle.InsertRecord(a, rid), NO_ERROR);
  int page_num, slot_num;
  EXPECT_EQ(rid.GetPageNum(page_num), NO_ERROR);
  EXPECT_EQ(rid.GetSlotNum(slot_num), NO_ERROR);
  std::cout << "page_num: " << page_num << std::endl;
  std::cout << "slot_num: " << slot_num << std::endl;
  RM_Record record;
  EXPECT_EQ(file_handle.GetRecord(rid, record), NO_ERROR);
  char* data;
  EXPECT_EQ(record.GetData(data), NO_ERROR);
  EXPECT_EQ(std::memcmp(a, data, record_size), 0);
  bpm->close();
}

}  // namespace recordmanager