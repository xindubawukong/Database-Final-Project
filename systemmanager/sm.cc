#include "sm.h"
#include "unistd.h"
#include <cstring>

namespace systemmanager {
  using filesystem::FileManager;
  using filesystem::BufPageManager;
  using indexing::IX_Manager;
  using recordmanager::RM_Manager;

  void SM_PrintError(int rc) {
    printf("SM error %d.\n", &rc);
  }

  SM_Manager* SM_Manager::getInstance() {
    if(_sm == nullptr) {
      _fm = new FileManager();
      _bpm = new BufPageManager(_fm);
      _ixm = new IX_Manager(_fm, _bpm);
      _rmm = new RM_Manager(_fm, _bpm);
      _sm = new SM_Manager(_fm, _bpm, _ixm, _rmm);
    }
    return _sm;
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
        if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) {
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

    int fileID;
    int rc = _fm->openFile("DatabaseList", fileID);
    if(rc != 0) {
      return rc;
    }

    int index;
    DatabaseList* dbList = (DatabaseList*) _bpm->getPage(fileID, 0, index);
    for(int i = 0; i < dbList->databaseCount; ++i) {
      if(strcmp(dbName, dbList->databaseName[i])) {
        if(dbList->databaseCount > 1) {
          memcpy(dbList->databaseName[i], dbList->databaseName[dbList->databaseCount - 1], MAX_LENGTH);
          dbList->databaseCount--;
          break;
        } else {
          memset(dbList, 0, sizeof(DatabaseList));
          break;
        }
      }
    }
    _bpm->markDirty(index);
    _bpm->writeBack(index);
    _fm->closeFile(fileID);
    return 0;
  }

  int SM_Manager::CloseDb() {
    int rc = chdir("..");
    return rc;
  }

  int SM_Manager::CreateTable(const char *relName, int attrCount, AttrInfo* attrInfos, int constraintCount, Constraint* constraints) {

  }

  int SM_Manager::AlterTable() {

  }

  int SM_Manager::DropTable(const char* relName) {

  }

  int SM_Manager::CreateIndex(const char* relName, const char* attrName) {

  }

  int SM_Manager::DropIndex(const char* relName, const char* attrName) {

  }


}