#include <algorithm>
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
    : rmm_(rmm), ixm_(ixm), smm_(smm), fm_(fm), bpm_(bpm) {}

QL_Manager::~QL_Manager() {}

int QL_Manager::Select(int nSelAttrs, const RelAttr selAttrs[], int nRelations,
                       const char *const relations[], int nConditions,
                       const Condition conditions[]) {
  std::cout << "\nQL_Manager::Select" << std::endl;

  int cnt = 0;
  if (nSelAttrs <= 0) return -1;
  if (nRelations <= 0) return -1;
  int rc;

  std::vector<systemmanager::TableInfo> tbinfos;
  for (int i = 0; i < nRelations; i++) {
    string tbname = relations[i];
    string meta_file = tbname + "_meta";
    int meta_file_id, index;
    if (!bpm_->openFile(meta_file.c_str(), meta_file_id)) return -1;
    auto addr = bpm_->getPage(meta_file_id, 0, index);
    bpm_->access(index);
    systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);
    tbinfos.push_back(table_info);
  }

  std::vector<recordmanager::RM_Record> now;
  std::vector<std::vector<recordmanager::RM_Record>> results;
  std::cout << "Dfs start." << std::endl;
  rc = Dfs(nRelations, relations, &now, &results);
  if (rc) return rc;

  std::cout << "Dfs end." << std::endl;

  for (const auto &records : results) {
    std::vector<char *> records_data;
    for (const auto &record : records) {
      char *data;
      record.GetData(data);
      records_data.push_back(data);
    }

    bool flag = true;
    for (int condition_id = 0; condition_id < nConditions; condition_id++) {
      auto &condition = conditions[condition_id];
      if (condition.bRhsIsAttr) {
        int l_rel_id = -1, l_attr_id = -1, l_attr_offset = 0;
        if (condition.lhsAttr.relName == NULL) {
          for (int i = 0; i < nRelations; i++) {
            auto &tbinfo = tbinfos[i];
            for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
              if (strcmp(tbinfo.attrInfos[j].attrName,
                         condition.lhsAttr.attrName) == 0) {
                l_rel_id = i;
                l_attr_id = j;
                l_attr_offset = offset;
                break;
              }
              offset += tbinfo.attrInfos[j].attrLength;
            }
            if (l_rel_id != -1) break;
          }
        } else {
          for (int i = 0; i < nRelations; i++) {
            if (strcmp(relations[i], condition.lhsAttr.relName) == 0) {
              l_rel_id = i;
              break;
            }
          }
          auto &tbinfo = tbinfos[l_rel_id];
          for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
            if (strcmp(tbinfo.attrInfos[j].attrName,
                       condition.lhsAttr.attrName) == 0) {
              l_attr_id = j;
              l_attr_offset = offset;
            }
            offset += tbinfo.attrInfos[j].attrLength;
          }
        }

        int r_rel_id = -1, r_attr_id = -1, r_attr_offset = 0;
        if (condition.lhsAttr.relName == NULL) {
          for (int i = 0; i < nRelations; i++) {
            auto &tbinfo = tbinfos[i];
            for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
              if (strcmp(tbinfo.attrInfos[j].attrName,
                         condition.rhsAttr.attrName) == 0) {
                r_rel_id = i;
                r_attr_id = j;
                r_attr_offset = offset;
                break;
              }
              offset += tbinfo.attrInfos[j].attrLength;
            }
            if (r_rel_id != -1) break;
          }
        } else {
          for (int i = 0; i < nRelations; i++) {
            if (strcmp(relations[i], condition.rhsAttr.relName) == 0) {
              r_rel_id = i;
              break;
            }
          }
          auto &tbinfo = tbinfos[r_rel_id];
          for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
            if (strcmp(tbinfo.attrInfos[j].attrName,
                       condition.rhsAttr.attrName) == 0) {
              r_attr_id = j;
              r_attr_offset = offset;
            }
            offset += tbinfo.attrInfos[j].attrLength;
          }
        }

        // 左右类型必须相同
        if (tbinfos[l_rel_id].attrInfos[l_attr_id].attrType !=
            tbinfos[r_rel_id].attrInfos[r_attr_id].attrType)
          return -1;
        AttrType attr_type = tbinfos[l_rel_id].attrInfos[l_attr_id].attrType;
        auto check_f = GetCheckFunction(attr_type, condition.op);
        int length =
            std::max(tbinfos[l_rel_id].attrInfos[l_attr_id].attrLength,
                     tbinfos[r_rel_id].attrInfos[r_attr_id].attrLength);
        char *l_data = new char[length];
        char *r_data = new char[length];
        std::memset(l_data, 0, length);
        std::memset(r_data, 0, length);
        std::memcpy(l_data, records_data[l_rel_id] + l_attr_offset,
                    tbinfos[l_rel_id].attrInfos[l_attr_id].attrLength);
        std::memcpy(r_data, records_data[r_rel_id] + r_attr_offset,
                    tbinfos[r_rel_id].attrInfos[r_attr_id].attrLength);
        flag &= check_f(l_data, r_data, length);
        delete[] l_data;
        delete[] r_data;
      } else {
        int l_rel_id = -1, l_attr_id = -1, l_attr_offset = 0;
        if (condition.lhsAttr.relName == NULL) {
          for (int i = 0; i < nRelations; i++) {
            auto &tbinfo = tbinfos[i];
            for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
              if (strcmp(tbinfo.attrInfos[j].attrName,
                         condition.lhsAttr.attrName) == 0) {
                l_rel_id = i;
                l_attr_id = j;
                l_attr_offset = offset;
                break;
              }
              offset += tbinfo.attrInfos[j].attrLength;
            }
            if (l_rel_id != -1) break;
          }
        } else {
          for (int i = 0; i < nRelations; i++) {
            if (strcmp(relations[i], condition.lhsAttr.relName) == 0) {
              l_rel_id = i;
              break;
            }
          }
          auto &tbinfo = tbinfos[l_rel_id];
          for (int j = 0, offset = 0; j < tbinfo.attrCount; j++) {
            if (strcmp(tbinfo.attrInfos[j].attrName,
                       condition.lhsAttr.attrName) == 0) {
              l_attr_id = j;
              l_attr_offset = offset;
            }
            offset += tbinfo.attrInfos[j].attrLength;
          }
        }

        if (condition.rhsValue.data == NULL) {
          if (tbinfos[l_rel_id].attrInfos[l_attr_id].attrType != AttrType::STRING) {
            std::cout << "We don't know if INT and FLOAT is NULL or 0." << std::endl;
            return -1;
          }
          int len = std::strlen(records_data[l_rel_id] + l_attr_offset);
          flag &= (condition.op == CompOp::EQ_OP && len == 0) || (condition.op == CompOp::NE_OP && len > 0);
        }
        else {
          // 左右类型必须相同
          if (tbinfos[l_rel_id].attrInfos[l_attr_id].attrType !=
              condition.rhsValue.type)
            return -1;
          AttrType attr_type = tbinfos[l_rel_id].attrInfos[l_attr_id].attrType;
          auto check_f = GetCheckFunction(attr_type, condition.op);
          int length = tbinfos[l_rel_id].attrInfos[l_attr_id].attrLength;
          char *l_data = new char[length];
          char *r_data = new char[length];
          std::memset(l_data, 0, length);
          std::memset(r_data, 0, length);
          std::memcpy(l_data, records_data[l_rel_id] + l_attr_offset, length);
          if (attr_type == AttrType::STRING) {
            int tmp = std::min(length,
                              (int)std::strlen((char *)condition.rhsValue.data));
            std::memcpy(r_data, condition.rhsValue.data, tmp);
          } else {
            std::memcpy(r_data, condition.rhsValue.data, length);
          }
          flag &= check_f(l_data, r_data, length);
          delete[] l_data;
          delete[] r_data;
        }
      }
      if (!flag) break;
    }
    if (flag) {  // 符合条件，输出records
      cnt ++;
      string output_string;
      if (nSelAttrs == 1 &&
          std::strcmp(selAttrs[0].attrName, "*") == 0) {  // select *
        output_string = "[";
        for (int i = 0; i < nRelations; i++) {
          if (i > 0) output_string += ", ";
          output_string += "(";
          for (int j = 0, offset = 0; j < tbinfos[i].attrCount; j++) {
            if (j > 0) output_string += ", ";
            if (tbinfos[i].attrInfos[j].attrType == AttrType::INT) {
              int x = *((int *)(records_data[i] + offset));
              output_string += std::to_string(x);
            } else if (tbinfos[i].attrInfos[j].attrType == AttrType::FLOAT) {
              float x = *((float *)(records_data[i] + offset));
              output_string += std::to_string(x);
            } else {
              char *s = records_data[i] + offset;
              output_string += (string)s;
            }
            offset += tbinfos[i].attrInfos[j].attrLength;
          }
          output_string += ")";
        }
        output_string += "]";
      } else {
        output_string = "[";
        for (int kk = 0; kk < nSelAttrs; kk++) {
          if (kk > 0) output_string += ", ";

          int rel_id = -1, attr_id = -1, attr_offset = 0;
          if (selAttrs[kk].relName == NULL) {
            for (int i = 0; i < nRelations; i++) {
              for (int j = 0, offset = 0; j < tbinfos[i].attrCount; j++) {
                if (strcmp(selAttrs[kk].attrName,
                           tbinfos[i].attrInfos[j].attrName) == 0) {
                  rel_id = i;
                  attr_id = j;
                  attr_offset = offset;
                  break;
                }
                offset += tbinfos[i].attrInfos[j].attrLength;
              }
              if (rel_id != -1) break;
            }
          } else {
            for (int i = 0; i < nRelations; i++) {
              if (strcmp(selAttrs[kk].relName, relations[i]) == 0) {
                rel_id = i;
                break;
              }
            }
            for (int j = 0, offset = 0; j < tbinfos[rel_id].attrCount; j++) {
              if (strcmp(selAttrs[kk].attrName,
                         tbinfos[rel_id].attrInfos[j].attrName) == 0) {
                attr_id = j;
                attr_offset = offset;
                break;
              }
              offset += tbinfos[rel_id].attrInfos[j].attrLength;
            }
          }

          if (tbinfos[rel_id].attrInfos[attr_id].attrType == AttrType::INT) {
            int x = *((int *)(records_data[rel_id] + attr_offset));
            output_string += std::to_string(x);
          } else if (tbinfos[rel_id].attrInfos[attr_id].attrType ==
                     AttrType::FLOAT) {
            float x = *((float *)(records_data[rel_id] + attr_offset));
            output_string += std::to_string(x);
          } else {
            char *s = records_data[rel_id] + attr_offset;
            output_string += (string)s;
          }
        }
        output_string += "]";
      }
      std::cout << output_string << std::endl;
    }
  }

  std::cout << "total: " << cnt << std::endl;

  return NO_ERROR;
}

