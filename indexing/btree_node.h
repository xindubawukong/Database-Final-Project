#ifndef BTREE_NODE_H
#define BTREE_NODE_H

#include "filesystem/bufmanager/BufPageManager.h"
#include "filesystem/fileio/FileManager.h"
#include "global.h"
#include "recordmanager/rm_rid.h"

namespace indexing {

class BTreeNode {
  public:
    BTreeNode(AttrType attrType, int attrLength, 
    filesystem::BufPageManager *bpm, int fileID, int pageID, 
    bool newPage = true);

    ~BTreeNode();
    int Destroy();

    friend class IX_IndexHandle;
    int IsValid() const;
    int GetMaxKeys() const;
    
    int GetNumKeys();
    int SetNumKeys(int newNumKeys);
    int GetLeft();
    int SetLeft(int pageNum);
    int GetRight();
    int SetRight(int pageNum);


    int GetKey(int pos, void* &key) const;
    int SetKey(int pos, const void* newkey);

    int Insert(const void* newKey, const recordmanager::RID& newRid);
    int Remove(const void* newkey);
    int Remove(const int pos);
    int RangeRemove(const int startPos, const int endPos);
    

    // return position if key already exists at position
    // if there are dups - returns rightmost position unless an RID is
    // specified.
    // if optional RID is specified, will only return a position if both
    // key and RID match.
    // return -1 if there was an error or if key does not exist
    int FindKey(const void* &key, const recordmanager::RID& r = recordmanager::RID(-1,-1)) const;
    int GetAddrByKey(const void* &key, recordmanager::RID& r) const;
    int GetAddrByPosition(const int pos, recordmanager::RID& r) const;

    // find a poistion instead of exact match
    int FindPositionByKey(const void* &key) const;
    int FindAddrByKey(const void* &key, recordmanager::RID& r) const;

    int Split(BTreeNode* rhs);
    int Merge(BTreeNode* rhs);

    recordmanager::RID GetPageRID() const;
    void SetPageRID(const recordmanager::RID&);
    int GetPageNum() const;

    int CmpKey(const void * k1, const void * k2) const;
    void* LargestKey() const;

  private:
    char  *keys;
    recordmanager::RID *rids;
    int numKeys;
    int attrLength;
    AttrType attrType;
    int capacity;
    recordmanager::RID pageRID;

};

}
#endif