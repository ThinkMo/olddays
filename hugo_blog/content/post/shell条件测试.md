+++
disqusIdentifier = "20170427655_shell"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2017-04-27T18:55:34+08:00"
showDate = true
clearReading = true
keywords = ["shell", "test"]
title = "shell条件测试总结"
categories = [ "linux" ]
+++

在编写shell脚本时，条件测试与判断必不可少，基于测试结果才能做进一步的处理，此文用来回顾下shell脚本中的条件测试。
## test([)
test有两种格式，即```test condition``` 或 ```[ condition ]```(注意条件两边空格)，
test主要用来测试文件、字符串及数字。

- 逻辑操作符
	
	-a 逻辑与 -o 逻辑或 ! 逻辑非(不是短路求值)
	
	**best practise：使用多个[，而不是-a与-o**
	
	- good : [ "$a" = "$b" ] && [ "$b" = "$c" ]
	- bad : [ "$a" = "$b" -a "$b" = "$c" ]
- 文件测试

 参数|含义|参数|含义
----|----|----|----
-d|目录|-s|文件长度大于0、非空
-f|普通文件|-w|可写
-L|符号链接|-u|文件有suid置位
-r|可读|-x|可执行

例如:

```
$if [ -f files ];then
> echo "files is a normal file"
> fi
files is a normal file
```
- 字符串测试(**注意符号两侧的空格**)

 参数|含义
----|----
-z|空串
-n|非空串
=|相等
!=|不相等

例如：

```
test "a" = "b"
echo $?
1
```
- 测试数值

 参数|含义
----|----
-eq|相等
-ge|大于等于
-gt|大于
-le|小于等于
-lt|小于
-ne|不等

## expr

expr一般用于正数值，也可用于字符串，一般格式为 expr arg operator arg，
支持 | &  <  <=  =  !=  >=  >  +  -  *  /  % 等操作，此外

- 模式匹配 STRING : REGEXP （匹配\(\)之间的字符串）

```
$expr "hello.world" : "\(.*\).world"
hello
```
- 求子串 substr STRING POS LENGTH （POS从1开始）

```
$expr substr "hello world" 7 5
world
```
- 求位置 index STRING CHARS（多个字符返回最前匹配的字符）

```
$expr index "hello world" e
2
```
- 求长度 length STRING

```
$expr length "hello world"
11
```

## [[

- [[比test功能更强大，只有bash、zsh等支持[[，而test更加便于移植(推荐使用[[)
- [[是关键字，test是command
- [[ 短路求值

<table><tbody><tr>  <td><p class="line891"><strong>Feature</strong></p></td>
  <td><p class="line891"><strong>new test</strong> <tt>[[</tt></p></td>
  <td><p class="line891"><strong>old test</strong> <tt>[</tt></p></td>
  <td><p class="line891"><strong>Example</strong></p></td>
</tr>
<tr>  <td rowspan="4"><span class="anchor" id="line-28"></span><p class="line862">string comparison</p></td>
  <td><p class="line891"><tt>&gt;</tt></p></td>
  <td><p class="line891"><tt>\&gt;</tt> <a href="/BashFAQ/031#np">(*)</a></p></td>
  <td><p class="line891"><tt>[[&nbsp;a&nbsp;&gt;&nbsp;b&nbsp;]]&nbsp;||&nbsp;echo&nbsp;"a&nbsp;does&nbsp;not&nbsp;come&nbsp;after&nbsp;b"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-29"></span><p class="line891"><tt>&lt;</tt></p></td>
  <td><p class="line891"><tt>\&lt;</tt> <a href="/BashFAQ/031#np">(*)</a></p></td>
  <td><p class="line891"><tt>[[&nbsp;az&nbsp;&lt;&nbsp;za&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"az&nbsp;comes&nbsp;before&nbsp;za"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-30"></span><p class="line891"><tt>=</tt> (or <tt>==</tt>)</p></td>
  <td><p class="line891"><tt>=</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;a&nbsp;=&nbsp;a&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"a&nbsp;equals&nbsp;a"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-31"></span><p class="line891"><tt>!=</tt></p></td>
  <td><p class="line891"><tt>!=</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;a&nbsp;!=&nbsp;b&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"a&nbsp;is&nbsp;not&nbsp;equal&nbsp;to&nbsp;b"</tt></p></td>
</tr>
<tr>  <td rowspan="6"><span class="anchor" id="line-32"></span><p class="line862">integer comparison</p></td>
  <td><p class="line891"><tt>-gt</tt></p></td>
  <td><p class="line891"><tt>-gt</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;5&nbsp;-gt&nbsp;10&nbsp;]]&nbsp;||&nbsp;echo&nbsp;"5&nbsp;is&nbsp;not&nbsp;bigger&nbsp;than&nbsp;10"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-33"></span><p class="line891"><tt>-lt</tt></p></td>
  <td><p class="line891"><tt>-lt</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;8&nbsp;-lt&nbsp;9&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"8&nbsp;is&nbsp;less&nbsp;than&nbsp;9"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-34"></span><p class="line891"><tt>-ge</tt></p></td>
  <td><p class="line891"><tt>-ge</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;3&nbsp;-ge&nbsp;3&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"3&nbsp;is&nbsp;greater&nbsp;than&nbsp;or&nbsp;equal&nbsp;to&nbsp;3"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-35"></span><p class="line891"><tt>-le</tt></p></td>
  <td><p class="line891"><tt>-le</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;3&nbsp;-le&nbsp;8&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"3&nbsp;is&nbsp;less&nbsp;than&nbsp;or&nbsp;equal&nbsp;to&nbsp;8"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-36"></span><p class="line891"><tt>-eq</tt></p></td>
  <td><p class="line891"><tt>-eq</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;5&nbsp;-eq&nbsp;05&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"5&nbsp;equals&nbsp;05"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-37"></span><p class="line891"><tt>-ne</tt></p></td>
  <td><p class="line891"><tt>-ne</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;6&nbsp;-ne&nbsp;20&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"6&nbsp;is&nbsp;not&nbsp;equal&nbsp;to&nbsp;20"</tt></p></td>
</tr>
<tr>  <td rowspan="2"><span class="anchor" id="line-38"></span><p class="line862">conditional evaluation</p></td>
  <td><p class="line891"><tt>&amp;&amp;</tt></p></td>
  <td><p class="line891"><tt>-a</tt> <a href="/BashFAQ/031#np2">(**)</a></p></td>
  <td><p class="line891"><tt>[[&nbsp;-n&nbsp;$var&nbsp;&amp;&amp;&nbsp;-f&nbsp;$var&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"$var&nbsp;is&nbsp;a&nbsp;file"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-39"></span><p class="line891"><tt>||</tt></p></td>
  <td><p class="line891"><tt>-o</tt> <a href="/BashFAQ/031#np2">(**)</a></p></td>
  <td><p class="line891"><tt>[[&nbsp;-b&nbsp;$var&nbsp;||&nbsp;-c&nbsp;$var&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"$var&nbsp;is&nbsp;a&nbsp;device"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-40"></span><p class="line862">expression grouping</p></td>
  <td><p class="line891"><tt>(...)</tt></p></td>
  <td><p class="line891"><tt>\(&nbsp;...&nbsp;\)</tt> <a href="/BashFAQ/031#np2">(**)</a></p></td>
  <td><p class="line891"><tt>[[&nbsp;$var&nbsp;=&nbsp;img*&nbsp;&amp;&amp;&nbsp;($var&nbsp;=&nbsp;*.png&nbsp;||&nbsp;$var&nbsp;=&nbsp;*.jpg)&nbsp;]]&nbsp;&amp;&amp;</tt><br>
<tt>echo&nbsp;"$var&nbsp;starts&nbsp;with&nbsp;img&nbsp;and&nbsp;ends&nbsp;with&nbsp;.jpg&nbsp;or&nbsp;.png"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-41"></span><p class="line862">Pattern matching</p></td>
  <td><p class="line891"><tt>=</tt> (or <tt>==</tt>)</p></td>
  <td><p class="line862">(not available)</p></td>
  <td><p class="line891"><tt>[[&nbsp;$name&nbsp;=&nbsp;a*&nbsp;]]&nbsp;||&nbsp;echo&nbsp;"name&nbsp;does&nbsp;not&nbsp;start&nbsp;with&nbsp;an&nbsp;'a':&nbsp;$name"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-42"></span><p class="line891"><a href="/RegularExpression">RegularExpression</a> matching</p></td>
  <td><p class="line891"><tt>=~</tt></p></td>
  <td><p class="line862">(not available)</p></td>
  <td><p class="line891"><tt>[[&nbsp;$(date)&nbsp;=~&nbsp;^Fri\&nbsp;...\&nbsp;13&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"It's&nbsp;Friday&nbsp;the&nbsp;13th!"</tt></p></td>
</tr>
</tbody></table>
(*)是POXIS扩展，可能不支持，(**)不建议使用

<table><tbody><tr>  <td><p class="line891"><strong>Description</strong></p></td>
  <td><p class="line891"><strong>Primitive</strong></p></td>
  <td><p class="line891"><strong>Example</strong></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-55"></span><p class="line862">entry (file or directory) exists</p></td>
  <td><p class="line891"><tt>-e</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;-e&nbsp;$config&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"config&nbsp;file&nbsp;exists:&nbsp;$config"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-56"></span><p class="line862">file is newer/older than other file</p></td>
  <td><p class="line891"><tt>-nt</tt> / <tt>-ot</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;$file0&nbsp;-nt&nbsp;$file1&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"$file0&nbsp;is&nbsp;newer&nbsp;than&nbsp;$file1"</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-57"></span><p class="line862">two files are the same</p></td>
  <td><p class="line891"><tt>-ef</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;$input&nbsp;-ef&nbsp;$output&nbsp;]]&nbsp;&amp;&amp;&nbsp;{&nbsp;echo&nbsp;"will&nbsp;not&nbsp;overwrite&nbsp;input&nbsp;file:&nbsp;$input";&nbsp;exit&nbsp;1;&nbsp;}&nbsp;</tt></p></td>
</tr>
<tr>  <td><span class="anchor" id="line-58"></span><p class="line862">negation</p></td>
  <td><p class="line891"><tt>!</tt></p></td>
  <td><p class="line891"><tt>[[&nbsp;!&nbsp;-u&nbsp;$file&nbsp;]]&nbsp;&amp;&amp;&nbsp;echo&nbsp;"$file&nbsp;is&nbsp;not&nbsp;a&nbsp;setuid&nbsp;file"</tt></p></td>
</tr>
</tbody></table>

## (( 算数求值与比较(bash)

 - 算术运算符+ - * / % **
 - 赋值运算符= *= /= %= += -= <<= >>= &= ^= |=
 - 逻辑运算符	> < >= <= == != ! && ||
 - 位操作符 ~ << >> & | ^
 - 其他 ++  -- ?: ()