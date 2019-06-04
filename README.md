# os\_final_design

操作系统期末课程设计

## 前后台

前后台是独立的进程，依靠linux的命名管道进行通信。具体实现是开两个单向的管道。

这里<font color=red>**有一个问题我先记一下，以后改，**</font>在多个前台一起工作的时候好像会出现取数据的错乱。目前想到的解决方案：

* 每一个前台开一个从后到前的管道
* 换一种通信

## 前后台分工

前台负责接收命令，后台主要处理文件。

我想把文件指针让前台存储，写入之类的都交给后台完成，这个看怎么方便吧。


## 文件系统设计

文件系统按照顺序包括：

- I_NODE位图 
- 数据区位图 
- I_NODE区
- 数据区

0---1K---100k---1M

I_NODE的地址和数据块的地址都用 unsigned int

这里面存的地址都是相对于各个数据块的<font color=red>**逻辑地址**</font>，单位为byte，而不是实际地址。

### I_NODE位图F

假设有256个INODE，那么大小为 256bit(32B)。 

### I_NODE区

每个大小100B，有256个I_NODE，总大小25600B = 25KB。

### 数据区

总大小有512M，每个数据块大小1KB，有 512*1024 块。

### 数据区位图

有 512*1024 块，那么总共占 64KB。

### 磁盘总大小

32B + 64KB + 25KB + 512MB

第一K存 I_NODE 位图

所以开513MB，第一个MB用来存其他内容。



