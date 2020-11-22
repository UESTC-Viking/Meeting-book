# JSFXXK

**这是什么?**

```
[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]][([][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]])[+!+[]+[+[]]]+([][[]]+[])[+!+[]]+(![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+[]]+([][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]])[+!+[]+[+[]]]+(!![]+[])[+!+[]]]((![]+[])[+!+[]]+(![]+[])[!+[]+!+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]+(!![]+[])[+[]]+(![]+[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]])[!+[]+!+[]+[+[]]]+[+!+[]]+(!![]+[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+!+[]]])[!+[]+!+[]+[+[]]])()
```



## []==![] //?

`[]==![]`这个表达式乍一看结果应该是`false`但是实际上确是`true`

而对于

```javascript
Boolean([]) //true
Boolean(![]) //false
//这段代码是做布尔型转换
```

**why？**

原来是，js在对对象做布尔型转换时会默认将对象转换称`true`

这里把`[]`看做一个对象则`![]`的布尔转换结果则为`false`

**but**

在`==`运算时规则却不一样，相等运算符如果有一边是数字，而另一边是对象，则会把对象转换成数字。（即`true->1` ，`false->0`）

所以在`[] == ![]`中`![] == 0` ，而`[]`作为空数组它的数字结果也是`0`所以得到`[] == ![] // true`

## 拓展

由上面的基础，我们可以知道：

- `false = ![]; true = !![] `
- `undefined = [][[]]`
  - 为什么呢，因为在js中数组的索引可以是字符串，在这种情况下，右边`[]`里的`[]`被视为字符串，而`[]`转换为字符串为空字符串`''`，那么此式就相当于`[]['']`显然我们的`[]`并没有`''`属性，故为`undefined`
- `NaN = +[![]]`
  - `NaN`是js的一个全局对象，(NaN 属性是代表非数字值的特殊值。该属性用于指示某个值不是数字。)
  - 这里的`+`并不是拼接的意思，而是用于把其后的操作数转换为数字，并且返回这个数字，这里会得到NaN(这里我也不是很清楚)
- `0 = +[]`
- `1 = +!+[]`

- `10 = [+!+[]]+[+[]]`
- `[]`还可用来获取Array字符串
- `eval = []['filter']['constructor']()(code)`
  - `eval()`函数是一个能将括号里的字符串视作源码的函数
  - 这里拿的是一个数组对象的`filter`属性(这是一个方法)下面的`constructor`属性,在js中任何方法都是`Function`对象的一个实例，`filter`是一个方法它的`constructor`就是`Function`构造函数本身

## 如何使用

我们要想通过 `()` `[]`和`!`的加密方式来执行一段js代码就需要构造出`Function()(code)`这样的格式

刚刚提到，我们可以用`[]["filter"]["constructor"]()(code) `获得这样的格式，所以只需加密字符串。

由

- `1 = +!+[]`

- `false = ![] `

- `true = !![] `

可拓展出

```javascript
'a' == 'false'[1] == (false + '')[1] == (![]+[])[+!+[]]
  
'l' == 'false'[2] == (false + '')[2] == (![]+[])[!+[]+!+[]]  

'e' == 'true'[0] == (true + '')[3] == (!![]+[])[!+[]+!+[]+!+[]]
  
'r' == 'true'[0] == (true + '')[1] == (!![]+[])[+!+[]]
 
't' == 'true'[0] == (true + '')[0] == (!![]+[])[+[]]
```

这样我们就得到了`alert`

**那么我们如何得到括号字符呢？**

方法的基本样式就是function name (){ code }这样我们只需要把它转换成字符串，按照上面的方法实现一遍就行了

jsfuck官方给出的方法

`([][[]]+[][(![]+[])[+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]]+(![]+[])[!+[]+!+[]]])[!+[]+!+[]+[!+[]+!+[]]]`

这样我们就可以通过`[],!,+`做任何想做的事情了！

参考：https://blog.csdn.net/qq_36539075/article/details/102463521
