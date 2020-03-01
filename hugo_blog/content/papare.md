# 基础

## 算法与数据结构

### 数据结构

- 数组与链表
- 栈与队列
- 优先队列
- 树
- 哈希表
- 并差集
- Trie树
- 布隆过滤器
- LRU cache
- 图

### 算法

- 贪心 哈夫曼编码、最短生成树prim and cruskarl
- 递归
- 遍历
- 深度优先与广度优先搜索
- 分治法
- 动态规划

### 数组与链表

- 基本操作时间复杂度

 op|数组array|链表linked lsit
----|----|----
access|O(1)|O(n)
insert|O(n)|O(1)
delete|O(n)|O(1)

- 常见题目

1. 翻转链表
2. 判断链表是否有环 (快慢指针)


### 栈与队列

- 栈 FILO
- 队列 FIFO
- 常见题目

1. 括号匹配
2. 队列实现栈，栈实现队列

### 优先队列（heap）

- 正常入，按优先级出
- 操作时间复杂度
- 斐波那契堆是一系列无序树的集合，每棵树是一个最小堆，满足最小堆的性质

op|binary|fibonacci
----|----|----
find-min|Θ(1)|Θ(1)
delete-min|Θ(log n)|O(log n)	
insert|O(log n)|Θ(1)
merge|Θ(n)|Θ(1)

- 常见题目

1. kth largest in stream
2. 滑动窗口

### 哈希表

- 2sum
- 3sum

### 树与二叉树

- binary search tree 验证: 带上下界递归遍历
- 最小公共祖先


### 递归与分治

- Pow(x, n)
- 一组数的大多数


### 贪心算法

问题能够分解成子问题，子问题的最优解能递推到最终问题的最优解

### 广度优先搜索

利用队列

### 深度优先搜索

利用栈

### 位运算

- x & 1 == 0 OR == 1 判断奇偶
- x = x & (x-1) 清零最低位的1
- x & -x 得到最低位的1

## 操作系统

### linux内核调度算法

- O(n)  0.1x-2.4

说明： 固定时间片counter(15ms)，从rq队列中选择优先级最高的，当无就绪进程时，counter重新赋值 counter=counter/2 + priority，对等待IO的进程且时间片未用完的有一定公平性

缺点：O(n)效率低，需要重复计算时间片，当进程用完时间片必须等待再次赋值，才可以调度

- O(1)  2.5-2.6

说明：O(1)将进程分为0-139个等级，0-99为实时进程优先级，100-139为普通进程优先级，数值越低优先级越高；调度器为每个CPU维护两个数组，active与expire，数组存储优先级队列，从active选择最小数第一个进程，时间片用完后将进程放入expire数组，当active为空时，active与expire互换。支持抢占

缺点：在很多情况下会失效，有一些著名的程序总能让该调度器性能下降，导致交互式进程反应缓慢

- CFS   2.6

说明：CFS使用“虚拟运行时”表示某个任务的时间量。任务的虚拟运行时越小，意味着任务被允许访问服务器的时间越短，其对处理器的需求越高，

CFS没有将任务维护在链表式的运行队列中，它抛弃了active/expire数组，而是对每个CPU维护一个以时间为顺序的红黑树。该树方法能够良好运行的原因在于：
- 红黑树可以始终保持平衡，这意味着树上没有路径比任何其他路径长两倍以上。
- 由于红黑树是二叉树，查找操作的时间复杂度为O(log n)。但是除了最左侧查找以外，很难执行其他查找，并且最左侧的节点指针始终被缓存。
- 对于大多数操作（插入、删除、查找等），红黑树的执行时间为O(log n)，而以前的调度程序通过具有固定优先级的优先级数组使用 O(1)。O(log n)性能可接受范围。
- 红黑树可通过内部存储实现，即不需要使用外部分配即可对数据结构进行维护。

