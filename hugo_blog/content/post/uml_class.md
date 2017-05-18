+++
disqusIdentifier = "16826451_uml"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-08-26T16:51:34+08:00"
showDate = true
clearReading = true
keywords = ["UML"]
title = "理解UML类图关系"
categories = [ "基础" ]
+++
理解UML类图关系

1、依赖(Dependency)

- 关系：uses temporarily，使用关系，作为局部变量、方法参数或者对静态方法的调用
- 代码示例：

        import B;
        public class A{
            public void method1(B b) { // ... }
            public void method2() { B tempB = new B(); // ... }
        }
- 图示(一套带箭头的虚线表示)：

![dependency](http://occ9e8dkg.bkt.clouddn.com/image/1/64/c5da58a00cd2fcbf112fa1be3e6b2.png)

2、聚合(Aggregation)

- 关系：is part of，整体与部分的关系，作为成员变量
- 代码：

        import Engine;
        public class Car{
        	private Engine engine;
        	publilc Engine getEngine(){
        		return engine;
        	}
        }
- 图示(一条带空心菱形箭头的直线表示)：

![aggregation](http://occ9e8dkg.bkt.clouddn.com/image/4/2d/e12355088b0e4fb201deec5772760.png)

3、组合(Composition)

- 关系：is entirely made of，更强的整体与部分关系，作为成员变量
- 代码:

        import Page;
        public class Book{
        	private List<Page> pages;
        	publilc List<Page> getPages(){
        		return pages;
        	}
        }
- 图示(一条带实心菱形箭头直线表示):

![composition](http://occ9e8dkg.bkt.clouddn.com/image/a/65/66b285efb154c3d638f874a006f08.png)

4、关联(Association)

- 关系：a usage relationship，包含以上三种关系，作为成员变量
- 图示(一套带箭头的直线表示):

![association](http://occ9e8dkg.bkt.clouddn.com/image/7/4d/d53d5daaf1c53fb39bfb66f40c0b3.png)

5、实现(Realization)

- 关系：interface implementation，接口实现
- 代码：

		import Car;
		public class SUV implements Car{
			...
		}
- 图示(用一条带空心箭头的虚线表示):

![realization](http://occ9e8dkg.bkt.clouddn.com/image/3/02/8e8cf926f50f551344dbffe284c56.png)

6、泛化(Generalization)

- 关系：inheritance between classes，继承关系
- 代码：
		
		import Widget;
		public class WindowWidget extends Widget{
			...
		}
- 图示(用一条带空心箭头的实线表示):

![generalization](http://occ9e8dkg.bkt.clouddn.com/image/5/6e/f8cd3332f700271df86588a86508d.png)


## 类图表现方式：

接口 ： \<\<interface\>\> Car

抽象类： 斜体

属性：可选，应该包含所有对象属性

	变量名:类型
	＋ public
	# protected
	- private
	~ package
	/ derived
	静态属性  下划线标示

方法：可选，可省略get/set方法，不应包含继承方法

	方法名(参数):返回类型
	＋ public
	# protected
	- private
	~ package
	/ derived
	静态属性  下划线标示
	参数： name:type







