+++
disqusIdentifier = "20161028spring_test"
showSocial = false
showTags = true
comments = true
showPagination = true
date = "2016-09-28T15:43:54+08:00"
showDate = true
clearReading = true
keywords = ["springmvc"]
title = "Spring＋Mybatis＋Velocity项目搭建"
categories = [ "java" ]
+++
Spring＋Mybatis＋Velocity项目搭建

一、开发工具

- JDK 1.8.0_91
- Intellij IDEA 15.0.6
- Mysql  5.5.44
- Maven 3

二、新建工程

1. 新建Maven工程，不选Create from archtype提供的工程模版，为了学习从头开始配置工程，点击next设置工程坐标，然后一路到finish。
![create_project](/images/create_project.png)

    	<groupId>red.itech</groupId>
    	<artifactId>blogDemo</artifactId>
    	<version>1.0-SNAPSHOT</version>

2. 如果使用git开发，可以新建.gitignore文件，使git忽略idea自动生成的文件（.ignore插件可以帮助生成.gitignore），项目git初始化

	```
	# Created by .ignore support plugin (hsz.mobi)
	.gitignore
	### OSX template
	*.DS_Store
	
	# IntelliJ project files
	.idea
	*.iml
	out
	target
	gen### Java template
	*.class
	
	# Package Files #
	*.jar
	*.war
	*.ear
	```	
三、Spring MVC

1. 编辑pom.xml添加Spring MVC、servlet依赖如下：
    
    	<dependencies>
        	<!-- spring -->
        	<dependency>
            	<groupId>org.springframework</groupId>
            	<artifactId>spring-core</artifactId>
            	<version>4.3.3.RELEASE</version>
        	</dependency>
        	<dependency>
            	<groupId>org.springframework</groupId>
            	<artifactId>spring-webmvc</artifactId>
            	<version>4.3.3.RELEASE</version>
        	</dependency>
        	<dependency>
            	<groupId>org.springframework</groupId>
            	<artifactId>spring-web</artifactId>
            	<version>4.3.3.RELEASE</version>
        	</dependency>

        	<!-- jsp -->
        	<dependency>
            	<groupId>javax.servlet</groupId>
            	<artifactId>servlet-api</artifactId>
            	<version>3.0-alpha-1</version>
        	</dependency>
    	</dependencies>
2. 增加Spring MVC框架支持

	![add_framework](/images/addframework.png)
	![web](/images/web.png)
	![springmvc](/images/springmvc.png)
	添加完成后对目录结构做一定调整，将web目录移到src/main下并重命名为webapp，调整后结构如下：
	
	![struct](/images/struct.png)

3. 修改web.xml

	web.xml的作用是配置Spring的DispatcherServlet，DispatcherServlet作为前端控制器，用于用户接口与Servlet间的映射。在web.xml的代码中,org.springframework.web.servlet.DispatcherServlet的实例名称为dispatcher，DispatcherServlet会从dispatcher－servlet.xml文件中加载应用上下文。

	![web.xml](/images/webxml.png)

	修改servlet-mapping中的url-pattern为/表示dispatcher会处理所有请求。

4. 修改dispatcher-servlet.xml
	
	```
	xmlns:mvc="http://www.springframework.org/schema/mvc"   //mvc注解需要
	<mvc:annotation-driven/>   //注解驱动
	<context:component-scan base-package="red.itech.blog.controller"/>  //自动扫描@Controller注册为beans
	//xsi:schemaLocation中添加
	http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd  //通配符的匹配很全面, 但无法找到元素 'mvc:annotation-driven' 的声明
	```
5. 实现控制器

		package red.itech.blog.controller;
	
		import org.springframework.stereotype.Controller;
		import org.springframework.web.bind.annotation.RequestMapping;
		import org.springframework.web.bind.annotation.RequestMethod;

		import javax.servlet.http.HttpServletResponse;
		import java.io.IOException;

		/**
 		* Created by you on 16/9/28.
 		*/

		@Controller
		public class BlogController {

   		@RequestMapping(value = "/home", method = RequestMethod.GET)
    		public void homePage(HttpServletResponse response) throws IOException{
        		response.getWriter().print("<h1>Welcome to my Blog!</h1>");
        		response.flushBuffer();
    		}
		}
		