int QL_Manager::Insert(const char *relName, int nValues, const Value values[]) {

  std::cout << "\nQL_Manager::Insert " << relName << std::endl;

  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  std::cout << "data_file: " << data_file << std::endl;
  std::cout << "meta_file: " << meta_file << std::endl;

  int meta_file_id, index;
  bpm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  bpm_->access(index);
  systemmanager::TableInfo table_info = *((systemmanager::TableInfo *)addr);

  // 检测属性个数及类型是否一致
  if (nValues != table_info.attrCount) return -1;

  // 得到要插入的data
  int record_size = 0;
  for (int i = 0; i < nValues; i++) {
    record_size += table_info.attrInfos[i].attrLength;
  }
  char *data = new char[record_size];
  std::memset(data, 0, record_size);
  for (int i = 0, offset = 0; i < nValues; i++) {
    if (values[i].data != NULL) {
      int len = std::min(table_info.attrInfos[i].attrLength, (int)std::strlen((char*)values[i].data));
      std::memcpy(data + offset, values[i].data, table_info.attrInfos[i].attrLength);
    }
    else {
      if (table_info.attrInfos[i].notNullFlag) {
        std::cout << "Can't insert record with NULL '" << table_info.attrInfos[i].attrName << "'" << std::endl;
        return -1;
      }
      std::memset(data + offset, 0, table_info.attrInfos[i].attrLength);
    }
    offset += table_info.attrInfos[i].attrLength;
  }

  // 将data插入文件中
  recordmanager::RM_FileHandle rm_filehandle;
  rc = rmm_->OpenFile(data_file, rm_filehandle);
  if (rc) return rc;
  RID rid;
  rc = rm_filehandle.InsertRecord(data, rid);
  if (rc) return rc;

  string output_string = "[";
  for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
    if (i > 0) output_string += ", ";
    if (table_info.attrInfos[i].attrType == AttrType::INT) {
      int x = *((int *)(data + offset));
      output_string += std::to_string(x);
    } else if (table_info.attrInfos[i].attrType == AttrType::FLOAT) {
      float x = *((float *)(data + offset));
      output_string += std::to_string(x);
    } else {
      char *s = data + offset;
      output_string += (string)s;
    }
    offset += table_info.attrInfos[i].attrLength;
  }
  output_string += "]";
  std::cout << output_string << std::endl;

  delete[] data;

  return NO_ERROR;
}

