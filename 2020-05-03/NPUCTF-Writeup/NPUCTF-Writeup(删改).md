# 认清形势，建立信心

附件: **task.py**

```python
from Crypto.Util.number import *
from gmpy2 import *
from secret import flag

p = getPrime(25)
e = # Hidden
q = getPrime(25)
n = p * q 
m = bytes_to_long(flag.strip(b"npuctf{").strip(b"}"))

c = pow(m, e, n)
print(c)
print(pow(2, e, n))
print(pow(4, e, n))
print(pow(8, e, n))

'''
169169912654178
128509160179202
518818742414340
358553002064450
'''
```

## 分析与解

考点：简单数论、DLP

### 分析

$$
c = m^e \pmod n
\\
c_1 = 2^e \pmod n
\\
c_2 = 4^e \pmod n
\\
c_3 = 8^e \pmod n
\\
\text{其中，} c, c_1, c_2, c_3 是已知的，其余的均未知。
$$

> 未知参数过少，正是麻烦所在。
>
> 基本思路是分别求出 `n`, `e`，之后就可以对 `c ` 进行解密

### 解析

#### 参数 `n`

$2, 4, 8$, 可以看成 $2^1, 2^2, 2^3$，故 $c_1, c_2, c_3$ 存在如下关系：
$$
c_1^2 = c_2 \pmod n
\\
c_1^3 = c_3 \pmod n
\\
\therefore n \, | \, gcd(c_1^2 - c_2, c_1^3 - c_3)
$$
所以可以得到 $n$

```python
import gmpy2
c1 = 128509160179202
c2 = 518818742414340
c3 = 358553002064450
gcd = gmpy2.gcd(c1**2-c2, c1**3-c3)
```

得到：

```python
gcd = 2 * 18195301 * 28977097
# 猜测&尝试
n = 18195301 * 28977097
```

#### 参数 `e`

$$
c=2^e \pmod {18195301 * 28977097}
$$

这是典型的 DLP（Discrete Logarithm Problem，离散对数问题）

#### Wolfram 爆破

可以用 Wolfram：（应该是用纯暴力枚举的方法，虽然底层是 C 语言，但之前二次剩余跑了十七个小时也没跑出来 :sneezing_face:）

```mathematica
Simplify[
	Reduce[
	  c == Mod[2^k, p] /. {c -> 128509160179202, p -> 527247002021197}, k,Integers]
]
```

得到结果：

```mathematica
C[1] \[Element] Integers && C[1] >= 0 && k == 808723997 + 4393724623740 C[1]
```

得到 `e=808723997`，并在终端进行简单验证：

```python
assert pow(2, 808723997, n) == c1
```


#### 常规解密

```python
# -*- coding: utf-8 -*-
# Python2.7
# Author: ZER0-Nu1L
import gmpy2
from Crypto.Util.number import *
c = 169169912654178
e = 808723997
p = 18195301
q = 28977097
n = q * p
phi = (p-1)*(q-1)
d = gmpy2.invert(e, phi)
m = long_to_bytes(pow(c, d, n))
print m
```

```
npuctf{345y}
```

#### 另解 BSGS 算法

> BSGS（Baby Step Giant Step） 虽然不是 DLP 算法中 时间空间复杂度最低的算法，但相对于 Pollard's $\rho$ 算法（当前认为最优的算法）实现起来容易很多，时间复杂度（$O(\sqrt n)$）相同，所以无论是 ICPC 还是 CTF 都是最优的选择之一。
>

C 语言需要调大整数库，实现起来稍微比较麻烦。

Python 的 BSGS 板子（没有优化）：

```python
def bsgs(g, a, p):
    # To solve g^e mod p = a and find e
    # define x = j*m + i
    res = []
    m = int(ceil(sqrt(p-1)))
    # Baby Step
    lookup_table = {pow(g, i, p): i for i in range(m)}
    # Giant Step Precomputation c = g^(-m) mod p
    c = pow(g, m*(p-2), p)
    # Giant Step
    for j in range(m):
        x = (a*pow(c, j, p)) % p
        if x in lookup_table:
			res.append( j*m + lookup_table[x])
    return res
```
先把原式子拆成：
$$
c=2^e \pmod {18195301 * 28977097} \Rightarrow
\begin{cases}
c=2^{e_1} \pmod {18195301}
\\
c=2^{e_2} \pmod {28977097}
\end{cases}
$$

