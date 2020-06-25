# URL编码解码与网站目录

**我们知道URL有一套特殊的编码解码规则**

## 引例

**PHP2**

![image-20200530154249443](image-20200530153250653.png)

这是界面，似乎没有什么线索对吧？



**用扫描器扫了一下目录**

![image-20200530161401056](image-20200530161344625.png)

出现了一个从来没见过的***index.phps***



查了一下，一下是phps文件的解释：

The PHPS file type is primarily associated with PHP Source by The PHP Group. Generally, PHP files will get interpreted by the Web server and PHP executable, and you will never see the code behind the PHP file. If you make the file extension .PHPS, a properly-configured server will output a color-formated version of the source instead of the HTML that would normally be generated. Not all servers are so configured.

**PHPS文件类型主要由PHP组与PHP源关联。通常，PHP文件将由Web服务器和PHP可执行文件解释，您将永远看不到PHP文件背后的代码。如果将文件扩展名设为.PHPS，配置正确的服务器将输出源代码的彩色格式版本，而不是通常生成的HTML。并非所有服务器都是这样配置的。**

(简单地说就是一个寄存php源码的文件)



点进去出现了php源码

```php
<?php
if("admin"===$_GET[id]) {     //“===” 严格比较
  echo("<p>not allowed!</p>");
  exit();
}

$_GET[id] = urldecode($_GET[id]);
if($_GET[id] == "admin")       // “==” 松散比较
{
  echo "<p>Access granted!</p>";
  echo "<p>Key: xxxxxxx </p>";
}
?>

Can you anthenticate to this website?
```

## urlencode()

此函数可以将文本url编码:

编码规则如下：

​		1.和字母不变。

　　2.空格变为"+"号。

　　3.其他被编码成"%"加上他们的ascii的十六进制，

如：'a' -> %61

## urldecode()

上面php源码中出现的urldecode()就是将url编码后的串按上述规则解码：

那么就简单了，上次讲过php弱类型比较。分析完，只需让：

**$id与"admin"严格比较为假**

**urldecode($id)与"admin"松散比较为真**。

这里取**%61dmin**即符合要求（因为urldecode("%61") === a)

但是：

![image-20200530163025119](image-20200530163025119.png)

这是为什么？

呃呃，查了资料过后发现：	**网页在解析变量时会自动把变量url解码一次。即：我们传入的id=%61dmin已经先被网页urldecode()了一次。这样进入php代码中的id又变成了admin.**



所以我们需要将**%61**再次urlencode()得**%2561**

再将**id=%2561dmin**传入即可得到

**key=cyberpeace{74a5153582004f530a70598cc30a3ce4}**

