#include "btree_node.h"
#include "return_code.h"

#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <memory.h>

namespace indexing {

BTreeNode::BTreeNode(AttrType attrType, int attrLength,
                    filesystem::BufPageManager *bpm, int fileID, 
                    int pageID, bool newPage) 
: keys(nullptr), rids(nullptr), attrLength(attrLength), attrType(attrType) {

  capacity = (int) std::floor(
    (kPageSize - sizeof(numKeys) - 2 * sizeof(int)) / 
    (attrLength + sizeof(recordmanager::RID))
  );

  char *pData = nullptr;
  int index;
  pData = (char*) bpm->getPage(fileID, pageID, index);
  

  keys = pData;
  rids = (recordmanager::RID*) (pData + capacity * attrLength);
  
  SetPageRID(recordmanager::RID(pageID, -1));

  if(newPage) {
    SetLeft(-1);
    SetRight(-1);
    SetNumKeys(0);
  } else {
    GetLeft();
    GetRight();
    GetNumKeys();
  }
  // std::cout << fileID << "," << pageID << "," << index << std::endl;
  assert(IsValid() == 0);
}

BTreeNode::~BTreeNode() {
  // keys = nullptr;
  // rids = nullptr;
}

int BTreeNode::Destroy() {
  assert(IsValid() == 0);
  if(numKeys != 0) {
    return -1;
  }
  keys = nullptr;
  rids = nullptr;
 
  return NO_ERROR;
}

int BTreeNode::IsValid() const {
  if(capacity <= 0) {
    return -1;
  }

  bool valid = (keys != nullptr) && (rids != nullptr) &&
               (numKeys <= capacity) && (numKeys >= 0);
  if(!valid) {
    return -2;
  }
  return NO_ERROR;
}

int BTreeNode::GetMaxKeys() const {
  return capacity;
}

int BTreeNode::GetNumKeys() {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity;
  numKeys = *((int*) location);
  return numKeys;
}

int BTreeNode::SetNumKeys(int newNumKeys) {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity;
  memcpy(location, &newNumKeys, sizeof(int));
  numKeys = newNumKeys;
  assert(IsValid() == 0);
  return 0;
}

int BTreeNode::GetLeft() {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity + sizeof(int);
  return *((int*)location);
}

int BTreeNode::SetLeft(int pageNum) {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity + sizeof(int);
  memcpy(location, &pageNum, sizeof(int));
  return 0;
}

int BTreeNode::GetRight() {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity + 2 * sizeof(int);
  return *((int*)location);
}

int BTreeNode::SetRight(int pageNum) {
  void* location = (char*)rids + sizeof(recordmanager::RID) * capacity + 2 * sizeof(int);
  memcpy(location, &pageNum, sizeof(int));
  return 0;
}

int BTreeNode::GetKey(int pos, void* &key) const {
  assert(IsValid() == 0);
  assert(pos >=0 && pos < numKeys);
  key = keys + attrLength * pos;
  return NO_ERROR;
}

int BTreeNode::SetKey(int pos, const void* newKey) {
  assert(IsValid() == 0);
  assert(pos >=0 && pos <capacity);

  memcpy(keys + attrLength * pos, newKey, attrLength);
  return NO_ERROR;
}

int BTreeNode::Insert(const void* newKey, const recordmanager::RID& newRid) {
  assert(IsValid() == 0);
  if(numKeys == capacity) return -1;
  int pos = -1;
  void* curKey = nullptr;
  for(pos = numKeys - 1; pos >= 0; --pos) {
    curKey = keys + attrLength * pos;
    if(CmpKey(newKey, curKey) >= 0) {
      break;
    }
    rids[pos + 1] = rids[pos];
    SetKey(pos + 1, curKey);
  }

  rids[pos + 1] = newRid;
  SetKey(pos + 1, newKey);
  SetNumKeys(numKeys + 1);
  return NO_ERROR;
}

int BTreeNode::Remove(const void* key) {
  assert(IsValid() == 0);
  int pos = FindKey(key);
  if(pos < 0) return -1;

  void* nextKey = nullptr;
  for(; pos < numKeys - 1; ++pos) {
    GetKey(pos + 1, nextKey);
    SetKey(pos, nextKey);
    rids[pos] = rids[pos + 1];
  }
  SetNumKeys(numKeys - 1);
  return NO_ERROR;
}

int BTreeNode::Remove(const int pos) {
  assert(IsValid() == 0);
  if(pos < 0 || pos >= numKeys) {
    return -1;
  }

  void* nextKey = nullptr;
  for(int i = pos; i < numKeys - 1; ++i) {
    GetKey(i + 1, nextKey);
    SetKey(i, nextKey);
    rids[i] = rids[i + 1];
  }
  SetNumKeys(numKeys - 1);
  return NO_ERROR;
}

int BTreeNode::RangeRemove(const int startPos, const int endPos) {
  assert(IsValid() == 0);
  if(startPos < 0 || startPos >= numKeys || endPos < 0 || endPos >= numKeys || startPos > endPos) {
    return -1;
  }

  int removeNums = endPos - startPos + 1;
  void* nextKey = nullptr;
  for(int i = startPos; i < numKeys - removeNums; ++i) {
    GetKey(i + removeNums, nextKey);
    SetKey(i, nextKey);
    rids[i] = rids[i + removeNums]; 
  }
  SetNumKeys(numKeys - removeNums);
  return NO_ERROR;
}

int BTreeNode::FindKey(const void* &key, const recordmanager::RID& r) const {
  assert(IsValid() == 0);
  void* curKey = nullptr;
  int pageNum;
  r.GetPageNum(pageNum);
  for(int i = numKeys - 1; i >= 0; --i) {
    GetKey(i, curKey);
    if(CmpKey(key, curKey) > 0) {
      return -1;
    }
    if(CmpKey(key, curKey) == 0) {
      if(pageNum != -1) {
        if(rids[i] == r) {
          return i;
        }
      } else {
        return i;
      }
    }
  }

  return -1;
}

int BTreeNode::GetAddrByKey(const void* &key, recordmanager::RID& r) const {
  assert(IsValid() == 0);
  int pos = FindKey(key);
  if(pos == -1) {
    r = recordmanager::RID(-1, -1);
    return 0;
  }
  else {
    return GetAddrByPosition(pos, r);
  }

}

int BTreeNode::GetAddrByPosition(const int pos, recordmanager::RID& r) const {
  assert(IsValid() == 0);
  if(pos >= numKeys || pos < 0) r = recordmanager::RID(-1, -1);
  else {
    r = rids[pos];
  }
  return 0;
}

int BTreeNode::FindPositionByKey(const void* &key) const {
  assert(IsValid() == 0);
  void* curKey = nullptr;
  for(int i = numKeys - 1; i >= 0; --i) {
    GetKey(i, curKey);
    if(CmpKey(key, curKey) == 0) {
      return i;
    } 

    if(CmpKey(key, curKey) > 0) {
      return i + 1;
    }

  }
  return 0;
}

int BTreeNode::FindAddrByKey(const void* &key, recordmanager::RID& r) const {
  assert(IsValid() == 0);
  int pos = FindPositionByKey(key);

  if(pos >= numKeys) {
    r = recordmanager::RID(-1, -1);
  }
  else r = rids[pos];
  return NO_ERROR;
}

int BTreeNode::Split(BTreeNode* other) {
  assert(IsValid() == 0);
  assert(other->IsValid() == 0);
  int startMovedPos = (numKeys + 1) >> 1;
  int numMoved = numKeys - startMovedPos;
  
  if(other->GetNumKeys() + numMoved > other->GetMaxKeys()) {
    return -1;
  }
  for(int i = startMovedPos; i < numKeys; ++i) {
    void* key = nullptr;
    GetKey(i, key);
    recordmanager::RID r = rids[i];
    other->Insert(key, r);
  }

  RangeRemove(startMovedPos, numKeys - 1);
  other->SetRight(this->GetRight());
  int otherPageNum;
  other->GetPageRID().GetPageNum(otherPageNum);
  this->SetRight(otherPageNum);
  int thisPageNum;
  this->GetPageRID().GetPageNum(thisPageNum);
  other->SetLeft(thisPageNum);

  assert(IsValid() == 0);
  assert(other->IsValid() == 0);
  return NO_ERROR;
}

int BTreeNode::Merge(BTreeNode* other) {
  assert(IsValid() == 0);
  assert(other->IsValid() == 0);

  if(numKeys + other->GetNumKeys() > capacity) {
    return -1;
  }

  for(int i = 0; i < other->GetNumKeys(); ++i) {
    void* key = nullptr;
    other->GetKey(i, key);
    recordmanager::RID r;
    other->GetAddrByPosition(i, r);
    this->Insert(key, r);
  }

  other->RangeRemove(0, other->GetNumKeys() - 1);

  int thisPageNum;
  this->GetPageRID().GetPageNum(thisPageNum);
  if(other->GetLeft() == thisPageNum) {
    this->SetRight(other->GetRight());
  } else {
    this->SetLeft(other->GetLeft());
  }

  assert(IsValid() == 0);
  assert(other->IsValid() == 0);

  return NO_ERROR;
}

recordmanager::RID BTreeNode::GetPageRID() const {
  return pageRID;
}

void BTreeNode::SetPageRID(const recordmanager::RID& r) {
  pageRID = r;
}

int BTreeNode::GetPageNum() const {
  int pageNum;
  pageRID.GetPageNum(pageNum);
  return pageNum;
}

int BTreeNode::CmpKey(const void* k1, const void* k2) const {
  assert(attrType >= INT && attrType <= STRING);

  if(attrType == STRING) {
    return memcmp(k1, k2, attrLength);
  }

  if(attrType == INT) {
    int value1 = *((int*)k1), value2 = *((int*)k2);
    if(value1 < value2) return -1;
    if(value1 == value2) return 0;
    return 1; 
  } 

  if(attrType == FLOAT) {
    float value1 = *((float*)k1), value2 = *((float*)k2);
    if(value1 < value2) return -1;
    if(value1 == value2) return 0;
    return 1;
  }

  return 0;
}

void* BTreeNode::LargestKey() const {
  assert(IsValid() == 0);
  if(numKeys > 0) {
    return keys + attrLength * (numKeys - 1);
  } else {
    return nullptr;
  }
  
}

}
