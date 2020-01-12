#include "ix.h"

#include <cstdio>
#include <cstring>
#include <sstream>

#include "return_code.h"

namespace indexing {

void IX_PrintError(int rc) { fprintf(stderr, "IX error %d.\n", rc); }

IX_IndexHandle::IX_IndexHandle() {
  bFileOpen = false;
  bpm = nullptr;
  bHeaderChanged = false;
  root = nullptr;
  path = nullptr;
  pathPos = nullptr;
  treeLargest = nullptr;
  header.height = 0;
  header.maxPageNum = 0;
}

IX_IndexHandle::~IX_IndexHandle() {
  if(root != nullptr && bpm != nullptr) {
    // WriteBack(header.rootPageNum);
    delete root;
    root = nullptr;
  }

  if(pathPos != nullptr) {
    delete[] pathPos;
    pathPos = nullptr;
  }

  if(path != nullptr) {
    for(int i = 0; i < header.height; ++i) {
      if(path[i] != nullptr) {
        if(bpm != nullptr) {
          // WriteBack(path[i]->GetPageNum());
          // delete path[i];
          // path[i] = nullptr;
        }
      }
    }
    delete[] path;
    path = nullptr;
  }

  if(bpm != nullptr) {
    delete bpm;
    bpm = nullptr;
  }

  if(treeLargest != nullptr) {
    delete (char*)treeLargest;
    treeLargest = nullptr;
  }
}

int IX_IndexHandle::InsertEntry(void* pData, recordmanager::RID &r) {
  if(pData == nullptr) {
    return IX_KEY_NULL_ERROR;
  }

  bool newLargest = false;
  void* prevKey = nullptr;
  int level = header.height - 1;
  BTreeNode* node = FindLeaf(pData);
  BTreeNode* newNode = nullptr;

  
  if(node->GetNumKeys() == 0 || node->CmpKey(pData, treeLargest) > 0) {
    newLargest = true;
    prevKey = treeLargest;
  }

  int ret = node->Insert(pData, r);
  std::cout << "now: " << *(int*)prevKey << std::endl;

  if(newLargest) {
    for(int i = 0; i < header.height - 1; ++i) {
      int pos = path[i]->FindKey((const void* &)prevKey);
      std::cout << path[i]->GetNumKeys() << std::endl;
      std::cout << "Pos:" << pos << std::endl;
      path[i]->SetKey(pos, pData);
    }

    memcpy(treeLargest, pData, header.attrLength);
  }

  void* insertKey = pData;
  recordmanager::RID insertRid = r;
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
    std::cout << "why: " << newNode->GetNumKeys() << std::endl;

    BTreeNode* curRight = FetchNode(newNode->GetRight());
    if(curRight != nullptr) {
      curRight->SetLeft(newNode->GetPageNum());
      delete curRight;
    }

    void* largestKey = nullptr;
    node->GetKey(node->GetNumKeys() - 1, largestKey);
    if(node->CmpKey(pData, largestKey) >= 0) {
      newNode->Insert(insertKey, insertRid);
      std::cout <<"Yes" << std::endl;
      std::cout << *(int*)insertKey << std::endl;
    } else {
      node->Insert(insertKey, insertRid);
    }

    level--;
    if(level < 0) break;
    int posAtParent = pathPos[level];
    BTreeNode* parent = path[level];
    parent->Remove(posAtParent);
    ret = parent->Insert(node->LargestKey(), node->GetPageRID());
    ret = parent->Insert(newNode->LargestKey(), newNode->GetPageRID());
    std::cout << "large: " << *(int*)node->LargestKey() << ", " << *(int*)newNode->LargestKey() << std::endl;
    node = parent;
    insertKey = newNode->LargestKey();
    insertRid = newNode->GetPageRID();

    delete newNode;
    newNode = nullptr;
  }

if(level < 0 ) {
    // Release(header.rootPageNum);
    WriteBack(header.rootPageNum);

    int pageNum;
    int index;
    GetNewPage(pageNum);
    // bpm->getPage(fileID, pageNum, index);
    // std::cout << pageNum << "," << header.rootPageNum << std::endl;
  
    root = new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum);
    
