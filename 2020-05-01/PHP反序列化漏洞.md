# PHP反序列化漏洞

## 1.基础

PHP 程序为了保存和转储对象，提供了序列化的方法，php序列化是为了在程序运行的过程中对对象进行转储而产生的。序列化可以将对象转换成字符串，但仅对保留对象里的成员变量，不保留函数方法。

* php序列化的函数：serialize
* php反序列化函数：unserialize

## 2.序列化

~~~php
<?php
     class Ctf {
         public $flag='flag{****}';
         public $name='cxk';
         public $age='18';
     }
     $ctfer = new Ctf();
     $ctfer->flag = 'flag{adad}';
     $ctfer->name = 'zesiar0';
     $ctfer->age = 	'19';
     echo serialize($ctfer);
?>     
~~~

输出：

![1](https://cdn.jsdelivr.net/gh/zesiar0/photo//images/20200428170603.png)

> O：代表是对象；
>
> 3：代表对象有3个字符；
>
> Ctf：代表对象的名称；
>
> 3：代表对象中有三个成员；
>
> 第一个变量序列化后为s:4:"flag";s:10:"flag{adad}"
>
> 其中分号左边代表的是变量的名称，右边代表的是变量的值

**注意：**

在PHP中会对类中不同属性的变量进行不同的修饰

1. public 属性：如上图所示
2. protected 属性：在变量名前加上%00*%00 （\0?）
3. private 属性：在变量名前加上%00类名%00

## 3.反序列化

```php
<?php
class Test{
    public $a='thisA';
    protected $b='thisB';
    private $c='thisC';
}
$test = new Test();
$test1 = serialize($test);
$test2 = unserialize($test1);
echo $test1."<br>";
var_dump($test2);
?>
```

运行结果：

<img src="https://cdn.jsdelivr.net/gh/zesiar0/photo//images/20200428173653.png" alt="3" style="zoom:150%;" />

可以看到成员变量都被还原了

## 3.魔术方法

> _construct 当一个对象创建时被调用
>
> _destruct 当一个对象销毁时被调用
>
> _toString 当一个对象被当作一个字符串使用
>
> _sleep 在对象被序列化之前运行
>
> _wakeup 在对象被反序列化之后被调用

### 实例：

test2.php的代码

```php
<?php
    class Test{
        public $a;
        public $b;
        public $c;
        function __construct()
        {
            $this->a="a";
            $this->b="b";
            $this->c="c";
        }
        function __wakeup()
        {
            echo "<br>".$this->a."<br>".$this->b."<br>".$this->c;
        }
        function __sleep()
        {
            echo $this->a."<br>".$this->b."<br>".$this->c."<br>";
        }
    }
    $test = new Test();
    $test1 = serialize($test);
?>
```

ctf.php的代码

```php
<?php
    include 'test2.php';
    class Ctf{
        public $flag = 'flag{****}';
        protected $name = 'cxk';
        private $age = '18';
    }
    $ctfer = new Ctf();
    echo serialize($ctfer);
    $test2 = unserialize('O:4:"Test":3:{s:1:"a";s:5:"thisa";s:1:"b";s:5:"thisb";s:1:"c";s:5:"thisc";}');
?>    
```

运行结果：

![QQ图片20200428233127](https://cdn.jsdelivr.net/gh/zesiar0/photo//images/20200428233135.png)

可以看出在test2.php中先创建了一个对象，然后调用了__construct()函数，之后有序列化函数，所以先调用了

__sleep()函数，然后对$test进行序列化操作。而在ctf.php中先创建了一个$ctfer对象，再对$ctfer进行序列化操作然后输出，这里可以明显的看出public,protected和private属性的不同。之后又有反序列化函数，所以调用了

__wakeup()函数，返回值为成员变量的新值。

### __wakeup()函数的绕过

> $test2 = unserialize('O:4:"Test":**4**:{s:1:"a";s:5:"thisa";s:1:"b";s:5:"thisb";s:1:"c";s:5:"thisc";}');

这里就要用到CVE-2016-7124漏洞，**当序列化字符串中表示对象属性个数的值大于真实的属性个数时会跳过__wakeup的执行**

![QQ图片20200428235747](https://cdn.jsdelivr.net/gh/zesiar0/photo//images/20200429000220.png)

这里__wakeup()函数就被成功跳过了