- tick中断
在CFS中，tick中断首先更新调度信息。然后调整当前进程在红黑树中的位置。调整完成后如果发现当前进程不再是最左边的叶子，就标记need_resched标志，中断返回时就会调用scheduler()完成进程切换。否则当前进程继续占用CPU。从这里可以看到 CFS抛弃了传统的时间片概念。Tick中断只需更新红黑树，以前的所有调度器都在tick中断中递减时间片，当时间片或者配额被用完时才触发优先级调整并重新调度。
- 红黑树键值计算
理解CFS的关键就是了解红黑树键值的计算方法。该键值由三个因子计算而得：一是进程已经占用的CPU时间；二是当前进程的nice值(-20-+19)；三是当前的cpu负载。进程已经占用的CPU时间对键值的影响最大，其实很大程度上我们在理解CFS时可以简单地认为键值就等于进程已占用的 CPU时间。因此该值越大，键值越大，从而使得当前进程向红黑树的右侧移动。另外CFS规定，nice值为1的进程比nice值为0的进程多获得10%的 CPU时间。在计算键值时也考虑到这个因素，因此nice值越大，键值也越大。
CFS为每个进程都维护两个重要变量：fair_clock和wait_runtime。这里我们将为每个进程维护的变量称为进程级变量，为每个CPU维护的称作CPU级变量，为每个runqueue维护的称为runqueue级变量。进程插入红黑树的键值即为fair_clock – wait_runtime。其中fair_clock从其字面含义上讲就是一个进程应获得的CPU时间，即等于进程已占用的CPU时间除以当前 runqueue中的进程总数；wait_runtime是进程的等待时间。它们的差值代表了一个进程的公平程度。该值越大，代表当前进程相对于其它进程越不公平。对于交互式任务，wait_runtime长时间得不到更新，因此它能拥有更高的红黑树键值，更靠近红黑树的左边。从而得到快速响应。
红黑树是平衡树，调度器每次总最左边读出一个叶子节点，该读取操作的时间复杂度是O(LgN)。

### 进程状态

- TASK_RUNNING 运行状态
- TASK_INTERRUPTIBLE 可中断睡眠状态，针对等待某件事或其他资源的睡眠进程设置的，在内核发送信号给该进程表明事件发生时，进程状态转变为TASK_RUNNING，只要调度器选中该进程即可恢复执行
- TASK_UNINTERRUPTIBLE由于内核指示而停用的睡眠进程，他们不能由外部信号唤醒，只能由内核亲自唤醒
- TASK_STOPPED 停止
- TASK_ZOMBIE 僵尸状态

top命令中的loadavg是5，10，15分钟的TASK_RUNNING+TASK_UNINTERRUPTINLE进程的平均数，数值为核数时负载满

### 内存管理

### 文件系统

### 设备管理

### 缓存与锁

## 网络

### 三次握手与4次挥手

### TIME_WAIT与CLOSE_WAIT

### select/pool/epoll

### openvswitch

## 设计模式


# 编程语言

## java

### 数据结构实现

### 并发

#### Synchronized 和 ReenTrantLock 的对比

- 两者都是可重入锁
- synchronized 依赖于 JVM 而 ReenTrantLock 依赖于 API
- ReenTrantLock 比 synchronized 增加了一些高级功能:①等待可中断；②可实现公平锁；③可实现选择性通知

### JVM

#### 垃圾回收机制

#### 垃圾回收算法

#### 内存模型

#### minor gc与full gc触发条件

#### STW

#### 双亲委派模型

### 新特性

### IO&NIO&AIO

## python

### 生成器与迭代器
### 装饰器

## go


# 软件技术

## Spring


### AOP&IOC的实现

- AOP 基于代理模式 
- IOC容器：通过引入IOC容器，利用依赖关系注入的方式，实现对象之间的解耦

IOC容器的工作模式看做是工厂模式的升华，可以把IOC容器看作是一个工厂，这个工厂里要生产的对象都在配置文件中给出定义，然后利用编程语言的的反射编程，根据配置文件中给出的类名生成相应的对象。从实现来看，IOC是把以前在工厂方法里写死的对象生成代码，改变为由配置文件来定义，也就是把工厂和对象生成这两者独立分隔开来，目的就是提高灵活性和可维护性。

### 动态代理实现

### 如何解决循环依赖(三级缓存)

### @Transactional如何实现


## Zookeeper

### ZAB

### 选举算法和流程

## KafKa