int QL_Manager::Delete(const char *relName, int nConditions,
                       const Condition conditions[]) {
  std::cout << "\nQL_Manager::Delete" << std::endl;
  
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  bpm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  bpm_->access(index);
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
  std::vector<recordmanager::RM_Record> records_to_delete;
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
        int length = std::max(table_info.attrInfos[l_id].attrLength,
                              table_info.attrInfos[r_id].attrLength);
        char *l_data = new char[length];
        char *r_data = new char[length];
        std::memset(l_data, 0, length);
        std::memset(r_data, 0, length);
        std::memcpy(l_data, record_data + l_offset,
                    table_info.attrInfos[l_id].attrLength);
        std::memcpy(r_data, record_data + r_offset,
                    table_info.attrInfos[r_id].attrLength);
        flag &= check_f(l_data, r_data, length);
        delete[] l_data;
        delete[] r_data;
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

        if (condition.rhsValue.data == NULL) {
          if (table_info.attrInfos[l_id].attrType != AttrType::STRING) {
            std::cout << "We don't know if INT and FLOAT is NULL or 0." << std::endl;
            return -1;
          }
          int len = std::strlen(record_data + l_offset);
          flag &= (condition.op == CompOp::EQ_OP && len == 0) || (condition.op == CompOp::NE_OP && len > 0);
        }
        else {
          // 左边属性的类型必须与右边value的类型相同
          AttrType attr_type = table_info.attrInfos[l_id].attrType;
          if (attr_type != condition.rhsValue.type) return -1;
          auto check_f = GetCheckFunction(attr_type, condition.op);
          int length = table_info.attrInfos[l_id].attrLength;
          char *l_data = new char[length];
          char *r_data = new char[length];
          std::memset(l_data, 0, length);
          std::memset(r_data, 0, length);
          std::memcpy(l_data, record_data + l_offset, length);
          if (attr_type == AttrType::STRING) {
            int tmp = std::min(length,
                              (int)std::strlen((char *)condition.rhsValue.data));
            std::memcpy(r_data, condition.rhsValue.data, tmp);
          } else {
            std::memcpy(r_data, condition.rhsValue.data, length);
          }
          flag &= check_f(l_data, r_data, length);
          delete[] l_data;
          delete[] r_data;
        }
      }
      if (!flag) break;
    }
    if (flag) {
      records_to_delete.push_back(record);
    }
  }
  rm_filescan.CloseScan();

  for (const auto &record : records_to_delete) {
    RID rid;
    record.GetRid(rid);
    rm_filehandle.DeleteRecord(rid);

    string output_string = "[";
    char *data;
    record.GetData(data);
    for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
      if (i > 0) output_string += ", ";
      if (table_info.attrInfos[i].attrType == AttrType::INT) {
        int x = *((int *)(data + offset));
        output_string += std::to_string(x);
      } else if (table_info.attrInfos[i].attrType == AttrType::FLOAT) {
        float x = *((float *)(data + offset));
        output_string += std::to_string(x);
      } else {
        char *s = data + offset;
        output_string += (string)s;
      }
      offset += table_info.attrInfos[i].attrLength;
    }
    output_string += "]";
    std::cout << output_string << std::endl;
  }

  return NO_ERROR;
}

