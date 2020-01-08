+++
keywords = ["elasticsearch"]
title = "构建ElasticSearch集群之前"
categories = ["ELK"]
disqusIdentifier = "elasticsearch"
comments = true
clearReading = true
date = 2018-12-12T17:03:30+08:00 
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

> 摘自[designing the perfect elasticsearch cluster ](https://thoughts.t37.net/designing-the-perfect-elasticsearch-cluster-the-almost-definitive-guide-e614eabc1a87)

本文包含了设计elasticsearch cluster需要了解的方方面面，本文不会告诉你如何设计一个完美的es集群。

- elasticsearch是一个搜索引擎，不是数据库，不会替代mysql
- elasticsearch是一个弹性搜索引擎，弹性体现在水平扩展、索引分片；elastic将索引拆分成多个分片存储在不同的host上，分片数量默认为5，需要根据实际workload调整以达到最优性能

- 高可用设计

```
elasticsearch集群包含一下4种节点类型：
master nodes: 主节点
data node: 数据节点
http node: 查询节点
coordinating node: 协作节点

最小高可用集群设计：
1. 3数据中心
2. 3主节点，奇数个主节点防止脑裂，将3个主机点分散在各个数据中心
3. 2查询节点，在主节点数据中心各一个
4. 任意数量的数据节点

ES的分片分配算法允许分片分配在不同区域内，根据rack配置将主分片、备份分片分配在不同的区域中
cluster.routing.allocation.awareness.attributes: "rack_id"
node.attr.rack_id: "dontsmokecrack"
```

- 理解luence: elasticsearch使用了luence库

```
1. 每个elasticsearch分片是一个luence索引，一个luence索引最大可包含2,147,483,519条记录。

2. luence将索引划分为多个段，luence顺序搜索这些段；当新写请求来时会创建段，写提交或关闭时，
段即不可变，有新记录添加到elasticsearch index时，luence会创建新的段，luence会不断的将小的段
合并成大的段。luence搜索段是串行的，所以段越多延迟越大。当luence合并时会占用CPU与I/O进而降
低index速度。

3. 当更新或删除文档时luence执行copy on write操作，删除只将文档标记为已删除，索引磁盘占用会
一直增长，除非整个索引删除。

4. luence执行merge时，会将两个段合并成一个新的段后再删除旧的段，merge要确保磁盘空间足够
```

- 硬件

```
CPU: 复杂查询

ElasticSearch主要通过thread_pools使用CPU资源，主要的thread_pool有generic、index、get、bulk、
search等，可用通过GET _nodes/thread_pool?pretty查看当前集群各个节点的thread_pool情况；
thread_pool的线程数、队列数可以通过配置设置，默认不会超过32；当队列满时请求会被ES拒绝，可以
适当调整queue的数值。

Memory：

ElasticSearch作为Java应用程序，内存分配与回收非常关键；ES使用CMS作为默认的垃圾收集器，而CMS
的最大问题是会导致JVM停止，在堆内存大于4G不推荐；Java8推出了新的垃圾收集器G1，专门针对堆大于
4G的情况，G1GC将堆划分为1-32M的区，然后利用后台线程先扫描含垃圾最多的区，不会造成stop the world
的情况；另外由于指针大小的原因不要将堆设置超过31G；禁止swap。
-XX:+UseG1GC -XX:MaxGCPauseMillis=400

Network：

带宽越大越好；可以增加mtu值，以减少网络延时

Storage

存储往往是ES的瓶颈；尽量不要使用机械磁盘，SSD是最好的选择。
在对数据完整性要求不高时，raid0是最佳选择；裸磁盘成本低，故障只会丢失一块盘的数据，但读写速率低；raid10成本高。
```

- Software

```
kernel: 4.9.x
JVM: 8
FS: EXT4，但节点数据大于1TB XFS更佳
```

- 索引设计

```
1. 分片
索引一旦创建，分片数量即不可改变，除非通过reindex；然而在一开始很难确定索引数据量的大小，应该设置多少分片。
< 300万documents    1分片
300万之间500万      2分片
> 500万             int (总数/5000000 +1)
2. 备份
可用利用elasticsearch zones来优化数据备份，将数据分布在不同的机柜
```
