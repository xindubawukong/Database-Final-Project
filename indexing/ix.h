#ifndef INDEXING_IX_H
#define INDEXING_IX_H

#include <functional>

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../recordmanager/rm_rid.h"
#include "../utils/bitmap.h"
#include "../global.h"
#include "btree_node.h"

namespace indexing {

  void IX_PrintError(int rc);

  struct IX_FileHeader {
    int numPages;
    int rootPageNum;
    int capacity;
    int height;
    AttrType attrType;
    int attrLength;
    int maxPageNum;
    int pageNumBitMap[1024];
  };

  class IX_IndexHandle {
    public:
      friend class IX_Manager;
      
      IX_IndexHandle();

      ~IX_IndexHandle();

      int InsertEntry(void *pData, recordmanager::RID& rid);

      int DeleteEnrty(void* pData, recordmanager::RID& rid);

      int ForcePages();

      // int Search(void* pData, recordmanager::RID &rid);

      int Open(filesystem::BufPageManager *bpm, int fileID, int rootPage);
      int GetFileHeader(const int &pageNum);
      int SetFileHeader(const int &pageNum) const;

      int GetNumPages() const { return header.numPages; }
      AttrType GetAttrType() const { return header.attrType; }
      int GetAttrLength() const { return header.attrLength; }

      int GetNewPage(int& pageNum);
      int DisposePage(const int& pageNum);

      int IsValid() const;


      BTreeNode* FindLeaf(const void *pData);
      BTreeNode* FindSmallestLeaf();
      BTreeNode* FindLargestLeaf();
      BTreeNode* ScanLeftFind(BTreeNode* right,
                                void *pData,
                                const recordmanager::RID& rid);
      BTreeNode* FindLeafForceRight(const void* pData);
      BTreeNode* FindLeafForceLeft(const void* pData);

      BTreeNode* FetchNode(recordmanager::RID r) const;
      BTreeNode* FetchNode(int p) const;

      int GetHeight() const;
      void SetHeight(const int&);

      //BTreeNode* GetRoot() const;

      // int Pin(int p);
      int Release(int p);
      int WriteBack(int p);

    private:
      //Unpinning version that will unpin after every call correctly
      // int GetThisPage(int p, filesystem::BufPageManager *bpm) const;

      IX_FileHeader header;
      bool bFileOpen;
      bool bHeaderChanged;
      filesystem::BufPageManager * bpm;
      int fileID;
      
      BTreeNode * root; // root in turn points to the other nodes
      BTreeNode ** path; // list of nodes that is the path to leaf as a
                        // result of a search.
      int* pathPos; // the position in the parent in the path the points to
                  // the child node.

      void * treeLargest; // largest key in the entire tree
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

      int OpenScan(const IX_IndexHandle &indexHandle, CompOp comOp, void *value, bool desc=false);

      int GetNextEntry(recordmanager::RID &rid);

      int GetNextEntry(void* &key, recordmanager::RID &r, int& numScanned);

      int CloseScan();

      std::function<bool(void*, void*)> GetCheckFunction(AttrType attrType, int attrLength, CompOp comOp);

      // for iterator to reset state for another open/close
      int ResetState();

      //bool IsOpen() const { return (bOpen && pixh != NULL); }
      //bool IsDesc() const { return desc; }
      private:
      int OpOptimize(); // Optimizes based on value of c, value and resets state
      int EarlyExitOptimize(void* now);

    private:
      std::function<bool(void*, void*)> checkFunc;
      IX_IndexHandle* pixh;
      BTreeNode* curNode;
      int curPos;
      void* curKey; // saved to check for delete on scan
      recordmanager::RID curRid; // saved to check for delete on scan
      bool bOpen;
      bool desc; // Is scan order ascending(def) or descending ?
      bool eof; // early EOF set by btree analysis - set by OpOpt
      bool foundOne; // flag that is set by getNext if it is ever successful
      BTreeNode* lastNode; // last node setup by OpOpt
      CompOp c; // save Op for OpOpt
      void* value; // save Op for OpOpt
  };

}

#endif