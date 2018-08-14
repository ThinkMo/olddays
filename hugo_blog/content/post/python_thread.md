+++
keywords = ["python", "GIL", "thread safe"]
title = "Python GIL与线程安全"
categories = ["python"]
comments = true
clearReading = true
disqusIdentifier = "python_0002"
date = "2018-08-10T15:17:59+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

#### python GIL与多线程

- GIL: Global Interpreter Lock(全局解释器锁)，python虚拟机一级的互斥机制，用来保护资源的互斥访问，尤其是python对象的引用计数。
- python代码执行

```
获得GIL
执行代码直到被阻塞或N条指令执行之后(py3:超时时间)被python 虚拟机调度
释放GIL

何时挂起当前线程：被阻塞或N条指令执行之后(py3:超时时间15ms)
如何选择等待的线程进行调度：由底层操作系统决定
```

- python多线程机制是在GIL基础上实现的
- python的多线程机制建立在操作系统的原生线程的基础之上

#### Thread safe in python

Python线程安全与传统的java、c有很大不同，python的很多操作都是原子的，例如sort函数本身是原子操作，不会被其他线程看到排序一半的list；而有一些操作不是原子的，例如+=

```
n=0
def add():
    global n
    n += 1

import dis   #将python byte code转为助记符号
dis.dis(add)
  3           0 LOAD_GLOBAL              0 (n)
              3 LOAD_CONST               1 (1)
              6 INPLACE_ADD
              7 STORE_GLOBAL             0 (n)
             10 LOAD_CONST               0 (None)
             13 RETURN_VALUE
```

操作步骤为：

1. 加载n到栈
2. 加载常量n到栈
3. 将两者相加
4. 将结果付给n

由于操作不是原子的，python解释器可能在某一步被抢占，造成不一致的情况，执行以下代码，将不一定会得到100

```
threads = []
for i in range(100):
    t = threading.Thread(target=add)
    threads.append(t)

for t in threads:
    t.start()

for t in threads:
    t.join()

print(n)
```

你需要使用锁保护共享数据的访问：

```
n = 0
lock = threading.Lock()

def add():
    global n
    with lock:
        n += 1
```

在实际编程时，可以遵循：

1. 总是在读、写共享变量时加锁，锁的代价很小；
2. 无需细粒度的锁
3. I/O密集型时用多线程，计算密集型考虑用多进程来实现正在的并行


#### 迭代器

- Any object that supports iter() and next() is said to be "iterable."
- make the object implement __iter__() and next()


#### 生成器

- A generator is a function that produces a sequence of results instead of a single value
- Instead of returning a value, you generate a series of values (using the yield statement)
- Calling a generator function creates an generator object
- A generator is a one-time operation. You can iterate over the generated data once, but if you want to do it again, you have to call the generator function again.
- 使用生成器可以很方便的支持迭代器协议
- 生成器是一种特殊的迭代器，内部支持了生成器协议，不需要明确定义__iter__()和next()方法。
生成器通过生成器函数产生，生成器函数可以通过常规的def语句来定义，但是不用return返回，而是用yield一次返回一个结果。