    root->Insert(node->LargestKey(), node->GetPageRID());
    root->Insert(newNode->LargestKey(), newNode->GetPageRID());
    std::cout << newNode->GetNumKeys() << std::endl;
    std::cout << *(int*)node->LargestKey() << ", " << *(int*)newNode->LargestKey() << std::endl;
    

    header.rootPageNum = root->GetPageNum();
    // bpm->getPage(fileID, header.rootPageNum, index);

    if(newNode != nullptr) {
      delete newNode;
      newNode = nullptr;
    }
    SetHeight(header.height + 1);
    std::cout << "hi:" << GetHeight() << std::endl;
    
  }
  return NO_ERROR;
}

BTreeNode* IX_IndexHandle::ScanLeftFind(BTreeNode* right, void* pData, const recordmanager::RID &r) {
  BTreeNode* curNode = FetchNode(right->GetLeft());
  while(curNode != nullptr) {
    int i = curNode->GetNumKeys() - 1;
    for(; i >= 0; --i) {
      void* key = nullptr;
      curNode->GetKey(i, key);
      
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
      BTreeNode* others = ScanLeftFind(node, pData, r);
      if(others != nullptr) {
        int removedPos = others->FindKey((const void* &)pData, r);
        others->Remove(removedPos); // maybe underflow
        return NO_ERROR;
      }
    }

    return -2;
  } else if(pos == node->GetNumKeys() - 1) {
    delLargest = true;
  }

  if(delLargest) {
    for(int i = header.height - 2; i >= 0; --i) {
      int temp_pos = path[i]->FindKey((const void* &)pData);

      if(temp_pos != -1) {
        void* leftKey = nullptr;
        leftKey = path[i + 1]->LargestKey();
        if(node->CmpKey(pData, leftKey) == 0) {
          int m_pos = path[i + 1]->GetNumKeys() - 2;
          if(m_pos < 0) {
            continue;
          }
          path[i + 1]->GetKey(m_pos, leftKey);
        }

        if(i == header.height - 2 || (temp_pos == path[i]->GetNumKeys() - 1)) {
          path[i]->SetKey(temp_pos, leftKey);
        }
      }
    }
  }

  node->Remove(pos);
  int level = header.height - 1;
  // std::cout << pos << "," << ret << "," << node->GetNumKeys() << std::endl;
  while(node->GetNumKeys() == 0) {
    level--;
    if(level < 0) {
      break;
    }

    int posAtParent = pathPos[level];
    BTreeNode* parent = path[level];
    parent->Remove(posAtParent);

    // if(level == 0 && parent->GetNumKeys() == 1 && ret == 0) {
    //   ret = -1;
    // }

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

    // node->Destroy();
    DisposePage(node->GetPageNum());
    // std::cout << 
    node = parent;
  }

  if(level < 0) {
    if(header.numPages == 2) {
      SetHeight(1);
      return NO_ERROR;
    }
    recordmanager::RID first;
    root->GetAddrByPosition(0, first);
    root = FetchNode(first);
    header.rootPageNum = root->GetPageNum();
    int index;
    bpm->getPage(fileID, header.rootPageNum, index);

    // node->Destroy();
    DisposePage(node->GetPageNum());

    SetHeight(header.height - 1);
    
  }
  return NO_ERROR;
}

BTreeNode* IX_IndexHandle::FindLeafForceRight(const void* pData) {
  FindLeaf(pData);

  if(path[header.height - 1]->GetRight() != -1) {
    int pos = path[header.height - 1]->FindKey(pData);

    if(pos != -1 && pos == path[header.height - 1]->GetNumKeys() - 1) {
      BTreeNode* right = FetchNode(path[header.height - 1]->GetRight());

      if(right != nullptr) {
        void* key = nullptr;
        right->GetKey(0, key);

        if(right->CmpKey(pData, key) == 0) {
          WriteBack(path[header.height - 1]->GetPageNum());

          delete path[header.height - 1];
          path[header.height - 1] = FetchNode(right->GetPageRID());
          pathPos[header.height - 2]++;
        }
      }
    }
  }

  return path[header.height - 1];
}