int QL_Manager::Update(const char *relName, const RelAttr &updAttr,
                       const int bIsValue, const RelAttr &rhsRelAttr,
                       const Value &rhsValue, int nConditions,
                       const Condition conditions[]) {
  std::cout << "\nQL_Manager::Update" << std::endl;
  
  if (relName == NULL) return -1;
  int rc;

  string data_file = (string)relName + "_data";
  string meta_file = (string)relName + "_meta";

  int meta_file_id, index;
  bpm_->openFile(meta_file.c_str(), meta_file_id);
  auto addr = bpm_->getPage(meta_file_id, 0, index);
  bpm_->access(index);
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
        int length = std::max(table_info.attrInfos[l_id].attrLength,
                              table_info.attrInfos[r_id].attrLength);
        char *l_data = new char[length];
        char *r_data = new char[length];
        std::memset(l_data, 0, length);
        std::memset(r_data, 0, length);
        std::memcpy(l_data, record_data + l_offset,
                    table_info.attrInfos[l_id].attrLength);
        std::memcpy(r_data, record_data + r_offset,
                    table_info.attrInfos[r_id].attrLength);
        flag &= check_f(l_data, r_data, length);
        delete[] l_data;
        delete[] r_data;
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
        int length = table_info.attrInfos[l_id].attrLength;
        char *l_data = new char[length];
        char *r_data = new char[length];
        std::memset(l_data, 0, length);
        std::memset(r_data, 0, length);
        std::memcpy(l_data, record_data + l_offset, length);
        if (attr_type == AttrType::STRING) {
          int tmp = std::min(length,
                             (int)std::strlen((char *)condition.rhsValue.data));
          std::memcpy(r_data, condition.rhsValue.data, tmp);
        } else {
          std::memcpy(r_data, condition.rhsValue.data, length);
        }
        flag &= check_f(l_data, r_data, length);
        delete[] l_data;
        delete[] r_data;
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
      std::memset(record_data + pos, 0, attr_length);
      std::memcpy(record_data + pos, rhsValue.data, attr_length);
    } else {  // 暂时只支持要update的右值为value
      return -1;
    }

    rm_filehandle.UpdateRecord(record);

    string output_string = "[";
    for (int i = 0, offset = 0; i < table_info.attrCount; i++) {
      if (i > 0) output_string += ", ";
      if (table_info.attrInfos[i].attrType == AttrType::INT) {
        int x = *((int *)(record_data + offset));
        output_string += std::to_string(x);
      } else if (table_info.attrInfos[i].attrType == AttrType::FLOAT) {
        float x = *((float *)(record_data + offset));
        output_string += std::to_string(x);
      } else {
        char *s = record_data + offset;
        output_string += (string)s;
      }
      offset += table_info.attrInfos[i].attrLength;
    }
    output_string += "]";
    std::cout << output_string << std::endl;
  }

  return NO_ERROR;
}

int QL_Manager::Dfs(
    int nRelations, const char *const relations[],
    std::vector<recordmanager::RM_Record> *now,
    std::vector<std::vector<recordmanager::RM_Record>> *results) {
  if (now->size() == nRelations) {
    results->push_back(*now);
    return NO_ERROR;
  }

  int t = now->size();
  string tbname = relations[t];
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
    rc = Dfs(nRelations, relations, now, results);
    if (rc) return rc;
    now->pop_back();
  }
  rm_filescan.CloseScan();

  return NO_ERROR;
}

}  // namespace querylanguage