#ifndef INDEXING_IX_H
#define INDEXING_IX_H

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "recordmanager/rm_rid.h"
#include "global.h"
#include "btree_node.h"

namespace indexing {

  void IX_PrintError(int rc);

  struct IX_FileHeader {
    int numPages;
    int rootPageNum;
    int pairSize;
    int capacity;
    int height;
    AttrType attrType;
    int attrLength;
  };

  class IX_IndexHandle {
    public:
      IX_IndexHandle();

      ~IX_IndexHandle();

      int InsertEntry(void *pData, recordmanager::RID& rid);

      int DeleteEnrty(void* pData, recordmanager::RID& rid);

      int ForcePages();

      int Search(void* pData, recordmanager::RID &rid);

      int Open(filesystem::BufPageManager *bpm, int fileID, int pairSize, int rootPage);
      int GetFileHeader(const int &pageNum);
      int SetFileHeader(const int &pageNum) const;

      bool HdrChanged() const { return bHdrChanged; }
      int GetNumPages() const { return header.numPages; }
      AttrType GetAttrType() const { return header.attrType; }
      int GetAttrLength() const { return header.attrLength; }

      int GetNewPage(int& pageNum);
      int DisposePage(const int& pageNum);

      int IsValid() const;

      // return NULL if the key is bad
      // otherwise return a pointer to the leaf node where key might go
      // also populates the path member variable with the path
      BTreeNode* FindLeaf(const void *pData);
      BTreeNode* FindSmallestLeaf();
      BTreeNode* FindLargestLeaf();
      BTreeNode* DupScanLeftFind(BTreeNode* right,
                                void *pData,
                                const recordmanager::RID& rid);
      // hack for indexscan::OpOptimize
      BTreeNode* FindLeafForceRight(const void* pData);

      BTreeNode* FetchNode(recordmanager::RID r) const;
      BTreeNode* FetchNode(int p) const;
      void ResetNode(BTreeNode*& old, int p) const;
      // Reset to the BTreeNode at the recordmanager::RID specified within Btree
      void ResetNode(BTreeNode*& old, recordmanager::RID r) const;

      // get/set height
      int GetHeight() const;
      void SetHeight(const int&);

      BTreeNode* GetRoot() const;

      int Pin(int p);
      int UnPin(int p);

    private:
      //Unpinning version that will unpin after every call correctly
      int GetThisPage(int p, filesystem::BufPageManager *bpm) const;

      IX_FileHeader header;
      bool bFileOpen;
      filesystem::BufPageManager * bpm;
      int fileID;
      bool bHdrChanged;
      BTreeNode * root; // root in turn points to the other nodes
      BTreeNode ** path; // list of nodes that is the path to leaf as a
                        // result of a search.
      int* pathP; // the position in the parent in the path the points to
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

      int OpenScan(const IX_IndexHandle &indexHandle, CompOp comOp, void *value);

      int GetNextEntry(recordmanager::RID rid);

      int CloseScan();
  };

}

#endif