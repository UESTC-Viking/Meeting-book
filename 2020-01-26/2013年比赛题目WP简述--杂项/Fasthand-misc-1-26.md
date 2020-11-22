# BackdoorCtf

## forensic

### 150

> H4x0r decided he needs one more extra layer of advanced security. His access key is a QR code, and keeps the only copy in his wallet. But when leaving his castle he fell into the moat and the QR code got wet and disfigured. Now he cant get back into his own castle and desperately needs a real hacker's help. His post on craigslist says so. This is your chance to get into his head. 
Here is the disfigured QR code:\-
The Flag is md5 of last 13 characters of password hidden in [the QR](wet_QR.png).
H4x0r promises to reward 150 h4x0rcoins anyone who gives him the password.
HINT: Hmmm... Can't I just try and reverse the fading somehow?


![](wet_QR.png)
  题目就是一个经过处理的二维码。

  但是没有WP解答。

  经过自己的加工（位图处理，区域反向，扫码）



![](image.png)


![](image.png)
加密结果，这就是flag





### 250



> h4x0r is fed up with all your attacks on his castle and has decided to retaliate by attacking us back. He sent us a mail with [this image](bbb.png) as a warning. Hope you can figure out his warning message:\-
MD5 of the message is the answer.


![](bbb.png)
使用stegsolve查看，发现一个异常图片



![](solved.bmp)


使用python脚本提取低位：

```python
from PIL import Image
flag_img = Image.open("solved.bmp")
w,h = flag_img.size
msg=''
for x in range(0, flag_img.size[1]):
    g,b,a = flag_img.getpixel((x,0))
    if g==255: msg +='0'
    else: msg+='1'
print (msg)
```
![](image.png)


将生成的ASCII码转化成16进制的数据，并且复制黏贴入winhex当中读出数据



> 0000000: 6d6f 6465 726e 7761 7266 6172 6500 0000  modernwarfare...

> 0000010: 000a

将message用MD5进行转化得到falg



### 300



> H4x0r has now learnt that simple text authentications are not the in thing today. Also since voice authentication is all the rage nowadays thanx to siri, he decided to get one for his castle. But he messed it big time and now has a audio file which he himself can't seem to decipher. So now he needs your help to get into his own castle. 
You can find the file [here](siri_audio.zip).
Find the hidden passphrase, the flag is the MD5 of the passphrase.
HINT: It is a audioWAVE. Sometimes you might have to see things instead of hearing them.


这是一道音频隐写题，使用adacity查看以后发现其波形图是有规律的。



![](image.png)
并且是每8个组合为一个小单位一个完整的正弦函数，余弦函数为一个0或1.

> 011000100110000101101011011001000110111101110010

同上转成16进制，翻译出其表达的文字意思，再转成MD5即是flag。






## bin

略
















# csaw\-quals\-2013

## Misc

### black\-and\-white\-100

毫无难度，直接StegSolve一套服务得出flag



![](image.png)




### deeeeeeaaaaaadbeeeeeeeeeef\-200



![](IMG_0707.png)
#### png的文件头：

* （固定）八个字节89 50 4E 47 0D 0A 1A 0A为png的文件头
* （固定）四个字节00 00 00 0D（即为十进制的13）代表数据块的长度为13
* （固定）四个字节49 48 44 52（即为ASCII码的IHDR）是文件头数据块的标示（IDCH）
* （可变）13位数据块（IHDR\)

   * 前四个字节代表该图片的宽

   * 后四个字节代表该图片的高

   * 后五个字节依次为：

   Bit depth、ColorType、Compression method、Filter method、Interlace method
* （可变）剩余四字节为该png的CRC检验码，由从IDCH到IHDR的十七位字节进行crc计算得到。
也就是说我们可以通过爆破来得到高度和宽度



这是一张照片，盲猜与图片大小有关。

查看其16进制格式。



![](image.png)
是Iphone5拍摄的，而且图片大小是3264\(宽\)x1681（长）。



![](image.png)


![](image.png)
得到flag。



## life\-300

nc题但是远程端口已经关闭，无法做题。



## network1

直接打开流量包，查找分析即可


## network2

和1一样。。










# hack\-lu\-ctf\-2013

## Misc

### Beer Pump Filtration

喝喝啤酒？？



> Our agents observed that the famous robot Bender is part of the robot forces. Somehow he looked pregnant but his big belly is now gone. We fear that they might have smuggled some of Benders freshly brewed BenderBräu into our beer supplies. This is why we need brave women and men to test whether BenderBräu can cause severe damage on the human body or not, to avoid poisoning the whole party. But you have to test quick\! Everyone is thirsty\!
For testing you need to drink 0.5 liter of beer, preferably wheat beer, as fast as you can.
Make a video of your brave tasting and hand it in. The video must contain a proof of the current date. Show to us the totally secure and random Nonce "17" as well as your teamname written on... something. Also show us the closed beer bottle before, the empty after the tasting, and prove that it is 0.5 liters.
We will judge the score as follows:
Score = 100 Points \- Seconds it takes to drink the beer
Bonuspoints for:
Girls \(Due to the law for gender equality we score women higher for the same effort\)
Dressing up like a robot or an Oktoberfest maid in her Dirndl
Robo Dance
Drinking Weizenbier \(wheat beer\)