6. 启动Web服务

- 修改index.jsp

		<%@ page contentType="text/html;charset=UTF-8" language="java" %>
		<html>
  			<body>
  				<%response.sendRedirect("home"); %>
  			</body>
		</html>

- 配置maven构建过程
	
		<packaging>war</packaging>   //打包格式
    	<build>
        	<finalName>blog</finalName>   //生成文件名
    	</build>

- 配置运行tomcat

![home](/images/home.png)

四、velocity支持（velocity使前后端分离）

1. 添加velocity库支持

	在pom.xml中添加velocity、velocity-tools、spring-context-support依赖如下：
	
		<dependency>
            <groupId>org.apache.velocity</groupId>
            <artifactId>velocity</artifactId>
            <version>1.7</version>
        </dependency>
        <dependency>
            <groupId>org.apache.velocity</groupId>
            <artifactId>velocity-tools</artifactId>
            <version>2.0</version>
        </dependency>
		<dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>4.3.3.RELEASE</version>
        </dependency>
2. 增加velocity视图解析

	在dispatcher-servlet.xml中配置velocity视图解析器如下：
		
		<bean id="velocityConfigure" class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
        	<property name="resourceLoaderPath" value="/WEB-INF/templates/"/>
        	<property name="velocityProperties">
            	<props>
                	<prop key="input.encoding">utf-8</prop>
                	<prop key="output.encoding">utf-8</prop>
            	</props>
        	</property>
        </bean>
    	<bean id="velocityViewResolver" class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
        	<property name="suffix" value=".vm"/>
        	<property name="contentType" value="text/html;charset=utf-8"/>
    	</bean>


3. 修改控制器代码

	控制器的作用是根据请求调用BLL的Service进行处理，获得处理结果后，将模型对象与逻辑视图返回。代码如下：
	
		package red.itech.blog.controller;
		import org.springframework.stereotype.Controller;
		import org.springframework.ui.Model;
		import org.springframework.web.bind.annotation.RequestMapping;
		import org.springframework.web.bind.annotation.RequestMethod;

		import javax.servlet.http.HttpServletRequest;

		/**
 		* Created by you on 16/9/28.
 		*/

		@Controller
		public class BlogController {
    		@RequestMapping(value = "/home", method = RequestMethod.GET)
    		public String homePage(HttpServletRequest request, Model model){
        		//1.调用BLL层的服务接口
        		//2.设置模型数据
        		model.addAttribute("username", "someone");
        		//3.返回逻辑视图名称
        		return "welcome";
    		}
		}
4. 创建模版文件WEB-INF/templates/welcome.vm，测试视图解析器如下

![vm](/images/vm.png)

![page](/images/page.png)


五、mybatis支持，mybatis是一种SQL－mapping的ORM实现，将结果转化为POJO(使用mybatis-generator简化实现)

1. 添加mybatis依赖

	 	<dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.3.0</version>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.2.3</version>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.17</version>
        </dependency>
        <dependency>
            <groupId>commons-dbcp</groupId>
            <artifactId>commons-dbcp</artifactId>
            <version>1.2.2</version>
        </dependency>

2. 配置数据库源
	
	修改applicationContext.xml加入dataSource
	
	    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
        	<property name="driverClassName" value="com.mysql.jdbc.Driver"/>
        	<property name="url" value="jdbc:mysql://127.0.0.1:3306/blog"/>
        	<property name="username" value="root"/>
        	<property name="password" value="xxxxxx"/>
    	</bean>
    	
    	<bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        	<property name="dataSource" ref="dataSource"/>
        	<property name="mapperLocations" value="classpath*:red/itech/blog/**/dal/**/*Mapper*.xml"/>
    	</bean>

    	<bean id="sqlSession" class="org.mybatis.spring.SqlSessionTemplate">
        	<constructor-arg index="0" ref="sqlSessionFactory"/>
    	</bean>

    	<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        	<property name="annotationClass" value="javax.annotation.Resource"></property>
        	<property name="basePackage" value="red.itech.blog.***.dal.mapper"/>
    	</bean>

