#include "sm.h"
#include "unistd.h"
#include <cstring>
#include <set>

namespace systemmanager {
  using filesystem::FileManager;
  using filesystem::BufPageManager;
  using indexing::IX_Manager;
  using recordmanager::RM_Manager;

  void SM_PrintError(int rc) {
    printf("SM error %d.\n", rc);
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
    // delete _rmm;
  }

  int SM_Manager::CreateDb(const char* dbName) {
    int rc = mkdir(dbName, S_IRWXU);
    if(rc != 0) {
      return rc;
    }
    chdir(dbName);
    _fm->createFile("TableList");
    chdir("..");
    return 0;
  } 

  int SM_Manager::OpenDb(const char* dbName) {
    int rc = chdir(dbName);
    return rc;
  }

  int SM_Manager::DropDb(const char* dbName) {
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
    rc = _fm->openFile("TableList", fileID);
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
    _fm->closeFile(fileID);

    rc = chdir("..");
    return rc;
  }

  int SM_Manager::ShowAllDb() {
    return 0;
  }

  int SM_Manager::CreateTable(const char *relName, int attrCount, AttrInfo* attrInfos, int constraintCount, Constraint* constraints) {
    int fileID;
    int rc = _fm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }
    int index;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(relName, tableList->tableName[i])) {
        return -1;
      }
    }

    if(tableList->tableCount == MAX_TABLES) {
      return -1;
    }
    _bpm->release(index);
    _fm->closeFile(fileID);

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
            if(strcmp(attrInfos[k].attrName, constraints[i].thisNameList.names[j])) {
              find = true;
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

        for(int j = 0; j < constraints[i].referencesNameList.attrCount; ++i) {
          if(!AttrExist(constraints[i].foreignTableName, 
          constraints[i].referencesNameList.names[j])) {
            return -1;
          }
        }
      }
    }

    std::string metaFile = std::string(relName) + "_meta";
    rc = _fm->createFile(metaFile.c_str());
    if(rc == 0) {
      return -1;
    }

    TableInfo* info = new TableInfo();
    info->attrCount = attrCount;
    memcpy(info->attrInfos, attrInfos, sizeof(AttrInfo) * attrCount);
    info->constraintCount = constraintCount;

    _fm->openFile(metaFile.c_str(), fileID);
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
    _fm->closeFile(fileID);

    std::string dataFile = std::string(relName) + "_data";
    rc = _fm->createFile(dataFile.c_str());
    if(rc == 0) {
      return -1;
    }

    _fm->openFile("TableList", fileID);
    tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    memcpy(tableList->tableName[++tableList->tableCount], relName, std::string(relName).size());
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _fm->closeFile(fileID);

    return 0;

  }

  int SM_Manager::AlterTable() {
    return 0;

  }

  int SM_Manager::DropTable(const char* relName) {
    int fileID;
    int index;
    int rc = _fm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }
    bool find = false;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(tableList->tableName[i], relName)) {
        find = true;
        memcpy(tableList->tableName[i], tableList->tableName[tableList->tableCount - 1], MAX_LENGTH);
        tableList->tableCount--;
        break;
      }
    }
    if(!find) {
      return -1;
    }
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _fm->closeFile(fileID);

    std::string metaFile = std::string(relName) + "_meta";
    rc = _fm->openFile(metaFile.c_str(), fileID);
    if(rc == 0) {
      return -1;
    }

    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < info->indexSize; ++i) {
      std::string indexName = std::string(relName) + "." + std::to_string(info->indexedAttr[i]);
      _fm->deleteFile(indexName.c_str());
    }

    _fm->deleteFile(metaFile.c_str());
    std::string dataFile = std::string(relName) + "_data";
    _fm->deleteFile(dataFile.c_str());

    return 0;
  }

  int SM_Manager::CreateIndex(const char* relName, const char* attrName) {
    if(!TableExist(relName)) {
      return -1;
    }
    int fileID, index;
    std::string metaFile = std::string(relName) + "_meta";
    int rc = _fm->openFile(metaFile.c_str(), fileID);
    if(rc == 0) {
      return -1;
    }
    TableInfo *info = (TableInfo*) _bpm->getPage(fileID, 0, index);
    int attrIndex = -1;
    for(int i = 0; i < info->attrCount; ++i) {
      if(strcmp(info->attrInfos[i].attrName, attrName)) {
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
    return 0;

  }

  int SM_Manager::DropIndex(const char* relName, const char* attrName) {
    return 0;

  }

  bool SM_Manager::TableExist(const char *tableName) {
    bool res = false;
    int fileID;
    int rc = _fm->openFile("TableList", fileID);
    if(rc == 0) {
      return -1;
    }
    int index;
    TableList* tableList = (TableList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableList->tableCount; ++i) {
      if(strcmp(tableName, tableList->tableName[i])) {
        res = true;
        break;
      }
    }
    _bpm->release(index);
    _fm->closeFile(fileID);
    return res;
  }

  bool SM_Manager::AttrExist(const char *tableName, const char *attrName) {
    bool res = false;
    int fileID;
    int index;
    std::string infoName = std::string(tableName) + "_meta";
    _fm->openFile(infoName.c_str(), fileID);

    TableInfo* tableInfo = (TableInfo*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < tableInfo->attrCount; ++i) {
      if(strcmp(attrName, tableInfo->attrInfos[i].attrName)) {
        res = true;
        break;
      }
    }
    _bpm->release(index);
    _fm->closeFile(fileID);
    return res;
  }


}