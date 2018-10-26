+++
disqusIdentifier = "20170401diskfull"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2017-04-01T18:00:54+08:00"
showDate = true
clearReading = true
keywords = ["disk 100%","linux"]
title = "Linux磁盘使用率100%异常排查"
categories = [ "linux" ]
+++

<!--more-->

## 缘起

线上堡垒机系统磁盘根目录/空间使用率100%导致无法添加账号，奇怪的是du查看实际只使用了8G多空间还有40多G，而df查看使用率却是100%，同事找过来让帮忙定位问题。

## du -h --max-depth=1 -x 查看/目录磁盘占用情况(-x排除挂载的其他磁盘)
![du](https://i.imgur.com/OZmT6xt.png)
## df查看/磁盘使用率在94%
![df](https://i.imgur.com/8xA5AWA.png)

## du与df机制对比

- df读取的是超级块的内容
- du是将所有文件对象大小加起来

两种机制的不同会造成两者结果不一致的地方，例如你在命令行下删除了一个文件，而这个文件正在被某个程序打开占用，实际上这个文件依然占用磁盘空间，只有在使用该文件的进程关闭时才真正清楚磁盘空间，这时du显示的数据会比df显示的值小。

## 解决思路一

通过lsof查看是否有已删除的文件仍被进程所使用：

```lsof | grep '(deleted)'```

找到某个大文件对应的进程，关闭该进程可以正确释放磁盘空间
但该方法在本例中并未解决问题！！！

## 解决思路二

在df的图中可以看到，根目录/下还挂载着/home与/home/admin是否有可能是**原根目录下/home目录被挂载的/home覆盖**，而原/home目录的数据并未清楚，导致磁盘占用

```mount -o bind / /mnt```

在mnt下果然看到了原来/home目录，该目录下的废弃的用户账户占用磁盘40G空间！ 之前同事增加磁盘做文件系统粗心导致旧的数据未删除，给自己挖了一个坑！
