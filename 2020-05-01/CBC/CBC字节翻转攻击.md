# CBC字节翻转攻击

#### 相关介绍

此攻击方法的精髓在于：**通过损坏密文字节来改变明文字节**。

加密过程：

![](.\016921e74bbeb0584b7050ea0323d6db4023ac4a.jpg)

**Plaintext**：待加密的数据。

**IV**：用于随机化加密的比特块，保证即使对相同明文多次加密，也可以得到不同的密文。

**Key**：被一些如AES的对称加密算法使用。

**Ciphertext**：加密后的数据。

在这里重要的一点是，CBC工作于一个固定长度的比特组，将其称之为*块*。

所以我们可以精简成公式：

- *Plaintext-0 = Decrypt(Ciphertext) XOR IV*—只用于第一个组块

- *Plaintext-N= Decrypt(Ciphertext) XOR Ciphertext-N-1*—用于第二及剩下的组块

  

  **Ciphertext-N-1\*（密文-N-1）是用来产生下一块明文***

  解密：

  ![](CBC%E5%AD%97%E8%8A%82%E7%BF%BB%E8%BD%AC%E6%94%BB%E5%87%BB/b597d6474d286cb7f67d4f0df7ba85ba024c9025.jpg)

  

  ![](CBC%E5%AD%97%E8%8A%82%E7%BF%BB%E8%BD%AC%E6%94%BB%E5%87%BB/157b4078de6a8615e7c4be958d4d89164f1810e9.jpg)

  #### 解密第一组时:

  设明文为x 密文为y 解密密钥k

  ``X[1]=k(Y[1]) Xor IV``

  对于``X[i]``的解密时，``X[i] = k(Y[i]) Xor Y[i-1]，k(Y[i])``部分是无法控制的，假如修改``Y[i]``的值，是无法确定``k(Y[i])``的值，由于最后是异或操作，因此可以仅修改``Y[i-1]``的内容为``Y’[i-1]``来控制最后的明文的值，设解密后的内容为``M[i]=k(Y[i]) Xor Y[i-1]。``
  
  将``Y[i-1]``的值设置为``Y[i-1] Xor M[i]``的值，新的``Y[i-1]``的值用``Y’[i-1]``表示。

  那么``X[i] = k(Y[i]) Xor Y’[i-1]=k(Y[i]) Xor Y[i-1] Xor M[i] = M[i] Xor M[i] = 0``

  这样就能将只修改``Y[i-1]``的内容来控制``X[i]``的值
  
  而此时``X[i-1]``的值肯定就会出错了，设修改``Y[i-1]``的值，导致解密后``X[i-1]``的值为``M[i-1]``，那么将``Y[i-2]``的值改为``Y[i-2]=Y[i-2] Xor M[i-1] Xor ``任意值，可以使得``X[i-1]``=任意值
  
  这样循环往前，最后一组就是根据M[1]的值修改``IV=IV Xor M[1] Xor`` 任意值，使得X[1]=任意值

  

  #### 举个例子：

  比方说，我们有这样的明文序列：
  
  ```
a:2:{s:4:"name";s:6:"sdsdsd";s:8:"greeting";s:20:"echo 'Hello sdsdsd!'";}
  ```

  我们的目标是将“`s:6`”当中的数字6转换成数字“7”。我们需要做的第一件事就是把明文分成16个字节的块：
  
  - Block 1:`a:2:{s:4:"name";`
  - Block 2:`s:6:"sdsdsd";s:8`
  - Block 3:`:"greeting";s:20`
  - Block 4:`:"echo 'Hello sd`
  - Block 5:`sdsd!'";}`

因此，我们的目标字符位于块2，这意味着我们需要改变块1的密文来改变第二块的明文。

有一条经验法则是（注：结合上面的说明图可以得到），你在密文中改变的字节，**只**会影响到在下一明文当中，具有相同偏移量的字节。所以我们目标的偏移量是2：

