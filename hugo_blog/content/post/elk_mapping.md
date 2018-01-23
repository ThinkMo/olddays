+++
keywords = ["ELK", "mapping"]
title = "ElasticSearch索引类型映射"
categories = ["ELK"]
comments = true
clearReading = true
disqusIdentifier = "elk_0119"
date = "2018-01-19T18:10:59+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

最近接手维护了几个ELK集群，对接的是IaaS、PaaS服务日志，简单的ELK架构，通过filebeat采集日志，发送到logstash结构化日志然后发送到ElasticSearch，用户可以通过Kibana查看服务日志定位问题、做一些性能分析图表等，同时利用elastalert做日志报警。

在ElasticSearch须要对用户的索引建立合适的类型映射(尤其是int类型)，才可以在kibana中对数据进行分析，关于ES的映射类型可以看[这里](https://www.elastic.co/guide/en/elasticsearch/reference/current/mapping.html#mapping-type)，需指出的是一旦某个field字段类型确定就很难更改该字段的类型(需reindex)。

Logstash将非结构化数据转化为结构化数据，通过JSON将数据发送给ElasticSearch，所有字段都会被当作string来处理，而ElasticSearch在自动判断字段类型建立映射这方面做的不足，与我们需求不符，那么如何正确建立索引类型映射呢？

一、通过Logstash的grok、mutate确定字段类型

- grok

  根据grok[官方文档](https://www.elastic.co/guide/en/logstash/current/plugins-filters-grok.html)，在grok正则表达式后可以添加一个数据类型，默认是string类型，如果你想使字段类型为int，你可以在表达式后加int，例如`%{NUMBER:num:int}`，那么num字段会从string类型变为int类型，目前只支持int和float。
  
- mutate
  
  通过mutate可以将field转化为integer、float、string，例如：
  
  ```
  filter {
      mutate {
         convert => { "num" => "integer" }
      }
  }
  ```  

二、ElasticSearch mapping template

映射(mappings)决定了一个字段(field)如何被ElasticSearch解释、存储，例如数据{"ip":"223.5.5.5"}发送给ES，ES会将ip字段存储为string类型，而不是ip类型，不能做IP范围查询同时造成存储空间浪费、查询效率低等。**不管在Logstash如何转换类型，ElasticSearch不会知道你的用意除非你正确映射。**所有整型会存为long，小数会存为float或double，关于最小类型可以看[这里](https://www.elastic.co/guide/en/elasticsearch/reference/5.6/number.html)，使用integer而不是long会有效的减少ELasticSearch负担。

- 映射模版

  编写模版文件my_template.json
  
  ```
  {
  		"index_patterns" : "index*",
  		"version" : 1,
  		"settings" : {
    		"index.refresh_interval" : "5s"
  		},
  		"mappings" : {
    		"_default_" : {
    			"properties":{
    				"host": { "type" : "string"},
    				"ip": {"type" : "ip"},
    				....
    			}
    		}
  		}
 	}
 ```
 上传template.json
 `curl -XPUT http://localhost:9200/_template/index_template?pretty -d @my_template.json`
	
 删除旧的索引数据
 `curl -XDELETE http://localhost:9200/index?pretty`
