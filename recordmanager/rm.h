#ifndef RECORDMANAGER_RM_H
#define RECORDMANAGER_RM_H

#include <functional>
#include <string>
#include <unordered_map>

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "global.h"
#include "rm_record.h"
#include "rm_rid.h"

namespace recordmanager {

void RM_PrintError(int rc);

//
// The first page (page 0), bytes from 4096 to 8191 is a BitMap, indicating
// wheather the 4096*8=32768 pages are full or not.
//
// From page 1, all pages are used to store records. In each of these pages,
// bytes from 0 to 31 is a BitMap, indicating wheather the 32 record slots are
// available. Bytes from 192 to 8191 are 32 record slots, and each record has
// 250 bytes memory. The 'record_size' must not be larger than 250.
//
class RM_FileHandle {
 public:
  RM_FileHandle();

  ~RM_FileHandle();

  // All size or position are in bytes.
  static const int kRecordSizePosition;
  static const int kPageBitMapStartPosition;
  static const int kMaxPageNum;
  static const int kBitMapStartPosition;
  static const int kBitMapLength;
  static const int kRecordStartPosition;
  static const int kRecordNumEachPage;
  static const int kRecordMaxLength;

  void Init(const std::string& file_name, int record_size,
            filesystem::FileManager* fm, filesystem::BufPageManager* bpm,
            int file_id);

  int GetRecord(const RID& rid, RM_Record& rec) const;

  int InsertRecord(const char* pdata, RID& rid);

  int DeleteRecord(const RID& rid);

  int UpdateRecord(const RM_Record& rec);

  int ForcePages();

  int GetFileID();

  int GetRecordSize();

  int GetMaxPageNum();

  int GetNextNotEmptySlot(int page_num, int slot_num);

 private:
  std::string file_name_;
  int record_size_;
  filesystem::FileManager* fm_;
  filesystem::BufPageManager* bpm_;
  int file_id_;
  int max_page_num_;
};

class RM_Manager {
 public:
  explicit RM_Manager(filesystem::FileManager* fm,
                      filesystem::BufPageManager* bpm);

  ~RM_Manager();

  int CreateFile(const std::string& file_name, int record_size);

  int DestroyFile(const std::string& file_name);

  int OpenFile(const std::string& file_name, RM_FileHandle& file_handle);

  int CloseFile(RM_FileHandle& file_handle);

 private:
  filesystem::FileManager* fm_;
  filesystem::BufPageManager* bpm_;
  std::unordered_map<std::string, int> fn_to_rs_;  // file_name -> record_size
};

//
// Example:
//    RM_FileScan file_scan;
//    file_scan.OpenScan(...);
//    RM_Record record;
//    while (file_scan.GetNextRecord(record) != EOF) {
//      // Do something.
//    }
//
class RM_FileScan {
 public:
  RM_FileScan();

  ~RM_FileScan();

  int OpenScan(RM_FileHandle* file_handle, AttrType attr_type, int attr_length,
               int attr_offset, CompOp comp_op, void* value);

  int GetNextRecord(RM_Record& record);

  int CloseScan();

 private:
  RM_FileHandle* file_handle_;
  int attr_length_;
  int attr_offset_;
  void* value_;
  std::function<bool(void*, void*, int)> check_;
  RID current_;
};

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_H