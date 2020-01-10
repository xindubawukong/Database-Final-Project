#ifndef SYSTEMMANAGER_SM_H
#define SYSTEMMANAGER_SM_H

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "indexing/ix.h"
#include "recordmanager/rm.h"
#include "global.h"

#include <cstdio>
#include <stdlib.h>

#define MAX_ATTRS 10
#define MAX_LENGTH 25
#define MAX_TABLES 20
#define MAX_DATABASES 20

namespace systemmanager {
  void SM_PrintError(int rc);

  struct AttrInfo {
    char attrName[MAX_LENGTH + 1];
    AttrType attrType;
    int attrLength;
    bool notNullFlag;
    int valueSize;
    void* defaultValue;
  };

  struct TableList {
    char tableName[MAX_TABLES][MAX_LENGTH];
    int tableCount;
  };

  struct DatabaseList {
    char databaseName[MAX_DATABASES][MAX_LENGTH];
    int databaseCount;
  };

  struct IdentList {
    char names[MAX_ATTRS][MAX_LENGTH];
    int attrCount;
  };

  struct Constraint {
    bool isPrimary;
    char constraintName[MAX_LENGTH];
    char foreiegnTableName[MAX_LENGTH];
    IdentList thisNameList, referencesNameList;
  };

  struct TableInfo {
    int attrCount;
    AttrInfo attrInfos[MAX_ATTRS];
    int indexedAttr[MAX_ATTRS];
    int indexSize;
    int constraintCount;
  };

  class SM_Manager {
    public:
      SM_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm, indexing::IX_Manager* ixm, recordmanager::RM_Manager* rmm);
      ~SM_Manager();

      int CreateDb(const char* dbName);
      int OpenDb(const char* dbName);
      int DropDb(const char* dbName);
      int CloseDb();
      int CreateTable(const char *relName, int attrCount, AttrInfo* attrInfos, int constraintCount, Constraint* constraints);
      int AlterTable();
      int DropTable(const char *relName);
      int CreateIndex(const char  *relName, const char *attrName);
      int DropIndex(const char *relName, const char *attrName);
      int Load(const char *relName, const char* fileName);
      int Help();
      int Help(const char *relName);
      int Print(const char *relName);
      int Set(const char *paramName, const char *value);

      static SM_Manager* getInstance();
    
    private:
      static filesystem::FileManager* _fm;
      static filesystem::BufPageManager* _bpm;
      static recordmanager::RM_Manager* _rmm;
      static indexing::IX_Manager* _ixm;
      static SM_Manager* _sm;

  };
}
#endif