#ifndef BUF_PAGE_MANAGER
#define BUF_PAGE_MANAGER
#include "../fileio/FileManager.h"
#include "../utils/MyBitMap.h"
#include "../utils/MyHashMap.h"
#include "../utils/MyLinkList.h"
#include "../utils/pagedef.h"
#include "FindReplace.h"
#include <cstring>
#include <map>
#include <set>

namespace filesystem {

/*
 * BufPageManager
 * 实现了一个缓存的管理器
 */
struct BufPageManager {
 private:
  int last;
  FileManager* fileManager;
  MyHashMap* hash;
  FindReplace* replace;

  // std::map<int, std::set<int>> fd_to_idx; 
  // std::map<int, int> idx_to_fd;
  std::map<int, bool> idx_pin;
  std::map<std::string, int> name_to_id;
  std::map<int, std::string> id_to_name;
  std::map<int, std::string> idx_to_name;

  // MyLinkList* bpl;
  bool* dirty;
  /*
   * 缓存页面数组
   */
  BufType* addr;

  BufType allocMem() { return new unsigned int[(PAGE_SIZE >> 2)]; }

  BufType fetchPage(int typeID, int pageID, int& index) {
    BufType b;
    index = replace->find();
    //std::cout << index << std::endl;
    while(idx_pin[index]) {
      index = replace->find();
    }

    b = addr[index];
    //std::cout << "b: " << b << std::endl;
    if (b == NULL) {
      b = allocMem();
      addr[index] = b;
    } else {
      
      if (dirty[index]) {
        int k1, k2;
        hash->getKeys(index, k1, k2);
        fileManager->writePage(k1, k2, b, 0);
        dirty[index] = false;

      }

      // int fd = idx_to_fd[index];
      // idx_to_fd[index] = -1;
      // fd_to_idx[fd].erase(index);
      // memset(b, 0, PAGE_SIZE);
      // b = nullptr;
    }
    hash->replace(index, typeID, pageID);
    return b;
  }

 public:
  /*
   * @函数名allocPage
   * @参数fileID:文件id，数据库程序在运行时，用文件id来区分正在打开的不同的文件
   * @参数pageID:文件页号，表示在fileID指定的文件中，第几个文件页
   * @参数index:函数返回时，用来记录缓存页面数组中的下标
   * @参数ifRead:是否要将文件页中的内容读到缓存中
   * 返回:缓存页面的首地址
   * 功能:为文件中的某一个页面获取一个缓存中的页面
   *           缓存中的页面在缓存页面数组中的下标记录在index中
   *           并根据ifRead是否为true决定是否将文件中的内容写到获取的缓存页面中
   * 注意:在调用函数allocPage之前，调用者必须确信(fileID,pageID)指定的文件页面不存在缓存中
   *           如果确信指定的文件页面不在缓存中，那么就不用在hash表中进行查找，直接调用替换算法，节省时间
   */
  BufType allocPage(int fileID, int pageID, int& index, bool ifRead = false) {
    BufType b = fetchPage(fileID, pageID, index);

    idx_to_name[index] = id_to_name[fileID];
    // idx_to_fd[index] = fileID;
    // fd_to_idx[fileID].insert(index);

    if (ifRead) {
      fileManager->readPage(fileID, pageID, b, 0);
    }
    return b;
  }

  /*
   * @函数名getPage
   * @参数fileID:文件id
   * @参数pageID:文件页号
   * @参数index:函数返回时，用来记录缓存页面数组中的下标
   * 返回:缓存页面的首地址
   * 功能:为文件中的某一个页面在缓存中找到对应的缓存页面
   *           文件页面由(fileID,pageID)指定
   *           缓存中的页面在缓存页面数组中的下标记录在index中
   *           首先，在hash表中查找(fileID,pageID)对应的缓存页面，
   *           如果能找到，那么表示文件页面在缓存中
   *           如果没有找到，那么就利用替换算法获取一个页面
   */
  BufType getPage(int fileID, int pageID, int& index) {
    index = hash->findIndex(fileID, pageID);
    //std::cout << "index: " << index << std::endl;
    if (index != -1) {
      std::string oldName = idx_to_name[index];
      std::string newName = id_to_name[fileID];
      // std::cout << oldName << ", " << newName << std::endl;
      if(oldName == newName) {
        access(index);
        return addr[index];
      }
    } 
    // std::string oldName = idx_to_name[index];
    //   std::string newName = id_to_name[fileID];
    //   std::cout << oldName << ", " << newName << std::endl;

    BufType b = fetchPage(fileID, pageID, index);
    //std::cout << (void*)b << std::endl;
    fileManager->readPage(fileID, pageID, b, 0);

    // std::cout << "old: " << idx_to_name[index] << std::endl;

    // fd_to_idx[fileID].insert(index);
    // idx_to_fd[index] = fileID;
    idx_to_name[index] = id_to_name[fileID];

    // std::cout << "new: " << idx_to_name[index] << std::endl;

    return b;
  }