- [0] = s
  - [1](http://drops.xmd5.com/uploads/2015/08/115.png) = :
  - [2](http://drops.xmd5.com/uploads/2015/08/216.png) =6
  

因此我们要改变在第一个密文块当中，偏移量是2的字节。正如你在下面的代码当中看到的，在第2行我们得到了整个数据的密文，然后在第3行中，我们改变块1中偏移量为2的字节，最后我们再调用解密函数。

1. `$v = "a:2:{s:4:"name";s:6:"sdsdsd";s:8:"greeting";s:20:"echo 'Hello sdsdsd!'";}";`
  2. `$enc = @encrypt($v);`
  3. `$enc[2] = chr(ord($enc[2]) ^ ord("6") ^ ord ("7"));`
  4. `$b = @decrypt($enc);`

  运行这段代码后，我们可以将数字6变为7：

  

  但在第三行我们是如何改变字节成为我们想要的值的呢？

  基于上述的解密过程，我们知道有，*A = Decrypt(Ciphertext)*与*B = Ciphertext-N-1*异或后最终得到*C = 6*。等价于：

  ```
  C = A XOR B
  ```

  所以，我们唯一不知道的值就是A（注：对于B，C来说）（*block cipher decryption*）;借由XOR，我们可以很轻易地得到A的值：

  ```
  A = B XOR C
  ```

  最后，A XOR B XOR C等于0。有了这个公式，我们可以在XOR运算的末尾处设置我们自己的值，就像这样：

  `A XOR B XOR C XOR "7"`会在块2的明文当中，偏移量为2的字节处得到7。

  

  ```php
  #!php
  define('MY_AES_KEY', "abcdef0123456789");
  function aes($data, $encrypt) {
      $aes = mcrypt_module_open(MCRYPT_RIJNDAEL_128, '', MCRYPT_MODE_CBC, '');
      $iv = "1234567891234567";
      mcrypt_generic_init($aes, MY_AES_KEY, $iv);
      return $encrypt ? mcrypt_generic($aes,$data) : mdecrypt_generic($aes,$data);
  }
  
  define('MY_MAC_LEN', 40);

  function encrypt($data) {
    return aes($data, true);
  }
  
  function decrypt($data) {
      $data = rtrim(aes($data, false), "\0");
      return $data;
  }
  $v = "a:2:{s:4:\"name\";s:6:\"sdsdsd\";s:8:\"greeting\";s:20:\"echo 'Hello sdsdsd!'\";}";
  echo "Plaintext before attack: $v\n";
  $b = array();
  $enc = array();
  $enc = @encrypt($v);
  $enc[2] =  chr(ord($enc[2]) ^ ord("6") ^ ord ("7"));
  $b = @decrypt($enc);
  echo "Plaintext AFTER attack : $b\n";
  ```

  #### 实际应用（题目）：

  ```php
  <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
  <html>
  <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Login Form</title>
  </head>
  
  <?php
  define("SECRET_KEY", 'kH3LH3sk45HLsd3n');
  define("METHOD", "aes-128-cbc");
  session_start();
  
  function get_random_iv(){
      $random_iv='';
      for($i=0;$i<16;$i++){
          $random_iv.=chr(rand(1,255));
      }
      return $random_iv;
  }
  
  function login($info){
      $iv = get_random_iv();
      $plain = serialize($info);
      $cipher = openssl_encrypt($plain, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $iv);
      $_SESSION['username'] = $info['username'];
      setcookie("iv", base64_encode($iv));
      setcookie("cipher", base64_encode($cipher));
  }
  
  function check_login(){
      if(isset($_COOKIE['cipher']) && isset($_COOKIE['iv'])){
          $cipher = base64_decode($_COOKIE['cipher']);
          $iv = base64_decode($_COOKIE["iv"]);
          if($plain = openssl_decrypt($cipher, METHOD, SECRET_KEY, OPENSSL_RAW_DATA, $iv)){
              $info = unserialize($plain) or die("<p>base64_decode('".base64_encode($plain)."') can't unserialize</p>");
              $_SESSION['username'] = $info['username'];
          }else{
              die("ERROR!");
          }
      }
  }
  
  function show_homepage(){
      if ($_SESSION["username"]==='admin'){
          echo '<p>Hello admin</p>';
          echo '<p>Flag is SKCTF{CBC_wEB_cryptography_6646dfgdg6}</p>';
      }else{
          echo '<p>hello '.$_SESSION['username'].'</p>';
          echo '<p>Only admin can see flag</p>';
      }
      echo '<p><a href="loginout.php">Log out</a></p>';
  }
  
  if(isset($_POST['username']) && isset($_POST['password'])){
      $username = (string)$_POST['username'];
      $password = (string)$_POST['password'];
      if($username === 'admin'){
          exit('<p>admin are not allowed to login</p>');
      }else{
          $info = array('username'=>$username,'password'=>$password);
          login($info);
          show_homepage();
      }
  }else{
      if(isset($_SESSION["username"])){
          check_login();
          show_homepage();
      }else{
          echo '<body class="login-body">
                  <div id="wrapper">
                      <div class="user-icon"></div>
                      <div class="pass-icon"></div>
                      <form name="login-form" class="login-form" action="" method="post">
                          <div class="header">
                          <h1>Login Form</h1>
                          <span>Fill out the form below to login to my super awesome imaginary control panel.</span>
                          </div>
                          <div class="content">
                          <input name="username" type="text" class="input username" value="Username" onfocus="this.value=\'\'" />
                          <input name="password" type="password" class="input password" value="Password" onfocus="this.value=\'\'" />
                          </div>
                          <div class="footer">
                          <input type="submit" name="submit" value="Login" class="button" />
                          </div>
                      </form>
                  </div>
              </body>';
      }
  }
  ?>
  </html>
  ```

  

```python
import urllib,base64,requests,re

url = "http://127.0.0.1/index.php"
datas = {
    "username" : "Admin",
    "password" : "admin"
}

r = requests.post(url,data=datas)
cipher = r.cookies.get("cipher")
cipher = base64.b64decode(urllib.unquote(cipher))
offset = 9
new_cipher = cipher[:offset] + chr(ord(cipher[offset])^ord("A")^ord("a")) + cipher[offset+1:]
new_cookies = requests.utils.dict_from_cookiejar(r.cookies)
new_cookies["cipher"] = urllib.quote_plus(base64.b64encode(new_cipher))

r2 = requests.get(url,cookies=new_cookies)
plain = base64.b64decode(re.findall("decode('(.*)')",r2.text)[0])
iv = base64.b64decode(urllib.unquote(new_cookies["iv"]))
old = plain[:len(iv)]
new = 'a:2:{s:8:"userna'
new_iv = "".join([chr(ord(iv[i])^ord(old[i])^ord(new[i])) for i in xrange(16)])
new_cookies["iv"] = urllib.quote_plus(base64.b64encode(new_iv))

r3 = requests.get(url,cookies=new_cookies)
print(r3.text)
```

> 需要注意的是，第一个加密的会因为修改之后而变化。

paddign oracle attack:

https://blog.csdn.net/qq_19876131/article/details/52674589