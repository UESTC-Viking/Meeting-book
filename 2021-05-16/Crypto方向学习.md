#### Crypto方向学习

##### 常见编码

- base家族

  > base16、base32、base36、base58、base62、base64、base85、base91、base92
  >
  > 参考链接：https://www.cnblogs.com/0yst3r-2046/p/11962942.html

- Morse

  摩斯电码由两种基本信号组成：`·`、`_`。

  ![image-20210516144747074](crypto%E6%96%B9%E5%90%91%E5%AD%A6%E4%B9%A0.assets/20210516144747.png)

- Ook!

  ![image-20210516145306704](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516145306.png)

- brainfuck

  ![image-20210516145355958](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516145355.png)

- Quoted-printable编码

  任何8-bit字节值可编码为3个字符：一个等号“=”后跟随两个十六进制数字（0–9或A–F）表示该字节的数值。除了可打印ASCII字符与换行符以外，所有字符必须表示为这种格式。quoted-printable编码的数据的每行长度不能超过76个字符。为满足此要求又不改变被编码文本，在QP编码结果的每行末尾加上“软换行”。即在每行末尾加上一个“=”, 但并不会出现在解码得到的文本中。eg:

  > 换页符对ASCII码的十进制：12
  >
  > 表示为：=0C

- 在做题过程中遇到的奇奇怪怪的编码：社会主义核心价值观编码？与佛伦禅？标准银河字母？JsFuck?等等

对于编码问题，只要你能认出他是什么编码就一切好说(因为网上有很多针对性的在线编码解码工具，当然能自己编写脚本是最好的)。

##### 古典密码

- 单表替换加密

  - 凯撒密码

    凯撒密码（Caesar）加密时会将明文中的 每个字母 都按照其在字母表中的顺序向后（或向前）移动固定数目（循环移动）作为密文，如下图表所示（ROT3）。

    ![image-20210516150218648](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516150218.png)

  - 仿射密码

    仿射密码的加密函数是$E(x)=ax+b(mod m)$，其中，$x$表示明文按照某种编码得到的数字，$a$与$m$互素，$m$是编码系统中字母的数目。

    对应地，解密函数是$D(x)=a^{-1}(x-b)(mod m)$，其中$a^{-1}$是$a$在$Z_m$群的乘法逆元。

  - 简单替换加密

    类似于凯撒加密，但是替换规则是加密方随机定义的替换规则，如下图所示。

    ![image-20210516151648014](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516151648.png)

  - 埃特巴什码

    埃特巴什码其实就是一个上面介绍的简单替换密码的特例，它使用字母表中的最后一个字母代表第一个字母，倒数第二个字母代表第二个字母，以此类推。

    ![image-20210516151809580](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516151809.png)

- 多表替换加密

  - 维基利亚密码

    维吉尼亚密码是使用一系列凯撒密码组成密码字母表的加密算法，属于多表密码的一种简单形式，在进行加密时需要**秘钥**。

    例如，选取加密信息`viking`，加密秘钥`good`，处理后如下：

    > v  i  k  i n g
    >
    > g o o d g o

    然后加密时只需要查表就行了，例如第一个信息就查`v`列`g`行即为`b`，最终密文为`bwyltu`

    ![image-20210516152616156](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516152616.png)

  - 希尔密码

    希尔密码使用每个字母在字母表中的顺序作为其对应的数字，即 $A=0，B=1，C=2$ 等，然后将明文转化为 n 维向量，跟一个$ n × n $的矩阵相乘，再将得出的结果模 26。注意用作加密的矩阵（即密匙）在 $Z_{26}^{n}$必须是可逆的，否则就不可能解码。只有矩阵的行列式和 26 互质，才是可逆的。

  - Polybius

    ![image-20210516153346731](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516153346.png)

    例如：加密字母`V`得到51(行号列号)

  - ADFGX/ADFGVVX加密

    ![image-20210516153501532](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516153501.png)

    ![image-20210516153743701](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516153743.png)

    先经过之前的棋盘加密再选取**密钥**进行下一步换位加密。

