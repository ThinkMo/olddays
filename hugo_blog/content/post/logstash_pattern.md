+++
keywords = ["logstash", "grok"]
title = "Logstash过滤插件grok正则解析"
categories = ["ELK"]
comments = true
clearReading = true
disqusIdentifier = "elk_1010"
date = "2017-10-10T21:31:09+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++
### Logstash过滤插件grok正则解析
#### 一、grok介绍

grok是Logstash中用来解析非结构化日志数据，将日志转化为可查询的结构化数据的最佳方法，可以用来处理syslog日志、apache等webserver日志、mysql日志以及用户自定义日志。

Logstash自带有120多种预定义好的正则表达式方便用户使用，你可以在[这里](https://github.com/logstash-plugins/logstash-patterns-core/tree/master/patterns)查看这些正则表达式，你也可以添加自己的匹配规则。

有两个网站可以帮助我们来构建正则表达式去匹配我们的日志：

- http://grokdebug.herokuapp.com
- http://grokconstructor.appspot.com (推荐)

#### 二、grok基础

grok匹配模式语法为：%{SYNTAX:SEMANTIC:TYPE}

SYNTAX: 正则表达式、预定义的正则表达式名称

SEMANTIC: 标识符，标识匹配后的数据

TYPE: 可选的类型，目前支持int、float

例如：NUMBER可以匹配3.44，IP可以匹配：192.168.21.2

一个复杂的日志格式如下：
```
192.168.21.2 GET /index.html 15823 0.023
```
grok匹配模式可以为：
```
${IP:client} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:duration}
```
更加实际的，该条日志可能来自一个文件：
```
input {
  file {
    path => "/var/log/http.log"
  }
}
filter {
  grok {
    match => { "message" => "%{IP:client} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:duration}" }
  }
}
```
在grok过滤后，可以得到额外一下字段：
```
client: 192.168.21.2
method: GET
request: /index.html
bytes: 15823
duration: 0.023
```
#### grok中的正则表达式

Grok采用了Oniguruma正则表达式库，在[on the Oniguruma site](https://github.com/kkos/oniguruma/blob/master/doc/RE)看到支持的正则语法。

如果logstash中没有你需要的匹配模式，你可以使用Oniguruma语法实现自定义正则匹配：
```
(?<field_name>the pattern here)
```

例如：匹配一个10到11位的16进制```(?<queue_id>[0-9A-F]{10,11})```

更佳的做法是，新建一个自定义模式文件：
- 创建一个正则模式目录以及包含正则表达式的文件
- 在这个文件中编写正则表达式，像logstash预定义的正则匹配名称一样
```
# ./pattern/postfix
POSTFIX_QUEUEID [0-9A-F]{10,11}
```
在logstash配置文件中便可以使用POSTFIX_QUQUEID了
```
filter {
  grok {
    patterns_dir => ["./patterns"]
    match => { "message" => "%{SYSLOGBASE} %{POSTFIX_QUEUEID:queue_id}: %{GREEDYDATA:syslog_message}" }
  }
}
```

#### grok配置选项

grok支持许多额外的配置选项，具体信息见[这里](https://www.elastic.co/guide/en/logstash/5.3/plugins-filters-grok.html#_synopsis_130)：

比较常用的有：

- add_field

    增加自定义字段

- add_tag

    增加tag
    
- overwrite

    重写字段，时间戳等
    
- match

    匹配过滤
