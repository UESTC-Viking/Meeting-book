## i 春秋---刷题记录

### crypto

#### rsa256

首先利用 `openssl` 得到 `N` , 然后利用 `rsatool` 生成私钥文件 `private.pem` , 再对密文文件逐个解密。

![image-20201014112157757](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201014112204.png)

结果：`flag{3b6d3806-4b2b-11e7-95a0-000c29d7e93d}`

#### 这是个假的私钥

我的解题步骤：

- 尝试使用 `openssl` 解析公钥文件 `public.pem` ，得到`n` 和 `e`，但发现 `n` 位数太多无法分解。

  ```
  openssl rsa -pubin -text -modulus -in warmup -in public.pem
  
  n=D12B639DF759A99C9ADB57500BBD635041AA70F8E73F6EA158B23B9AF575915CF68CF8E57B045BBEBCF78A9BCA72BF0E63CB6EC353D66142048CB69EB5F20CDC6BAF6C85E77E6F2AA906DC5868FCB0F0330DEB55076EDF1B226EF54926DD2AD3C943C8EB35CB8C85848E05EA8680988A182701B6A0DC54967760CAC28136AD5B3D9F1CF7952B898CFAAF863A90BFD58DF0FA3F358A7EB8BCBE1BFCF13872BBB9FCC2B330A38F3FD689467FEF22F027549F53D460E9FEBB48F1AE15F7BFBA93069641FD53C46FC971560F5955D8F6E419F5981A9BA393718D785F58659607F511F6CC4077834E059F368EB05BCA7964EA2DC8CD1B13F62A29EA244A3876FF5967
  e=65537
  ```

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022190355.png" alt="image-20201022190348034" style="zoom:70%;" />

- 尝试使用 `openssl` 将私钥文件转换为公钥文件，发现与已知公钥文件 `public.pem` 一致。

  ```
  openssl rsa -in privatekey.pem -pubout -out pubkey.pem
  ```

  ![image-20201022190530465](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022190530.png)

- 尝试使用 `openssl` 解析私钥文件 `privatekey.pem` ，得到了关于 `p` 的信息。

  ```python
  openssl rsa -in privatekey.pem -text -out private.txt
  
  p=ee4e189845cc78efef4ac3e81d8aef997f735d5833b5c7e8494b9174ae211ba88231e2567ee6df9901328e0c6dbc5e24b343774785ae7e88ec409ca1d72901e32a582f291260eb9851fcbb0fff20805d000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000(已知现在的p是经过原本的p移位变化得到)
  ## 私钥文件解析
   modulus INTEGER, // RSA合数模 n 
   publicExponent INTEGER, //RSA公开幂 e 
   privateExponent INTEGER, //RSA私有幂 d 
   prime1 INTEGER, //n的素数因子p 
   prime2 INTEGER, //n的素数因子q 
   exponent1 INTEGER, //值 d mod (p-1) 
   exponent2 INTEGER, //值 d mod (q-1) 
   coefficient INTEGER, //CRT系数 (inverse of q) mod p 
   otherPrimeInfos OtherPrimeInfos OPTIONAL
  ```

  <img src="https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022190821.png" alt="image-20201022190821334" style="zoom: 60%;" />

