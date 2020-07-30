# SSTI模板注入

## SSTI介绍

​	`SSTI`主要为python的一些框架 `jinja2 mako tornado django，PHP`框架`smarty twig，java`框架jade velocity等等使用了渲染函数，这些函数对用户的输入信任，造成了模板注入漏洞，可以造成文件泄露，rce等漏洞。

## 漏洞原理

这是一个安全的代码：

```python
#/www
from flask import Flask,request,render_template
from jinja2 import Template
app = Flask(__name__)
app.config['SECRET'] = "root:password"
 
@app.route('/')
@app.route('/index')
def index():
    return render_template("index.html",title='SSTI_TEST',name=request.args.get("name"))
 
if __name__ == "__main__":
    app.run()
```

```html
<!--/www/templates/index.html-->
<html>
  <head>
    <title>{{title}} - cl4y</title>
  </head>
 <body>
      <h1>Hello, {{name}} !</h1>
  </body>
</html>

```

我们在`index.html`里面构造了两个渲染模板，用户通过传递`name`参数可以控制回显的内容。

![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588228856232.png)

即使用户输入渲染模板，更改语法结构，也不会造成`	SSTI`注入。

![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588228925579.png)

原因是：服务端先将`index.html`渲染，然后读取用户输入的参数，模板其实已经固定，用户的输入不会更改模板的语法结构。

但是如果代码如下这种：

```python
from flask import Flask,request
from jinja2 import Template
app = Flask(__name__)
app.config['SECRET_KEY'] = "password:123456789"
@app.route("/")
def index():
    name = request.args.get('name', 'guest')
    t = Template('''
<html>
  <head>
    <title>SSTI_TEST - cl4y</title>
  </head>
 <body>
      <h1>Hello, %s !</h1>
  </body>
</html>
                '''% (name))
    return t.render()
if __name__ == "__main__":
    app.run()

```

我们再进行测试：

![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588229245911.png)

可以看到，我们输入的内容被服务器渲染然后输出，形成`SSTI`模板注入漏洞。

## 基础知识

* \_\_class\_\_ 

  万物皆对象，而`class`用于返回该对象所属的类，比如某个字符串，他的对象为字符串对象，而其所属的类为\<class 'str'>

  ![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588245529180.png)

* \_\_bases\_\_

  以元组的形式返回一个类所直接继承的类。

* \_\_base\_\_

  以字符串的形式返回一个类所直接继承的第一个类。

* \_\_mro\_\_

  返回解析方法调用的顺序

  ![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588246346346.png)

  可以看到\_\_bases\_\_返回了`test()`的两个父类，\_\_base\_\_返回了`test()`的第一个父类，\_\_mro\_\_按照子类到父类到父父类解析的顺序返回所有类。

  * \_\_subclasses()\_\_

    获取类的所有子类。

  ![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588246555959.png)

* \_\_init\_\_

  所有自带类都包含init()方法。

* \_\_globals\_\_

  function.\_\_globals\_\_，用于获取function所处空间下可使用的模块、方法以及所有变量。

  ![file](http://www.cl4y.top/wp-content/uploads/2020/04/image-1588247279196.png)

## payload

### 注入思路

* 随便找一个内置类对象用\_\_class\_\_拿到他所对应的类
* 用\_\_bases\_\_拿到基类(\<class 'object'>) 
* 用\_\_subclasses\_\_()拿到子类列表 
* 在子类列表中直接寻找可以利用的类getshell

```python
''.__class__.__bases__[0].__subclasses__()

().__class__.__mro__[2].__subclasses__()

request.__class__.__mro__[1]

```

### 找可利用的类

在利用该漏洞的时候只要找到可以执行代码的函数或者其他读文件的函数都可以。

就比如说我们找到了`file`类，而我们可以通过这个类对文件进行读写操作，例如`''.__class__.__mro__[2].__subclasses__()[40]('/etc/passwd').read() }}`。

所以方法不止一种，找到对的继承链就可以了。



















