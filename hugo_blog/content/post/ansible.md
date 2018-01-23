+++
keywords = ["ansible"]
title = "ansible自动化部署配置工具"
categories = ["linux"]
comments = true
clearReading = true
disqusIdentifier = "ansible_0721"
date = "2017-07-21T16:18:29+08:00"
showSocial = false
showPagination = true
showTags = true
showDate = true
+++

#### ansible自动化部署配置工具

##### 初识ansible

- 简介

  ansible是运维自动化工具，可以实现系统配置、软件部署，可以通过编排实现更高级的功能如持续集成、热更新等；ansible具有简单易用、安全、agentless等特点，通过ssh来完成与管理节点的交互。

- inventory

  ansible管理的主机信息称为inventory，默认存储在/etc/ansible/hosts，也可以在运行时通过-i <path>指定inventory文件位置，通常为ini格式，如下：

  ```
  [jumpserver]
  44.33.22.11:1024	ansible_ssh_user=xxx  # xxx用户ssh端口1024，2.0后参数变为ansible_user
  [other]
  192.168.1.[2:255]    # 范围2-255
  [localhost]
  localhost	ansible_connection=local
  ```

  处于[]中的为组名，一个节点可以在多个组下，组下为机器名可指定端口与参数，支持的参数详情见[文档](http://docs.ansible.com/ansible/intro_inventory.html)。

- patterns:

  ansible执行ad-hoc 命令的基本格式如下：

  ```
  ansible <pattern_goes_here> -m <module_name> -a <arguments>
  ```

  ansible中的模式patterns决定了inventory中哪些主机将执行指定命令，默认是all(*)所有机器，可以指定多个组的集合如other:localhost(并集) other:!localhost(差集)、other:&localhost(交集)等

- ad-hoc命令

  ad-hoc命令是指执行简单短小的、无需保存结果的任务

  ```bash
  # 查看localhost组的uptime，-m指定模块，默认为command, -a指定参数, -f并发数	
  ansible localhost -m command -a "/usr/bin/uptime"
  # -u username切换登录用户   --become 切换到root执行 --become-user otheruser 切换到其他用户执行
  ansible localhost -a "/usr/bin/uptime" -u sa
  # 文件传输
  ansible jumpserver -m copy 	-a "src=/etc/hosts dest=/tmp/hosts mode=600"
  # 包管理
  ansible jumpserver -m yum -a "name=vim state=present"   #确保vim包已安装
  ansible jumpserver -m yum -a "name=vim state=absent"    #vim包卸载
  # 用户管理
  ansible jumpserver -m user -a "name=xiao password=xxx"  #增加用户xiao	
  # 服务管理
  ansible jumpserver -m service -a "name=httpd state=restarted"  
  # 信息收集
  ansible jumpserver -m setup
  ```
- 常用配置项

  ```
  配置项读取顺序如下：
  * ANSIBLE_CONFIG (一个环境变量)
  * ansible.cfg (位于当前目录中)
  * .ansible.cfg (位于家目录中)
  * /etc/ansible/ansible.cfg

  常用配置项：
  [defaults]
  log_path = ./ansible.log    # 日志路径
  remote_user = admin         # ssh用户
  remote_port = 22            # ssh端口
  gathering = explicit
  roles_path = roles
  retry_files_save_path = /tmp/
  host_key_checking = False
  vault_password_file = vault-pass
  forks = 100                 # 并行进程数
  #private_key_file=/path/to/file.pem       # 私钥
  action_plugins = ./plugins/actions
  filter_plugins = ./plugins/filters
  callback_plugins = ./plugins/callbacks
  vars_plugins = ./plugins/vars
  library = ./library

  ansible_managed = This file is managed by Ansible.%n template: {file}  # 修改报告

  [ssh_connection]
  ssh_args = -o StrictHostKeyChecking=no -o ControlMaster=auto -o ControlPersist=30m
  pipelining=true

  [privilege_escalation]
  become=True           # 提权
  ```
  [所有配置](http://docs.ansible.com/ansible/latest/intro_configuration.html)

- 配置优先级

  ```
  1. extra vars (在命令行中使用 -e)优先级最高
  2. 然后是在inventory中定义的连接变量(比如ansible_ssh_user)
  3. 接着是大多数的其它变量(命令行转换,playbook中的变量,included的变量,role中的变量等)
  4. 然后是在inventory定义的其它变量
  5. 然后是由系统发现的facts
  6. 然后是 "role默认变量", 这个是最默认的值,很容易丧失优先权
  ```

- Playbook

  playbooks是ansible配置、部署与编排语言，包含了对远程主机一系列指令操作，采用YAML语法。下面看一个例子：

  ```yaml
  ---
  - name: install and start httpd   # task名称
    hosts: webservers               # hosts主机组pattern
    remote_user: web                # 登录用户
    become: yes                     # 用户切换
    become_user: root
    tasks:                          # 子task
      - name: install and start httpd
        apt:                        # 模块 幂等
          name: httpd
          state: present
          handlers:                 # handlers在task完成时被触发执行
            - name: start httpd
              service: name=httpd state=started
  ```

  通过`ansible-playbook playbook.yml -f 10`来执行playbook，在执行之前可以运行`ansible-playbook playbook.yaml  --syntax-check`进行语法检查


  ```
  handlers:
    - name: restart memcached
      service: name=memcached state=restarted
      listen: "restart web services"
    - name: restart apache
      service: name=apache state=restarted
      listen: "restart web services"

  tasks:
    - name: restart everything
      command: echo "this task will restart the web services"
      notify: "restart web services"
  ```
  ansible 2.2开始，handlers可以监听事件，tasks可以通过notify事件来触发handlers，如上例。
 
##### Playbook高阶

 - Role
 - 变量
 - 条件与循环

##### 最佳实践

 -  项目组织

    ```envs/
    envs/
    	production/
    	staging/
    	testing/
    		hosts           # 主机列表
    		group_vars/     # 组变量
    		host_vars/      # 主机变量
    library/                # 脚本库
    plugins/
    roles/
    	common/
    		tasks/
    		templates/
    		defaults/
    		meta/            # 依赖
    		handlers/        
    	app/
    playbooks/
        xxx.yaml            # 由可复用roles组成
    ```

##### 参考文档

[Ansible Documentation](http://docs.ansible.com/ansible)

[模块索引](http://docs.ansible.com/ansible/modules_by_category.html)

[ansible进阶技巧](https://www.ibm.com/developerworks/cn/linux/1608_lih_ansible/index.html)
