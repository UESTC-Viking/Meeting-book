## Padding Oracle Attack

#### 概述和一些基础知识

在``Eurocrypt 2002``大会上，``Vaudenay``介绍了针对CBC模式的``“Padding Oracle Attack”``。他可以在不知道密钥的情况下，通过对padding bytes的尝试，还原明文，或者构造任意密文。

在2010年的``BlackHat``欧洲大会上，``Juliano Rizzo``与``Thai Duong``介绍了“Padding Oracle”的实际攻击案例，并公布了ASP.NET存在的Padding Oracle问题。在2011年的``Pwnie Rewards``中，ASP.NET的这个漏洞被评为“最具价值的服务器端漏洞”。

Padding：填充

Oracle：一种填充模式

​	CBC加密模式回顾

​	![](Padding Oracle Attack.assets/cbcencrypt.png)



​	CBC解密模式回顾

​	![](Padding Oracle Attack.assets/cbcdecrypt.png)

​	Padding Oracle Attack顾名思义，其关键点在于加密和解密是的填充(Padding),，所以关于Padding Oracle Attack我们首先要了解一下什么是Padding以及其规则。

​	对于分组加密的模式，我们的明文可以为任意长度，但是由于分组加密的原理，我们的密文必须把分为规定大小的数据块，这也就意味着我们需要在分组时，对无法被数据填满的数据块进行一个填充，已得到一个完整的数据块。

​	那么对于填充我们有一些标准，比较常见的是PKCS#5标准中定义的规则。：PKCS#5规定：明文的最后一个数据块包含N个字节的填充数据（N取决于明文最后一个块的数据长度）。（每个数据块8字节长）。

​	关于PKCS#5标准可参考：https://www.di-mgt.com.au/cryptopad.html#PKCS5

​	我们看一张图

​	![](Padding Oracle Attack.assets/padding.png)

​	这里有一点要注意的是，对于AVOCADO这段明文，我们只填充一个字节的0x01；而对于PLANTAIN这段明文，我们则额外填充一个数据块，每个字节为0x08。因此我们总结一下填充规律，对于要加密的数据，其最后几个字节必须是以下几种情况：

```
data 0x01
data 0x02 0x02
data 0x03 0x03 0x03
data 0x04 0x04 0x04 0x04
data 0x05 0x05 0x05 0x05 0x05 
data 0x06 0x06 0x06 0x06 0x06 0x06
data 0x07 0x07 0x07 0x07 0x07 0x07 0x07
data 0x08 0x08 0x08 0x08 0x08 0x08 0x08 0x08
```

#### 假设一个Padding Oracle Attack 的场景

在网上看到一个具体的例子：（很清晰）

```
某个应用使用Query String参数来传递一个用户加密的用户名，公司ID和角色ID。参数使用CBC加密，每次都使用不同的初始化向量(IV)并添加在密文前端
```

当应用接收到加密后的值以后，它将返回三种情况：

接收到正确的密文之后（padding正确且包含合法的值），应用正常返回（200-OK）

接收到非法的密文之后（解密后发现填充不正确），应用抛出一个解密异常（500-Internal Server Error）

接收到合法的密文（填充正确）但解密后得到一个非法的值，应用显示自定义错误（200-OK）

上述场景是一个典型的填充提示（Padding Oracle），我们可以利用应用的行为判断某个加密的值填充是否正确。

下面有这样一个示例：

用户名（BRIAN），公司ID（12）及角色ID（12）：因此这里的明文是“**BRIAN;12;2;**”。以下则是经过加密的Query String实例，请注意加密后的UID参数使用了ASCII十六进制表示法。

``http://sampleapp/home.jsp?UID=7B216A634951170FF851D6CC68FC9537858795A28ED4AAC6``

实际情况中，攻击者是并不知道这里所对应的明文是多少的，但是作为一个示例，我们已经知道了明文、填充以及加密后的值，像这样：

<img src="Padding Oracle Attack.assets/example.png" style="width:1000px height:300px" />

攻击者可以根据加密后值的长度来推测出数据块的大小。由于长度（这里是24）能被8整除但不能被16整除，因此可以得知数据块的大小是8个字节。现在我们来观察下加密和解密的内部实现，下图便展示了字节级别的运算方式，这对以后攻击方式的讨论很有帮助。请注意，其中带圆圈的加号表示XOR（异或）操作。

所以加密过程如图：

<img src="Padding Oracle Attack.assets/encrypt_process.png" style="zoom:200%;" />

解密过程：

<img src="Padding Oracle Attack.assets/decrypt_process.png" style="zoom:200%;" />

解密过程结束后得到的最后一个数据块中，其结尾应该包含一个正确的填充序列，如果没有满足这点，那么加/解密程序会抛出一个padding异常。