- [SageMath在线转换](https://sagecell.sagemath.org/)。

  ```python
  # 参考：https://github.com/Zui-Qing-Feng/RSA/blob/master/03.Factoring%20with%20high%20bits%20known%20Attack
  n = 0xD12B639DF759A99C9ADB57500BBD635041AA70F8E73F6EA158B23B9AF575915CF68CF8E57B045BBEBCF78A9BCA72BF0E63CB6EC353D66142048CB69EB5F20CDC6BAF6C85E77E6F2AA906DC5868FCB0F0330DEB55076EDF1B226EF54926DD2AD3C943C8EB35CB8C85848E05EA8680988A182701B6A0DC54967760CAC28136AD5B3D9F1CF7952B898CFAAF863A90BFD58DF0FA3F358A7EB8BCBE1BFCF13872BBB9FCC2B330A38F3FD689467FEF22F027549F53D460E9FEBB48F1AE15F7BFBA93069641FD53C46FC971560F5955D8F6E419F5981A9BA393718D785F58659607F511F6CC4077834E059F368EB05BCA7964EA2DC8CD1B13F62A29EA244A3876FF5967L
  p_fake = 0xee4e189845cc78efef4ac3e81d8aef997f735d5833b5c7e8494b9174ae211ba88231e2567ee6df9901328e0c6dbc5e24b343774785ae7e88ec409ca1d72901e32a582f291260eb9851fcbb0fff20805d000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000L
  
  pbits = 1024  
  kbits = 384
  pbar = p_fake & (2^pbits-2^kbits)  
  print ("upper %d bits (of %d bits) is given" % (pbits-kbits, pbits))
     
  PR.<x> = PolynomialRing(Zmod(n))  
  f = x + pbar  
  x0 = f.small_roots(X=2^kbits, beta=0.4)[0]  # find root < 2^kbits with factor >= n^0.3  
  p = int(x0 + pbar)
  print('p=',p)
  
  #结果
  #p=167343506005974003380506069679607737381940204686173214188860057004909006055220516074283090160430833007424970980655748310232878462615469792561310560310363430669700009093597847018287568821792168143170329382585883857083334915378884054389878477389765792275111293420203613159303898365894897865177093362621517279751
  ```

  ![image-20201022193704475](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022193704.png)

- `python` 中 `n/p` 得到 `q`。

  ```python
  q=157790417717035275943197904823645145281147085252905247447260034051878691034747684303715336348507267921249655103263347914128144476912685213431110454636244692224328066884510063590700506729345331153483633231327359450199822698241355428609085077662488946173655043172957247264543259611018596088670385591091710018977
  ```

- 使用 `rsatools` 生成私钥文件。

  ```
  python3 rsatool.py -o private.pem -e 65537 -p 167343506005974003380506069679607737381940204686173214188860057004909006055220516074283090160430833007424970980655748310232878462615469792561310560310363430669700009093597847018287568821792168143170329382585883857083334915378884054389878477389765792275111293420203613159303898365894897865177093362621517279751 -q 157790417717035275943197904823645145281147085252905247447260034051878691034747684303715336348507267921249655103263347914128144476912685213431110454636244692224328066884510063590700506729345331153483633231327359450199822698241355428609085077662488946173655043172957247264543259611018596088670385591091710018977
  ```

  ![image-20201022195142758](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022195142.png)

- `WP.py`

  ```python2
  #coding:utf-8
  from Crypto.PublicKey import RSA
  from Crypto.Cipher import PKCS1_v1_5 as Cipher_pkcs1_v1_5
  import base64
  import gmpy2
  
  cipher_text = "qzogS7X8M3ZOpkUhJJcbukaRduLyqHAPblmabaYSm9iatuulrHcEpBmil7V40N7gbsQXwYx5EBH5r5V2HRcEIOXjgfk5vpGLjPVxBLyXh2DajHPX6KvbFpQ8jNpCQbUNq8Hst00yDSO/6ri9dk6bk7+uyuN0b2K1bNG5St6sCQ4qYEA3xJbsHFvMqtvUdhMiqO7tNCUVTKZdN7iFvSJqK2IHosIf7FqO24zkHZpHi31sYU7pcgYEaGkVaKs8pjq6nbnffr4URfoexZHeQtq5UAkr95zD6WgvGcxaTDKafFntboX9GR9VUZnHePiio7nJ3msfue5rkIbISjmGCAlj+w=="
  n=26405201714915839490865227813246218372938736243516916108608439705738170543023112509150522274284238701776297205717958250714972924576706985074311737321016048409831557758205687745692399643151467933196930799657476449865271038382866908177517793954543176769652784274788769353482450910551831498252972857285424471782215525406445071432588374802623485148684255853068532820859835479998199886719945699488858505070686919320144576280217196858823521754407597888769668827432569034617434944912323704501156532854074083408527717809315663187405585840074689387865750105223058720511199252150772925124516509254841404742306560035497627834727
  e=65537
  p=167343506005974003380506069679607737381940204686173214188860057004909006055220516074283090160430833007424970980655748310232878462615469792561310560310363430669700009093597847018287568821792168143170329382585883857083334915378884054389878477389765792275111293420203613159303898365894897865177093362621517279751
  
  q=n/p
  phi = (p-1)*(q-1)
  d = gmpy2.invert(e,phi)
  print q
  
  f = open('D:/ctf/ichunqiu/Crypto/private.pem','r')
  key = RSA.importKey(f.read())
  cipher = Cipher_pkcs1_v1_5.new(key)
  flag = cipher.decrypt(base64.b64decode(cipher_text),'ERROR')
  print flag
  
  #flag{Nes5un0_puo_emend4rs1_d4l_p3ccato_che_sc0rre_nele_vene}
  ```

  ![image-20201022195948855](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201022195948.png)

#### 传感器1 

![image-20201023181904186](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201023181911.png)

- 曼彻斯特编码

  - 曼彻斯特编码，也叫做相位编码，是一种**双向编码**。

  - 是一个同步时钟编码技术，被物理层使用来编码一个同步位流的时钟和数据。

  - 曼彻斯特编码被用在[以太网](https://baike.baidu.com/item/以太网)媒介系统中。

  - 它也是通过电平的高低转换来表示“0”或“1”，每位中间的电平转换既表示了数据代码，也作为定时信号使用。

  - 在曼彻斯特编码中，每一位的中间有一跳变，位中间的跳变既作时钟信号，又作数据信号。

    - **从高到低跳变表示“1”**
    - **从低到高跳变表示“0”**

  - 可能的情况：

    - 0101 -> 11 -----------  **5**
    - 0110 -> 10 -----------  **6**
    - 1001 -> 01 -----------  **9**
    - 1010 -> 01 -----------  **A**

  - 优点：1个比特的中间有一次电平跳变，两次电平跳变的时间间隔可以是*T*/2或*T*；利用电平跳变可以产生收发双方的同步信号；曼彻斯特编码是一种自同步的编码方式，即时钟同步信号就隐藏在数据波形中。在曼彻斯特编码中，每一位的中间有一跳变，该跳变既可作为时钟信号，又可作为数据信号。因此，发送曼彻斯特编码信号时无须另发同步信号。

  - 特点

    - 传输流的速率是原始数据流的两倍，要占用较宽的频带；
    - 信号恢复简单，只要找到信号的边缘进行异步提取即可；
    - 10Mb/s以太网采用曼彻斯特码。

  - 例：BUUCTF---传感器

    下载附件解压得到题目.txt，其中内容为：

    > 5555555595555A65556AA696AA6666666955
    > 这是某压力传感器无线数据包解调后但未解码的报文(hex)
    >
    > 已知其ID为0xFED31F，请继续将报文完整解码，提交hex。
    >
    > 提示1：曼联

    对体重所给的那串数字进行decode得到：
    `11111111 11111111 01111111 11001011 11111000 00100110 00001010 10101010 10011111`
    然后，`bin->hex`，对比ID并不重合，根据八位倒序传输协议将二进制每八位reverse，转hex即可
    最终所得结果为`flag{FFFFFED31F645055F9}`。

- 差分曼彻斯特编码

  - 差分曼彻斯特编码是一种**使用中位转变来计时**的编码方案。
  - 数据通过在数据位开始处加一转变来表示。
  - 令牌环局域网就利用差分曼彻斯特编码方案。
  - 差分曼彻斯特编码在每个时钟周期的中间都有一次电平跳变，这个跳变做同步之用。 
  - 在每个时钟周期的***起始处***：**跳变则说明该比特是0，不跳变则说明该比特是1**。
  - 优点：收发双方可以根据编码自带的时钟信号来保持同步，无需专门传递同步信号的线路，因此成本低。
  - 缺点：实现技术复杂。

- `Wp.py`

  ```python
  def differential_Manchester_encoding_decode(code):
      str = ""
      for i in range(len(code[2:])/2):
          code1 = code[i*2:i*2+2]
          code2 = code[i*2+2:i*2+4]
          if code2 !='10' and code2 !='01':
              continue
          if code1 !='10' and code1 !='01':
              continue
          if code1 != code2:
              str+='1'
          else:
              str+='0'
      return str
  
  a = 0x3EAAAAA56A69AA55A95995A569AA95565556
  b = 0x3EAAAAA56A69AA556A965A5999596AA95656
  str1 = ""
  str2 = ""
  
  a1 = bin(a)
  print a1
  str1 = differential_Manchester_encoding_decode(a1)
  print str1
  print hex(int(str1,2)).upper()+"\n"
  
  b1 = bin(b)
  print b1
  str2 = differential_Manchester_encoding_decode(b1)
  print str2
  print hex(int(str2,2)).upper()
  
  #结果，由于在bin2hex时，前面的00被省略了，所以最终结果为
  #0x0024D 8893CA58 4181L
  #0x0024D 8845ABF3 4119L
  #flag{8845ABF3}
  ```

![image-20201023190100282](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201023190100.png)

#### 传感器2

![image-20201023182024427](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201023182024.png)

- 发现：所给未解码报文 `3EAAAAA56A69AA55A95995A569AA95565556` 与 `传感器1` 中的第一个未解码报文一致。

- 经过 [CRC（循环冗余校验）在线计算](http://www.ip33.com/crc.html),发现在 `CRC-8` 情况下计算得到的校验位 `81` 与原有的一致。

  ![image-20201023192127753](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201023192127.png)

- 将 `02 4D DE AD BE EF 41` 和 `02 4D BA AD A5 55 41` 带入计算可得结果分别为 `B5` 和 `15` 。则最后结果为 `flag{B515}`

#### 小心猪圈

题目内容：

```
R29kIGpvYjo1NzY1NkM2QzIwNjQ2RjZFNjUzQTRBMzU1ODQ3NTkzNjRBNDE0RTM1NTg0NzRCNDk0NDU0NEY1MjUzNTg0MTQ5NDQ0MjRGMzU1MTU4NTM0RjUzNTI0NzQ5MzQ1ODRCNTc1MjU0NEE0QTU1NDc0OTUzNDM1NzRGNEU0RDU2NTE1NTU0NTE0RDQ5NUE0NDRCMzY1MzUwNEU1NTM0NTc1NTU5NEM0RjQ5NDkzMzU3NEIzMzRDNTU0RjQyNTI1NzMyMzQ0MjU0NEQ0QTRDNTQ1MTM1NDQ0MzQ3NEEzNDQ4NDc1OTRDNEE0NzQ2MzI1NzQ1NTYzMzU5NDc0RTQ3NDY0RjM0NDQ1NzRENTY0NDU3NTczNTQ0NDU0OTM0NTk1ODRENTk1MzQ4NDc1NjM0NTc0NTMzNEE1QTQ3NDI1MzQ3MzI0RDRDMzI0RDVBNDk1NDMyNTA0OTNE
```

- [base64在线](https://www.ltool.net/base64-encode-and-decode-in-simplified-chinese.php)。

  ```
  God job:57656C6C20646F6E653A4A35584759364A414E3558474B4944544F525358414944424F355158534F5352474934584B5752544A4A5547495343574F4E4D56515554514D495A444B3653504E55345755594C4F494933574B334C554F425257323442544D4A4C5451354443474A344847594C4A4746325745563359474E47464F3444574D56445757354445493459584D5953484756345745334A5A47425347324D4C324D5A49543250493D 
  ```

- [16进制到文本字符串](https://www.bejson.com/convert/ox2str/)。

  ```
  Well done:J5XGY6JAN5XGKIDTORSXAIDBO5QXSOSRGI4XKWRTJJUGISCWONMVQUTQMIZDK6SPNU4WUYLOII3WK3LUOBRW24BTMJLTQ5DCGJ4HGYLJGF2WEV3YGNGFO4DWMVDWW5DEI4YXMYSHGV4WE3JZGBSG2ML2MZIT2PI=
  ```

- [Base32编码解码](https://www.qqxiuzi.cn/bianma/base.php)。

  ```
  Only one step away:Q29uZ3JhdHVsYXRpb25zOm9janB7emtpcmp3bW8tb2xsai1ubWx3LWpveGktdG1vbG5ybm90dm1zfQ==
  ```

- [base64在线](https://www.ltool.net/base64-encode-and-decode-in-simplified-chinese.php)。

  ```
  Congratulations:ocjp{zkirjwmo-ollj-nmlw-joxi-tmolnrnotvms} 
  ```

- 直接使用 `ocjp{zkirjwmo-ollj-nmlw-joxi-tmolnrnotvms}` 提交，报错。然后题目提示了与 `猪圈密码` 有关。

  ![image-20201023202448400](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201023202448.png)

- 将 `ocjp` 替换为 `flag` ,发现转换关系为：**将左右两边的转换交换了**，如上图所示。

- `flag{vbriasdf-fcca-edcs-aftr-xdfceiefxzdw}`。

- WP.py

  ```python
  s='ocjp{zkirjwmo-ollj-nmlw-joxi-tmolnrnotvms}'
  dic={'a':'j','b':'k','c':'l','d':'m','e':'n','f':'o','g':'p','h':'q','i':'r','s':'w','t':'x','v':'z','u':'y'}
  temp={}
  for i in dic:
      temp[dic[i]]=i
  for i in temp:
      dic[i]=temp[i]
  flag=''
  for i in s:
      if i.isalpha():
          flag += dic[i]
      else:
          flag+=i
  print(flag)
  ```


#### Round Rabins!

- 前置知识——Rabin密码体制

Rabin密码体制是对RSA算法的一种修正，它具有两个特点：

1. 它不是以一一对映的单项陷门函数为基础的，对同一密文可能有两个以上对应的明文。
2. 破译该体制等价于对大数进行分解。

（RSA算法中选取的公钥`e`满足$1<e<\phi(n)$，且$gcd(e,\phi(n))=1$，而在Rabin密码体制中选取$e=2$）。

- 密钥的产生

随机选择两个大素数，满足$p\equiv q\equiv 3mod4$，即这两个素数的形式为$4k+3$；计算$n=p\times q$，其中n为公钥，p,q为私钥。

- 加密

$$
c\equiv m^2modn
$$

其中m为明文分组，c是对应的密文分组。

- 解密

解密就是求c模n的平方根，即解$x^2\equiv cmodn$，该方程等价于方程组：
$$
\begin{cases}
x^2\equiv cmodp \\
x^2\equiv cmodq
\end{cases}
$$
由于$p\equiv q\equiv 3mod4$，所以可以解出每个方程有两个解
$$
x\equiv ymodp,x\equiv -ymodp \\
x\equiv zmodq,x\equiv -zmodq
$$
两两组合可得到4个同余方程组：
$$
1、\begin{cases}
x\equiv y \space mod \space p \\
x\equiv z \space mod \space q
\end{cases}  \\
2、\begin{cases}
x\equiv \space y \space mod \space p \\
x\equiv -z \space mod \space q
\end{cases}  \\
3、\begin{cases}
x\equiv -y \space mod \space p \\
x\equiv \space z \space mod \space q
\end{cases}  \\
4、\begin{cases}
x\equiv -y \space mod \space p \\
x\equiv -z \space mod \space q
\end{cases}
$$
最终，利用中国剩余定理，可解出每个方程组的解，共四个，即每一密文对应的明文不唯一。为有效确定明文一般可根据m中的某些代表性信息来判断。

```python
#题目描述：John gave up on RSA and moved to Rabin. ...he still did it wrong though flag.txt  What a box! 

#flag.txt内容
N=0x6b612825bd7972986b4c0ccb8ccb2fbcd25fffbadd57350d713f73b1e51ba9fc4a6ae862475efa3c9fe7dfb4c89b4f92e925ce8e8eb8af1c40c15d2d99ca61fcb018ad92656a738c8ecf95413aa63d1262325ae70530b964437a9f9b03efd90fb1effc5bfd60153abc5c5852f437d748d91935d20626e18cbffa24459d786601

c=0xd9d6345f4f961790abb7830d367bede431f91112d11aabe1ed311c7710f43b9b0d5331f71a1fccbfca71f739ee5be42c16c6b4de2a9cbee1d827878083acc04247c6e678d075520ec727ef047ed55457ba794cf1d650cbed5b12508a65d36e6bf729b2b13feb5ce3409d6116a97abcd3c44f136a5befcb434e934da16808b0b
```

- 将`N`转换为十进制，并在线分解。

![image-20201201153017245](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20201201153024.png)

```
#结果
p=q=8683574289808398551680690596312519188712344019929990563696863014403818356652403139359303583094623893591695801854572600022831462919735839793929311522108161
```

- `WP.py`

  ```python
  #!/usr/bin/env python
  def legendre_symbol(a, p):
      ls = pow(a, (p - 1)/2, p)
      if ls == p - 1:
          return -1
      return ls
   
  def prime_mod_sqrt(a, p):
      a %= p
      
      if a == 0:
          return [0]
      if p == 2:
          return [a]
      
      if legendre_symbol(a, p) != 1:
          return []
  
      if p % 4 == 3:
          x = pow(a, (p + 1)/4, p)
          return [x, p-x]
  
      q, s = p - 1, 0
      while q % 2 == 0:
          s += 1
          q //= 2
  
      z = 1
      while legendre_symbol(z, p) != -1:
          z += 1
      c = pow(z, q, p)
  
      x = pow(a, (q + 1)/2, p)
      t = pow(a, q, p)
      m = s
      while t != 1:
  
          i, e = 0, 2
          for i in range(1, m):
              if pow(t, e, p) == 1:
                  break
              e *= 2
  
          b = pow(c, 2**(m - i - 1), p)
          x = (x * b) % p
          t = (t * b * b) % p
          c = (b * b) % p
          m = i
   
      return [x, p-x]
   
  def egcd(a, b):
      if a == 0:
          return (b, 0, 1)
      else:
          g, y, x = egcd(b % a, a)
          return (g, x - (b // a) * y, y)
   
  def modinv(a, m):
      g, x, y = egcd(a, m)
      if g != 1:
          raise Exception('modular inverse does not exist')
      else:
          return x % m
   
   
  # This finds a solution for c = x^2 (mod p^2)
  def find_solution(c, p):
      n = p ** 2
  
      r = prime_mod_sqrt(c,p)[0]
   
      inverse_2_mod_n = modinv(2, n)
      inverse_r_mod_n = modinv(r, n)
   
      new_r = r - inverse_2_mod_n * (r - c * inverse_r_mod_n)
   
      return new_r % n
   
  if __name__ == "__main__":
      n = 0x6b612825bd7972986b4c0ccb8ccb2fbcd25fffbadd57350d713f73b1e51ba9fc4a6ae862475efa3c9fe7dfb4c89b4f92e925ce8e8eb8af1c40c15d2d99ca61fcb018ad92656a738c8ecf95413aa63d1262325ae70530b964437a9f9b03efd90fb1effc5bfd60153abc5c5852f437d748d91935d20626e18cbffa24459d786601
      p = 0xa5cc6d4e9f6a893c148c6993e1956968c93d9609ed70d8366e3bdf300b78d712e79c5425ffd8d480afcefc71b50d85e0914609af240c981c438acd1dcb27b301
      c = 0xd9d6345f4f961790abb7830d367bede431f91112d11aabe1ed311c7710f43b9b0d5331f71a1fccbfca71f739ee5be42c16c6b4de2a9cbee1d827878083acc04247c6e678d075520ec727ef047ed55457ba794cf1d650cbed5b12508a65d36e6bf729b2b13feb5ce3409d6116a97abcd3c44f136a5befcb434e934da16808b0b
   
      solution = find_solution(c, p)
      print hex(solution)[2:-1].decode("hex")
  ```

  结果：`IceCTF{john_needs_to_get_his_stuff_together_and_do_things_correctly}`
