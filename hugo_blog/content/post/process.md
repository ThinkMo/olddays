+++
disqusIdentifier = "159101007_process"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2015-09-10T10:07:34+08:00"
showDate = true
clearReading = true
keywords = ["linux process"]
title = "进程监视"
categories = [ "linux" ]
+++
# 进程监视
## 1、ps监视进程的主要工具
	
	ps -ef 查看每一个进程
	ps aux 可以展示系统进程的全貌
	ps lax 更快，省去了将uid转换为用户名
	ps -ejH  or  ps axjf 进程树
	ps -eLf  or  ps axms 线程信息

### 字段含义
字段 | 内容
---- | ----
USER|进程属主用户名
PID|进程ID
CPU|进程使用CPU百分比
MEM|进程使用内存百分比
VSZ|进程虚拟内存大小KiB
RSS|驻留集大小，非swap中的内存大小
STAT|进程状态 S:可中断睡眠 D:不可中断睡眠 R:running s会话头 
TIME|运行时间
COMMAND|命令行
NI|nice值
WCHAN|等待的资源

## 2、top动态查看


	top对活动进程及所使用的资源情况提供了汇总信息
	
## 3、proc文件系统
	proc文件系统提供了内核产生的所有状态信息与数据系统，包括进程相关的信息，linux的ps、top都是从/proc目录读取进程的状态信息。进程特有的信息存储在/proc/pid下。
		
		
## 4、strace
	strace -p pid可以追踪进程的系统调用及信号，在调试进程、理解程序执行过程非常有帮助。

## 5、vmstat
	vmstat提供了关于进程、内存、内存页、块IO、陷阱、磁盘及CPU的活动信息。

###字段含义
字段 | 内容
---- | ----
r|等待执行的进程数
b|不可中断睡眠的进程数
swpd|使用虚拟内存大小
free|空闲内存
cache|用作cache的内存大小
si|swap in
so|swap out
bi|block in从块设备收到数据块数
bo|block out
in|每秒中断数
cs|每秒上下文切换数
us|user time
sy|system time
id|idle time
wa|wait for io