BTreeNode* IX_IndexHandle::FindLeafForceLeft(const void* pData) {
  FindLeaf(pData);

  while(path[header.height - 1]->GetLeft() != -1) {
    
    void* firstKey = nullptr;
    path[header.height - 1]->GetKey(0, firstKey);

    if(path[header.height - 1]->CmpKey(firstKey, pData) == 0) {
      BTreeNode* left = FetchNode(path[header.height - 1]->GetLeft());

      if(left != nullptr) {
        void* key = nullptr;
        left->GetKey(0, key);

        if(left->CmpKey(pData, key) == 0) {
          WriteBack(path[header.height - 1]->GetPageNum());

          delete path[header.height - 1];
          path[header.height - 1] = FetchNode(left->GetPageRID());
          pathPos[header.height - 2]--;
        } else {
          return left;
        }
      }
    }
  }

  return path[header.height - 1];
}


int IX_IndexHandle::GetNewPage(int &pageNum) {
  utils::BitMap bitmap(header.pageNumBitMap, 32768);
  if(bitmap.IsFull()) {
    return -1;
  }
  pageNum = bitmap.FindFirstZeroPosition();
  bitmap.SetOne(pageNum);
  header.maxPageNum = std::max(header.maxPageNum, pageNum);

  // int index;
  ///bpm->getPage(fileID, pageNum, index);
  header.numPages++;
  bHeaderChanged = true;
  return NO_ERROR;
}

int IX_IndexHandle::GetFileHeader(const int& pageNum) {
  int index;
  char* buf = (char*) bpm->getPage(fileID, pageNum, index);
  memcpy(&header, buf, sizeof(header));
  return NO_ERROR;
}

int IX_IndexHandle::Open(filesystem::BufPageManager* bpm, int fileID, 
int rootPage) {
  
  if(bFileOpen || this->bpm != nullptr) {
    return -1;
  }

  if(bpm == nullptr) {
    return -2;
  }
  bFileOpen = true;
  this->bpm = bpm;
  this->fileID = fileID;
  int index;
  bpm->getPage(fileID, 0, index);
  this->GetFileHeader(0);
  utils::BitMap bitmap(header.pageNumBitMap, 32768);
  bitmap.SetOne(0);

  bool newPage = true;
  if(header.height > 0) {
    SetHeight(header.height);
    newPage = false;
    bpm->getPage(fileID, rootPage, index);
  } else {
    this->GetNewPage(rootPage);
    header.rootPageNum = rootPage;
    SetHeight(1);
  }

  root = new BTreeNode(header.attrType, header.attrLength, bpm, fileID, rootPage, newPage);

  path[0] = root;
  header.capacity = root->GetMaxKeys();
  bHeaderChanged = true;
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
  bpm->markDirty(index);
  bpm->writeBack(index);
  return NO_ERROR;
}

int IX_IndexHandle::ForcePages() {
  int index, used;
  utils::BitMap bitmap(header.pageNumBitMap, 32768);
  for(int i = 1; i <= header.maxPageNum; ++i) {
    bitmap.Get(i, used);
    if(used == 1) {
      bpm->getPage(fileID, i, index);
      bpm->markDirty(index);
      bpm->writeBack(index);
    }
  }
  return NO_ERROR;
}

int IX_IndexHandle::DisposePage(const int& pageNum) {
  utils::BitMap bitmap(header.pageNumBitMap, 32768);
  bitmap.SetZero(pageNum);
  // int index;
  // bpm->getPage(fileID, pageNum, index);
  // bpm->release(index);
  header.numPages--;
  bHeaderChanged = true;
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
    std::cout << pos << std::endl;

    if(pos == path[i - 1]->GetNumKeys()) {
      pos--;
      path[i - 1]->GetAddrByPosition(pos, r);
    }
    if(path[i] != nullptr) {
      // WriteBack(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    // int index;
    // bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathPos[i - 1] = pos;
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
      WriteBack(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    int index;
    // bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathPos[i - 1] = 0;
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
      WriteBack(path[i]->GetPageNum());
      delete path[i];
      path[i] = nullptr;
    }
    path[i] = FetchNode(r);
    // int index;
    // bpm->getPage(fileID, path[i]->GetPageNum(), index);
    pathPos[i - 1] = path[i - 1]->GetNumKeys() - 1;
  }

  return path[header.height - 1];
}

BTreeNode* IX_IndexHandle::FetchNode(recordmanager::RID r) const {
  int pageNum;
  r.GetPageNum(pageNum);
  if(pageNum < 0) {
    return nullptr;
  }
  // int index;
  //bpm->getPage(fileID, pageNum, index);
  //bpm->access(index);
  return new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum, false);
}

