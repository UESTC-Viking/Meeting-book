# php之serialize

## php serialize()方法(序列化)

* [深度剖析serialize](https://www.cnblogs.com/youyoui/p/8610068.html)
* 可通过序列化表单创建URL编码文本字符串
* 用途
  * 对象字节序列可永久地保存到硬盘
  * 在网络上传送对象的字节序列
* 序列化后格式:
  * Object型(类) —— O:属性名长度:属性名:属性名大小(含多少元素); {元素数据类型: 元素长度: 元素名}
     * String型 —— s: size: value
     * Integer型 —— i: value
     * Boolean 型 —— b: size: value(1或0)
     * Null型 —— N;
     * Array 型 —— a: size: {key definition; value definition; (重复之前的动作)}

### __sleep()方法

* 若序列化对象中存在该魔术方法，则先调用该方法

* 原型:  public array __sleep(void)【可重载该方法】

* 返回： 

  * 包含对象所有应被序列化的变量名称的数组

  * 若无返回内容，则NULL被序列化，并产生一个E_NOTICE级别的错误

  * 不能返回父类private成员名字。只能用Serializable接口代替

  * sleep中未被提及的字段会被省略

    

* 用处：大对象清理工作，或避免保存敏感属性

### 存储

建议保存于本地的文件或缓存中



## php unserialize(反序列化)

### 使用mixed  unserialize(string $str)

* 若传递的字符不可序列化，返回false，产生一个E_NOTICE
* 返回为转换后的值(exp: string, integer, array...)
* 被反序列化的为一个对象，在成功重新构造对象之后，PHP会自动地试图去调用`__wakeup()`成员函数（如果存在的话）
* __wakeup()函数在对象构建后执行
  * 绕过wakeup函数（绕过特性，当输入的参数个数小于你定义的参数个数时）
  * 反序列化是，会尽量将变量进行匹配并复制给序列化后的对象











 