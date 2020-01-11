#include <vector>

#include "ql.h"
#include "return_code.h"

using recordmanager::RID;
using std::string;

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
  if (nSelAttrs <= 0) return -1;
  if (nRelations <= 0) return -1;
  int rc;

  std::unordered_map<string, systemmanager::TableInfo> tbname_to_tbinfo;
  for (int i = 0; i < nRelations; i++) {
    string tbname = relations[i];
    string meta_file = tbname + "_meta";
    int meta_file_id, index;
    fm_->openFile(meta_file.c_str(), meta_file_id);
    auto addr = bpm_->getPage(meta_file_id, 0, index);
    systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);
    tbname_to_tbinfo[tbname] = table_info;
  }

  std::vector<recordmanager::RM_Record> now;
  std::vector<std::vector<recordmanager::RM_Record>> results;
  rc = Dfs(nRelations, relations, tbname_to_tbinfo, &now, &results);
  if (rc) return rc;

  for (const auto& records : results) {

  }

  return NO_ERROR;
}

int QL_Manager::Insert(const char *relName, int nValues, const Value values[]) {
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  fm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);

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
  char *data = new char[record_size];
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

  return NO_ERROR;
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
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);

  recordmanager::RM_FileHandle rm_filehandle;
  rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  recordmanager::RM_FileScan rm_filescan;
  int value = 0;
  rc = rm_filescan.OpenScan(&rm_filehandle, AttrType::INT, 4, 0, CompOp::NO_OP,
                            &value);
  if (rc) return rc;
  recordmanager::RM_Record record;
  std::vector<RID> rids_to_delete;
  while (rm_filescan.GetNextRecord(record) != RM_EOF) {
    char *record_data;
    record.GetData(record_data);
    bool flag = true;
    for (int condition_id = 0; condition_id < nConditions; condition_id++) {
      auto &condition = conditions[condition_id];
      if (condition.bRhsIsAttr) {
        int l_id = -1, l_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.lhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            l_id = i;
            l_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (l_id == -1) return -1;

        int r_id = -1, r_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.rhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            r_id = i;
            r_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (r_id == -1) return -1;

        // 左右两个属性必须类型相同
        if (table_info.attrInfos[l_id].attrType !=
            table_info.attrInfos[r_id].attrType)
          return -1;
        AttrType attr_type = table_info.attrInfos[l_id].attrType;
        auto check_f = GetCheckFunction(attr_type, condition.op);
        int attr_length = table_info.attrInfos[l_id].attrLength;
        flag &= check_f(record_data + l_offset, record_data + r_offset,
                        attr_length);
      } else {
        int l_id = -1, l_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.lhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            l_id = i;
            l_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (l_id == -1) return -1;

        // 左边属性的类型必须与右边value的类型相同
        AttrType attr_type = table_info.attrInfos[l_id].attrType;
        if (attr_type != condition.rhsValue.type) return -1;
        auto check_f = GetCheckFunction(attr_type, condition.op);
        int attr_length = table_info.attrInfos[l_id].attrLength;
        flag &= check_f(record_data + l_offset, condition.rhsValue.data,
                        attr_length);
      }
      if (!flag) break;
    }
    if (flag) {
      RID rid;
      record.GetRid(rid);
      rids_to_delete.push_back(rid);
    }
  }
  rm_filescan.CloseScan();

  for (const auto &rid : rids_to_delete) {
    rm_filehandle.DeleteRecord(rid);
  }

  return NO_ERROR;
}

