# 数据库大作业实验报告

## 成员

计65	尹一帆	2016011368

计65	丁相允	201601136

## 整体架构

参考斯坦福CS346的`Redbase`课程项目，我们的项目结构整体可以分为六大部分，文件系统、记录管理、索引管理、系统管理、查询语句以及最终面向用户的`Parser`。具体的模块间调用关系如下图所示：

![database](C:\Users\paolino\Desktop\database.png)

项目文件结构为每一个模块一个子目录，模块拥有自己的命名空间以防命名冲突，规范代码及开发流程。使用`Bazel`来管理项目的编译、测试、运行等操作。

## 模块详细设计以及主要接口

### 文件系统

助教提供了一个带有缓存管理的简易文件系统，但是就使用体验而言实在缺乏鲁棒性，存在诸多问题。

首先，哈希函数的映射设计的过于简单，仅仅只是文件ID与页号之和取模，很容易碰撞，不过这不影响正确性。

有问题的是，当我们打开过一个文件并读入某一页到缓存中再关闭后，打开另一文件的相同页号时，由于之前释放了文件ID的资源，很可能获取到相同的文件ID，然后访问到相同的缓存地址，读取到的内容并不清楚是本身之前的缓存还是其他文件的遗留物。

一个简易的处理方法是，使用`BufPageManager`管理打开的文件名与文件ID间的映射以及缓存index与文件名间的映射关系。打开文件时，判断文件名是否有对应ID，防止重复打开；关闭时，释放ID资源。缓存index则在文件访问到时，判断是否是对应的文件名，是则直接读取，否则替换并重新绑定文件名。并且增加了`Pin()`和`UnPin()`的接口，防止经常访问的页被替换。

所有打开、关闭等文件相关操作均在`BufPageManager`中实现，不直接访问`FileManager`。

### 索引管理

索引管理主要分为两小部分，`B+树`的实现，以及调用`B+树`实现加速查询的接口。

#### B+树

项目中，一页为8092Bytes，定义一个节点占有完整的一页，每页依次存储所有属性值、属性值对应的`RID`，然后依次是当前键值个数、总容量、左邻居节点页号以及右邻居节点页号，最后是节点自身的页号。节点类本身存放属性长度以及属性类型，并且不区分内部节点和叶子节点，只是内部节点的`RID`部分存子节点的页号，叶子节点的`RID`则存记录的实际位置。创建节点时，根据传的参数`attrLength`来决定可以存放的最大`RID`数量。

这棵`B+树`的属性值位置存放的是对应子节点的最大属性值，也就是说，节点内部以及整体都是升序组织的。

主要接口包括各种`Set/Get`函数：

```c++
int GetMaxKeys() const;
int GetNumKeys();
int SetNumKeys(int newNumKeys);
int GetLeft();
int SetLeft(int pageNum);
int GetRight();
int SetRight(int pageNum);
recordmanager::RID GetPageRID() const;
void SetPageRID(const recordmanager::RID&);
int GetPageNum() const;
void* LargestKey() const;
```

比较重要的有获取和设置键值的接口`SetKey`以及`GetKey`，针对节点的插入与删除操作：

```c++
int Insert(const void* newKey, const recordmanager::RID& newRid);
int Remove(const void* newkey);
int Remove(const int pos);
int RangeRemove(const int startPos, const int endPos);
```

由于希望支持重复键值，因此采取最右匹配的策略，即重复键值取最右的结果，不过这也带来许多麻烦。

还包括根据位置、`RID`、键值等约束条件的查询函数：

```c++
int FindKey(const void* &key, const recordmanager::RID& r = recordmanager::RID(-1,-1)) const;

//精确匹配，不匹配则返回-1
int GetAddrByKey(const void* &key, recordmanager::RID& r) const;
int GetAddrByPosition(const int pos, recordmanager::RID& r) const;

//返回应该插入的位置
int FindPositionByKey(const void* &key) const;
int FindAddrByKey(const void* &key, recordmanager::RID& r) const;
```

由于并未完整实现`B+树`的删除合并，所以最重要的还是分裂函数：

```c++
int Split(BTreeNode* rhs);
```

分裂策略为当节点已满时，分出后一半的数据插入到新节点中，并且建立新的邻居关系。

#### 索引管理

分为三个部分，`IX_Manager`管理索引文件的创建、删除、打开以及关闭。`IX_IndexHandle`实际处理索引文件中数据的插入和删除，`IX_IndexScan`完成对索引文件的扫描，并根据指定条件获取相应数据。

##### IX_Manager

```c++
int CreateIndex(const char *filename, int indexNo, AttrType attrType,  int attrLength);
int DestroyIndex(const char *filename, int indexNo);
int OpendIndex(const char *filename, int indexNo, IX_IndexHandle &index_handle);
int CloseIndex(IX_IndexHandle &index_handle);
```

值得注意的是，我们将文件的第一页用于存放该文件的元信息，包括总页数、根节点页号、树的高度、属性种类和长度等，并且用位图数组维护已经实际使用的页号，用于利用销毁的节点的页号。

##### IX_IndexHandle

暴露给外部的接口主要有插入、删除、全部写回：

```c++
int InsertEntry(void *pData, recordmanager::RID& rid);
int DeleteEnrty(void* pData, recordmanager::RID& rid);
int ForcePages();
```

在访问索引文件时，首先通过根节点找到自己对应的叶子节点，访问时保存相关路径。其次才考虑后续的插入删除或者遍历。因此`BTreeNode* FindLeaf(const void *pData)`为基础函数。而又因为之前说过的重复键值的问题，最右匹配得到的不一定是需要的，在重复键值跨了节点时，甚至还需要遍历相邻节点找到真正的对应节点。

插入完整实现了，子节点已满时，分裂成两个子节点，并删除父节点中旧的键值，分别将两个新子节点的最大值插入父节点，然后递归。

删除则是懒删除，直到子节点为空，才删除该节点并向上递归。

##### IX_IndexScan

外部接口如下：

```c++
int OpenScan(const IX_IndexHandle &indexHandle, CompOp comOp, void *value, bool desc=false);
int GetNextEntry(recordmanager::RID &rid);
int CloseScan();
```

主要也是先找到叶子节点的位置，然后根据查询的条件设置优化的查询方式。

比如设定升序查询小于某值的情况，那么可以将找到的叶子节点设置为终节点再从头遍历；而当我们找到或没找到符合条件的值的时候，也可以进行相应判断，提早终结遍历，比如升序查找小于的值，当我们失败之后，就不必再继续查询了。重复键值的情况下效率会降低更多。