  /*
   * @函数名access
   * @参数index:缓存页面数组中的下标，用来表示一个缓存页面
   * 功能:标记index代表的缓存页面被访问过，为替换算法提供信息
   */
  void access(int index) {
    if (index == last) {
      return;
    }
    replace->access(index);
    last = index;
  }

  /*
   * @函数名markDirty
   * @参数index:缓存页面数组中的下标，用来表示一个缓存页面
   * 功能:标记index代表的缓存页面被写过，保证替换算法在执行时能进行必要的写回操作，
   *           保证数据的正确性
   */
  void markDirty(int index) {
    dirty[index] = true;
    access(index);
  }

  /*
   * @函数名release
   * @参数index:缓存页面数组中的下标，用来表示一个缓存页面
   * 功能:将index代表的缓存页面归还给缓存管理器，在归还前，缓存页面中的数据不标记写回
   */
  void release(int index) {
    dirty[index] = false;

    // int fd = idx_to_fd[index];
    // idx_to_fd[index] = -1;
    // fd_to_idx[fd].erase(index);
    // unpin(index);

    replace->free(index);
    hash->remove(index);
  }

  /*
   * @函数名writeBack
   * @参数index:缓存页面数组中的下标，用来表示一个缓存页面
   * 功能:将index代表的缓存页面归还给缓存管理器，在归还前，缓存页面中的数据需要根据脏页标记决定是否写到对应的文件页面中
   */
  void writeBack(int index) {
    if (dirty[index]) {
      int f, p;
      hash->getKeys(index, f, p);
      fileManager->writePage(f, p, addr[index], 0);
      dirty[index] = false;
    }

    // int fd = idx_to_fd[index];
    // idx_to_fd[index] = -1;
    // fd_to_idx[fd].erase(index);
    // unpin(index);
    // memset(addr[index], 0, PAGE_SIZE);

    replace->free(index);
    hash->remove(index);
  }

  /*
   * @函数名close
   * 功能:将所有缓存页面归还给缓存管理器，归还前需要根据脏页标记决定是否写到对应的文件页面中
   */
  void close() {
    for (int i = 0; i < CAP; ++i) {
      writeBack(i);
    }
  }

  void closeFile(int fileID) {
    std::string name = id_to_name[fileID];
    name_to_id.erase(name);
    id_to_name.erase(fileID);

    // while(!fd_to_idx[fileID].empty()) {
    //  writeBack(*(fd_to_idx[fileID].begin()));
    // }
    fileManager->closeFile(fileID);
  }

  void pin(int index) {
    idx_pin[index] = true;
  }

  void unpin(int index) {
    idx_pin[index] = false;
  }

  bool openFile(const char* fileName, int& fileID) {
    bool rc = true;
    std::string name(fileName);
    if(name_to_id.count(name) > 0) {
      fileID = name_to_id[name];
    } else {
      rc = fileManager->openFile(name.c_str(), fileID);
      if(!rc) {
        return false;
      }
      name_to_id[name] = fileID;
      id_to_name[fileID] = name;
    }
    return true;
  }

  bool createFile(const char *fileName) {
    return fileManager->createFile(fileName);
  }

  bool deleteFile(const char *fileName) {
    return fileManager->deleteFile(fileName);
  }

  /*
   * @函数名getKey
   * @参数index:缓存页面数组中的下标，用来指定一个缓存页面
   * @参数fileID:函数返回时，用于存储指定缓存页面所属的文件号
   * @参数pageID:函数返回时，用于存储指定缓存页面对应的文件页号
   */
  void getKey(int index, int& fileID, int& pageID) {
    hash->getKeys(index, fileID, pageID);
  }
  
  /*
   * 构造函数
   * @参数fm:文件管理器，缓存管理器需要利用文件管理器与磁盘进行交互
   */
  BufPageManager(FileManager* fm) {
    int c = CAP;
    int m = MOD;
    last = -1;
    fileManager = fm;
    // bpl = new MyLinkList(CAP, MAX_FILE_NUM);
    dirty = new bool[CAP];
    addr = new BufType[CAP];
    hash = new MyHashMap(c, m);
    replace = new FindReplace(c);
    for (int i = 0; i < CAP; ++i) {
      dirty[i] = false;
      addr[i] = NULL;
    }
  }
};

}  // namespace filesystem

#endif
