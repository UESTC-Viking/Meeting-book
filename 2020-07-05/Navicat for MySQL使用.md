## Navicat for MySQL使用

### mysql安装与配置

参考文档：https://zhuanlan.zhihu.com/p/46069784

注：当在配置时，MySQL服务器无法通过命令行启动时，可在电脑的输入栏输入搜索services.msc，找到MySQL，并启动。

### Navicat for MySQL的使用

1. 创建连接：点击连接  ->  输入连接名  ->  确认主机名或IP地址  ->  输入数据库的用户名和密码(用户名一般默认为root)

   <img src="C:\Users\20180\AppData\Roaming\Typora\typora-user-images\image-20200705201913524.png" alt="image-20200705201913524" style="zoom: 67%;" />

2. 创造数据库：右键打开所创连接  ->  右键选择新建数据库  ->  输入数据库名称(字符集、排序规则一般默认)，然后点击确认即可

   <img src="C:\Users\20180\AppData\Roaming\Typora\typora-user-images\image-20200705202336276.png" alt="image-20200705202336276" style="zoom:67%;" />

3. 创建表：右键打开数据库  ->  选择新建表  ->  根据图形化界面先输入列名  ->  保存并输入表名

4. 然后根据图形化（或ER图）界面完成查询，创建外键、视图、函数等约束。

### 利用Navicat for MySQL实现本地数据库上传到云服务器中

1. 利用Navicat for MySQL将本地数据库导出为SQL文件备用
2. 将导出的SQL文件上传到服务器
3. 在服务器中创建对应的数据库
4. 运行上传的SQL文件