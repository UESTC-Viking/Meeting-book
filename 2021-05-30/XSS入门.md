# XSS入门

******

## 同源策略：

**同域名、同端口、同协议。限制了Cookie的作用域。**

eg: http: // www . xyz.com : 8080 / abc.js

<u>协议 子域名 主域名 端口</u> 资源地址

**原理：A网站只能访问A的Cookie，B网站只能访问B的Cookie，浏览器根据不同的网站自动填充不同的Cookie。**

- 浏览器的同源策略，限制了不同源的JS，对当前页面的资源和属性的权限。同源策略保护了同域名下的资源不被其他网页的脚本读取或篡改。

******

## XSS类型：

1. 反射型
2. 存储型
3. DOM型



eg：

```php+HTML
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>XSS原理重现</title>
</head>
<body>
<form action="" method="get">
<input type="text" name="xss_input">
<input type="submit">
</form>
<hr>
<?php
    $xss = $_GET['xss_input'];
    $test = "你输入的字符是{$xss}";
    //$test = "<input type='text' name='inputname' readonly='readonly' value='你输入的字符是{$xss}' />";
    echo $test;
?>
</body>
</html>
```

攻击代码

```
<script>alert(1)</script>
标签：<a href=javascript:alert(1) />1</a>
事件：<img src=# onerror=alert(1) />
```



## DOM XSS

### 1.什么是Dom Based XSS

DOM是一个JS可以操纵浏览器和网页显示内容的接口。

### 2.Dom Based XSS

Document对象使我们可以从脚本中对HTML页面中的所有元素进行访问

2.1）Document对象属性

1. document.body ：对<body>元素直接访问
2. document.cookie : 获取cookie值。
3. ducument.domain ：返回域名
4. document.lastModified：返回文档被最后修改的容器和时间 。可以用来判断伪静态。时间变了就动态，时间不变就静态。
5. ducument.referrer：返回载入文档的文档URL
6. ducument.title：返回文档标题
7. ducument.URL ：返回文档URL
8. document.write ：输入的内容会输出到文档上。Eg: document.write(‘<script>alert(1) </script>‘)

注意：前端代码是在客户的浏览器里面执行的，不占用服务器资源。

2.2）三种页面的区别

1. 静态页面 ：无动态语言，也不存在SQL注入。
2. 伪静态页面 => 就是动态页面
3. 动态页面

2.3）dom型XSS（经过JS处理后触发的xss都可以认为dom型XSS）

1. eval // 高危函数，字符串被当做代码输出

### 3.DOM型XSS靶场演示

1.<iframe onload=alert(1) // >

2.document.write() // 支持js编码



## 字符过滤和绕过

xss-labs靶场

[XSS绕过](https://mp.weixin.qq.com/s?src=11&timestamp=1622219310&ver=3096&signature=XPrGQqUpaVuwlYp4HCvbpAHIpUp6rQh6LlOQV5smfxA72v9iNXzKFJoSlFLja35NLR3PNoEAgxD*r2bP-KQ-rupudr4A-eDYvUB1WmzYCy4COLUyKoeiKID0KDleb2YH&new=1)



## XSS平台使用实例

