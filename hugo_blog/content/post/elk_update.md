+++
keywords = ["ELK", "update"]
title = "ELK集群2.x到5.6升级记录"
categories = ["ELK"]
comments = true
clearReading = true
disqusIdentifier = "elk_update"
date = "2018-04-02T20:10:59+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++
   
考虑到目前使用的ELK集群版本与开源版本的版本差距有点大，而ELK5.6相较2.3版本性能有较大提升，尤其是Logstash grok插件，最近对测试环境的两个ELK集群进行了升级，对升级过程进行一个记录；升级主要参考了官方[文档](https://www.elastic.co/guide/en/elastic-stack/5.6/upgrading-elastic-stack.html)

#### 当前系统运行版本

- filebeat：1.2.3
- kibana：4.5.1
- logstash：2.3.2
- elasticsearch：2.3.3   JVM: 1.7

#### 升级顺序

- jvm(略)
- elasticsearch
- kibana
- logstash
- beats

#### 升级Elasticsearch

###### 升级之前

- 升级前使用[Elasticsearch Migration Plugin](https://github.com/elastic/elasticsearch-migration/)查看潜在问题
- 最好先在测试环境升级ELK
- 在升级之前先备份数据

###### 升级（跨大版本整个集群需重启）

1. 禁止自动分配分片，在停止elasticsearch时减少不必要的I/O

    ```
    curl -XPUT 'localhost:9200/_cluster/settings?pretty' -H 'Content-Type: application/json' -d'
    {
        "persistent": {
            "cluster.routing.allocation.enable": "none"
        }
    }'
    ```
2. 同步刷新以加快分片恢复

    ```
    curl -XPOST 'localhost:9200/_flush/synced?pretty'
    ```
3. 停止并升级各个节点（安装新的rpm或deb，更新配置文件，配置文件有较大改动）

	```
	/etc/init.d/elasticsearch stop
	apt-get pruge elasticsearch(所有deb包已在本地apt源中,删除原配置文件)
	apt-get install elasticsearch=5.6.5
    ```
4. 升级插件（需删除旧的插件）

	```
    /usr/share/elasticsearch/bin/elasticsearch-plubin remove license
    /usr/share/elasticsearch/bin/elasticsearch-plubin remove marvel
    /usr/share/elasticsearch/bin/elasticsearch-plubin install x-pack
    ```
5. 启动集群各个节点，保证所有节点均加入集群(优先启动master node)

	```
	curl -XGET 'localhost:9200/_cat/health?pretty'
	curl -XGET 'localhost:9200/_cat/nodes?pretty'
	```
6. 等待集群状态变为yellow，即所有主分片已恢复
7. 使能自动分配分片
    
    ```
    curl -XPUT 'localhost:9200/_cluster/settings?pretty' -H 'Content-Type: application/json' -d'
    {
        "persistent": {
            "cluster.routing.allocation.enable": "all"
        }
    }'
    ```
8. 等待集群状态变为green

#### 升级Kibana

1. 停止kibana服务
	
	```
	/etc/init.d/kibana stop	
	```
2. 使用rpm、apt-get安装新的kibana

	```
	apt-get purge kibana (保证配置文件删除)
	apt-get install kibana=5.6.5
	```
3. 更新插件

	```
	/usr/share/kibana/bin/kibana install x-pack
	```
4. 启动kibana

	```
	/etc/init.d/kibana start	
	```

#### 升级logstash

1. 停止logstash

	```
	/etc/init.d/logstash stop
	```
2. 使用rpm、apt-get安装新的logstash
	```
	apt-get purge logstash (保证配置文件删除)
	apt-get install logstash=5.6.5-1
	```
3. 更新配置文件，并测试配置文件，2.3.2版本配置文件与5.6.5配置文件改动较大

	```
	logstash --config.test_and_exit -f logstash.conf
	```
4. 启动logstash

	```
	/etc/init.d/logstash start
	```

#### 升级filebeat


1. 停止filebeat

	```
	/etc/init.d/filebeat stop
	```
2. 安装filebeat 5.6.5

	```
	apt-get purge filebeat (保证配置文件删除)
	apt-get install filebeat=5.6.5
	```
3. 更新配置文件，配置文件改动较大

    ```
    /usr/share/packetbeat/scripts/migrate_beat_config_1_x_to_5_0.py filebeat.yml
    ```
4. 启动

    ```
	/etc/init.d/filebeat start
	```
	
#### Tips：
	
	filebeat-1.2.3\logstash-2.3.2\kibana-5.6.5\elasticsearch-5.6.5兼容

[兼容性](https://www.elastic.co/support/matrix#matrix_compatibility)

[参考链接](https://www.elastic.co/guide/en/elastic-stack/5.6/upgrading-elastic-stack.html)
