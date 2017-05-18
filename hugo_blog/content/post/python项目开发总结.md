+++
disqusIdentifier = "16526127_python_project"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-05-27T13:27:04+08:00"
showDate = true
clearReading = true
keywords = ["python develop"]
title = "python项目开发总结"
categories = [ "python" ]
+++
### python项目开发总结

#### 编码规范flake8


- 安装 pip install flake8 或 brew install flake8
- 安装vim插件 [syntastic](https://github.com/scrooloose/syntastic)，语法检测插件，很方便

		由于使用vundle管理插件，只需在.vimrc中添加 Plugin 'scrooloose/syntastic' 
		打开vim，在命令行模式输入 PluginInstall
		最后在.vimrc中添加set lcs=extends:>,precedes:<,tab:>-,trail:·
		vundle相关：https://github.com/VundleVim/Vundle.vim
		
#### 使用virtualenv，独立的开发环境

	新建project 
	virtualenv project
	环境使能
	cd project
	source bin/activate
	退出virtualenv环境
	deactive
	获得当前环境依赖
	pip freeze
	

#### python单例

- metaclass元类就是用来创建这些类（对象）的，元类就是类的类,type就是Python的内建元类,用来创建类

		1)   拦截类的创建

		2)   修改类

		3)   返回修改之后的类


		class Singleton(type):  ＃子类化type，__call__相当于重载了括号运算符
    		_instances = {}
    		def __call__(cls, *args, **kwargs):
        		if cls not in cls._instances:
            		cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        		return cls._instances[cls]

		class Logger(object):
    		__metaclass__ = Singleton
		#Or in Python3

		class Logger(metaclass=Singleton):
    		pass
		If you want to run __init__ every time the class is called, add

        		else:
            		cls._instances[cls].__init__(*args, **kwargs)
		to the if statement in Singleton.__call__.

- 继承

		class Singleton(object):
  			_instances = {}
  			def __new__(class_, *args, **kwargs):
    			if class_ not in class_._instances:
        			class_._instances[class_] = super(Singleton, class_).__new__(class_, *args, **kwargs)
        			class_._instances[class_].__init__()  # if you want init once
    			return class_._instances[class_]

		class MyClass(Singleton):
  			pass

		c = MyClass()
		
		最终采用了继承的方式，metaclass在测试时有问题


#### nosetest单元测试框架

	class A():
		def setUp(self):
			....
		
		def tearDown(self):
			....
			
		def testxxx(self):
			....
			
	每个test的运行顺序是  setUp->testxxx->tearDown->setUp->testxxx->....
		