### For whom the bell tolls

连接端口，挂了。





### Geolocation Flag



> https://ctf.fluxfingers.net/2013/challenges/24
This challenge is a special challenge. You can collect some minor extra points here by proving that you are a truly international player. Each time you visit your reference URL from a different country, that flag will be activated and you gain an additional point. You already have 0/222 points.
This challenge cannot be solved in archive mode.
Disclaimer: Please do not attempt to hack real\-world systems for a single point. That is illegal and we assure you it is not worth a single point\!




链接给挂了。。



### OTP


> Some robots are on the Oktoberfest and want to take some tasty oil in a tent. But they hadn't reserved a table and all tents are full. No one gets access. They found a back entrance and managed to spy the credentials while an employee enters. They captured the username “admin” and password “supersafepw”. But the employee also entered a strange number \(168335\). As they were sure nobody's looking, they tried the captured data to get in the tent, but it didn't work. Help the robots to get their tasty tasty oil. \(Or they have to build their own tent with blackjack and hookers\)
Here is your challenge: https://ctf.fluxfingers.net:1318


链接挂了




### Roboparty



> Robot LHCH is happy. He made it into the castings for the tenth roman musical. He even is so happy that he went on the Oktoberfest to drink some beer. Unfortunately it seems that he drank too much so now he is throwing up part of his source code. Can you decipher the secret he knows?
Warning: Viewing this page is not recommended for people that suffer from epilepsy. We are dead serious.
And here is your totally eye\-friendly challenge: https://ctf.fluxfingers.net/static/downloads/roboparty/index.html
Hints:
The flag starts with 'Y4Y,'
It's neither Velato nor Fugue.


挂








# pico\-ctf\-2013

## misc

### black\-hole



> Near the galactic core, you find a beacon indicating that an ancient civilization hid one of their worlds [within a nearby black hole](https://2013.picoctf.com/problems/blackhole.img). Is this what passes for intergalactic humor, or is there actually something in there?
The disk image can be found on the shell machines at `/problems/blackhole.img` and the contents of the image are available in `/problems/blackhole/`
[Offline file](blackhole.img)


题目中只给了一个img文件，还给出了其中的路径。

fdisk查看磁盘位置。



![](image.png)


![](image.png)


和网上资料不太一样啊QAQ



![](image.png)
不管了，不设置相关偏移还是可以成功挂载。



之后提取图片。



后面的确想不出，提出了一个需要异或的操作，提取出一个图片



At the end of the file, the string “EVENT HORIZON EVENT HORIZON” is repeated a few times… strange. 

And at the beginning of the file, we see traces of the event horizon as well:

```
\blackhole # hexdump -C masked_key.png | head -n 4
00000000  76 88 b1 a7 0d 1a 50 4c  49 46 00 48 1f 0d 0a 06  |v.....PLIF.H....|
00000010  48 4f 50 a7 5a 4f 4c 78  4d 50 45 4e 54 75 c1 31  |HOP.ZOLxMPENTu.1|
00000020  eb 5a 4f 4e 24 36 14 0c  1a 5c 40 47 5a 35 52 2b  |.ZON$6...\@GZ5R+|
00000030  c6 20 45 76 45 07 10 09  1b 2a d5 0e f3 97 50 bc  |. EvE....*....P.|
```
Very strange. We have reason to believe that this is a PNG file, 

so on a hunch I decided to XOR the first few bytes of the file with the PNG header.

```
Masked:
76 88 B1 A7 0D 1A 50 4C
PNG:
89 50 4E 47 0D 0A 1A 0A
XOR:
FF D8 FF E0 00 10 4A 46
```
This sequence exists in the file as the last 64 bytes.

```
000effc0  ff d8 ff e0 00 10 4a 46  49 46 00 45 56 45 4e 54  |......JFIF.EVENT|
000effd0  48 4f 52 49 5a 4f 4e 20  45 56 45 4e 54 48 4f 52  |HORIZON EVENTHOR|
000effe0  49 5a 4f 4e 20 45 56 45  4e 54 48 4f 52 49 5a 4f  |IZON EVENTHORIZO|
000efff0  4e 20 45 56 45 4e 54 48  4f 52 49 5a 4f 4e ff d9  |N EVENTHORIZON..|
```


```
Python 3.2.3 (default, Oct 24 2012, 19:34:37)
[GCC 4.5.4] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> mask_in = open('masked_key.png','rb')
>>> key = bytearray(mask_in.read(983040-64))
>>> mask= bytearray(mask_in.read(64))
>>> for x in range(983040-64):
...     key[x] ^= mask[x%64]
...
>>> key_out = open('unmasked_key.png','wb')
>>> key_out.write(key)
982976
>>> key_out.close()
>>>
```
![](solve.png)


### Chromatophoria

> While refueling at a gas giant, you are hailed by a race of cuttlefish\-people who dwell within it. [Their transmission](https://2013.picoctf.com/problems/steg.png) is entirely visual; you suspect that they may be communicating through the color values.
[Offline file](steg.png)


![](steg.png)
LSB隐写


### darkstar

又是一个img文件。

需要的flag藏在里面。



![](solve.png)


































































