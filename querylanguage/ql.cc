#include "ql.h"
#include "return_code.h"

using std::string;
using recordmanager::RID;

namespace querylanguage {

QL_Manager::QL_Manager(recordmanager::RM_Manager *rmm,
                       indexing::IX_Manager *ixm,
                       systemmanager::SM_Manager *smm,
                       filesystem::FileManager *fm,
                       filesystem::BufPageManager *bpm)
    : rmm_(rmm), ixm_(ixm), smm_(smm) {}

QL_Manager::~QL_Manager() {}

int QL_Manager::Select(int nSelAttrs, const RelAttr selAttrs[], int nRelations,
                       const char *const relations[], int nConditions,
                       const Condition conditions[]) {
  assert(nSelAttrs > 0);
}

int QL_Manager::Insert(const char *relName, int nValues, const Value values[]) {
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  fm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo*)addr);

  // 检测属性个数及类型是否一致
  if (nValues != table_info.attrCount) return -1;
  for (int i = 0; i < nValues; i++) {
    if (values[i].type != table_info.attrInfos[i].attrType) return -1;
  }

  // 得到要插入的data
  int record_size = 0;
  for (int i = 0; i < nValues; i++) {
    record_size += table_info.attrInfos[i].attrLength;
  }
  char* data = new char[record_size];
  int offset = 0;
  for (int i = 0; i < nValues; i++) {
    memcpy(data + offset, values[i].data, table_info.attrInfos[i].attrLength);
  }

  // 将data插入文件中
  recordmanager::RM_FileHandle rm_filehandle;
  rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  RID rid;
  rc = rm_filehandle.InsertRecord(data, rid);
  if (rc) return rc;

  delete[] data;
}

int QL_Manager::Delete(const char *relName, int nConditions,
                       const Condition conditions[]) {
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  fm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo*)addr);

  recordmanager::RM_FileHandle rm_filehandle;
  rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  recordmanager::RM_FileScan rm_filescan;
  int value = 0;
  rc = rm_filescan.OpenScan(&rm_filehandle, AttrType::INT, 4, 0, CompOp::NO_OP,
                            &value);
  if (rc) return rc;
  recordmanager::RM_Record record;
  while (rm_filescan.GetNextRecord(record) != RM_EOF) {
    if (true) {
      // rm_filehandle.DeleteRecord(rid);
    }
  }
}

int QL_Manager::Update(const char *relName, const RelAttr &updAttr,
                       const int bIsValue, const RelAttr &rhsRelAttr,
                       const Value &rhsValue, int nConditions,
                       const Condition conditions[]) {}

}  // namespace querylanguage