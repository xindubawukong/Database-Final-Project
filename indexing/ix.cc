#include "ix.h"

#include <cstdio>
#include <cstring>

#include "return_code.h"

namespace indexing {

void IX_PrintError(int rc) { fprintf(stderr, "IX error %d.\n", rc); }

IX_IndexHandle::IX_IndexHandle() {
  bFileOpen = false;
  bpm = nullptr;
  bHdrChanged = false;
  root = nullptr;
  path = nullptr;
  pathP = nullptr;
  treeLargest = nullptr;
  header.height = 0;
}

IX_IndexHandle::~IX_IndexHandle() {
  if(root != nullptr && bpm != nullptr) {
    int index;
    bpm->getPage(fileID, header.rootPageNum, index);
    bpm->release(index);
    delete root;
    root = nullptr;
  }

  if(pathP != nullptr) {
    delete[] pathP;
    pathP = nullptr;
  }

  if(path != nullptr) {
    for(int i = 0; i < header.height; ++i) {
      if(path[i] != nullptr) {
        if(bpm != nullptr) {
          UnPin(path[i]->GetPageNum());
          delete path[i];
        }
      }
      delete[] path;
      path = nullptr;
    }
  }

  if(bpm != nullptr) {
    delete bpm;
    bpm = nullptr;
  }

  if(treeLargest != nullptr) {
    delete treeLargest;
    treeLargest = nullptr;
  }
}

int IX_IndexHandle::InsertEntry(void* pData, recordmanager::RID &r) {
  if(pData == nullptr) {
    return -1;
  }

  bool newLargest = false;
  void* prevKey = nullptr;
  int level = header.height - 1;
  BTreeNode* node = FindLeaf(pData);
  BTreeNode* newNode = nullptr;

  if(node->FindKey((const void* &)pData) != -1) {
    return -2;
  }

  if(node->GetNumKeys() == 0 || node->CmpKey(pData, treeLargest) > 0) {
    newLargest = true;
    prevKey = treeLargest;
  }

  int ret = node->Insert(pData, r);

  if(newLargest) {
    for(int i = 0; i < header.height - 1; ++i) {
      int pos = path[i]->FindKey((const void* &)prevKey);
      path[i]->SetKey(pos, pData);
      
    }

    memcpy(treeLargest, pData, header.attrLength);
  }

  void* failedKey = pData;
  recordmanager::RID failRid = r;
  while(ret != 0) {
    char* charPtr = new char[header.attrLength];
    void* oldLargest = charPtr;
    if(node->GetNumKeys() == 0) {
      oldLargest = nullptr;
    } else {
      node->GetKey(node->GetNumKeys() - 1, oldLargest);
    }
    delete[] charPtr;

    int pageNum;
    GetNewPage(pageNum);
    int index;
    bpm->getPage(fileID, pageNum, index);

    newNode = new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum);

    node->Split(newNode);

    BTreeNode* curRight = FetchNode(newNode->GetRight());
    if(curRight != nullptr) {
      curRight->SetLeft(newNode->GetPageNum());
      delete curRight;
    }

    BTreeNode* nodeInsertInto = nullptr;
    void* largestKey = nullptr;
    node->GetKey(node->GetNumKeys() - 1, largestKey);
    if(node->CmpKey(pData, largestKey) >= 0) {
      newNode->Insert(failedKey, failRid);
      nodeInsertInto = newNode;
    } else {
      node->Insert(failedKey, failRid);
      nodeInsertInto = node;
    }

    level--;
    if(level < 0) break;
    int posAtParent = pathP[level];
    BTreeNode* parent = path[level];
    parent->Remove(posAtParent);
    ret = parent->Insert(node->LargestKey(), node->GetPageRID());
    ret = parent->Insert(newNode->LargestKey(), newNode->GetPageRID());
    node = parent;
    failedKey = newNode->LargestKey();
    failRid = newNode->GetPageRID();

    delete newNode;
    newNode = nullptr;
  }

  if(level >= 0) {
    return NO_ERROR;
  } else {
    UnPin(header.rootPageNum);

    int pageNum;
    int index;
    GetNewPage(pageNum);
    bpm->getPage(fileID, pageNum, index);

    root = new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum);

    root->Insert(node->LargestKey(), node->GetPageRID());
    root->Insert(newNode->LargestKey(), newNode->GetPageRID());

    header.rootPageNum = root->GetPageNum();
    int index;
    bpm->getPage(fileID, header.rootPageNum, index);

    if(newNode != nullptr) {
      delete newNode;
      newNode = nullptr;
    }

