#ifndef INDEXING_IX_H
#define INDEXING_IX_H

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "recordmanager/rm_rid.h"
#include "global.h"

namespace indexing {

  void IX_PrintError(int rc);

  class IX_IndexHandle {
    public:
      IX_IndexHandle();

      ~IX_IndexHandle();

      int InsertEntry(void *pData, recordmanager::RID rid);

      int DeleteEnrty(void* pData, recordmanager::RID rid);

      int ForcePages();

  };

  class IX_Manager {
    public:
      IX_Manager(filesystem::FileManager *fm, filesystem::BufPageManager *bpm);

      ~IX_Manager();

      int CreateIndex(const char *filename, int indexNo, AttrType attrType,  int attrLength);

      int DestroyIndex(const char *filename, int indexNo);

      int OpendIndex(const char *filename, int indexNo, IX_IndexHandle &index_handle);

      int CloseIndex(IX_IndexHandle &index_handle);
    
    private:
      filesystem::FileManager *fm_;
      filesystem::BufPageManager *bpm_;
  };

  
  class IX_IndexScan {
    public:
      IX_IndexScan();

      ~IX_IndexScan();

      int OpenScan(const IX_IndexHandle &indexHandle, CompOp comOp, void *value);

      int GetNextEntry(recordmanager::RID rid);

      int CloseScan();
  };

}

#endif