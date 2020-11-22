## CTF tools for RSA

### RSA算法原理

#### ① RSA获取公钥和私钥

>随意选择两个大的素数***p***和***q***，***p***不等于***q***，计算***N=pq***
>
>根据欧拉函数，求得***r = (p-1)(q-1)***
>
>选择一个大于1且小于 r 的整数***e***，且***e***与***r***互素，然后求***e*** 关于模 ***r*** 的***逆元***，得到***d***
>
>***(N,e)***是公钥，***d***是私钥

#### ② 加密明文m得到密文c

$$
c = m^e(mod N)
$$

#### ③ 密文c解密得到明文m

$$
m = c^d(mod N) = m^ed(mod N) = m^{kr+1}(mod N) = m(mod N) =  m
$$

###  RSA工具

#### ①openssl

- openssl可以实现：秘钥证书管理、对称加密和非对称加密 

- 安装

  ``` 
  sudo apt-get install openssl
  ```

- 文档

  - 中文：https://github.com/KaiZhang890/openssl-howto
  - 英文：https://www.madboa.com/geek/openssl/

- 用途

  - **生成PKCS#1私钥**

    ``` 
     # 生成私钥到控制台，默认为1024位
     openssl genrsa
     # 2048位密钥，保存到prikey.pem
     openssl genrsa -out prikey.pem 2048
    ```

    ![image-20200715232705263](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200715232705.png)

    打开生成的文件rsa_prikey.pem，其中格式为

    ```
    -----BEGIN RSA PRIVATE KEY-----
    -----END RSA PRIVATE KEY-----
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200715233108.png" alt="image-20200715233108215" style="zoom:67%;" />

  - **把RSA私钥转换成PKCS8格式**

    ```
    openssl pkcs8 -topk8 -inform PEM -in rsa_prikey.pem -outform PEM -nocrypt -out prikey.pem
    ```

    打开生成的文件prikey.pem，其中格式为

    ```
    -----BEGIN  PRIVATE KEY-----
    -----END PRIVATE KEY-----
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716090749.png" alt="image-20200716090749067" style="zoom:67%;" />

  - **根据私钥生成公钥**

    ```
    # 根据私钥生成公钥放在控制台
    openssl rsa -in prikey.pem -pubout
    # 根据私钥生成公钥并保存到pubkey.pem
    openssl rsa -in prikey.pem -pubout -out pubkey.pem
    ```

    ![image-20200716091420772](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716091420.png)

    打开生成的文件pubkey.pem，其中格式为

    ```
    -----BEGIN PUBLIC KEY-----
    -----END PUBLIC KEY-----
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716091627.png" alt="image-20200716091627690" style="zoom:67%;" />

  - **提取PEM RSAPublicKey格式公钥**

    ```
    openssl rsa -in rsa_prikey.pem -RSAPublicKey_out -out key.pem
    ```

    打开生成的文件key.pem，其中格式为

    ```
    -----BEGIN RSA PUBLIC KEY-----
    -----END RSA PUBLIC KEY-----
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716092419.png" alt="image-20200716092220174" style="zoom:67%;" />

  - **公钥加密文件**

    ```
    openssl rsautl -encrypt -in input.file -inkey pubkey.pem -pubin -out output.file
        -in 指定被加密的文件
        -inkey 指定加密公钥文件
        -pubin 表面是用纯公钥文件加密
        -out 指定加密后的文件
    ```

    建立明文文件Mingwen.txt，其中内容为

    ```
    Welcome_to_Viking
    ```

    然后利用openssl以及刚刚生成的公钥加密，命令如下所示

    ![image-20200716093557162](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716093557.png)

    然后打开flag.enc，其中内容为：

    ![image-20200716093700080](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716093700.png)

  - **私钥解密文件**

    ```
    openssl rsautl -decrypt -in input.file -inkey key.pem -out output.file
        -in 指定需要解密的文件
        -inkey 指定私钥文件
        -out 指定解密后的文件
    ```

    直接选择把解密后的内容放在控制台，得到明文，如下所示。

    ![image-20200716094014169](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716094048.png)