    SetHeight(header.height + 1);
    return NO_ERROR;
  }
}

BTreeNode* IX_IndexHandle::DupScanLeftFind(BTreeNode* right, void* pData, const recordmanager::RID &r) {
  BTreeNode* curNode = FetchNode(right->GetLeft());
  while(curNode != nullptr) {
    int i = curNode->GetNumKeys() - 1;
    for(; i >= 0; --i) {
      void* key = nullptr;
      int ret = curNode->GetKey(i, key);
      // ???
      if(curNode->CmpKey(pData, key) < 0) {
        continue;
      }

      if(curNode->CmpKey(pData, key) > 0) {
        return nullptr;
      }

      recordmanager::RID curRid;
      curNode->GetAddrByPosition(i, curRid);
      if(curRid == r) {
        return curNode;
      }
      
    }
    curNode = FetchNode(curNode->GetLeft());
  }
  return nullptr;
}

int IX_IndexHandle::DeleteEnrty(void* pData, recordmanager::RID &r) {
  if(pData == nullptr) {
    return -1;
  }

  bool delLargest = false;
  BTreeNode* node = FindLeaf(pData);
  int pos = node->FindKey((const void* &)pData, r);
  if(pos == -1) {
    int p = node->FindKey((const void* &)pData);
    if(p != -1) {
      BTreeNode* others = DupScanLeftFind(node, pData, r);
      if(others != nullptr) {
        int removedPos = others->FindKey((const void* &)pData, r);
        others->Remove(removedPos);
        return NO_ERROR;
      }
    }

    return -2;
  } else if(pos == node->GetNumKeys() - 1) {
    delLargest = true;
  }

  if(delLargest) {
    for(int i = header.height - 2; i >= 0; --i) {
      int pos = path[i]->FindKey((const void* &)pData);

      if(pos != -1) {
        void* leftKey = nullptr;
        leftKey = path[i + 1]->LargestKey();
        if(node->CmpKey(pData, leftKey) == 0) {
          int pos = path[i + 1]->GetNumKeys() - 2;
          if(pos < 0) {
            continue;
          }
          path[i + 1]->GetKey(path[i + 1]->GetNumKeys() - 2, leftKey);
        }

        if(i == header.height - 2 || pos == path[i]->GetNumKeys() - 1) {
          path[i]->SetKey(pos, leftKey);
        }
      }
    }
  }

  int ret = node->Remove(pos);
  int level = header.height - 1;
  while(ret != 0) {
    level--;
    if(level < 0) {
      break;
    }

    int posAtParent = pathP[level];
    BTreeNode* parent = path[level];

    if(level == 0 && parent->GetNumKeys() == 1 && ret == 0) {
      ret = -1;
    }

    BTreeNode* left = FetchNode(node->GetLeft());
    BTreeNode* right = FetchNode(node->GetRight());
    if(left != nullptr) {
      if(right != nullptr) {
        left->SetRight(right->GetPageNum());
      } else {
        left->SetRight(-1);
      }
    }

    if(right != nullptr) {
      if(left != nullptr) {
        right->SetLeft(left->GetPageNum());
      } else {
        right->SetLeft(-1);
      }
    }

    if(right != nullptr) {
      delete right;
    }
    if(left != nullptr) {
      delete left;
    }

    node->Destroy();
    DisposePage(node->GetPageNum());
    node = parent;

  }

  if(level >= 0) {
    return NO_ERROR;
  } else {
    if(header.height == 1) {
      return NO_ERROR;
    }
    recordmanager::RID first;
    root->GetAddrByPosition(0, first);
    root = FetchNode(first);
    header.rootPageNum = root->GetPageNum();
    int index;
    bpm->getPage(fileID, header.rootPageNum, index);

    node->Destroy();
    DisposePage(node->GetPageNum());

    SetHeight(header.height - 1);
    return NO_ERROR;
  }
}


int IX_IndexHandle::GetNewPage(int &pageNum) {
  int index;
  bpm->allocPage(fileID, pageNum, index);
  UnPin(pageNum);
  header.numPages++;
  bHdrChanged = true;
  return NO_ERROR;
}

int IX_IndexHandle::GetFileHeader(const int& pageNum) {
  int index;
  char* buf = (char*) bpm->getPage(fileID, pageNum, index);
  memcpy(&header, buf, sizeof(header));
  return NO_ERROR;
}

