#ifndef SYSTEMMANAGER_SM_H
#define SYSTEMMANAGER_SM_H

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "indexing/ix.h"
#include "recordmanager/rm.h"
#include "global.h"
namespace systemmanager {
  void SM_PrintError(int rc);

  struct SM_AttrInfo {
    char* attrName;
    AttrType attrType;
    int attrLength;
    bool notNullFlag;
    void* defaultValue;
  };

  class SM_Manager {
    public:
      SM_Manager(indexing::IX_Manager &ixm, recordmanager::RM_Manager &rmm);
      ~SM_Manager();

      int OpenDb(const char* dbName);
      int CloseDb();
      int CreateTable(const char *relName, int attrCount, SM_AttrInfo *attributes);
      int DropTable(const char *relName);
      int CreateIndex(const char  *relName, const char *attrName);
      int DropIndex(const char *relName, const char *attrName);
      int Load(const char *relName, const char* fileName);
      int Help();
      int Help(const char *relName);
      int Print(const char *relName);
      int Set(const char *paramName, const char *value);
  };
}
#endif