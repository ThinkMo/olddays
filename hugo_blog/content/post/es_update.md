+++
keywords = ["Elasticsearch", "thread_pool"]
title = "ElasticSearch配置滚动更新"
categories = ["ELK"]
comments = true
clearReading = true
disqusIdentifier = "elk_0806"
date = "2018-08-06T19:47:59+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

#### 起因
最近Logstash经常打印如下日志内容，虽然只是INFO级别的错误但还是引起了我的注意，大概意思是ES的bulk thread pool大小为32已经满负荷，队列也满了，所以拒绝了Logstash的bulk请求

```
[2018-06-23T08:00:47,281][INFO ][logstash.outputs.elasticsearch] retrying failed action with response code: 429 ({"type"=>"es_rejected_execution_exception", "reason"=>"rejected execution of org.elasticsearch.transport.TransportService$7@177d229b on EsThreadPoolExecutor[bulk, queue capacity = 1000, org.elasticsearch.common.util.concurrent.EsThreadPoolExecutor@66a1a8c3[Running, pool size = 32, active threads = 32, queued tasks = 1019, completed tasks = 832616617]]"})
```

#### 解决思路

- 扩大处理bulk的thread pool线程数量

```
当前使用的服务器CPU核数为176(cat /proc/cpuinfo)，而在ES源码中为了避免引起Java oom线程数会取min(32, 核数)，所以造成默认的thread_pool.bulk.size为32，为了扩大线程容量需在ES中添加以下配置
processors: 96
thread_pool.bulk.size: 96
```
- 增加等待队列长度

```
thread_pool.bulk.queue_size: 3000
```

#### 更新配置

这些配置不能通过ES Setting API来更改，只能通过滚动重启的方式，下面简单记录下过程

- 禁止分配分片

```
curl -X PUT "ES_HOST:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
  "transient": {
    "cluster.routing.allocation.enable": "none"
  }
}
'
```
- 停止索引，可以加快恢复

```
curl -X POST "ES_HOST:9200/_flush/synced"
```
- 更新配置，重启ES
- 查看节点信息，确保新节点加入集群

```
curl -X GET "ES_HOST:9200/_cat/nodes"
```
- 使能分片分配

```
curl -X PUT "ES_HOST:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
  "transient": {
    "cluster.routing.allocation.enable": "all"
  }
}
'
```
- 查看集群状态直到变为Green

```
curl -X GET "ES_HOST:9200/_cat/health"
curl -X GET "ES_HOST:9200/_cat/recovery"
```

- 重复以上步骤直到集群中所有节点配置都已更新完毕


#### ES优化点

```
内存：31G 不是越大越好

硬盘：raid0 SSD更佳（SSD使用deadline、noop调度策略）

CPU：多核>多cpu

JVM 8

最小主节点数：此设置应该始终被配置为 master 候选节点的法定个数（大多数个）。法定个数就是 ( master 候选节点个数 / 2) + 1

集群恢复配置（避免部分启动造成的数据均衡）：
gateway.expected_nodes: 10
gateway.recover_after_time: 5m

禁止内存交换：
sudo swapoff -a
vm.swappiness = 1
bootstrap.mlockall: true
```