Kafka通过Zookeeper管理集群配置，选举leader，以及在Consumer Group发生变化时进行rebalance

Producer只将该消息发送到该Partition的Leader。Leader会将该消息写入其本地Log。每个Follower都从Leader pull数据

为了提高性能，每个Follower在接收到数据后就立马向Leader发送ACK，而非等到数据写入Log中

### 特点

- 以时间复杂度为O(1)的方式提供消息持久化能力
- 高吞吐率
- 支持Kafka Server间的消息分区，及分布式消费
- 同时支持离线数据处理和实时数据处理
- Scale out：支持在线水平扩展

### 高性能

- 利用Partition实现并行处理，可以把一个Partition当作一个非常长的数组，可通过这个“数组”的索引（offset）去访问其数据
- ISR实现可用性与数据一致性的动态平衡，Leader可以按顺序接收大量消息，最新的一条消息的Offset被记为High Wartermark。而只有被ISR中所有Follower都复制过去的消息才会被Commit，Consumer只能消费被Commit的消息commit offset；由于Follower的复制是严格按顺序的，所以被Commit的消息之前的消息肯定也已经被Commit过

```
1. 由于Leader可移除不能及时与之同步的Follower，故与同步复制相比可避免最慢的Follower拖慢整体速度，也即ISR提高了系统可用性。

2. ISR中的所有Follower都包含了所有Commit过的消息，而只有Commit过的消息才会被Consumer消费，故从Consumer的角度而言，ISR中的所有Replica都始终处于同步状态，从而与异步复制方案相比提高了数据一致性。

3. ISR可动态调整，极限情况下，可以只包含Leader，极大提高了可容忍的宕机的Follower的数量。与Majority Quorum方案相比，容忍相同个数的节点失败，所要求的总节点数少了近一半。
```
- 顺序写磁盘，Kafka删除Segment的方式，是直接删除Segment对应的整个log文件和整个index文件
- 充分利用Page Cache
- 支持多Disk Drive
- 零拷贝sendfile，4次拷贝
- 批处理
- 数据压缩降低网络负载
- 高效的序列化方式

## ElasticSearch

#### Lucene中最重要的就是它的几种数据结构，这决定了数据是如何被检索的，本文再简单描述一下几种数据结构：

- FST（有限状态转移）：保存term字典，可以在FST上实现单Term、Term范围、Term前缀和通配符查询等。
- 倒排链：保存了每个term对应的docId的列表，采用skipList的结构保存，用于快速跳跃。
- BKD-Tree：BKD-Tree是一种保存多维空间点的数据结构，用于数值类型(包括空间点)的快速查找。
- DocValues：基于docId的列式存储，由于列式存储的特点，可以有效提升排序聚合的性能。


#### Lucene数据类型

Lucene中包含了四种基本数据类型，分别是：

- Index：索引，由很多的Document组成。
- Document：由很多的Field组成，是Index和Search的最小单位。
- Field：由很多的Term组成，包括Field Name和Field Value。
- Term：由很多的字节组成。一般将Text类型的Field Value分词之后的每个最小单元叫做Term。

在lucene中，读写路径是分离的。写入的时候创建一个IndexWriter，而读的时候会创建一个IndexSearcher


#### 写入流程