int QL_Manager::Update(const char *relName, const RelAttr &updAttr,
                       const int bIsValue, const RelAttr &rhsRelAttr,
                       const Value &rhsValue, int nConditions,
                       const Condition conditions[]) {
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  fm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);

  recordmanager::RM_FileHandle rm_filehandle;
  rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  recordmanager::RM_FileScan rm_filescan;
  int value = 0;
  rc = rm_filescan.OpenScan(&rm_filehandle, AttrType::INT, 4, 0, CompOp::NO_OP,
                            &value);
  if (rc) return rc;
  std::vector<recordmanager::RM_Record> records_to_update;
  recordmanager::RM_Record record;
  while (rm_filescan.GetNextRecord(record) != RM_EOF) {
    char *record_data;
    record.GetData(record_data);
    bool flag = true;
    for (int condition_id = 0; condition_id < nConditions; condition_id++) {
      auto &condition = conditions[condition_id];
      if (condition.bRhsIsAttr) {
        int l_id = -1, l_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.lhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            l_id = i;
            l_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (l_id == -1) return -1;

        int r_id = -1, r_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.rhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            r_id = i;
            r_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (r_id == -1) return -1;

        // 左右两个属性必须类型相同
        if (table_info.attrInfos[l_id].attrType !=
            table_info.attrInfos[r_id].attrType)
          return -1;
        AttrType attr_type = table_info.attrInfos[l_id].attrType;
        auto check_f = GetCheckFunction(attr_type, condition.op);
        int attr_length = table_info.attrInfos[l_id].attrLength;
        flag &= check_f(record_data + l_offset, record_data + r_offset,
                        attr_length);
      } else {
        int l_id = -1, l_offset = 0;
        for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
          if (strcmp(condition.lhsAttr.attrName,
                     table_info.attrInfos[i].attrName) == 0) {
            l_id = i;
            l_offset = offset;
            break;
          }
          offset += table_info.attrInfos[i].attrLength;
        }
        if (l_id == -1) return -1;

        // 左边属性的类型必须与右边value的类型相同
        AttrType attr_type = table_info.attrInfos[l_id].attrType;
        if (attr_type != condition.rhsValue.type) return -1;
        auto check_f = GetCheckFunction(attr_type, condition.op);
        int attr_length = table_info.attrInfos[l_id].attrLength;
        flag &= check_f(record_data + l_offset, condition.rhsValue.data,
                        attr_length);
      }
      if (!flag) break;
    }
    if (flag) {
      records_to_update.push_back(record);
    }
  }
  rm_filescan.CloseScan();

  for (auto &record : records_to_update) {
    int id = -1, pos = 0;
    for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
      if (strcmp(updAttr.attrName, table_info.attrInfos[i].attrName) == 0) {
        id = i;
        pos = offset;
        break;
      }
      offset += table_info.attrInfos[i].attrLength;
    }
    if (id == -1) return -1;

    char *record_data;
    record.GetData(record_data);
    if (bIsValue) {
      if (table_info.attrInfos[id].attrType != rhsValue.type) return -1;
      int attr_length = table_info.attrInfos[id].attrLength;
      memcpy(record_data + pos, rhsValue.data, attr_length);
    } else {  // 暂时只支持要update的右值为value
      return -1;
    }

    rm_filehandle.UpdateRecord(record);
  }

  return NO_ERROR;
}

int QL_Manager::Dfs(
    int nRelations, const char *const relations[],
    std::unordered_map<string, systemmanager::TableInfo> tbname_to_tbinfo,
    std::vector<recordmanager::RM_Record> *now,
    std::vector<std::vector<recordmanager::RM_Record>> *results) {

  if (now->size() == nRelations) {
    results->push_back(*now);
    return NO_ERROR;
  }
  
  int t = now->size();
  string tbname = relations[t];
  auto tbinfo = tbname_to_tbinfo[tbname];
  string data_file = tbname + "_data";
  recordmanager::RM_FileHandle rm_filehandle;
  int rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  recordmanager::RM_FileScan rm_filescan;
  int value = 0;
  rc = rm_filescan.OpenScan(&rm_filehandle, AttrType::INT, 4, 0, CompOp::NO_OP,
                            &value);
  if (rc) return rc;

  recordmanager::RM_Record record;
  while (rm_filescan.GetNextRecord(record) != RM_EOF) {
    now->push_back(record);
    rc = Dfs(nRelations, relations, tbname_to_tbinfo, now, results);
    if (rc) return rc;
    now->pop_back();
  }
  rm_filescan.CloseScan();

  return NO_ERROR;
}

}  // namespace querylanguage