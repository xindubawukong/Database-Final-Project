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

    friend class BtreeNodeTest;
    friend class IX_IndexHandle;
    int IsValid() const;
    int GetMaxKeys() const;
    
    // structural setters/getters - affect PF_page composition
    int GetNumKeys();
    int SetNumKeys(int newNumKeys);
    int GetLeft();
    int SetLeft(int pageNum);
    int GetRight();
    int SetRight(int pageNum);


    int GetKey(int pos, void* &key) const;
    int SetKey(int pos, const void* newkey);

    // return 0 if insert was successful
    // return -1 if there is no space
    int Insert(const void* newKey, const recordmanager::RID& newRid);

  // return 0 if remove was successful
  // return -1 if key does not exist
  // kpos is optional - will remove from that position if specified
  // if kpos is specified newkey can be NULL
    int Remove(const void* newkey);
    int Remove(const int pos);
    int RangeRemove(const int startPos, const int endPos);
    
    // exact match functions

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

    // split or merge this node with rhs node
    int Split(BTreeNode* rhs);
    int Merge(BTreeNode* rhs);

    // print
    // void Print(ostream & os);

    // get/set pageRID
    recordmanager::RID GetPageRID() const;
    void SetPageRID(const recordmanager::RID&);

    int CmpKey(const void * k1, const void * k2) const;
    // bool isSorted() const;
    int LargestKey(void* &key) const;

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