int IX_IndexHandle::Open(filesystem::BufPageManager* bpm, int fileID, int pairSize, 
int rootPage) {
  if(bFileOpen || this->bpm != nullptr) {
    return -1;
  }

  if(bpm == nullptr || pairSize <= 0) {
    return -2;
  }
  bFileOpen = true;
  this->bpm = bpm;
  this->fileID = fileID;
  int index;
  bpm->getPage(fileID, 0, index);
  this->GetFileHeader(0);

  bool newPage = true;
  if(header.height > 0) {
    SetHeight(header.height);
    newPage = false;
    bpm->getPage(fileID, rootPage, index);
  } else {
    int newPageNum;
    // TO DO
    this->GetNewPage(newPageNum);
    header.rootPageNum = newPageNum;
    SetHeight(1);
  }

  // TO DO
  root = new BTreeNode(header.attrType, header.attrLength, bpm, fileID, rootPage, newPage);
  path[0] = root;
  header.capacity = root->GetMaxKeys();
  bHdrChanged = true;
  treeLargest = (void*) new char[header.attrLength];
  if(!newPage) {
    BTreeNode* node = FindLargestLeaf();

    if(node->GetNumKeys() > 0) {
      void* key = nullptr;
      node->GetKey(node->GetNumKeys() - 1, key);
      memcpy(treeLargest, key, header.attrLength);
    }
  }
  return NO_ERROR;
}

int IX_IndexHandle::SetFileHeader(const int &pageNum) const {
  int index;
  char* buf = (char*) bpm->getPage(fileID, pageNum, index);
  memcpy(buf, &header, sizeof(header));
  return NO_ERROR;
}

int IX_IndexHandle::ForcePages() {
  return NO_ERROR;
}

int IX_IndexHandle::DisposePage(const int& pageNum) {
  return NO_ERROR;
}

BTreeNode* IX_IndexHandle::FindLeaf(const void* pData) {
  if(root == nullptr) {
    return nullptr;
  }

  if(header.height == 1) {
    path[0] = root;
    return root;
  }

  for(int i = 1; i < header.height; ++i) {
    recordmanager::RID r;
    path[i - 1]->FindAddrByKey(pData, r);
    int pos = path[i - 1]->FindPositionByKey(pData);

    if(pos == path[i-1]->GetNumKeys()) {
      pos = path[i - 1]->GetNumKeys() - 1;
      path[i - 1]->GetAddrByPosition(pos, r);
    }
    if(path[i] != nullptr) {
      UnPin(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    int index;
    bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathP[i - 1] = pos;
  }

  return path[header.height - 1];
}

BTreeNode* IX_IndexHandle::FindSmallestLeaf() {
  if(root == nullptr) {
    return nullptr;
  }

  if(header.height == 1) {
    path[0] = root;
    return root;
  }

  for(int i = 1; i < header.height; ++i) {
    recordmanager::RID r;
    path[i - 1]->GetAddrByPosition(0, r);
    if(r == recordmanager::RID(-1, -1)) {
      return nullptr;
    }
    if(path[i] != nullptr) {
      UnPin(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    int index;
    bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathP[i - 1] = 0;
  }

  return path[header.height - 1];
}

BTreeNode* IX_IndexHandle::FindLargestLeaf() {
  if(root == nullptr) {
    return nullptr;
  }

  if(header.height == 1) {
    path[0] = root;
    return root;
  }

  for(int i = 1; i < header.height; ++i) {
    recordmanager::RID r;
    path[i - 1]->GetAddrByPosition(path[i - 1]->GetNumKeys() - 1, r);
    if(r == recordmanager::RID(-1, -1)) {
      return nullptr;
    }
    if(path[i] != nullptr) {
      UnPin(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    int index;
    bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathP[i - 1] = path[i - 1]->GetNumKeys() - 1;
  }

  return path[header.height - 1];
}

BTreeNode* IX_IndexHandle::FetchNode(recordmanager::RID r) const {
  int pageNum;
  r.GetPageNum(pageNum);
  if(pageNum < 0) {
    return nullptr;
  }
  int index;
  bpm->getPage(fileID, pageNum, index);
  return new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum, false);
}

BTreeNode* IX_IndexHandle::FetchNode(int pageNum) const {
  if(pageNum < 0) {
    return nullptr;
  }
  int index;
  bpm->getPage(fileID, pageNum, index);
  return new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum, false);
}

int IX_IndexHandle::UnPin(int pageNum) {
  if(bpm == nullptr) {
    return -1;
  }
  int index;
  bpm->getPage(fileID, pageNum, index);
  bpm->release(index);
  return NO_ERROR;
}

}