分别用 BSGS 算法求出指数，然后用中国剩余定理（下面是 [0xDktb 师傅的代码](https://0xdktb.top/2020/04/19/WriteUp-NPUCTF-Crypto/)）

```python
from Crypto.Util.number import *
p = 18195301
q = 28977097
c1_p, c1_q = c1 % p, c1 % q
e_1, e_2 = bsgs(2, c1_p, p), bsgs(2, c1_q, q)
phi = (p - 1) * (q - 1)
e_n = [] # e % n
for e_p in e_1:
    for e_q in e_2:
        try:
            e_n.append(crt([e_p, e_q], [p - 1, q - 1])) # e % phi
        except:
            pass

more_e_n = []
for e in e_n:
    i = (n - e) // phi
    more_e_n += [e + j * phi for j in range(1, i + 1)]
e_n += more_e_n

d_n = [inverse(e, phi) for e in e_n]

m_n = set()
c = 169169912654178
for d in d_n:
    m_n.add(pow(c, d, n))
m_n = list(m_n)

for m in m_n:
    print(b'npuctf{' + long_to_bytes(m) + b'}')
```

---

# Mersenne_twister

附件1：cipher.txt

```
cef4876036ee8b55aa59bca043725bf350a5e491debdef7ef7d63e9609a288ca1e2c82a7fe566bd8709e73c8d495ea504a486ed11189faf8e6fb35617e47d2d1ad5e4783e96afeaae9f7104ec477fb39fe4ec619bf58289709e15c4449f03fc51cba918cd0ebfdc12376b41e7815406482733b3b200826b6c78d86563edaea94dccf459a4291517a4b8367d7b4a53aeecd7e0accf661bfc726f5ba62e1c0e04100108ad32e7d5711f780185cba5cf31d328bee84066be4ab9582cf9d4bfe3c6f96a7732e1c37d800c90fd46277147f0a26c149dcd5eeb0f2df0c075627bc220be5eefdd67186056ac28c21e155a7f247664aaecdb498134de274df10114d1f06f84dd21820f150d69c9439d909dec0f5ccfeab61b62db2ea91d31bc8163ff16c7f458006bd5ac4a5f5bfae2770b23ccfb7195b76aa0a9aa146831667a7b9fe08c19e691afadccb3ca5169ef3fabaa3dad47d536e89ed4cee6f788bc969c3ad3137850ebfc46a73af2b0c036c3da4b4a16506f499445c604dd73eeb846a52f881515a3ad0ab448b4f9ed3e0ab1fffac60b223dde6450ba6198e90e14de107aaf2
```

附件2：Mersenne_twister.py

```python
from hashlib import *
from itertools import *
from binascii import hexlify , unhexlify

from flag import flag ,seed

assert len(flag) == 26
assert flag[:7] == 'npuctf{'
assert flag[-1] == '}'

XOR = lambda s1 ,s2 : bytes([x1 ^ x2 for x1 ,x2 in zip(s1 , s2)])

class mt73991:
    def __init__(self , seed):
        self.state = [seed] + [0] * 232
        self.flag = 0
        self.srand()
        self.generate()
    def srand(self):
        for i in range(232):
            self.state[i+1] = 1812433253 * (self.state[i] ^ (self.state[i] >> 27)) - i
            self.state[i+1] &= 0xffffffff


    def generate(self):
        for i in range(233):
            y = (self.state[i] & 0x80000000) | (self.state[(i+1)%233] & 0x7fffffff)
            temp = y >> 1
            temp ^= self.state[(i + 130) % 233]
            if y & 1:
                temp ^= 0x9908f23f
            self.state[i] = temp
    def getramdanbits(self):
        if self.flag == 233:
            self.generate()
            self.flag = 0
        bits = self.Next(self.state[self.flag]).to_bytes(4 , 'big')
        self.flag += 1
        return bits
        
    def Next(self , tmp):
        tmp ^= (tmp >> 11)
        tmp ^= (tmp << 7) & 0x9ddf4680
        tmp ^= (tmp << 15) & 0xefc65400
        tmp ^= (tmp >> 18) & 0x34adf670
        return tmp

def encrypt(key , plain):
    tmp = md5(plain).digest()
    return hexlify(XOR(tmp , key))

if __name__ == "__main__":
    flag = flag.encode()
    random = mt73991(seed)
    f = open('./cipher.txt' , 'wb')
    for i in flag:
        key = b''.join([random.getramdanbits() for _ in range(4)])
        cipher = encrypt(key , chr(i).encode())
        f.write(cipher)
```


## 分析

提供了它这里使用的梅森旋转算法

加密思路是：

​	每次加密明文：flag 里的每个字符
​	密钥：由 mt73991 对象生成的伪随机数（四个拼在一起）
​	加密函数：对明文一次 md5 加密，之后，加密后的明文和密钥逐位异或

## 解析

尝试写了一个对伪随机数预测的程序:

```python
from Mersenne_twister import *
from hashlib import *
from itertools import *
from binascii import hexlify , unhexlify
from string import ascii_letters, digits
from Crypto.Util.number import bytes_to_long
import flag

class TemperInverser:
    __b = 0x9ddf4680    # 比特遮罩1
    __c = 0xefc65400    # 比特遮罩2
    __d = 0x34adf670    # 比特遮罩3
    __kMaxBits      =  0xffffffff

    def __inverse_right_shift_xor(self, value, shift, mask):
        i, result = 0, 0
        while i * shift < 32:
            part_mask = ((self.__kMaxBits << (32 - shift)) & self.__kMaxBits) >> (i * shift)
            part = value & part_mask
            value ^= (part >> shift) & mask
            result |= part
            i += 1
        return result

    def __inverse_left_shift_xor(self, value, shift, mask):
        i, result = 0, 0
        while i * shift < 32:
            part_mask = (self.__kMaxBits >> (32 - shift)) << (i * shift)
            part = value & part_mask
            value ^= (part << shift) & mask
            result |= part
            i += 1
        return result

    def __inverse_temper(self, tempered):
        value = tempered
        value = self.__inverse_right_shift_xor(value, 18, self.__d)
        value = self.__inverse_left_shift_xor(value, 15, self.__c)
        value = self.__inverse_left_shift_xor(value, 7, self.__b)
        value = self.__inverse_right_shift_xor(value, 11, self.__kMaxBits)
        return value

    def __call__(self, tempered):
        return self.__inverse_temper(tempered)

class MersenneTwisterCracker:
    __n = 233
    def __init__(self, mt_obj):
        inverser  = TemperInverser()
        register  = [inverser(int.from_bytes(random(), 'big')) for _ in range(self.__n)]
        self.__mt = mt73991(0)
        # self.__state = 0
        # self.__register = register
        self.__mt.load_register(register) # 纠正

    def __call__(self):
        return self.__mt()

def encrypt(key , plain):
    tmp = md5(plain).digest()
    return hexlify(XOR(tmp , key))

all_letters = ascii_letters + digits + '.,;'
def decrypt_md5(md5_value):
    if len(md5_value)!=32:
        print('error')
        return
    md5_value=md5_value.lower()
    for k in range(5,10):
        for item in permutations(all_letters,k):
            item=''.join(item)
            if md5(item.encode()).hexdigest()==md5_value:
                return item
```

但是发现没有 seed 还是不行。

> 至少加深了对算法理解（自慰安慰 :cry: ）

估计算法的深处藏有爆破 seed 的方法（否则 $2^{32}$ 没办法得到 seed）。



# 共模攻击

附件1：**hint.py**

> 后来这个文件被下架了，直接给出其实 :fire::angry::fire: .（好吧，其实没差，hhh）

```python
from gmpy2 import *
from Crypto.Util.number import *
from secret import hint

m = bytes_to_long(hint)
p = getPrime(256)
c = pow(m, 256, p)
print(p)

p, q = getPrime(256), getPrime(256)
n = p * q
e1, e2 = getPrime(32), getPrime(32)
c1, c2 = pow(c, e1, n), pow(c, e2, n)
print(n)
print(e1, c1)
print(e2, c2)

'''
107316975771284342108362954945096489708900302633734520943905283655283318535709
6807492006219935335233722232024809784434293293172317282814978688931711423939629682224374870233587969960713638310068784415474535033780772766171320461281579
2303413961, 1754421169036191391717309256938035960912941109206872374826444526733030696056821731708193270151759843780894750696642659795452787547355043345348714129217723
2622163991,1613454015951555289711148366977297613624544025937559371784736059448454437652633847111272619248126613500028992813732842041018588707201458398726700828844249
'''
```

**task.py**

```python
from gmpy2 import *
from Crypto.Util.number import *
from secret import flag

flag = flag.strip(b"npuctf{").strip(b"}")
m = bytes_to_long(flag)

p, q = getPrime(512), getPrime(512)
n = p * q
e1, e2 = p, q
c1, c2 = pow(m, e1, n), pow(m, e2, n)

print(n)
print(c1)
print(c2)

'''
128205304743751985889679351195836799434324346996129753896234917982647254577214018524580290192396070591032007818847697193260130051396080104704981594190602854241936777324431673564677900773992273463534717009587530152480725448774018550562603894883079711995434332008363470321069097619786793617099517770260029108149
96860654235275202217368130195089839608037558388884522737500611121271571335123981588807994043800468529002147570655597610639680977780779494880330669466389788497046710319213376228391138021976388925171307760030058456934898771589435836261317283743951614505136840364638706914424433566782044926111639955612412134198
9566853166416448316408476072940703716510748416699965603380497338943730666656667456274146023583837768495637484138572090891246105018219222267465595710692705776272469703739932909158740030049375350999465338363044226512016686534246611049299981674236577960786526527933966681954486377462298197949323271904405241585
'''
```

## hint.py

### 共模攻击

**第一部分：**

```python
p, q = getPrime(256), getPrime(256)
n = p * q
e1, e2 = getPrime(32), getPrime(32)
c1, c2 = pow(c, e1, n), pow(c, e2, n)
print(n)
print(e1, c1)
print(e2, c2)
```

典型的共模攻击，破解：

```python
from gmpy2 import *
_, r, s = gcdext(e1, e2)
c = pow(c1, r, n) * pow(c2, s, n) % n
print c
```

### 高次剩余

**第二部分：**

```python
m = bytes_to_long(hint)
p = getPrime(256)
c = pow(m, 256, p)
print(p)
```

$$
c = m^{256} \pmod p
$$

这里要进行高次剩余方程求解。

不过网上的板子几乎都是用 C++ 写的（ICPCer 的作品...）我也尝试过用 [N次剩余(详解+例题+代码)](https://blog.csdn.net/dreamzuora/article/details/52744666) 的 CPP 板子，然而…… `c ` 是在太大，不得行…… :slightly_smiling_face:

> 关于算法，有篇博客写得很全面：[数论算法 剩余系相关 学习笔记 (基础回顾,(ex)CRT,(ex)lucas,(ex)BSGS,原根与指标入门,高次剩余,Miller_Robin+Pollard_Rho)](https://www.cnblogs.com/zhouzhendong/p/Number-theory-Residue-System.html) （仰望 ICPCer）
>
> 可以调用大整数库，写下算法……（濒危动物表示写不动 :sob: ）

#### Sympy

后来发现 Sympy 里有求 高次剩余的函数……

```python
from sympy.ntheory.residue_ntheory import nthroot_mod
from Crypto.Util.number import *

c = 19384002358725759679198917686763310349050988223627625096050800369760484237557
n = 107316975771284342108362954945096489708900302633734520943905283655283318535709
mlst = nthroot_mod(c, 256, n, all_roots=True)
for m in mlst:
    assert pow(m, 256, n) == c
    print long_to_bytes(m)
```

输出：

```
m.bit_length() < 400
��}�{�6�k����Zs�A����U�H���
�%��������)=�-q������L�P\?�
�CAz
KA���UA��X*gp}-LX��
```

#### mma / Sage

用 mma：

```mathematica
Simplify[
 Reduce[
  c == Mod[m^256, p] /.
   {c -> 1938400235872575967919891768676331034905098822362762509605080\
0369760484237557,
    p -> 1073169757712843421083629549450964897089003026337345209439052\
83655283318535709},
  m, Integers
  ]
]
```

或者用 SageMath：

```python
p = 107316975771284342108362954945096489708900302633734520943905283655283318535709
c = 1938400235872575967919891768676331034905098822362762509605080036976048423755
PR.<x> = PolynomialRing(Zmod(p))
f = x^256 - c
f.monic().roots()
```

+Python

```python
mlist = [623314401187286670257694436875298172611109072944,11812523831006378212663497485315375112114782670326938195125719701658999904789, 95504451940277963895699457459781114596785519963407582748779563953624318630920, 107316975771284342108362954944473175307713015963476826507029985482672209462765]
msglst =[]
for m in mlist:
    assert pow(m, 256, p) == c
    msglst.append(long_to_bytes(m))
msglst
```

输出：

```python
['m.bit_length() < 400',
 '\x1a\x1d\xa6\xcc}\xb9\x1f{\xe36\xe5k\xb3\xe6\xf3\xecZs\xb0A\xa2\x01\x88\x8b\x8aU\xf1H\xaa\xa6\xda\x15',
 '\xd3%\x9a\xad\x8c\x91\xfc\xc5\xf7\xa7)=\x0e\x89-\x15q\xf1\xe6\xff\xd4\xcc\xfbL\xa4P\\?\xce\x1a\\\x08',
 '\xedCAz\nK\x1cA\xda\xde\x0e\xa8UA\xbe\x98X\x06*\xdc\x08g\x0fp\x06}-LX\x8d\x05\xed']
```

所以，我们费了半天功夫得到的提示是：

```
m.bit_length() < 400
```

> :sweat_smile::sweat_smile::sweat_smile::sweat_smile::sweat_smile::sweat_smile::sweat_smile::sweat_smile:
>
> 这不是废话吗？？？
>
> :face_with_head_bandage::face_with_head_bandage::face_with_head_bandage: 稍后打脸

## task.py

$$
n = q*p
\\
c_1 = m^p \pmod n
\\
c_2 = m^q \pmod n
$$

确实是共模攻击，和后来下架 `hint.py` 之后的新的问题提示一样，此“共模攻击”非彼“共模攻击”。

模数确实一样，但我们不知道密钥……

看了 WP，发现 Hint 是给 Coppersmith 定理攻击 的可行性的提示……

> Coppersmith 定理指出在一个 e 阶的 $\pmod n$ 多项式 $f(x)$ 中，如果有一个根小于$n^{1/e}$，就可以运用一个 $O(log n)$ 的算法求出这些根。

以下是分析：

hint 给出 m 比特长度上界，联想到 coppersmith，再可行性分析如下：
$$
c_1 = m^p = m \pmod p
\\
c_2 = m^q = m \pmod q
\\
\therefore n | (c_1-m)(c_2-m)
\\
\text{上界：} \, \frac{1}{2}n^{1/2}  \approx 2^{511}
$$
因为 m.bit_length() < 400，所以可行性分析通过。

Sage：

```python
n = 128205304743751985889679351195836799434324346996129753896234917982647254577214018524580290192396070591032007818847697193260130051396080104704981594190602854241936777324431673564677900773992273463534717009587530152480725448774018550562603894883079711995434332008363470321069097619786793617099517770260029108149
c1= 96860654235275202217368130195089839608037558388884522737500611121271571335123981588807994043800468529002147570655597610639680977780779494880330669466389788497046710319213376228391138021976388925171307760030058456934898771589435836261317283743951614505136840364638706914424433566782044926111639955612412134198
c2= 9566853166416448316408476072940703716510748416699965603380497338943730666656667456274146023583837768495637484138572090891246105018219222267465595710692705776272469703739932909158740030049375350999465338363044226512016686534246611049299981674236577960786526527933966681954486377462298197949323271904405241585
PR.<y> = PolynomialRing(Zmod(n))
g = (c1-y) * (c2-y)
g.monic().small_roots(X=2^400, beta=1)
long_to_bytes(4242839043019782000788118887372132807371568279472499477998758466224002905442227156537788110520335652385855)
```

```
'verrrrrrry_345yyyyyyy_rsaaaaaaa_righttttttt?'
```

# ezRSA

附件：**difficultrsa.py**

```python
from gmpy2 import lcm, powmod, invert, gcd, mpz
from Crypto.Util.number import getPrime
from sympy import nextprime
from random import randint
p = getPrime(1024)
q = getPrime(1024)
n = p * q
gift = lcm(p - 1 , q - 1)
e = 54722
flag = b'NPUCTF{******************}'
m = int.from_bytes(flag , 'big')
c = powmod(m, e, n)
print('n: ', n)
print('gift: ', gift)
print('c: ', c)

#n: 17083941230213489700426636484487738282426471494607098847295335339638177583685457921198569105417734668692072727759139358207667248703952436680183153327606147421932365889983347282046439156176685765143620637107347870401946946501620531665573668068349080410807996582297505889946205052879002028936125315312256470583622913646319779125559691270916064588684997382451412747432722966919513413709987353038375477178385125453567111965259721484997156799355617642131569095810304077131053588483057244340742751804935494087687363416921314041547093118565767609667033859583125275322077617576783247853718516166743858265291135353895239981121
#gift:  2135492653776686212553329560560967285303308936825887355911916917454772197960682240149821138177216833586509090969892419775958406087994054585022894165950768427741545736247918410255804894522085720642952579638418483800243368312702566458196708508543635051350999572787188236243275631609875253617015664414032058822919469443284453403064076232765024248435543326597418851751586308514540124571309152787559712950209357825576896132278045112177910266019741013995106579484868768251084453338417115483515132869594712162052362083414163954681306259137057581036657441897428432575924018950961141822554251369262248368899977337886190114104
#c:  3738960639194737957667684143565005503596276451617922474669745529299929395507971435311181578387223323429323286927370576955078618335757508161263585164126047545413028829873269342924092339298957635079736446851837414357757312525158356579607212496060244403765822636515347192211817658170822313646743520831977673861869637519843133863288550058359429455052676323196728280408508614527953057214779165450356577820378810467527006377296194102671360302059901897977339728292345132827184227155061326328585640019916328847372295754472832318258636054663091475801235050657401857262960415898483713074139212596685365780269667500271108538319
```

## 分析

###  λ(n) instead of φ(n)

在 [RSA Theory](https://www.di-mgt.com.au/rsa_theory.html) 的 “Using λ(n) instead of φ(n)” 部分提到过：

$d = e^{-1} \pmod {\phi(n)}$ 可以替换成  $d' = e^{-1} \pmod {\lambda(n)}$，其中 $ \lambda(n) = lcm(p-1, q-1) = \frac{(p-1)(q-1)}{gcd(p-1, q-1)}$.
这样一来，我们就可以用 λ(n)（题目中的 gift）来代替我们原本生成密钥的时候对 φ(n) 的需求。

需要注意的这道的中：$gcd(e, lcm) = 2$，不在 gift 的剩余系内（即没有逆元），所以需要进行适当地处理：

把 $c=m^e \pmod n$ 看成是 $c=(m^2)^{(\frac{e}{2})} \pmod n$。
$$
e'=\frac{e}{2}
\\
d' = (e^{'})^{-1} \pmod {\lambda(n)}
\\
m^2 = c^{d'} \pmod {n}
$$
之后就是一个二次剩余的问题了。

### 二次剩余

#### Wolfram 爆破

```mathematica
Simplify[
 Reduce[
  c == Mod[m^2, p] /.
   {c -> 4457739276450750973807362088089319606097011997747961409022906575971021744219518190210017002304776543765491793897149413559709081776139101961,
    p -> 17083941230213489700426636484487738282426471494607098847295335339638177583685457921198569105417734668692072727759139358207667248703952436680183153327606147421932365889983347282046439156176685765143620637107347870401946946501620531665573668068349080410807996582297505889946205052879002028936125315312256470583622913646319779125559691270916064588684997382451412747432722966919513413709987353038375477178385125453567111965259721484997156799355617642131569095810304077131053588483057244340742751804935494087687363416921314041547093118565767609667033859583125275322077617576783247853718516166743858265291135353895239981121},
  m, Integers
  ]
]
```

其他的方法也卡住了 :sob:

---

WP 告诉了我们新的思路：“非有限域下的二次根求解难度在m足够大的时候几乎不可行，于是思路转向分解n”。

:star: 这道题考察的点实际上是已知(e, n, d)来分解 n

TODO: https://0xdktb.top/2020/04/19/WriteUp-NPUCTF-Crypto/#EzRSA



# 参考资料

2. [数论算法 剩余系相关 学习笔记 (基础回顾,(ex)CRT,(ex)lucas,(ex)BSGS,原根与指标入门,高次剩余,Miller_Robin+Pollard_Rho)](https://www.cnblogs.com/zhouzhendong/p/Number-theory-Residue-System.html) 
3. [N次剩余(详解+例题+代码)](https://blog.csdn.net/dreamzuora/article/details/52744666)
4. [WriteUp(NPUCTF) - Crypto](https://0xdktb.top/2020/04/19/WriteUp-NPUCTF-Crypto/#EzRSA) 
5. [RSA Theory](https://www.di-mgt.com.au/rsa_theory.html)
6. [DLP and Baby Step Giant Step Algorithm](https://masterpessimistaa.wordpress.com/2018/01/14/dlp-and-baby-step-giant-step-algorithm/)