- 其他密码

  - 栅栏密码

    栅栏密码把要加密的明文分成 N 个一组，然后把每组的第 1 个字连起来，形成一段无规律的话。

    例如：`It_is_good`分为两组`It_is`和`_good`，则加密信息为`I_tg_oiosd`。

  - 培根密码

    培根密码使用两种不同的字体，代表 A 和 B，结合加密表进行加解密。

    ![image-20210516154210218](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516154210.png)

  - 云影密码

    云影密码又称01248密码，使用 0，1，2，4，8 四个数字，其中 0 用来表示间隔，其他数字以加法可以表示出 如：28=10，124=7，18=9，再用 1->26 表示 A->Z。

    例如：字母`G`（对应数字7）可加密为124、421、2221等。

  - 曲路密码

    曲路密码是一种换位密码，需要事先双方约定密钥（也就是曲路路径）以及约定好填充的行数列数。然后在填充后按照约定的曲路路径读取信息。

    > 原文：The quick brown fox jumps over the lazy dog
    >
    > 密文：gesfc inpho dtmwu qoury zejre hbxva lookT

    ![image-20210516154742189](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516154742.png)

  - 列移位加密

    特殊的一种曲路密码。

    > 原文：The quick brown fox jumps over the lazy dog
    >
    > 密文：qoury inpho Tkool hbxva uwmtd cfseg erjez

    ![image-20210516155109694](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516155109.png)

  - 猪圈密码

    猪圈密码是一种以格子为基础的简单替代式密码，格子如下。

    ![image-20210516155537871](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516155537.png)

  - 圣堂武士密码

    变种猪圈密码？

    ![image-20210516155950541](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516155950.png)

  - 键盘密码

    例题：[NCTF2019]Keyboard

    ```
    ooo yyy ii w uuu ee uuuu yyy uuuu y w uuu i i rr w i i rr rrr uuuu rrr uuuu t ii uuuu i w u rrr ee www ee yyy eee www w tt ee
    ```

    先是26键对应到数字。

    ```
    1 2 3 4 5 6 7 8 9 0
    q w e r t y u i o p
    ```

    然后根据数字寻找九键，该字符串有几个字符就对应键盘上数字对应字母的第几个。例如`ooo`就对应九键的数字9键的第三字母y

    ![image-20210427182227417](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210427182234.png)

    - WP

    ```python
    cipher="ooo yyy ii w uuu ee uuuu yyy uuuu y w uuu i i rr w i i rr rrr uuuu rrr uuuu t ii uuuu i w u rrr ee www ee yyy eee www w tt ee"
    base=" qwertyuiop"
    a=[" "," ","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"]
    for part in cipher.split(" "):
        s=base.index(part[0])
        count=len(part)
        print(a[s][count-1],end="")
    ```

  - 舞动的小人密码

    ![image-20210516155847074](https://raw.githubusercontent.com/XiDuoEr/typroa-pictures/master/20210516155847.png)

##### 现代密码学

- 对称加密

  - DES算法
  - LFSR(线性反馈移位寄存器)

- 非对称加密

  - RSA算法

    RSA获取公钥和私钥

    >随意选择两个大的素数***p***和***q***，***p***不等于***q***，计算***N=pq***
    >
    >根据欧拉函数，求得***r = (p-1)(q-1)***
    >
    >选择一个大于1且小于 r 的整数***e***，且***e***与***r***互素，然后求***e*** 关于模 ***r*** 的***逆元***，得到***d***
    >
    >***(N,e)***是公钥，***d***是私钥

    加密明文m得到密文c：

    $$
    c = m^e(mod N)
    $$
    密文c解密得到明文m：
    $$
    m = c^d(mod N) = m^ed(mod N) = m^{kr+1}(mod N) = m(mod N) =  m
    $$

  - ECC算法

  - ElGamal算法

- 哈希函数

  - MD5

##### 常见题型？

解密、解密、解密