BTreeNode* IX_IndexHandle::FetchNode(int pageNum) const {
  if(pageNum < 0) {
    return nullptr;
  }
  // int index;
  // bpm->getPage(fileID, pageNum, index);
  return new BTreeNode(header.attrType, header.attrLength, bpm, fileID, pageNum, false);
}

int IX_IndexHandle::WriteBack(int pageNum) {
  if(bpm == nullptr) {
    return -1;
  }
  int index;
  bpm->getPage(fileID, pageNum, index);
  bpm->markDirty(index);
  bpm->writeBack(index);
  return NO_ERROR;
}

int IX_IndexHandle::Release(int pageNum) {
  if(bpm == nullptr) {
    return -1;
  }
  int index;
  bpm->getPage(fileID, pageNum, index);
  bpm->release(index);
  return NO_ERROR;

}

// int IX_IndexHandle::Pin(int pageNum) {
//   return NO_ERROR;
// }

int IX_IndexHandle::GetHeight() const {
  return header.height;
}

void IX_IndexHandle::SetHeight(const int& h)
{
  for(int i = 1;i < header.height; ++i)
    if (path != nullptr && path[i] != nullptr) {
      delete path[i];
      path[i] = nullptr;
    }
  if(path != nullptr) delete [] path;
  if(pathPos != nullptr) delete [] pathPos;

  header.height = h;
  path = new BTreeNode* [header.height];
  for(int i = 1;i < header.height; ++i)
    path[i] = nullptr;
  path[0] = root;

  pathPos = new int [header.height-1]; 
  for(int i = 0;i < header.height-1; ++i)
    pathPos[i] = -1;
}


IX_IndexScan::IX_IndexScan() {
  bOpen = false;
  desc = false;
  eof = false;
  lastNode = nullptr;
  pixh = nullptr;
  curNode = nullptr;
  curPos = -1;
  curKey = nullptr;
  curRid = recordmanager::RID(-1, -1);
  c = EQ_OP;
  value = nullptr;
}

IX_IndexScan::~IX_IndexScan() {
  if(curNode != nullptr) {
    delete curNode;
    curNode = nullptr;
  }
  if(lastNode != nullptr) {
    delete lastNode;
    lastNode = nullptr;
  }
  // if(pixh != nullptr) {
  //   delete pixh;
  //   pixh = nullptr;
  // }
}

std::function<bool(void*, void*)> IX_IndexScan::GetCheckFunction(
    AttrType attr_type, int attrLength, CompOp comp_op) {
  if (attr_type == AttrType::INT) {
    std::function<int(void*)> f = [](void* x) -> int { return *(int*)x; };

    if (comp_op == CompOp::EQ_OP) {
      return [f](void* a, void* b) -> bool { return f(a) == f(b); };
    } else if (comp_op == CompOp::LT_OP) {
      return [f](void* a, void* b) -> bool { return f(a) < f(b); };
    } else if (comp_op == CompOp::GT_OP) {
      return [f](void* a, void* b) -> bool { return f(a) > f(b); };
    } else if (comp_op == CompOp::LE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) <= f(b); };
    } else if (comp_op == CompOp::GE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) >= f(b); };
    } else if (comp_op == CompOp::NE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) != f(b); };
    } else {
      return [](void*, void*) -> bool { return true; };
    }
  } else if (attr_type == AttrType::FLOAT) {
    std::function<float(void*)> f = [](void* x) -> float {
      return *((float*)x);
    };
    if (comp_op == CompOp::EQ_OP) {
      return [f](void* a, void* b) -> bool { return f(a) == f(b); };
    } else if (comp_op == CompOp::LT_OP) {
      return [f](void* a, void* b) -> bool { return f(a) < f(b); };
    } else if (comp_op == CompOp::GT_OP) {
      return [f](void* a, void* b) -> bool { return f(a) > f(b); };
    } else if (comp_op == CompOp::LE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) <= f(b); };
    } else if (comp_op == CompOp::GE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) >= f(b); };
    } else if (comp_op == CompOp::NE_OP) {
      return [f](void* a, void* b) -> bool { return f(a) != f(b); };
    } else {
      return [](void*, void*) -> bool { return true; };
    }
  } else {  // attr_type == AttrType::STRING
    if (comp_op == CompOp::EQ_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) != (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::LT_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) < (*y)) return true;
          if ((*x) > (*y)) return false;
          x++;
          y++;
        }
        return false;
      };
    } else if (comp_op == CompOp::GT_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) > (*y)) return true;
          if ((*x) < (*y)) return false;
          x++;
          y++;
        }
        return false;
      };
    } else if (comp_op == CompOp::LE_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) < (*y)) return true;
          if ((*x) > (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::GE_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) > (*y)) return true;
          if ((*x) < (*y)) return false;
          x++;
          y++;
        }
        return true;
      };
    } else if (comp_op == CompOp::NE_OP) {
      return [attrLength](void* a, void* b) -> bool {
        char* x = (char*)a;
        char* y = (char*)b;
        for (int i = 0; i < attrLength; i++) {
          if ((*x) != (*y)) return true;
          x++;
          y++;
        }
        return false;
      };
    } else {
      return [](void*, void*) -> bool { return true; };
    }
  }
}