- 根据routing规则获得primary shard
- 到达primary shard写lucene创建好索引，写translog，flush到磁盘
- 写Lucene内存后，并不是可被搜索的，需要通过Refresh把内存的对象转成完整的Segment后，然后再次reopen后才能被搜索，一般这个时间设置为1秒钟，导致写入Elasticsearch的文档，最快要1秒钟才可被从搜索到，所以Elasticsearch在搜索方面是NRT（Near Real Time）近实时的系统
- 可靠性：由于Lucene的设计中不考虑可靠性，在Elasticsearch中通过Replica和TransLog两套机制保证数据的可靠性。
- 一致性：Lucene中的Flush锁只保证Update接口里面Delete和Add中间不会Flush，但是Add完成后仍然有可能立即发生Flush，导致Segment可读。这样就没法保证Primary和所有其他Replica可以同一时间Flush，就会出现查询不稳定的情况，这里只能实现最终一致性。
- 原子性：Add和Delete都是直接调用Lucene的接口，是原子的。当部分更新时，使用Version和锁保证更新是原子的。
- 隔离性：仍然采用Version和局部锁来保证更新的是特定版本的数据。
- 实时性：使用定期Refresh Segment到内存，并且Reopen Segment方式保证搜索可以在较短时间（比如1秒）内被搜索到。通过将未刷新到磁盘数据记入TransLog，保证对未提交数据可以通过ID实时访问到。
- 性能：性能是一个系统性工程，所有环节都要考虑对性能的影响，在Elasticsearch中，在很多地方的设计都考虑到了性能，一是不需要所有Replica都返回后才能返回给用户，只需要返回特定数目的就行；二是生成的Segment现在内存中提供服务，等一段时间后才刷新到磁盘，Segment在内存这段时间的可靠性由TransLog保证；三是TransLog可以配置为周期性的Flush，但这个会给可靠性带来伤害；四是每个线程持有一个Segment，多线程时相互不影响，相互独立，性能更好；五是系统的写入流程对版本依赖较重，读取频率较高，因此采用了versionMap，减少热点数据的多次磁盘IO开销。Lucene中针对性能做了大量的优化。后面我们也会有文章专门介绍Lucene中的优化思路。

https://zhuanlan.zhihu.com/p/34669354

https://zhuanlan.zhihu.com/p/35285514

https://zhuanlan.zhihu.com/p/34858035

## Redis

### 数据类型
### zset跳表的数据结构
### 数据过期策略
### LRU过期策略的实现
### 持久化aof/rdb
### 缓存穿透与缓存雪崩

## etcd

#### 支持的操作

- put(key, vlaue)/delete(key)
- get(key)/get(keyFrom, keyEnd)
- watch(key/keyPrefix)
- transactions(if/then/else ops).commit
- leases 租期


#### 版本机制

- term 全局递增，raft leader任期
- revision 全局递增，任何数据变更后会自增
- KeyValue

```
create_revision 创建时全局revision值
mod_revision    修改时全局revision值
version         keyvalue的修改版本
```

- mvcc

```
一个数据有多个版本
通过定期compaction清理数据
```

#### 典型应用场景

- 元数据存储 (k8s api资源对象)
- 服务发现
- 选主
- 分布式系统并发控制

## MySQL

### 最左前缀匹配
### 非聚集索引与聚集索引(innodb)
### 事物隔离级别

- read uncommited
- read commited
- read repeated + mvcc
- serialize

### innodb表锁与行锁

### explain如何解析SQL

### MVCC多版本并发控制

### binlog & redolog & undolog

# 分布式架构

## CAP

- consistency一致性
- avaliable可用性
- partition分区容忍性

P是一定存在的，所以一般选择为AP，CP

[相关问题](https://github.com/CyC2018/CS-Notes/blob/master/notes/%E5%88%86%E5%B8%83%E5%BC%8F.md#%E4%B8%80%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81)

### 2PC

### 分布式一致性模式

#### 弱一致性 VOIP

读可能会拿到最新的写

#### 最终一致性 DNS

读最终会拿到最新的写

#### 强一致性 关系型数据库

读总能拿到最新的写

### 基础理论及模式

### paxos

### raft

## Cloud Native

### K8S

#### k8s/mesos/swarm差异
#### borg/omega/k8s差异
#### API对象
#### operator

### ServiceMesh

服务发现与治理下沉到基础设施上

### prometheus

### helm

## Spring Cloud（了解）

SpringCloud的基础功能：

- 服务治理： Spring Cloud Eureka
- 客户端负载均衡： Spring Cloud Ribbon
- 服务容错保护： Spring Cloud Hystrix
- 声明式服务调用： Spring Cloud FeignA
- API网关服务：Spring Cloud Zuul
- 分布式配置中心： Spring Cloud Config

# 参考


[基础](https://github.com/CyC2018/CS-Notes)

[系统设计](https://github.com/donnemartin/system-design-primer)

[SRE](https://github.com/mxssl/sre-interview-prep-guide)

[论文](https://github.com/dyweb/papers-notebook)
