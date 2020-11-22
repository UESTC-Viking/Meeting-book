#Docker

##Docker核心概念

**仓库(Registry)**

仓库其实是镜像仓库，里面有很多别人已经打包好的镜像，可以直接用docker pull拉到本地，默认的仓库是docker的官方仓库Docker Hub Registry

因为墙的原因 官方仓库用着比较慢，可以配一个官方的中国加速镜像，方法：修改/etc/docker/daemon.json,加入下面的键值：

{  

​	"registry-mirrors": ["https://registry.docker-cn.com"] 

}

举一个例子：

在仓库里搜关键字tutorial的话会有一些学习用的镜像

比如ddfddf/tutorial ,然后可以用docker pull命令拉到本地

当然你也可以在镜像名后面加tag来规定版本，比如docker pull ubuntu：14.04，这里tag就是14.04，当然tag也可以是单词，比如latest，如果你不加tag，默认拉取latest。

**镜像与容器**

（镜像的定义其实有点晦涩

镜像是一堆只读层的统一视角

[![](D:\LeanringFile\Viking\2.png)]

（容器的定义也差不多晦涩

容器也是一堆层的统一视角，不同的是容器最上面那层是可读可写的

![](D:\LeanringFile\Viking\3.png)

所以基本可以理解成

容器 = 镜像 + 可读可写层

但这里有一个问题 我们没有说是否要运行容器



**运行态容器**

一个运行态容器被定义为一个可读写的统一文件系统加上隔离的进程空间和包含其中的进程。

![](D:\LeanringFile\Viking\4.png)



![](D:\LeanringFile\Viking\5.png)

比如

docker run ubuntu touch viking.txt

即便这个Ubuntu容器不在运行，我们依旧能在主机的文件系统上找到这个viking.txt

所以根据这些我们来理解一下docker的命令

docker create <image-id>

![](D:\LeanringFile\Viking\6.png)

docker start <container-id>

![](D:\LeanringFile\Viking\7.png)

docker run <image-id>

![](D:\LeanringFile\Viking\8.png)

docker build

![](D:\LeanringFile\Viking\9.png)

docker ps

docker ps -a

docker images

docker images -a

docker commit

docker exec <running-container-id>



**关于docker的分层机制**

docker run ddfddf/tutorial apt-get install -y ping装ping命令

然后尝试

docker run ddfddf/tutorial ping www.baidu.com

发现报错

"ping": executable file not found in $PATH

说明没有装ping命令

此时我们执行

docker commit 0299878039f0 ddfddf/ping

再试一下我们新提交的learn/ping

docker run ddfddf/ping ping www.baidu.com

发现可以ping

![](D:\LeanringFile\Viking\10.png)



![](D:\LeanringFile\Viking\11.png)

**dockerfile**

如果你想要从一个基础镜像开始建立一个自定义镜像，可以选择一步一步进行构建，也可以选择写一个配置文件，然后一条命令（`docker build`）完成构建，显然配置文件的方式可以更好地应对需求的变更，这个配置文件就是Dockerfile。

示例

~~~

~~~

FROM debian:stretch

ARG DEBIAN_FRONTEND=noninteractive
ARG JAVA_VERSION=8
ARG JAVA_UPDATE=172
ARG JAVA_BUILD=11
ARG JAVA_PACKAGE=jdk
ARG JAVA_HASH=a58eab1ec242421181065cdc37240b08

ENV LANG C.UTF-8
ENV JAVA_HOME=/opt/jdk
ENV PATH=${PATH}:${JAVA_HOME}/bin

RUN set -ex \
 && apt-get update \
 && apt-get -y install ca-certificates wget unzip \
 && wget -q --header "Cookie: oraclelicense=accept-securebackup-cookie" \
         -O /tmp/java.tar.gz \
         http://download.oracle.com/otn-pub/java/jdk/${JAVA_VERSION}u${JAVA_UPDATE}-b${JAVA_BUILD}/${JAVA_HASH}/${JAVA_PACKAGE}-${JAVA_VERSION}u${JAVA_UPDATE}-linux-x64.tar.gz \
 && CHECKSUM=$(wget -q -O - https://www.oracle.com/webfolder/s/digest/${JAVA_VERSION}u${JAVA_UPDATE}checksum.html | grep -E "${JAVA_PACKAGE}-${JAVA_VERSION}u${JAVA_UPDATE}-linux-x64\.tar\.gz" | grep -Eo '(sha256: )[^<]+' | cut -d: -f2 | xargs) \
 && echo "${CHECKSUM}  /tmp/java.tar.gz" > /tmp/java.tar.gz.sha256 \
 && sha256sum -c /tmp/java.tar.gz.sha256 \
 && mkdir ${JAVA_HOME} \
 && tar -xzf /tmp/java.tar.gz -C ${JAVA_HOME} --strip-components=1 \
 && wget -q --header "Cookie: oraclelicense=accept-securebackup-cookie;" \
         -O /tmp/jce_policy.zip \
         http://download.oracle.com/otn-pub/java/jce/${JAVA_VERSION}/jce_policy-${JAVA_VERSION}.zip \
 && unzip -jo -d ${JAVA_HOME}/jre/lib/security /tmp/jce_policy.zip \
 && rm -rf ${JAVA_HOME}/jar/lib/security/README.txt \
       /var/lib/apt/lists/* \
       /tmp/* \
       /root/.wget-hsts

~~~

~~~

这里讲几个关键词

FROM :后面跟一个基础镜像，以这个镜像为基础进行构建

RUN：`RUN`后面跟的其实就是一些shell命令，通过&&将这些脚本连接在了一行执行，这么做的原因是为了减少镜像的层数，每多一行`RUN`都会给镜像增加一层，所以这里选择将所有命令联结在一起执行以减少层数

ARG：这个指令可以进行一些宏定义，比如我定义`ENV JAVA_HOME=/opt/jdk`，之后RUN后面的shell命令中的`${JAVA_HOME}`都会被`/opt/jdk`代替

ENV:可以看出这个指令的作用是在shell中设置一些环境变量（其实就是export）