int IX_IndexScan::OpenScan(const IX_IndexHandle &indexHandle, CompOp comOp, void* value, bool desc) {
  if(bOpen) {
    return -1;
  }
  if(comOp > NO_OP || comOp < EQ_OP) {
    return -2;
  }
  pixh = const_cast<IX_IndexHandle*> (&indexHandle);
  if(pixh == nullptr) {
    return -3;
  }

  bOpen = true;
  this->desc = desc;
  foundOne = false;
  this->c = comOp;
  checkFunc = GetCheckFunction(pixh->GetAttrType(), pixh->GetAttrLength(), comOp);
  this->value = value;
  OpOptimize();
  return NO_ERROR;
}

int IX_IndexScan::GetNextEntry(recordmanager::RID &r) {
  void* key = nullptr;
  int i = -1;
  return GetNextEntry(key, r, i);
}

int IX_IndexScan::GetNextEntry(void* &key, recordmanager::RID &r, int& numScanned) {
  if(!bOpen) {
    return -1;
  }
  if(eof) {
    return -2;
  }

  bool curDeleted = false;
  if(curNode == nullptr && curPos == -1) {
    if(!desc) {
      curNode = pixh->FetchNode(pixh->FindSmallestLeaf()->GetPageRID());
      curPos = -1;
    } else {
      curNode = pixh->FetchNode(pixh->FindLargestLeaf()->GetPageRID());
      curPos = curNode->GetNumKeys();
    }
  } else {
    if(curKey != nullptr && curNode != nullptr && curPos != -1) {
      void* k = nullptr;
      curNode->GetKey(curPos, k);
      if(curNode->CmpKey(curKey, k) != 0) {
        curDeleted = true;
      } else {
        recordmanager::RID rid;
        curNode->GetAddrByPosition(curPos, rid);
        if(!(rid == curRid)) {
          curDeleted = true;
        }
      }
    }
  }
  //std::cout << curPos << "," << curDeleted << std::endl;

  while(curNode != nullptr) {
    int i = -1;
    if(!desc) {
      if(curDeleted) {
        i = curPos;
        curDeleted = false;
      } else {
        i = curPos + 1;
      }
      //std::cout << i << "," << curNode->GetNumKeys() << std::endl;
      

      for(; i < curNode->GetNumKeys(); ++i) {
        void* k = nullptr;
        curNode->GetKey(i, k);
        numScanned++;
        curPos = i;
        // std::cout << i << "," << curNode->GetNumKeys() << std::endl;
        if(curKey == nullptr) {
          curKey = (void*) new char[pixh->GetAttrLength()];
        }
        memcpy(curKey, k, pixh->GetAttrLength());
        curNode->GetAddrByPosition(i, curRid);
        //std::cout << *(int*)curKey << "," << *(int*)value << std::endl;
        //std::cout << checkFunc(k, value) << std::endl;
        if(checkFunc(k, value)) {
          key = k;
          //std::cout << "why"<< std::endl;
          curNode->GetAddrByPosition(i, r);
          foundOne = true;

          return NO_ERROR;
        } else {
          if(foundOne) {
            EarlyExitOptimize(k);
            if(eof) {
              return -2;
            }
          }
        }
      }

    } else {
      if(curDeleted) {
        i = curPos;
        curDeleted = false;
      } else {
        i = curPos - 1;
      }

      for(; i >= 0; --i) {
        void* k = nullptr;
        curNode->GetKey(i, k);
        numScanned++;
        curPos = i;
        if(curKey == nullptr) {
          curKey = (void*) new char[pixh->GetAttrLength()];
        }
        curNode->GetAddrByPosition(i, curRid);
        memcpy(curKey, k, pixh->GetAttrLength());

        if(checkFunc(k, value)) {
          key = k;
          curNode->GetAddrByPosition(i, r);
          foundOne = true;
          return NO_ERROR;
        } else {
          if(foundOne) {
            EarlyExitOptimize(k);
            if(eof) {
              return -2;
            }
          }
        }
      }
    }

    if(lastNode != nullptr && curNode->GetPageRID() == lastNode->GetPageRID()) {
      break;
    }

    if(!desc) {
      int right = curNode->GetRight();
      pixh->WriteBack(curNode->GetPageNum());
      delete curNode;
      curNode = pixh->FetchNode(right);
      // if(curNode != nullptr) {
      //   pixh->Pin(curNode->GetPageNum());
      // }
      curPos = -1;
    } else {
      int left = curNode->GetLeft();
      pixh->WriteBack(curNode->GetPageNum());
      delete curNode;
      curNode = pixh->FetchNode(left);
      if(curNode != nullptr) {
        // pixh->Pin(curNode->GetPageNum());
        curPos = curNode->GetNumKeys();
      }
    }
    
  }
  return -2;
}