#### 利用Padding Oracle进行解密

现在我们考虑如何用Padding Oracle Attack进行一个解密。我们每次操作一个单独的加密块。

这里我们先操作第一块也就是IV后面那块，在这个数据库前面加上全为NULL的IV值并发送过去：

```
Request: http://sampleapp/home.jsp?UID=0000000000000000F851D6CC68FC9537
Response: 500 - Internal Server Error
```

返回500错误代码，这是意料之内的事情

因为这个值在被解密后完全非法，在其结尾出现了非法填充（我们这里只解密单个数据块所以其结尾必须是合法填充）

这里用一张图解释一哈：

<img src="Padding Oracle Attack.assets/illegal.png" style="zoom:200%;" />

我们发现的确是非法填充

现在把IV加一并发送同样的密文看看会发生什么：

```
Request: http://sampleapp/home.jsp?UID=0000000000000001F851D6CC68FC9537
Response: 500 - Internal Server Error
```

还是500错误，但我们细品一下：

<img src="Padding Oracle Attack.assets/illegal_02.png" style="zoom:200%;" />

我们发现最后一个字节变了（0x3D变成了0x3C）

所以我们就这样一直给IV加一直到0xFF，我们必定会得到一种情况是产生了一个合法的单字节填充序列。对于0x00到0xFF这256个值中必然只有一个值会帮助我们得到正确的填充。这个值对应⬇这种情况：

```
Request: http://sampleapp/home.jsp?UID=000000000000003CF851D6CC68FC9537
Response: 200 OK
```

对应情况是⬇：

<img src="Padding Oracle Attack.assets/llegal.png" style="zoom:200%;" >

这个情况下我们来推断一下中间值：

```
因为 [Intermediary Byte] ^ 0×3C == 0×01, 
得到 [Intermediary Byte] == 0×3C ^ 0×01, 
所以 [Intermediary Byte] == 0×3D
```

我们得到中间值后回忆一下解密过程，中间值的每个字节都会与密文的前一个数据块中的对应字节进行一个异或操作（对于我们这里第一个数据块来说是IV），所以我们用原来我们已知的IV中的最后一个字节（0x0F）

也就是0x3D^0x0F=0x32，这表示数字2（铭文中第一个数据块的最后一个字节）

现在我们知道了第一个数据块的第八个字节了，应该关注第七个字节的解密了.

对于第七个字节我们要做的事情类似与之前，不同在于我们现在要求解密后的明文的第七和第八字节都为0x02（合法的padding），因为我们知道了中间值的第八个字节为0x3D了所以我们把IV的第八个字节设为0x3F（因为0x3D^0x3F=0x02)，然后再对IV第七个字节进行从0x00到0xFF的暴力枚举就可以了

<img src="Padding Oracle Attack.assets/seven.png" style="zoom:200%;" />

枚举成功：

<img src="Padding Oracle Attack.assets/seven_01.png" style="zoom:200%;" />

利用这种攻击方法我们可以从后向前破解所有中间值并最终得到解密的值

<img src="Padding Oracle Attack.assets/final.png" style="zoom:200%;" />

#### NPYCTF中的一道题目：

web🐕

源码：

```php
<?php 
error_reporting(0);
include('config.php');   # $key,$flag
define("METHOD", "aes-128-cbc");  //定义加密方式
define("SECRET_KEY", $key);    //定义密钥
define("IV","6666666666666666");    //定义初始向量 16个6
define("BR",'<br>');
if(!isset($_GET['source']))header('location:./index.php?source=1');


#var_dump($GLOBALS);   //听说你想看这个？
function aes_encrypt($iv,$data)
{
    echo "--------encrypt---------".BR;
    echo 'IV:'.$iv.BR;
    return base64_encode(openssl_encrypt($data, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $iv)).BR;
}
function aes_decrypt($iv,$data)
{
    return openssl_decrypt(base64_decode($data),METHOD,SECRET_KEY,OPENSSL_RAW_DATA,$iv) or die('False');
}
if($_GET['method']=='encrypt')
{
    $iv = IV;
    $data = $flag;    
    echo aes_encrypt($iv,$data);
} else if($_GET['method']=="decrypt")
{
    $iv = @$_POST['iv'];
    $data = @$_POST['data'];
    echo aes_decrypt($iv,$data);
}
echo "我摊牌了，就是懒得写前端".BR;

if($_GET['source']==1)highlight_file(__FILE__);
?>
```



参考：https://blog.gdssecurity.com/labs/2010/9/14/automated-padding-oracle-attacks-with-padbuster.html

​			https://www.freebuf.com/articles/web/15504.html

