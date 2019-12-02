#include "rm.h"

#include <cstdio>

#include "return_code.h"

namespace recordmanager {

void RM_PrintError(int rc) { fprintf(stderr, "RM error %d.\n", rc); }

RM_Manager::RM_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm) {
  this->fm_ = fm;
  this->bpm_ = bpm;
}

int RM_Manager::CreateFile(const std::string& file_name, int record_size) {
  if (fn_to_size_.find(file_name) != fn_to_size_.end()) {
    return RM_MANAGER_DUPLICATE_FILE_NAME_ERROR;
  }
  fm_->createFile(file_name.c_str());
  fn_to_size_[file_name] = record_size;
  return NO_ERROR;
}

int RM_Manager::DestroyFile(const std::string& file_name) {
  fn_to_size_.erase(file_name);
  return NO_ERROR;
}

}  // namespace recordmanager