int IX_IndexScan::CloseScan() {
  if(!bOpen) {
    return -1;
  }

  bOpen = false;
  curNode = nullptr;
  curPos = -1;
  if(curKey != nullptr) {
    delete[] ((char*) curKey);
    curKey = nullptr;
  }
  curRid = recordmanager::RID(-1, -1);
  lastNode = nullptr;
  eof = false;
  return NO_ERROR;
}

int IX_IndexScan::ResetState() {
  curNode = nullptr;
  curPos = -1;
  lastNode = nullptr;
  eof = false;
  foundOne = false;

  return OpOptimize();
}

int IX_IndexScan::EarlyExitOptimize(void* nowKey) {
  if(!bOpen) {
    return -1;
  }

  if(value == nullptr) {
    return NO_ERROR;
  }

  if(curNode != nullptr) {
    int cmp = curNode->CmpKey(nowKey, value);

    if(c == EQ_OP && cmp != 0) {
      eof = true;
      return NO_ERROR;
    }

    if((c == LE_OP) && cmp > 0 && !desc) {
      eof = true;
      return NO_ERROR;
    }

    if((c == LT_OP) && cmp >= 0 && !desc) {
      eof = true;
      return NO_ERROR;
    }

    if((c == GE_OP) && cmp < 0 && desc) {
      eof = true;
      return NO_ERROR;
    }

    if((c == GT_OP) && cmp <= 0 && desc) {
      eof = true;
      return NO_ERROR;
    }

  }

  return NO_ERROR;
}

int IX_IndexScan::OpOptimize() {
  if(!bOpen) {
    return -1;
  }

  if(value == nullptr) {
    return NO_ERROR;
  }

  if(c == NE_OP) {
    return NO_ERROR;
  }

  if(curNode != nullptr) {
    delete curNode;
  }

  curNode = pixh->FetchNode(pixh->FindLeafForceRight(value)->GetPageRID());
  curPos = curNode->FindKey((const void* &)value);
  //std::cout << curPos << std::endl;
  if(desc) {
    if(c == LE_OP || c == LT_OP) {
      lastNode = nullptr;
      curPos = curPos + 1;
    }

    else if(c == EQ_OP) {
      if(curPos == -1) {
        delete curNode;
        eof = true;
        return NO_ERROR;
      }

      lastNode = nullptr;
      curPos = curPos + 1;
    }
    else if(c == GE_OP) {
      delete curNode;
      lastNode = nullptr;
      curNode = nullptr;
      curPos = -1;
    } else {
      lastNode = pixh->FetchNode(curNode->GetPageRID());
      delete curNode;
      curNode = nullptr;
      curPos = -1;
    }
  } else {
    if(c == LE_OP || c == LT_OP) {
      lastNode = pixh->FetchNode(curNode->GetPageRID());
      delete curNode;
      curNode = nullptr;
      curPos = -1;
    }
    if(c == GT_OP) {
      lastNode = nullptr;
    }
    if(c == GE_OP) {
      delete curNode;
      curNode = nullptr;
      curPos = -1;
      lastNode = nullptr;
    }
    if(c == EQ_OP) {
      if(curPos == -1) { 
        delete curNode;
        eof = true;
        return 0;
      }
      lastNode = pixh->FetchNode(curNode->GetPageRID());
      delete curNode;
      curNode = nullptr;
      curPos = -1;
    }
  }
  
  //int first = -1;
  //if(curNode != NULL) first = curNode->GetPageNum();
  //int last = -1;
  //if(lastNode != NULL) last = lastNode->GetPageNum();
  return NO_ERROR;
}

