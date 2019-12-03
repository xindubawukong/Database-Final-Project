#ifndef RECORDMANAGER_RM_H
#define RECORDMANAGER_RM_H

#include <string>
#include <unordered_map>

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "rm_rid.h"
#include "rm_record.h"

namespace recordmanager {

void RM_PrintError(int rc);

class RM_FileHandle {
 public:
  RM_FileHandle();
  
  ~RM_FileHandle();

  int GetRecord(const RID& rid, RM_Record& rec) const;

  int InsertRecord(const char* pdata, RID& rid);

  int DeleteRecord(const RID& rid);

  int UpdateRecord(const RM_Record& rec);

  int ForcePages();

 private:
  static const int kPageSize;
  static const int kRecordSizePosition;
  static const int kPageBitMapStartPosition;
  static const int kMaxPageNum;
  static const int kBitMapStartPosition;
  static const int kBitMapLength;
  static const int kRecordStartPosition;
  static const int kRecordNumEachPage;
  static const int kRecordMaxLength;

  std::string file_name_;
  int record_size_;
  filesystem::FileManager* fm_;
  filesystem::BufPageManager* bpm_;
  int file_id_;
};

class RM_Manager {
 public:
  RM_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm);
  
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

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_H