3. 修改mybatis-generator配置文件(放置到resource目录下)，如下

		<?xml version="1.0" encoding="UTF-8" standalone="no"?>
		<!DOCTYPE generatorConfiguration PUBLIC "-//mybatis.org//DTD MyBatis Generator Configuration 1.0//EN"
        "http://mybatis.org/dtd/mybatis-generator-config_1_0.dtd">
		<generatorConfiguration>
    		<context id="base_resource" targetRuntime="MyBatis3">
        		<property name="suppressDate" value="true"/>
        		<!-- 是否去除自动生成的注释 true：是 ： false:否 -->
        		<property name="suppressAllComments" value="true"/>
        		<property name="useActualColumnNames" value="false"/>
        		<!--数据库链接URL，用户名、密码 -->
        		<jdbcConnection
                		connectionURL="jdbc:mysql://127.0.0.1:3306/blog?useUnicode=true&amp;characterEncoding=UTF-8"
                driverClass="org.gjt.mm.mysql.Driver" password="xxx" userId="xxx"/>
        		<javaTypeResolver>
            		<property name="forceBigDecimals" value="true"/>
        		</javaTypeResolver>
        		<!-- 生成模型的包名和位置 -->
        		<javaModelGenerator targetPackage="red.itech.blog.dal.model" targetProject="src/main/java">
            		<property name="enableSubPackages" value="true"/>
            		<property name="trimStrings" value="true"/>
        		</javaModelGenerator>
        		<!-- 生成映射文件的包名和位置 -->
        		<sqlMapGenerator targetPackage="red.itech.blog.dal.mapper" targetProject="src/main/resources">
            		<property name="enableSubPackages" value="true"/>
        		</sqlMapGenerator>
        		<!-- 生成DAO的包名和位置 -->
        		<javaClientGenerator targetPackage="red.itech.blog.dal.mapper" targetProject="src/main/java"
                             type="XMLMAPPER">
            		<property name="enableSubPackages" value="true"/>
        		</javaClientGenerator>
        		<table domainObjectName="User" tableName="user">
            		<generatedKey column="id" sqlStatement="JDBC"/>
        		</table>
    		</context>
		</generatorConfiguration>

4. 生成POJO对象文件与映射

	在pom.xml中添加mybatis-generator插件如下：
	
		<build>
        	<finalName>blog</finalName>
        	<plugins>
            	<plugin>
                	<groupId>org.mybatis.generator</groupId>
                	<artifactId>mybatis-generator-maven-plugin</artifactId>
                	<version>1.3.2</version>
                	<configuration>
                   		<overwrite>true</overwrite>
                	</configuration>
                	<dependencies>
                   		<dependency>
                        	<groupId>org.mybatis.generator</groupId>
                        	<artifactId>mybatis-generator-core</artifactId>
                        	<version>1.3.2</version>
                    	</dependency>
                    	<!-- 数据库驱动 -->
                    	<dependency>
                        	<groupId>mysql</groupId>
                        	<artifactId>mysql-connector-java</artifactId>
                        	<version>5.1.17</version>
                    	</dependency>
                	</dependencies>
            	</plugin>
        	</plugins>
    	</build>

	在项目目录下执行：
	
	mvn -Dmybatis.generator.overwrite=true -Dmybatis.generator.configurationFile=src/main/resources/generatorConfig.xml mybatis-generator:generate
	
5. 实现Service层（接口与实现）

	根据业务逻辑实现相应逻辑代码，简化控制器层
	
6. 修改控制器代码
