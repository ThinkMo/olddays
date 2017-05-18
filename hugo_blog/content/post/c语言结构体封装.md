+++
disqusIdentifier = "23213942C_struct"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-05-11T10:23:54+08:00"
showDate = true
clearReading = true
keywords = ["c struct"]
title = "C语言结构体封装"
categories = [ "C" ]
+++

# C语言结构体封装
*源自：http://www.catb.org/esr/structure-packing/*
## 1、简介	
通过手动填充、封装C语言结构体以减少C程序的内存使用，在嵌入式系统、操作系统内核模块等内存使用有限制的场景，这种知识非常有效。当你对程序的高速缓存行命中率非常在意时，理解这部分知识也是至关重要的。
## 2、对齐
在X86与ARM处理器上，C语言基本类型并不是随意地存储在内存地址上的。除了char，每一个类型都有对齐要求。char可以存储在任何起始地址，2字节short存储的起始地址应为奇数，4字节的int、float起始地址应为4的倍数，8字节long、double起始地址应为8的倍数。C语言的基本类型是自动对齐的。字节对齐使得访问更快速，因为通过单个指令就可以对数据进行读写。如果没有对齐要求，数据可能存储在多个机器字节内，造成访问次数增加。字符是个特例。
## 3、填充
如果你不知道数据对齐的知识，你可能认为下面的3个变量会占据连续的内存空间，在32位机器上时，4字节指针、1字节字符和4字节整型。

```
char *p;     实际       char *p;     /* 4 or 8 bytes */
char c;     ======>    char c;      /* 1 byte */
int x;                 char pad[3];  /* 3 byte */
                       int x;        /* 4 byte */
```

实际上的存储如右所示，pad[3]字符数组为了对齐而浪费的存储空间。看看其他两种情况。

```
char *p;              char *p;      /* 4 or 8 bytes */
char c;     =======>  char c;       /* 1 byte */
short x;              char pad[1];  /* 1 byte */
                      short x;      /* 2 byte */
```
```
char *p;              char *p;      /* 4 or 8 byte */
char c;     =======>  char c;       /* 1 byte */
long x;               char pad[7];  /* 7 byte */
                      long x;       /* 8 byte */
```
```
char c;               char c;
char *p;    =======>  char pad1[M];
int x;                char *p;
                      char pad2[N];
                      int x;
```
N肯定是0，M可能为0-7（大端、小端字符位置决定），为了节省内存空间使用，可以交换x与c的定义位置。
## 4、结构体对齐与填充
实际上，结构体根据最宽的成员长度进行对齐的。编译器通过这种方式可以保证成员对齐提高访问速度。在c中结构体的地址与首个成员的地址相同。下面两个结构体大小均为24bytes。

```
struct foo1 {
    char *p;
    char c;
    long x;
};
struct foo2 {
    char *p;     /* 8 bytes */
    long x;      /* 8 bytes */
    char c;      /* 1 byte */
};
```

```
struct foo10 {
    char c;
    struct foo10 *p;
    short x;
};
struct foo10 {
    char c;          /* 1 byte */
    char pad1[7];    /* 7 bytes */
    struct foo10 *p; /* 8 bytes */
    short x;         /* 2 bytes */
    char pad2[6];    /* 6 bytes */
};
```
改变上面结构体内元素顺序，可以减少内存占用。

```
struct foo11 {
    struct foo11 *p;
    short x;
    char c;
};
struct foo11 {
    struct foo11 *p; /* 8 bytes */
    short x;         /* 2 bytes */
    char c;          /* 1 byte */
    char pad[5];     /* 5 bytes */
};
```

## 5、位域Bitfields
位域可以使你在结构体成员中声明小于一个字符宽度的成员，甚至一个比特位，C99标准保证在不跨越存储单元边界的情况下尽可能的整合位域。在32位机器上，结构体foo6布局如下：

```
struct foo6 {
    short s;       /* 2 bytes */
    char c;        /* 1 byte */
    int flip:1;    /* total 1 bit */
    int nybble:4;  /* total 5 bits */
    int pad1:3;    /* pad to an 8-bit boundary */
    int septet:7;  /* 7 bits */
    int pad2:25;   /* pad to 32 bits */
};
```
由于C标准没有指明位域按从低位到高位分配，结构体foo6布局也可能如下(C99标准没有指定填充)：

```
struct foo6 {
    short s;       /* 2 bytes */
    char c;        /* 1 byte */
    int pad1:3;    /* pad to an 8-bit boundary */
    int flip:1;    /* total 1 bit */
    int nybble:4;  /* total 5 bits */
    int pad2:25;   /* pad to 32 bits */
    int septet:7;  /* 7 bits */
};
```
位域不能跨越机器字边界：

```
struct foo7 {
    int bigfield:31;      /* 32-bit word 1 begins */
    int littlefield:1;
};

struct foo8 {
    int bigfield1:31;     /* 32-bit word 1 begins /*
    int littlefield1:1;
    int bigfield2:31;     /* 32-bit word 2 begins */
    int littlefield2:1;
};

struct foo9 {
    int bigfield1:31;     /* 32-bit word 1 begins */
    int bigfield2:31;     /* 32-bit word 2 begins */
    int littlefield1:1;
    int littlefield2:1;   /* 32-bit word 3 begins */
};
```
