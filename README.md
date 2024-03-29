# bjfu_os\_final_design

北林操作系统期末课程设计

## 运行

操作系统为Linux

```bash
mkdir target
cd target
cmake ../
make
# 运行后台
./daemon
# 在另一个窗口运行客户端
./client
```
## 前后台

前后台是独立的进程，依靠linux的命名管道进行通信。具体实现是开两个单向的管道。

这里<font color=red>**有一个问题我先记一下，以后改，**</font>在多个前台一起工作的时候好像会出现取数据的错乱。

这个后来也没改，不同时写命令就没问题了。老师检查的时候也没发现。

## 前后台分工

前台负责接受和输出，后台负责文件读写。

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


## 实现命令如下


<br>
共16条
<br>

命令 | 功能
:-: | :-: 
init | 创建 /tmp/fake_disk，并初始化文件系统
open | 挂载 /tmp/fake_disk
ls | 显示目录
pwd| 打印当前工作路径
cd| 改变工作路径
touch| 创建文件
mkdir| 创建文件夹
cp| 复制
rm| 删除
rename| 重命名
vim| 写入文件
cat| 显示文件
export| 导出到系统磁盘
import| 从磁盘导入
clear| 清屏
exit| 退出


