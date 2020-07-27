+++
keywords = ["ext2fs"]
title = "Ext2设计与实现(译)"
categories = ["linux"]
comments = true
clearReading = true
disqusIdentifier = "ext2fs1712"
date = "2017-04-06T19:10:59+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

## Ext2设计与实现

### 1、介绍
Linux第一版只支持Minix文件系统，Minix文件系统有两大亟待解决的限制：块地址存储在16位整型，最大文件大小为64MB；另外目录和文件名长度最大支持14个字符。

我们设计与实现了两种新的文件系统:EXT 、 EXT2

在这篇论文中，我们将简述Linux文件系统的历史，简单介绍Unix文件系统的基础概念；介绍Linux VFS层的实现并详细介绍EXT2内核代码与用户工具；最后，是Linux、BSD下EXT2性能测试对比。

### 2、Linux文件系统历史（略）
### 3、文件系统概念
Linux文件系统的实现基于Unix操作系统通用概念：文件通过inodes来表示，目录是一种简单的文件包含许多列表项，设备是可以发起I/O请求的特殊文件。
#### 3.1 Inode
每个文件都使用inode结构体来表示，每个inode包含了描述文件元数据：文件类型、访问权限、所有者、时间戳、大小、数据块指针。分配给文件的数据块地址存储在文件的inode节点中，当用户对文件发起I/O请求时，内核代码将当前文件偏移量转为块号，使用该数字作为块地址表的索引来读写物理块，如下图所示：

![](/images/ext2-inode.gif)

#### 3.2 目录
目录按层次树结构组织，每一个目录可以包含文件和子目录。

目录实现为一种特殊的文件。事实上，目录是一种包含列表项的文件，每一项包含一个inode号和一个文件名，当一个进程使用一个路径名时，内核代码会搜索目录查找对应的inode号，在路径文件名被转换为inode号后，inode结构会被存储到内存中用以后序请求。目录如下图：

![](/images/ext2-dir.gif)
#### 3.3 链接
Unix文件系统提出了链接的概念，若干个文件名可以关联到一个inode节点，inode节点包含一个存储链接数的域。增加一个链接会创建一个目录项并增加inode链接计数。当删除链接时，内核会递减链接计数，为0时删除inode。

这种类型的链接称为硬链接，只能在一个文件系统中使用，且不能链接到一个目录，避免引起环路。

另一种链接存在于大多数Unix系操作系统，符号链接：只包含文件名的简单文件，当内核inode转换时遇到符号链接，会将软链接文件内容替换链接名。因为软链接不包含inode，它可以跨文件系统，可以指向任何类型的文件，甚至不存在的文件。但软链接会占用磁盘空间、inode、在路径名到inode转换时引起额外消耗。

#### 3.4 设备文件
在Unix系操作系统，设备被当作特殊文件访问。一个设备文件并不占用文件系统空间，只作为设备驱动访问接口。

有两类特殊文件：字符设备、块设备。主设备号决定类型，次设备号决定哪一个设备。

### 4 VFS
VFS是一个文件系统抽象层，定义了一个文件系统应该实现的操作，对上层屏蔽了底层不同文件系统的实现，一图概之：

![](/images/ext2-vfs.gif)
### 5 EXT2
#### 5.1 起因
修复EXT文件系统问题，提供一个强大的文件系统，实现unix文件语义并提供高级特性
#### 5.2 标准 ext2fs特性
- 支持标准Unix文件类型：普通文件、目录、设备文件、符号链接
- 支持最大4TB文件系统
- 长文件名：255字节，可扩展至1012
- 为root保留空间以便修复

#### 5.3 高级 ext2fs特性
- 属性继承
- 软链接：目标名存储在inode中
- 创建文件系统时可选择逻辑块大小
- fsck
- mount options
- Append-only files 

#### 5.4 物理结构
受BSD文件系统的影响，文件系统由块组构成，但块组并没有与磁盘的物理结构块绑定，因为现代驱动趋势是优化顺序访问和对操作系统隐藏物理结构。

文件系统物理结构：
<table border="">
<tbody><tr>
<td>Boot<br>Sector</td>
<td>Block<br>Group 1</td>
<td>Block<br>Group 2</td>
<td>...<br>...</td>
<td>Block<br>Group N</td>
</tr>
</tbody></table>

每一个块组包含一份冗余的文件系统控制信息（超级块和文件系统描述信息），并包含一部分文件系统（块位图、inode位图、inode表、数据块）：
<table border="">
<tbody><tr>
<td>Super<br>Block</td>
<td>FS<br>descriptors</td>
<td>Block<br>Bitmap</td>
<td>Inode<br>Bitmap</td>
<td>Inode<br>Table</td>
<td>Data<br>Blocks</td>
</tr>
</tbody></table>

使用block group提高系统可靠性，冗余超级块信息可以简化文件系统恢复；inode表与数据块一起存储提高寻道时间，提高性能。

在ext2fs中，目录使用链表管理变长目录项，每一项包含inode号、记录项长度、文件名和文件名长度。通过变长目录项可以实现长文件名同时减少磁盘空间浪费，目录项结构如下：
<table border="">
<tbody><tr>
<td>inode number</td><td>entry length</td>
<td>name length</td><td>filename</td>
</tr>
</tbody></table>

#### 5.5 性能优化

- 预读：当读取一个块时，内核会请求连续的若干块，当读取下一个块时会从buffer cache读取
- 分配优化： 块组的inode、data分配会在当前组中以减少寻道时间
- 写时预分配

### 6 Ext2fs library （略）

译自：http://e2fsprogs.sourceforge.net/ext2intro.html
