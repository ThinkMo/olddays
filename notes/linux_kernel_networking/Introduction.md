#####Linux网络协议栈

- OSI7层模型

应用层|
----|----
表示层|
会话层|
传输层|
网络层|
链路层|
物理层|

- Linux内核协议栈主要处理L2、L3、L4这3层

L4(TCP/UDP,...)|
----|----
L3(IPv4, IPv6,...)|
L2(Ethernet,...)|

- 网络设备结构体struct net_device

结构体net_device包含了：

	1. 中断号 IRQ
	2. MTU  以太网为1500
	3. MAC地址
	4. 设备名 eth0 or eth1
	5. flags
	6. 多播地址
	7. 混杂模式计数  类型为int，当值大于0，处于混杂模式，接收所有报文
	8. 特性支持  GSO、TSO、GRO
	9. 网络设备回调函数  net_device_ops
	10. ethtool回调函数  ethtool eth0 tso on
	11. Tx  Rx queues
	12. 时间戳
	
	
－ NAPI

	旧的网络设备驱动采用中断驱动模式，意味着没接收一个数据报就有一次中断请求，当网络流量高时造成CPU计算资源的浪费。NAPI应运而生，NAPI由2.5/2.6内核引入，2.4支持；在高负载下，网络设备驱动采用Polling模式，收到报文时不再触发中断请求，而是将报文存储在驱动缓存区中，内核轮询驱动获得数据报。
	＊为了支持对延时有较高要求的应用，3.11后的内核添加了Busy Poll Socket。
	
- 报文接收与发送

		网络设备驱动的主要任务是：	
		* 接收目的地址为本机的报文，将报文传给网络层处理
		* 发送本地产生的报文或转发本地接收的报文
	
		发送或接收报文时路由子系统会执行路由查找。此外，5个Netfilter callback也会决定报文在网络协议栈的传输，netfilter回调函数由nf_register_hook()、nf_register_hooks()注册。用户态iptables实现依赖底层netfilter子系统。
	
		除了netfilter，报文还受Ipsec子系统的影响，IPsec是网络层安全解决方案，它使用了AH(认证头)、ESP(封装安全载荷)协议；IPsec在ipv4中是可选的，在ipv6中是必须的；IPsec有两种模式：传输模式与隧道模式。
	
		IPv4头中的ttl也会影响数据报的传输，报文每经过一个网络设备ttl值便会减1，当ttl＝0时报文会被丢弃；在IPv6中同样的原理，变量为hop_limit.
	
		报文过大时，在发送时会被分片，接收时组合，这发生在网络层；多播组由IGMP协议管理；
	
		为了更好的理解报文的传输，需了解报文在内核中如何表示,即结构体struct sk_buff，定义在include/linux/skbuff.h
	
- 套接字缓存区  SKB

		需要操作sk_buff结构体时需使用SKB API，如需访问skb->data使用skb_pull_inline()或skb_pull()，访问传输层头调用skb_transport_header()，访问网络层头调用skb_network_header()，访问链路层头调用skb_mac_header().
		
		当收到一个报文时，网络设备驱动会调用netdev_alloc_skb()或dev_alloc_skb()(改函数调用netdev_alloc_skb()首个参数为NULL)分配一个SKB，当一个报文被丢弃时，会调用kfree_skb或dev_kfree_skb释放SKB。
		
		SKB中的某些成员值是在链路层中决定的。例如：在函数eth_type_trans()中根据目的mac地址将决定pkt_type的值，如果mac地址是多播地址，则pkt_type设置为PACKET_MULTICAST，如果mac地址为广播地址，则设置为PACKET_BROADCAST，如果mac地址为本地地址，则设置为PACKET_HOST，大多数网络设备驱动在RX路径上会调用eth_type_trans()函数，该函数也会根据二层头ethertype设置SKB中protocol字段，该函数同时会调用skb_pull_inline()将data增加14(ETH_LEN)字节，即二层头长度，调用eth_type_trans()后会进入网络层处理，所以将data移动以指向网络层头地址。
		
		SKB包含报文头与数据内容，在报文处理过程中各层头可以被添加与移除。
		
		每个SKB都包含一个网络设备结构体net_device实例dev，对收到的报文dev指向接收设备，对发送的报文dev指向发出的设备。通过dev可以获得网络设备相关信息比如MTU等。SKB有一个指示相关套接字对象的sk成员，如果是转发报文，则sk为NULL，因为该报文不是本地生成。
		
- 各章节内容介绍

- linux内核网络开发模式

		git  版本控制
		LKML 



