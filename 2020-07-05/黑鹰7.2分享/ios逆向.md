#### 逆向工程的分析工具

进行iOS逆向工程的一个关键就是工具的使用，工欲善其事，必先利其器。

| 工具分类       |             工具名称              |
| :------------- | :-------------------------------: |
| 越狱工具       |             盘古越狱              |
| 查看文件工具   | PP助手, iExplorer, iFunbox, iTool |
| 砸壳工具       |       dumpdecrypted, Clutch       |
| 查看头文件工具 |            class-dump             |
| 反汇编工具     |          Hopper, IDA Pro          |
| 调试器         |           Cycript, gdb            |
| UI分析工具     |              Reveal               |
| 网络分析工具   |              Charles              |

一 、ios 越狱

进入开发者模式

桌面出现Cydia

![img](https://upload-images.jianshu.io/upload_images/2121032-3528fe9ead6f1d2d.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/93/format/webp)





###### 第一种登录方式可以使用WIFI

具体使用步骤

- 确保Mac和iphone在同一个局域网下（连接同一个WIFI）
- 在Mac的终端输入ssh账户名@服务器主机地址，比如ssh root@10.1.1.168（这里服务器是手机） 初始密码 alpine
- 登录成功后就可以使用终端命令行操作iphone
- 退出登录 exit

ps：ios下2个常用账户 root、moblie

- root： 最高权限账户，HOME是 /var/root
- moblie :普通权限账户，只能操作一些普通文件，不能操作别的文件,HOME是/var/mobile
- 登录moblie用户：root moblie@服务器主机地址
   root和mobli用户的初始登录密码都是alpine

------

###### 第二种登录方式 通过USB进行SSH登录

- 22端口
- 端口就是设备对外提供服务的窗口，每个端口都有个端口号,范围是0--65535，共2^16个
- 有些端口是保留的，已经规定了用途，比如 21端口提供FTP服务，80端口是提供HTTP服务，22端口提供SSH服务，更多保留端口号课参考 [链接](https://baike.baidu.com/item/端口号/10883658#4_3)
- iphone 默认是使用22端口进行SSH通信，采用的是TCP协议
- 默认情况下，由于SSH走的是TCP协议，Mac是通过网络连接的方式SSH登录到iphone，要求iPhone连接WIFI，为了加快传输速度，也可以通过USB连接的方式进行SSH登录，Mac上有个服务程序usbmuxd（开机自动启动），可以将Mac的数据通过USB传输到iphone，路径是/System/Library/PrivateFrameworks/mobileDevice.framework/Resources/usbmuxd

- usbmuxd的使用
- 下载usbmuxd工具包，下载v1.0.8版本，主要用到里面的一个python脚本： tcprelay.py, 下载[链接](https://cgit.sukimashita.com/usbmuxd.git/snapshot/usbmuxd-1.0.8.tar.gz)
- 将iphone的22端口（SSH端口）映射到Mac本地的10010端口
   cd ~/Documents/usbmux-1.08/python-client
   python tcprelay.py -t 22:10010
   加上 -t 参数是为了能够同时支持多个SSH连接，端口映射完毕后，以后如果想跟iphone的22端口通信，直接跟Mac本地的10010端口通信就可以了，新开一个终端界面，SSH登录到Mac本地的10010端口，usbmuxd会将Mac本地10010端口的TCP协议数据，通过USB连接转发到iphone的22 端口，远程拷贝文件也可以直接跟Mac本地的10010端口通信，如：scp -p 10010 ~/Desktop/1.txt root@localhost:~/test 将Mac上的/Desktop/1.txt文件，拷贝到iphone上的/test路径。

- 先开一个终端，先完成端口映射
   *cd 到usbmuxd文件夹路径
- python tcprelay.py -t 22:10010

![img](https:////upload-images.jianshu.io/upload_images/2121032-4bad2bff34a3b634.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

15237725002208.jpg

- 再开一个端口
   注入手机

1. ssh root@localhost -p 10010

2. Zhanghua123:~ root# cycript -p SpringBoard

   ![img](https:////upload-images.jianshu.io/upload_images/2121032-a94c700b1af5a29c.jpg?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp)

   15237725473159.jpg

------

ps： 切记第一个终端不可以关闭，才可以保持端口映射状态











IPA砸壳



从APPStore 商店下载安装的APP 默认都被苹果加了一层壳，加了壳后我们就无法使用 class-dump , Hook 等功能，这里的壳与Win平台上的加密壳类似，但脱壳的难度比Win平台的低。



PP助手，下载后的IPA包即是已经砸过壳的应用，但有一个缺点就是架构比较唯一，一般是 armv7 的架构，但新CPU理论兼容旧架构，如果你手头没有越狱手机，这也不失为一种方法。



使用 Clutch 对越狱手机上应用进行砸壳



1. 下载Clutch:

   已编译版本：

   https://github.com/KJCracks/Clutch/releases

   未编译可用代码

   ```
       $ git clone https://github.com/KJCracks/Clutch
       $ cd Clutch
       $ xcodebuild -project Clutch.xcodeproj -configuration Release ARCHS="armv7 armv7s arm64" build #使用Xcode编译
   ```

   ```
   这里也可以直接用Xcode打开编译
   ```

2. 将工具拷贝到手机：

   /usr/bin/

    路径下

   - 工具：**iFunBox** 或者其他的第三方工具

   - 命令：

     ```
                     scp Clutch/clutch root@<你设备IP>:/usr/bin/
     ```

3. 确保手机已经连接了WIFI并且你的手机的WIFI和你的电脑要在同一个局域网内,打开终端输入:

   ```
       ssh mailto:root@xx.xx.xx.xx,(xx.xx.xx.xx)为你的手机的IP地址-->Enter
   ```

4. 接着会提示你输入password:这里的密码一般情况下是**alpine**,但是如果你更改过的话就用你更改的**这里的密码是手机openssh的密码,不是电脑密码也不是手机解锁密码**

5. 输入

   ```
          cd /usr/bin
   ```

6. 截止到上一步，Clutch已经安装成功了，下面就可以开始砸壳了，输入:

   ```
               Clutch -i
   ```

   ```
   获取已安装应用的包名，显示如下：
   ​```bash
                   #Installed apps:
                   #1:WeChat <com.tencent.xin>
                   #2:DingTalk <com.laiwang.DingTalk>
   ​```
   
   ```

7. 有了包名就可以执行砸壳了，这里以微信为例子，输入以下命令

   ```
               $ clutch -d com.tencent.xin
   ```

   显示如下

   ```
               # com.tencent.xin contains watchOS 2 compatible application. It's not possible to dump watchOS 2 apps with Clutch 2.0.4 at this moment.
               # Zipping WeChat.app
               # Swapping architectures..
               # ASLR slide: 0xb3000
               # ...
               # writing new checksum
               #DONE:/private/var/mobile/Documents/Dumped/com.tencent.xin-iOS7.0-(Clutch-2.0.4).ipa
               # Finished dumping com.tencent.xin in 76.9 seconds
   ```

   ```
   **注意DONE**后面就是砸壳成功后保存的路径
   ```

8. 我们把这个路径下的IPA导出到Mac上，砸壳就结束了











- RSA加密算法
   一种非对称的加密算法，用于通信。这种算法通常是客户端持有公钥，服务端持有私钥。客户端利用公钥加密。服务端可以用私钥解密。服务利用私钥加密数据发给客户端，客户端可以利用公钥解密出来。【简单来说就是：公钥加密的数据，利用私钥可以解密；私钥加密的数据，公钥同样能解密】
- 数字签名
   客户端把【数据】，并利用公钥加密【数据的MD5】，然后把这【数据】与【机密后的MD5】发给服务器
   服务器获取到数据包后，先求出【数据】的MD5，并解密【数据的MD5】，然后两者进行对比，如果不一样就代表数据被截获串改了

#### XCode如何将App安装到手机的【首先这个流程会涉及到2次数字签名】

1.Mac电脑本地生成公钥和私钥，把自己的公钥打包成CSR文件发给服务器。



![img](https:////upload-images.jianshu.io/upload_images/1975331-a3863afcbe1e9283.png?imageMogr2/auto-orient/strip|imageView2/2/w/673/format/webp)



2.苹果服务器利用自己的私钥，对 Mac电脑的公钥进行数字签名，生成证书与描述文件，将证书与描述文件返回给Mac电脑。



![img](https:////upload-images.jianshu.io/upload_images/1975331-950e0503a67197be.png?imageMogr2/auto-orient/strip|imageView2/2/w/798/format/webp)




 3.Mac电脑利用私钥对App的可执行文件的Hash值进行加密，生成App的签名
 4.Mac 将 App的可执行文件、App的签名、证书【关联Mac的私钥】、描述文件 打包成一个App传输给手机

![img](https:////upload-images.jianshu.io/upload_images/1975331-625a3a663440501f.png?imageMogr2/auto-orient/strip|imageView2/2/w/806/format/webp)

5.手机将会使用苹果的公钥，对证书解析，获得Mac的公钥。利用Mac的公钥，解析App签名，获取Hash值进行认证，认证成功则App成功安装上，认证失败就无法安装。



#### 远程登录到手机：

##### SSH介绍

SSH是Secure Shell的缩写,意为"安全外壳协议"，是远程登录会话和其他网络服务提供安全性的协议，用于计算机之间的加密登录，使用SSH,可以把所有传输的数据进行加密,"中间人"攻击方式就不可能实现,防止DNS欺骗和IP欺骗，`OpenSSH` 是SSH协议的免费开源的实现，是一个运用ssh、提供ssh登录的一个软件。

- SSH过程

> client首先向server发送登录请求
>  server收到请求以后发送一个公钥给client
>  client将密码用公钥加密以后发给server
>  server收到这段信息，用私钥解密，如果是正确的密码就登录成功

![img](https://upload-images.jianshu.io/upload_images/1460329-3bbfa6cc5a487de6.png?imageMogr2/auto-orient/strip|imageView2/2/w/591/format/webp)

SSH登录原理

- ssh安全隐患-中间人攻击（MITM攻击）

> 在client向server发送请求的时候，在client与server之间架起伪server然后拦截client的请求
>  用伪server向server发送一个登录请求，
>  server生成密钥A：公钥A，私钥A 反给伪server
>  伪server拿到公钥A以后，立马生成一对密钥B：公钥B、私钥B，将公钥B反给client
>  client拿到公钥B以后进行密码加密，发送server，然后伪server拦截
>  伪server拿到这段密文，用私钥B进行解密拿到密码，发送给黑客server，再将密码用公钥A进行加密发送给server

![img](https:////upload-images.jianshu.io/upload_images/1460329-835de87b79bcd2be.png?imageMogr2/auto-orient/strip|imageView2/2/w/700/format/webp)

SSH中间人攻击

