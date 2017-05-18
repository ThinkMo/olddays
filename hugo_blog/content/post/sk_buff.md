+++
date = "2017-04-04T19:07:48+08:00"
showSocial = false
showPagination = true
coverCaption = "A beautiful sunrise"
disqusIdentifier = "fdsF342234"
title = "认识sk_buff结构体"
comments = true
clearReading = true
showDate = true
showTags = true
keywords = ["linux","network"]
categories = [ "network" ]

+++


## sk_buff结构体

	struct sk_buff {
	/* These two members must be first. */
	struct sk_buff		*next;
	struct sk_buff		*prev;
	/* sk_buff双向链表 */
	ktime_t			tstamp;
	/* 报文接收时间戳，是个偏移量 */
	struct sock		*sk;
	/* 拥有此skb的socket */
	struct net_device	*dev;
	/* SKB收发的网络设备 */

	/*
	 * This is the control buffer. It is free to use for every
	 * layer. Please put your private variables there. If you
	 * want to keep them across layers you have to do a skb_clone()
	 * first. This is owned by whoever has the skb queued ATM.
	 */
	char			cb[48] __aligned(8);
	/* 控制缓存区，TCP、buletooth协议都有用到 */

	unsigned long		_skb_refdst;
	/* 指向dst_entry地址 */
	unsigned int		len,
				data_len;
	/* len:报文总长度，data_len:数据长度，仅当非线性存储报文时使用 */
	__u16			mac_len,
				hdr_len;
	/* mac_len:2层头长度，hdr_len:3层头长度*/
	union {
		__wsum		csum;
		struct {
			__u16	csum_start;
			__u16	csum_offset;
		};
	};
	/* 校验和 */
	__u32			priority;
	/* 报文队列优先级 */
	kmemcheck_bitfield_begin(flags1);
	/* 位域开始指针 */
	__u8			local_df:1,   //分片标志
				cloned:1,         //克隆标志，仅克隆SKB数据共享
				ip_summed:2,      //IP层校验和标志位 CHECKSUM_NONE需软件计算校验和 
				nohdr:1,          //报文头标志位，不需要头时置位
				nfctinfo:3;       //连接追踪，NAT中使用
	__u8			pkt_type:3,   //报文类型：多播、广播、单播、非本地
				fclone:2,         //克隆状态
				ipvs_property:1,  //ip virtual server标志，4层负载均衡
				peeked:1,         //是否被查看标志
				nf_trace:1;       //netfilter trace标志
	kmemcheck_bitfield_end(flags1);
	__be16			protocol;     //协议

	void			(*destructor)(struct sk_buff *skb);   //释放skb的回调函数

	int			skb_iif;
    //接收skb的接口索引
	__u32			rxhash;
    //源目的IP地址＋port计算hash值，保证SMP下相同flow被同一CPU处理，提高Cache命中率
	__be16			vlan_proto;
	//vlan协议 802.1q 、802.1ad
	__u16			vlan_tci;
	//vlan tag:包括低12位id，1位标志位，高3位优先级
	__u16			queue_mapping;
	//多队列网卡队列映射
	kmemcheck_bitfield_begin(flags2);
	__u8			pfmemalloc:1;    //标志位，pfmemalloc分配的skb
	__u8			ooo_okay:1;      //out of order
	__u8			l4_rxhash:1;     //4元组hash
	__u8			wifi_acked_valid:1;  
	__u8			wifi_acked:1;
	__u8			no_fcs:1;        //使网卡SKB最后4字节为帧间序
	__u8			head_frag:1;
	/* Encapsulation protocol and NIC drivers should use
	 * this flag to indicate to each other if the skb contains
	 * encapsulated packet or not and maybe use the inner packet
	 * headers if needed
	 */
	__u8			encapsulation:1;   //是否为封装报文，如vxlan
	/* 7/9 bit hole (depending on ndisc_nodetype presence) */
	kmemcheck_bitfield_end(flags2);
	union {
		__u32		mark;            //标记skb，用于netfilter
		__u32		dropcount;          
		__u32		reserved_tailroom;
	};

	sk_buff_data_t		inner_transport_header;
	sk_buff_data_t		inner_network_header;
	sk_buff_data_t		inner_mac_header;
	sk_buff_data_t		transport_header;
	sk_buff_data_t		network_header;
	sk_buff_data_t		mac_header;
	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;       //指向数据尾  skb_put
	sk_buff_data_t		end;        //指向缓存区尾
	unsigned char		*head,      //指向缓存区头  
				*data;              //指向数据开头  skb_push skb_pull
	unsigned int		truesize;   //总大小 包括skb结构体与block
	atomic_t		users;  //引用计数
	};
	
## skb_shared_info结构体(位于block后，skb->end)

 	260 struct skb_shared_info {
 	261     unsigned char   nr_frags;   //数组frags大小
 	262     __u8        tx_flags;       //tx标志位
 	263     unsigned short  gso_size;           
 	264     /* Warning: this field is not always filled in (UFO)! */
 	265     unsigned short  gso_segs;
 	266     unsigned short  gso_type;
 	267     struct sk_buff  *frag_list;      //分片
 	268     struct skb_shared_hwtstamps hwtstamps;
 	269     __be32          ip6_frag_id;
 	270
 	271     /*
 	272      * Warning : all fields before dataref are cleared in __alloc_skb()
 	273      */
 	274     atomic_t    dataref;    //引用计数
 	275
 	276     /* Intermediate layers must ensure that destructor_arg
 	277      * remains valid until skb destructor */
 	278     void *      destructor_arg;
 	279
 	280     /* must be last field, see pskb_expand_head() */
 	281     skb_frag_t  frags[MAX_SKB_FRAGS];
 	282 };
