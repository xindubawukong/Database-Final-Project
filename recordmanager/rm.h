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
  std::unordered_map<std::string, int> fn_to_size_;
};

}  // namespace recordmanager

#endif  // RECORDMANAGER_RM_H