#### ②rsatool

- **安装**

  ```
  git clone https://github.com/ius/rsatool.git
  cd rsatool  //进入这个目录
  python setup.py install
  ```

  - 安装可能遇到的问题(安装第三步)

    ImportError:No module named setuptools

    解决办法 --- 安装setuptools

    进入(https://pypi.org/),搜索setuptools,找到你想要的版本，然后如下图所示

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200715154458.png" alt="image-20200715154458885" style="zoom: 67%;" />

    ```
    # 进入kali,进行如下操作
    wget https://files.pythonhosted.org/packages/2f/8e/38259f4a44944a92068d5ff77230511a4c685604b47a81318f9e5cf2cc24/setuptools-49.2.0.zip  #下载setuptools（链接为刚刚复制所得）
    upzip setuptools-49.2.0.zip  #解压 --- 也可直接先手动解压
    cd setuptools-49.2.0    #进入目录
    python3 setup.py build  #组建，使用别人的python包都要组建再安装
    python3 setup.py install  #安装
    ```

- **用途**

  - 提供模数和私有指数

    ```
    # 生成PEM到控制台
    python3 rsatool.py -f PEM -n num1 -d num2
    # 生成PEM保存到文件key.pem
    python3 rsatool.py -f PEM -o key.pem -n num1 -d num2
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716102159.png" alt="image-20200716102140068" style="zoom: 80%;" />

  - 提供p、q生成私钥文件

    ```
    # 生成PEM到控制台
    python3 rsatool.py -f PEM -n num1 -q num2
    # 生成PEM保存到文件key.pem
    python3 rsatool.py -f PEM -o prikey.pem -n num1 -q num2
    ```

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716103206.png" alt="image-20200716103206779"  />

#### ③RSAtool

- 安装

  下载地址：http://www.skycn.net/soft/appid/39911.html

- 用途

  `RSAtool`是一个非常方便实用的小工具，可以用来计算 RSA 中的几个参数(**n、p、q、e、d**)、生成密钥、加解密，一些不太复杂的破解工作也可以用它。

- 界面分析

  - 主界面

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716110152.png" alt="image-20200716110152111" style="zoom: 80%;" />

  - 点击Test后进入加解密界面

    ![image-20200716110739520](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716110739.png)

#### ④yafu

- 安装

  下载地址：https://sourceforge.net/projects/yafu/

- 使用

  - 命令行进入yafu安装目录下

  - 输入`yafu-x64.exe`

  - 执行分解n

    ```
    factor(n)  #n为要分解数的值
    ```

- 对于比较小的数，可采用`yafu`，如果数字过于太大，`yafu`的效率就不是很高，所以一般不介意使用`yafu`分解n，一般采用[在线分解](http://factordb.com/)。

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200714181543.png" alt="image-20200714181518005" style="zoom:50%;" />

#### ⑤RsaCtfTool

- 安装

  ```
  安装之前必须先安装这四个库(PyCrypto,GMPY2,SymPy,requests)
  
  git clone https://github.com/Ganapati/RsaCtfTool.git 
  cd RsaCtfTool  //进入这个目录
  安装python第三方库
  pip install -r requirements.txt
  ```

- 用途

  参考文档：https://github.com/Headorteil/RsaCtfTool

  - 攻击RSA
  - 给定n和e创建公钥文件
  - 从PEM / DER格式的公钥或私钥中转储公共和/或私人号码

### 实例

#### 0x01 easy_RSA

题目描述：

> 解答出来了上一个题目的你现在可是春风得意，你们走向了下一个题目所处的地方 你一看这个题目傻眼了，这明明是一个数学题啊！！！可是你的数学并不好。扭头看向小鱼，小鱼哈哈一笑 ，让你在学校里面不好好听讲现在傻眼了吧~来我来！三下五除二，小鱼便把这个题目轻轻松松的搞定了。flag格式为cyberpeace{小写的你解出的答案}

附件内容

> 在一次RSA密钥对生成中，假设p=473398607161，q=4511491，e=17
> 求解出d

这里，我们可以直接编写脚本，但是采用RSAtool更加简单。

<img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716143954.png" alt="image-20200716143850835" style="zoom:67%;" />

#### 0x02 Easy RSA

题目描述：

> 还记得veryeasy RSA吗？是不是不难？那继续来看看这题吧，这题也不难。
>
> 已知一段RSA加密的信息为：0xdc2eeeb2782c且已知加密所用的公钥：
>
> (N=322831561921859 e = 23)
>
> 请解密出明文，提交时请将数字转化为ascii码提交
>
> 比如你解出的明文是0x6162，那么请提交字符串ab
>
> 提交格式:PCTF{明文字符串}

这里，我们可以先在线将n分解然后编写脚本实现，但是这里采用RSAtool更加简单。

<img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716145553.png" alt="image-20200716145553764" style="zoom:67%;" />

进如页面之后，在使用解密之前，必须要先点一次加密，所以我现在明文框里输入了`123`，点击`Encrypt`。之后再将密文转换为十六进制数`242094131279916`,然后在密文框内输入，点击`Decrypt`，最终得到结果`3a5Y`，即`PCTF{3a5Y}`。

![image-20200716150325064](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200716150325.png)

#### 0x03 Normal_RSA

题目描述：

> 你和小鱼走啊走走啊走，走到下一个题目一看你又一愣，怎么还是一个数学题啊 小鱼又一笑，hhhh数学在密码学里面很重要的！现在知道吃亏了吧！你哼一声不服气，我知道数学 很重要了！但是工具也很重要的，你看我拿工具把他解出来！你打开电脑折腾了一会还真的把答案 做了出来，小鱼有些吃惊，向你投过来一个赞叹的目光

附件内容：密文文件flag.enc和公钥文件pubkey.pem

```
#flag.enc文件内容
m>·ß#îáÓ‡¾ºx ‡Žœe½=ImÚd’A™y

#pubkey.pem文件内容
-----BEGIN PUBLIC KEY-----
MDwwDQYJKoZIhvcNAQEBBQADKwAwKAIhAMJjauXD2OQ/+5erCQKPGqxsC/bNPXDr
yigb/+l/vjDdAgMBAAE=
-----END PUBLIC KEY-----
```

**解题步骤**

- 使用 openssl 解密.pem 中参数

  ![image-20200714150823780](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200714150844.png)

  - 发现：

    ```
    e = 65537
    n = C2636AE5C3D8E43FFB97AB09028F1AAC6C0BF6CD3D70EBCA281BFFE97FBE30DD
    ```

- 参数十六进制转换为十进制

  - 利用[在线转换](https://tool.lu/hexconvert/)

  - 直接命令行转换`python`---->`0xC2636AE5C3D8E43FFB97AB09028F1AAC6C0BF6CD3D70EBCA281BFFE97FBE30DD`

    ![image-20200714151405403](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200714151405.png)

- 利用 factor 对大整数进行分解，得到 p 和 q

  - [在线分解](http://factordb.com/)n得到p = 275127860351348928173285174381581152299、 q =  319576316814478949870590164193048041239

    ![image-20200714171343022](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200714171343.png)

  - yafu分解n------速度较慢，不建议使用。

    <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200714181543.png" alt="image-20200714181518005" style="zoom:50%;" />

- 用 rsatool 生成私钥文件: private.pem

  使用命令行：`python3 rsatool.py -o private.pem -e 65537 -p 275127860351348928173285174381581152299 -q 319576316814478949870590164193048041239`

  ![image-20200715112533262](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200715112533.png)

- 用 private.pem 解密 flag.enc

  由于我之前生成的privite.pem是在rsatool目录下生成的，将其移动到与flag.enc同目录下，然后使用命令行`openssl rsautl -decrypt -in flag.enc -inkey private.pem`解密。

  ![](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20200715112740.png)

则最终结果为`PCTF{256b_i5_m3dium}`





