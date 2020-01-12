#include "sm.h"
#include "return_code.h"
#include "unistd.h"
#include <cstring>
#include <set>
#include <map>

namespace systemmanager {
  using filesystem::FileManager;
  using filesystem::BufPageManager;
  using indexing::IX_Manager;
  using recordmanager::RM_Manager;

  void SM_PrintError(int rc) {
    printf("SM error %d.\n", rc);
  }

  void Print(TableInfo tableInfo) {
    std::cout << "TableInfo:" << std::endl;
    std::cout << "This table has " << tableInfo.attrCount << " attrs." << std::endl;
    std::map<int, std::string> name;
    for(int i = 0; i < tableInfo.attrCount; ++i) {
      std::cout << i + 1 << ". ";
      Print(tableInfo.attrInfos[i]);
      name[i] = std::string(tableInfo.attrInfos[i].attrName);
    }
    if(tableInfo.indexSize > 0) {
      std::cout << tableInfo.indexSize << " attrs below have already been indexed:" << std::endl;
      for(int i = 0; i < tableInfo.indexSize; ++i) {
        std::cout << name[tableInfo.indexedAttr[i]] << "  ";
      }
      std::cout << std::endl;
    } else {
      std::cout << "No indexed attrs." << std::endl;
    }
    
    std::cout << "This table has " << tableInfo.constraintCount << " constraints." << std::endl;
  }

  void Print(AttrInfo attrInfo) {
    std::cout << "AttrName: " << attrInfo.attrName << ", AttrType: " << attrInfo.attrType;
    std::cout << ", AttrLength: " << attrInfo.attrLength << ", NotNull: " << attrInfo.notNullFlag << ", DefaultValue: ";
    switch (attrInfo.attrType)
    {
      case INT:
        std::cout << *((int*)attrInfo.defaultValue);
        break;
      case FLOAT:
        std::cout << std::to_string(*((float*)attrInfo.defaultValue));
        break;
      case STRING:
        std::cout << attrInfo.defaultValue;
      default:
        break;
    }
    std::cout << std::endl;
  }

  void Print(Constraint constraint) {
    if(constraint.isPrimary) {
      std::cout << "PrimaryKey: ";
      Print(constraint.thisNameList);
    } else {
      std::cout << "ForeignKey " << constraint.constraintName << ":";
      Print(constraint.thisNameList);
      std::cout << " references " << constraint.foreignTableName;
      Print(constraint.referencesNameList);
    }
    std::cout << std::endl;
  }

  void Print(AttrList attrList) {
    std::cout << "(";
    for(int i = 0; i < attrList.attrCount - 1; ++i) {
      std::cout << attrList.names[i] << ", ";
    }
    std::cout << attrList.names[attrList.attrCount - 1] << ")";
  }

  void CopyStr(char *dst, const char *src, int maxLength) {
    memset(dst, 0, maxLength);
    memcpy(dst, src, strlen(src));
  }
 
  SM_Manager::SM_Manager(FileManager* fm, BufPageManager* bpm, IX_Manager *ixm, RM_Manager *rmm) {
    _fm = fm;
    _bpm = bpm;
    _ixm = ixm;
    _rmm = rmm;
  } 

  SM_Manager::~SM_Manager() {
    delete _fm;
    delete _bpm;
    delete _ixm;
    delete _rmm;
  }

  int SM_Manager::CreateDb(const char* dbName) {
 
    int rc = _bpm->createFile("DatabaseList");
    
    int fileID, index;
    rc = _bpm->openFile("DatabaseList", fileID);
    DatabaseList *dbList = (DatabaseList*) _bpm->getPage(fileID, 0, index);

    if(dbList->databaseCount == MAX_DATABASES) {
      return -1;
    }
    for(int i = 0; i < dbList->databaseCount; ++i) {
      if(strcmp(dbList->databaseName[i], dbName) == 0) {
        return -1;
      }
    }
    CopyStr(dbList->databaseName[dbList->databaseCount++], dbName, MAX_LENGTH);
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);

    rc = mkdir(dbName, S_IRWXU);
    if(rc != 0) {
      return rc;
    }