IX_Manager:: IX_Manager(filesystem::FileManager* fm, filesystem::BufPageManager* bpm) {
  this->bpm_ = bpm;
  this->fm_ = fm;
}

IX_Manager::~IX_Manager() {

}

int IX_Manager::CreateIndex(const char* fileName, int indexNo, AttrType attrType, int attrLength) {
  if(indexNo < 0 || attrType < INT || attrType > STRING || fileName == nullptr) {
    return -1;
  }

  if(attrLength >= kPageSize - (int)sizeof(recordmanager::RID) || attrLength <= 0) {
    return -2;
  }

  if((attrType == INT && (unsigned int)attrLength != sizeof(int)) ||
     (attrType == FLOAT && (unsigned int)attrLength != sizeof(float))
     ||
     (attrType == STRING && 
      ((unsigned int)attrLength <= 0 || 
       (unsigned int)attrLength > 255)))
      return -3;
  
  std::stringstream name;
  name << fileName << "." << indexNo;

  bpm_->createFile(name.str().c_str());
  int fileID;
  bpm_->openFile(name.str().c_str(), fileID);
  int index;
  char* data = (char*) bpm_->getPage(fileID, 0, index);
  IX_FileHeader header;
  header.numPages = 1;
  header.capacity = -1;
  header.height = 0;
  header.rootPageNum = -1;
  header.attrType = attrType;
  header.attrLength = attrLength;

  memcpy(data, &header, sizeof(header));

  bpm_->markDirty(index);
  bpm_->writeBack(index);
  bpm_->closeFile(fileID);
  return NO_ERROR;
}

int IX_Manager::DestroyIndex(const char* fileName, int indexNo) {
  if(indexNo < 0 || fileName == nullptr) {
    return -1;
  }

  std::stringstream name;
  name << fileName << "." << indexNo;
  
  bool ret = bpm_->deleteFile(name.str().c_str());
  if(!ret) {
    return -1;
  }

  return NO_ERROR;
}

int IX_Manager::OpendIndex(const char* fileName, int indexNo, IX_IndexHandle& ixh) {
  if(indexNo < 0 || fileName == nullptr) {
    return -1;
  }

  std::stringstream name;
  name << fileName << "." << indexNo;

  int fileID;
  bpm_->openFile(name.str().c_str(), fileID);
  int index;
  char* data = (char*) bpm_->getPage(fileID, 0, index);
  IX_FileHeader header;
  memcpy(&header, data, sizeof(header));

  ixh.Open(bpm_, fileID, header.rootPageNum);
  bpm_->release(index);
  return NO_ERROR;
}

int IX_Manager::CloseIndex(IX_IndexHandle& ixh) {
  if(!ixh.bFileOpen || ixh.bpm == nullptr) {
    return -1;
  }
  if(ixh.bHeaderChanged) {
    // int index;
    // ixh.bpm->getPage(ixh.fileID, 0, index);
    ixh.SetFileHeader(0);
    // ixh.bpm->markDirty(index);
    // ixh.bpm->writeBack(index);
    ixh.ForcePages();
  }

  bpm_->closeFile(ixh.fileID);
  // ixh.Close();
  // ixh.bpm = nullptr;
  ixh.bFileOpen = false;
  return NO_ERROR;
}

}