+++
disqusIdentifier = "network_params_723"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-06-17T21:03:24+08:00"
showDate = true
clearReading = true
keywords = ["kernel network params"]
title = "认识Linux内核网络参数"
categories = [ "linux" ]
+++
## 认识Linux内核网络参数
- 本地端口
	
	TCP、UDP使用的本地端口范围
	
	```net.ipv4.ip_local_port_range = 1024 65535  （/etc/sysctl.conf，service network restart）```
	

- 优化短链接

选项|含义
----|----
net.ipv4.tcp_fin_timeout = 15|处于FIN-WAIT-2状态的时间，建议10
net.ipv4.tcp_tw_reuse = 1|允许将 TIME-WAIT sockets重新用于新的TCP连接
net.ipv4.tcp_tw_recycle = 1 |表示开启TCP连接中TIME-WAIT sockets的快速回收(建议关闭)
net.ipv4.tcp_syncookies| 防止syn flood攻击，当syn backlog满时发送syncookies(建议关闭)
net.ipv4.tcp_syn_retries| 初始SYN重传次数，默认5
net.ipv4.tcp_keepalive_intvl|tcp keepalive探测间隔时间，默认75s，可减小
net.ipv4.tcp_keepalive_probes|最大探测次数，默认9，可减小
net.ipv4.tcp_keepalive_time|只有在SO_KEEPALIVE设置时才启用，链接空闲多久发送keepalive，默认7200s，空闲后大概11min关闭连接，可减小
	
	

- 缓冲区大小
	
	套接字的缓冲区大小限制,从Linux2.6.7内核会根据传输情况自动调整
	
	默认值：
	
	net.ipv4.tcp_rmem = 4096 87380 4194304
	net.ipv4.tcp_wmem = 4096 16384 4194304
	
	TCP读写缓存区，缓存区超过4194304，tcp包会丢弃
	
	BDP(带宽延时积)=B*D (带宽＊延时)  反推 带宽=BDP/延时   4194304/(0.015\*2)/1024/1024=133M/s
	
	建议值：
	
选项|含义
----|----
net.core.rmem_default = 262144|默认套接字接受缓存区大小
net.core.wmem_default = 262144|默认套接字发送缓存区大小
net.core.rmem_max = 16777216|接收最大值
net.core.wmem_max = 16777216|发送最大值
net.ipv4.tcp_rmem = 4096 87380 16777216|tcp接收缓存区大小
net.ipv4.tcp_wmem = 4096 65536 16777216|tcp发送缓存区大小

- 增大初始拥塞窗口
	
	依据：慢启动
	
	- 规则
		窗口从一个小的值开始
		指数增长
		上限阈值
	- 合理性
		避免淹没慢的接收方
		避免网络瘫痪
	- 问题
		往往慢启动还没终止，连接已经结束
		用户的速度极限还没到
	
	根据实验结果进行设置慢启动窗口大小 ip route change
	```
	ip route | while read p; do
		ip route change $p initcwnd 10
	done
	```
	
	提高性能百分比，降低TCP重传率的影响

- 拥塞控制算法

	net.ipv4.tcp_congestion_control = cubic

- 服务器开发

	* 操作的对象是socket读写缓冲区
	* send/write成功并不代表已经发送到对端
	* 应用程序中响应时间的含义
	* 明白设置缓冲区大小的影响
	* 关键应用需要保证可靠性
	* 需要应用级别的心跳检测
	
	* 合理使用重要的TCP选项
		* TCP_DEFER_ACCEPT
		* TCP_CORK
		* TCP_NODELAY
	* 使用writev/readv
	* 真正理解non-blocking的套接字编程
	* 真正理解epoll

- 队列
	
	* Listen队列
		* net.ipv4.tcp_max_syn_backlog = 16384 
		* net.core.somaxconn = 2048		
    * 网卡的接收队列
		* net.core.netdev_max_backlog = 10000
	* 网卡发送队列(网卡中断绑定)
		* ifconfig eth0 txqueuelen 10000

```
man listen
The behavior of the backlog argument on TCP sockets changed with Linux 2.2.
Now  it  specifies  the queue  length  for  completely  established sockets 
waiting to be accepted, instead of the number of incomplete connection 
requests.  The maximum length of the queue for incomplete sockets can  be  set 
using /proc/sys/net/ipv4/tcp_max_syn_backlog.  When syncookies are enabled 
there is no logical maxi‐mum length and this setting is ignored.  See tcp(7) 
for more information.

If the backlog argument is greater than  the  value  in  /proc/sys/net/core/
somaxconn,  then  it  is silently  truncated to that value; the default value 
in this file is 128.  In kernels before 2.4.25,this limit was a hard coded 
value, SOMAXCONN, with the value 128.

net.ipv4.tcp_max_syn_backlog：最大半开连接数，未完成TCP 3次握手的连接数，如果设置了
net.ipv4.tcp_syncookies则该设置被忽略

net.core.somaxconn：最大连接数(完成TCP 3次握手)，listen函数backlog含义相同，当backlog大
于somaxconn时设置为somaxconn
```
	
－ TCP拥塞控制
		
		手段
			慢启动
			拥塞避免
			快速重传
			快速恢复
		目的
			探测网络速度
			保证传输顺畅
			
		TIME_WAIT
			net.ipv4.tcp_max_tw_buckets 处于TIME_WAIT状态sockets最大值
			主动关闭   服务端不主动关闭socket连接不会走到TIME_WAIT状态，no zuo no die
			2＊MSL
		CLOSE_WAIT
			被动关闭
			99%意味着程序有bug  从TCP状态图来看，收到FIN进入CLOSE_WAIT，未关闭socket、发送FIN，停留在该状态