    chdir(dbName);
    _bpm->createFile("TableList");
    chdir("..");
    return 0;
  } 

  int SM_Manager::OpenDb(const char* dbName) {
    int rc = chdir(dbName);
    return rc;
  }

  int SM_Manager::DropDb(const char* dbName) {

    int fileID, index;
    int rc = _bpm->openFile("DatabaseList", fileID);
    if(rc == 0) {
      return -1;
    }
    DatabaseList *dbList = (DatabaseList*) _bpm->getPage(fileID, 0, index);
    bool find = false;
    for(int i = 0; i < dbList->databaseCount; ++i) {
      if(strcmp(dbList->databaseName[i], dbName) == 0) {
        if(dbList->databaseCount > 1) {
          CopyStr(dbList->databaseName[i], dbList->databaseName[dbList->databaseCount - 1], MAX_LENGTH);
        }
        --dbList->databaseCount;
        find = true;
        break;
      }
    }

    if(!find) {
      return -1;
    }
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);

    DIR* dirp = opendir(dbName);    
    if(!dirp) {
        return -1;
    }
    std::string dirName = std::string(dbName);
    struct dirent *dir;
    while((dir = readdir(dirp)) != nullptr) {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }    
        std::string sub_path = dirName + '/' + dir->d_name; 
        unlink(sub_path.c_str());   
    }

    if(rmdir(dbName) == -1) {
        closedir(dirp);
        return -1;
    }
    closedir(dirp);

    return 0;
  }

  int SM_Manager::CloseDb() {
    int rc = chdir("..");
    return rc;
  }

  int SM_Manager::ShowDb(const char* name) {
    int rc = chdir(name);
    if(rc != 0) {
      return rc;
    }

    int fileID;
    rc = _bpm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }

    int index;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);

    std::cout << "Database " << name << " has tables below:" << std::endl; 
    for(int i = 0; i < tableList->tableCount; ++i) {
      std::cout << tableList->tableName[i] << std::endl;
    }
    _bpm->release(index);
    _bpm->closeFile(fileID);

    rc = chdir("..");
    std::cout << std::endl;
    return rc;
  }

  int SM_Manager::ShowAllDb() {
    int fileID, index;
    int rc = _bpm->openFile("DatabaseList", fileID);
    if(rc == 0) {
      return -1;
    }
    DatabaseList *dbList = (DatabaseList*) _bpm->getPage(fileID, 0, index);

    for(int i = 0; i < dbList->databaseCount; ++i) {
      rc = ShowDb(dbList->databaseName[i]);
      if(rc != 0) {
        return rc;
      }
    }
    
    std::cout << std::endl;
    return 0;
  }

  int SM_Manager::CreateTable(const char *relName, int attrCount, AttrInfo* attrInfos, int constraintCount, Constraint* constraints) {
    int fileID;
    int rc = _bpm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }
    
    int index;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);

    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(relName, tableList->tableName[i]) == 0) {
        return -1;
      }
    }

    if(tableList->tableCount == MAX_TABLES) {
      return -1;
    }
    _bpm->closeFile(fileID);

    std::set<std::string> constraintNameSet;
    std::set<std::string> foreginTableNameSet;
    int primaryCnt = 0;

    for(int i = 0; i < constraintCount; ++i) {
      if(constraintNameSet.count(std::string(constraints[i].constraintName)) > 0) {
        return -1;
      }
      constraintNameSet.insert(std::string(constraints[i].constraintName));

      if(constraints[i].isPrimary) {
        if(primaryCnt++ > 1) {
          return -1;
        }

        
        for(int j = 0; j < constraints[i].thisNameList.attrCount; ++j) {
          bool find = false;
          for(int k = 0; k < attrCount; ++k) {
            if(strcmp(attrInfos[k].attrName, constraints[i].thisNameList.names[j]) == 0) {
              find = true;
              if(!attrInfos[k].notNullFlag) {
                return -1;
              }
              break;
            }
          }
          if(!find) {
            return -1;
          }
        }

      } else {

        if(!TableExist(constraints[i].foreignTableName)) {
          return -1;
        }

        if(foreginTableNameSet.count(std::string(constraints[i].foreignTableName)) > 0) {
          return -1;
        }

        foreginTableNameSet.insert(std::string(constraints[i].foreignTableName));

        for(int j = 0; j < constraints[i].thisNameList.attrCount; ++j) {
          bool find = false;
          for(int k = 0; k < attrCount; ++k) {
            if(strcmp(attrInfos[k].attrName, constraints[i].thisNameList.names[j]) == 0) {
              find = true;
              break;
            }
          }
          if(!find) {
            return -1;
          }
        }

        // for(int j = 0; j < constraints[i].referencesNameList.attrCount; ++i) {
        //   if(!AttrExist(constraints[i].foreignTableName, 
        //   constraints[i].referencesNameList.names[j])) {
        //     return -1;
        //   }
        // }

        int fileID, index;
        std::string fkMetaFile = std::string(constraints[i].foreignTableName) + "_meta";
        _bpm->openFile(fkMetaFile.c_str(), fileID);
        TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
        _bpm->pin(index);
        int find = false;
        for(int j = 0; j < info->constraintCount; ++j) {
          int index;
          Constraint *cons = (Constraint*) _bpm->getPage(fileID, j + 1, index);
          if(cons->isPrimary) {
            find = true;
            if(!AttrListEqual(cons->thisNameList, constraints[i].referencesNameList)) {
              find = false;
            }
            break;
          }
        }
        _bpm->unpin(index);
        _bpm->closeFile(fileID);
        if(!find) {
          return -1;
        }
      }
    }

    std::string metaFile = std::string(relName) + "_meta";
    rc = _bpm->createFile(metaFile.c_str());
    if(rc == 0) {
      return -1;
    }


    TableInfo* info = new TableInfo();
    info->attrCount = attrCount;
    memcpy(info->attrInfos, attrInfos, sizeof(AttrInfo) * attrCount);
    info->constraintCount = constraintCount;

    _bpm->openFile(metaFile.c_str(), fileID);
    char* buffer = (char*) _bpm->getPage(fileID, 0, index);
    memcpy(buffer, info, sizeof(TableInfo));
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    
    for(int i = 0; i < constraintCount; ++i) {
      buffer = (char*) _bpm->getPage(fileID, i + 1, index);
      memcpy(buffer, &constraints[i], sizeof(Constraint));
      _bpm->markDirty(index);
      _bpm->writeBack(index);
    }
    _bpm->closeFile(fileID);

    std::string dataFile = std::string(relName) + "_data";

    int recordSize = 0;
    for(int i = 0; i < attrCount; ++i) {
      recordSize += attrInfos[i].attrLength;
    }

    rc = _rmm->CreateFile(dataFile, recordSize);

    if(rc != 0) {
      return -1;
    }


    _bpm->openFile("TableList", fileID);
    tableList = (TableList*) _bpm->getPage(fileID, 0, index);

    CopyStr(tableList->tableName[tableList->tableCount++], relName, MAX_LENGTH);
    // std::cout << "counts after create: " << tableList->tableCount << std::endl;
    // std::cout << "create name: " << tableList->tableName[tableList->tableCount - 1] << std::endl;
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);
    return 0;
  }

  int SM_Manager::ShowTable(const char* relName) {
    if(!TableExist(relName)) {
      return -1;
    }
    std::cout << "Table " << relName << ":" << std::endl;

    std::string metaFile = (std::string)relName + "_meta";
    
    int fileID, index;
    int rc = _bpm->openFile(metaFile.c_str(), fileID);

    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);

    int *offset = new int[info->attrCount];
    offset[0] = 0;
    for(int i = 1; i < info->attrCount; ++i) {
      // std::cout << info->attrInfos[i].attrLength << std::endl;
      offset[i] = offset[i - 1] + info->attrInfos[i - 1].attrLength;
      // std::cout << offset[i] << std::endl;
    }
    Print(*info);
    _bpm->pin(index);
    for(int i = 0; i < info->constraintCount; ++i) {
      int index;
      Constraint *cons = (Constraint*) _bpm->getPage(fileID, i + 1, index);
      Print(*cons);
    }
    // _bpm->unpin(index);
    // _bpm->closeFile(fileID);
    std::string dataFile = (std::string)relName + "_data";

    // std::cout << dataFile << std::endl;
    
    recordmanager::RM_FileHandle fileHandle;
    rc = _rmm->OpenFile(dataFile, fileHandle); 
    if(rc != 0) {
      return rc;
    }
    recordmanager::RM_FileScan fileScan;
    fileScan.OpenScan(&fileHandle, info->attrInfos[0].attrType, info->attrInfos[0].attrLength, 0, NO_OP, nullptr);

    recordmanager::RM_Record record;
    // std::cout << "here " << std::endl;
    while(fileScan.GetNextRecord(record) != RM_EOF) {
      // std::cout << "here" << std::endl;
      char *data;
      rc = record.GetData(data);
      // std::cout << *((int*)data )<< std::endl;

      if(rc != 0) {
        return -1;
      }


      for(int i = 0; i < info->attrCount; ++i) {
        // std::cout << "i = " << i <<" " << info->attrInfos[i].attrType << " " << offset[i]<< std::endl;
        
        switch (info->attrInfos[i].attrType)
        {
          case INT:
            std::cout << *((int*)(data + offset[i]));
            break;
          case FLOAT:
            std::cout << *((float*)(data + offset[i]));
            break;
          case STRING:
            std::cout << (char*)(data + offset[i]);
            break;
          default:
            break;
        }
        std::cout << ", ";
      }

      std::cout << std::endl;
    }

    _bpm->unpin(index);
    _bpm->closeFile(fileID);
    fileScan.CloseScan();
    _rmm->CloseFile(fileHandle);

    std::cout << std::endl;

    return 0;
  }

  int SM_Manager::ShowAllTable() {
    int fileID, index;
    int rc = _bpm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }

    TableList *tbList = (TableList*) _bpm->getPage(fileID, 0, index);
    _bpm->pin(index);
    for(int i = 0; i < tbList->tableCount; ++i) {
 
      rc = ShowTable(tbList->tableName[i]);
      if(rc != 0) {
        return -1;
      }
    }
    _bpm->unpin(index);
    std::cout << std::endl;
    return 0;
  }

  int SM_Manager::AlterPrimaryKey(const char *relName, bool add, AttrList* attrList) {
    if(!TableExist(relName)) {
      return -1;
    }
    std::string metaFile = std::string(relName) + "_meta";
    int fileID, index;
    _bpm->openFile(metaFile.c_str(), fileID); 
    
    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    _bpm->pin(index);
    Constraint *pri = nullptr;
    int indexPri = -1;
    for(int i = 0; i < info->constraintCount; ++i) {
      Constraint *cons = (Constraint*) _bpm->getPage(fileID, i + 1, indexPri);
      if(cons->isPrimary) {
        pri = cons;
        break;
      }
    }
    if(add) {
      if(pri != nullptr) {
        return -1;
      }
      for(int i = 0 ; i < attrList->attrCount; ++i) {
        bool find = false;
        for(int j = 0; j < info->attrCount; ++j) {
          if(strcmp(info->attrInfos[j].attrName, attrList->names[i])==0) {
            find = true;
            if(!info->attrInfos[j].notNullFlag) {
              find = false;
            }
            break;
          }
        }
        if(!find) {
          return -1;
        }
      }
      _bpm->unpin(index);
      indexPri = ++info->constraintCount;
      _bpm->markDirty(index);
      

      Constraint *newCons = (Constraint*) _bpm->getPage(fileID, indexPri, index);
      CopyStr(newCons->constraintName, "PrimaryKey", MAX_LENGTH);
      newCons->isPrimary = true;
      newCons->thisNameList = *attrList;
      _bpm->markDirty(index);
      _bpm->closeFile(fileID);

    }  else {
      if(pri == nullptr) {
        return -1;
      }

      int index_last;
      Constraint *lastCons = (Constraint*) _bpm->getPage(fileID, info->constraintCount, index_last);
      memcpy(pri, lastCons, sizeof(Constraint));
      memset(lastCons, 0, sizeof(Constraint));
      _bpm->markDirty(indexPri);
      info->constraintCount--;
      _bpm->markDirty(index);
      _bpm->closeFile(fileID);
    }

    return 0;

  }

  int SM_Manager::AlterForeignKey(const char *relName, const char *fkName, const char *tbName, AttrList* thisList, AttrList* otherList) {
    return 0;
  }

  int SM_Manager::DropTable(const char* relName) {
    int fileID;
    int index;
    int rc = _bpm->openFile("TableList", fileID);

    if(rc == 0) {
      return -1;
    }

    bool find = false;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(tableList->tableName[i], relName) == 0) {
        find = true;
        CopyStr(tableList->tableName[i], tableList->tableName[tableList->tableCount - 1], MAX_LENGTH);
        tableList->tableCount--;
        break;
      }
    }

    if(!find) {
      return -1;
    }
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);
    
    std::string metaFile = std::string(relName) + "_meta";
    rc = _bpm->openFile(metaFile.c_str(), fileID);
    if(rc == 0) {
      return -1;
    }

    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < info->indexSize; ++i) {
      std::string indexName = std::string(relName) + "." + std::to_string(info->indexedAttr[i]);
      _bpm->deleteFile(indexName.c_str());
    }

    _bpm->deleteFile(metaFile.c_str());
    std::string dataFile = std::string(relName) + "_data";
    _bpm->deleteFile(dataFile.c_str());

    return 0;
  }

  int SM_Manager::CreateIndex(const char* relName, const char* attrName) {
    if(!TableExist(relName)) {
      return -1;
    }
    int fileID, index;
    std::string metaFile = std::string(relName) + "_meta";
    int rc = _bpm->openFile(metaFile.c_str(), fileID);
    if(rc == 0) {
      return -1;
    }
    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    int attrIndex = -1;
    for(int i = 0; i < info->attrCount; ++i) {
      if(strcmp(info->attrInfos[i].attrName, attrName) == 0) {
        attrIndex = i;
        break;
      }
    }
    if(attrIndex == -1) {
      return -1;
    }
    bool indexed = false;
    for(int i = 0; i < info->indexSize; ++i) {
      if(info->indexedAttr[i] == attrIndex) {
        indexed = true;
        break;
      }
    }
    if(indexed) {
      return -1;
    }
    
    info->indexedAttr[info->indexSize++] = attrIndex;
    rc = _ixm->CreateIndex(relName, attrIndex, info->attrInfos[attrIndex].attrType, info->attrInfos[attrIndex].attrLength);

    if(rc != 0) {
      return rc;
    }

    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);
    return 0;

  }

  int SM_Manager::DropIndex(const char* relName, const char* attrName) {
    if(!TableExist(relName)) {
      return -1;
    }

    int fileID, index;
    std::string metaFile = std::string(relName) + "_meta";
    int rc = _bpm->openFile(metaFile.c_str(), fileID);
    if(rc == 0) {
      return -1;
    }
    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    int attrIndex = -1;
    for(int i = 0; i < info->attrCount; ++i) {
      if(strcmp(info->attrInfos[i].attrName, attrName) == 0) {
        attrIndex = i;
        break;
      }
    }
    if(attrIndex == -1) {
      return -1;
    }
    bool indexed = false;
    for(int i = 0; i < info->indexSize; ++i) {
      if(info->indexedAttr[i] == attrIndex) {
        indexed = true;
        break;
      }
    }
    if(!indexed) {
      return -1;
    }
    info->indexedAttr[attrIndex] = info->indexedAttr[--info->indexSize];
    rc = _ixm->DestroyIndex(relName, attrIndex);
    if(rc != 0) {
      return -1;
    }

    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _bpm->closeFile(fileID);
    return 0;
  }

  bool SM_Manager::TableExist(const char *tableName) {
    bool res = false;
    int fileID;
    int rc = _bpm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }
    int index;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(tableName, tableList->tableName[i]) == 0) {
        res = true;
        break;
      }
    }
    _bpm->release(index);
    _bpm->closeFile(fileID);
    return res;
  }

  bool SM_Manager::AttrExist(const char *tableName, const char *attrName) {
    bool res = false;
    int fileID;
    int index;
    std::string infoName = std::string(tableName) + "_meta";
    _bpm->openFile(infoName.c_str(), fileID);

    TableInfo* tableInfo = (TableInfo*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableInfo->attrCount; ++i) {
      if(strcmp(attrName, tableInfo->attrInfos[i].attrName) == 0) {
        res = true;
        break;
      }
    }
    _bpm->release(index);
    _bpm->closeFile(fileID);
    return res;
  }

  bool SM_Manager::AttrListEqual(AttrList a, AttrList b) {
    if(a.attrCount != b.attrCount) {
      return false;
    }
    std::set<std::string> cmp;
    for(int i = 0; i < a.attrCount; ++i) {
      cmp.insert(std::string(a.names[i]));
    }
    for(int i = 0; i < b.attrCount; ++i) {
      if(cmp.count(std::string(b.names[i])) == 0) {
        return false;
      }
    }
    return true;
  }


}