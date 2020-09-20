# Paillier cryptosystem

## Preface

现代公钥密码系统中，其实远远不止RSA、DSA、ECC等众所周知的公钥密码系统，最近还学习到了一种比较年轻的公钥密码系统 —— **paillier cryptosystem** 但是wiki上并没有给出该方案的解密的proof。

## Introduce

Paillier密码，于1999年由Pascal Paillier发明，是一种用于公钥加密的概率非对称算法。该算法具有加法同态的性质 ; 这意味着，仅给出公钥和m1、m2加密，可以计算出 $m_1 + m_2$

##  Key generation

### The frist pattern：

1. 随机选择两个大质数 $p$、$q$ 满足 $gcd(pq, (p-1)\times(q-1))=1$ 
1. 计算 $n=p \times q， \lambda = lcm(p-1, q-1)$   
1. 选择随机整数 $g$，
1. 公钥为 $(n, g)$, 私钥为 $({\lambda}，{\mu})$.

### The second pattern（a simpler variant）：

其余参数不变，主要改变了 $g，{\lambda}，{\mu}$ 的定义：

1. $g = n+1$
1. $ {\lambda} = {\varphi}(n) = (p-1)*(q-1)$ 
1. $ {\mu} = {\varphi}(n)^{-1} \pmod n$ 

## Encryption

1. 设 $m$ 为要加密的消息，显然需要满足，$0 \le m < n$
1. 选择随机 $r$，保证 $gcd(r, n) = 1$
1. 密文c：$c = (g^m) *(r^n) \pmod {n^2}$

## Decryption

1. $m =  L( c^{\lambda} \pmod {n^2}  * {\mu} ) \pmod n$

### Proof

#### The first pattern

由 $L(c^{\lambda} \pmod{n^2}*{\mu}) \pmod{n}$

有 $L(g^{m{\lambda}}*r^{n{\lambda}} \pmod{n^2})*{\mu} \pmod{n}$ ①

其中 ${\lambda} = \frac{(p-1)*(q-1)}{gcd((p-1)*(q-1))}, {\mu} = L(g^{\lambda} \pmod{n^2})^{-1} \pmod{n}$

所以①式=> $L(g^{m{\lambda}}*r^{n{\lambda}} \pmod{n^2})*L(g^{\lambda} \pmod{n^2})^{-1} \pmod{n}$ ②

$\because (p-1)|{\lambda}, (q-1)|{\lambda}$

$ \therefore  {\lambda} = k_1(p-1) = k_2(q-1)$

$ \therefore  g^{\lambda} = g^{k_1(p-1)}\equiv 1 \pmod{p}，即 (g^{\lambda} -1) | p$

$ g^{\lambda} = g^{k_2(q-1)}\equiv 1 \pmod{q}， 即 (g^{\lambda} -1) | q$

$ \therefore   (g^{\lambda} -1) | lcm(p,q) ，即 (g^{\lambda} -1) | pq，即g^{\lambda} \equiv 1 \pmod{n} $

$ \therefore  g^{\lambda} \pmod{n^2} \equiv 1\pmod{n}$

$ \therefore  g^{\lambda}\pmod{n^2} = k_gn+1; k<n$

$ \therefore  L(g^{\lambda}\pmod{n^2}) = k_g$

且有

1. $1+k_n \equiv 1+kn \pmod{m^2}$
1. $(1+kn)^2 \equiv 1+2kn+(kn)^2 equiv 1+2kn \pmod{m^2}$
1. $(1+kn)^3 \equiv 1+3(kn)^2+3kn+(kn)^3 \equiv 1+3kn \pmod{m^2}$这里我们就不用数学分析法了，就直接易得了=> $(1+kn)^{m} \equiv knm+1 \pmod{n^2}$

$ \therefore  g^{m{\lambda}} = (1+k_gn)^{m} \equiv k_gnm+1 \pmod{n^2}$

$ \therefore  r^{n{\lambda}} = (1+k_nn)^{n} \equiv k_nn^2+1 \equiv 1\pmod{n^2}$

$ \therefore  L(g^{m{\lambda}}*r^{n{\lambda}}\pmod{n^2}) = L(k_gnm+1)=mk_g$

又$ \therefore  L(g^{\lambda}\pmod{n^2}) = k_g$

∴②式： $L(g^{m{\lambda}}*r^{n{\lambda}} \pmod{n^2})*L(g^{\lambda} \pmod{n^2})^{-1} \pmod{n}$ => $mk_g*k_g^{-1} \equiv m \pmod n$

证毕.

#### The second pattern

由 $L(c^{\lambda} \pmod{n^2}*{\mu}) \pmod{n}$

有 $L(g^{m{\lambda}}*r^{n{\lambda}} \pmod{n^2}*{\mu}) \pmod{n}$ ①

其中${\lambda} = (p-1)*(q-1)， {\mu} = {\varphi}(n)^{-1} \pmod{n}$

$\because r^{n{\lambda}} = r^{n(p-1)*(q-1)} = r^{{\varphi}(n^2)}$

由欧拉定理：$r^{{\varphi}(n^2)} \equiv 1 \pmod{n^2}$

①式 => $L(g^{m{\lambda}} \pmod{n^2}*{\mu}) \pmod{n}$ ②

$\because g = n+1$

$ \therefore  g^{m{\lambda}} = (1+n)^{m{\lambda}} \equiv nm{\lambda}+1 \pmod{n^2}$

②式=> $L((nm{\lambda}+1)*{\mu}) \pmod{n}$

=> $ \frac{(nm{\lambda}+1)-1}{n}*{\mu} \pmod{n}$

=>$(m{\lambda}*{\mu}) \pmod{n}$ ③

$\because{\lambda} = {\varphi}(n)，{\mu} = {\varphi}(n)^{-1} \pmod{n}$

∴③式： $(m{\lambda}*{\mu}) \equiv m\pmod{n}$

证毕

## DASCTF 四月月赛 not RSA

```python
from Crypto.Util.number import getPrime as bytes_to_long
from secret import flag,p,q
from sympy import isprime,nextprime
import random

m=bytes_to_long(flag)
n=p*q
g=n+1
r=random.randint(1,n)

c=(pow(g,m,n*n)*pow(r,n,n*n))%(n*n)

print "c=%d"%(c)
print "n=%d"%(n)
```

可以看到，这一题就是用的 paillier cryptosystem，且参数用的是上文中的The second pattern

但是我们计算 ${\lambda} = {\varphi}(n) = (p-1)*(q-1)$ ，需要用到p和q

这里我们直接上yafu分解n发现可以成功分解，原因是p与q其实非常接近，所以其实直接对n开根然后再在附近寻找素数就能找到p、q了。

所以构造解密脚本

```python
# -*- coding: utf-8 -*-
from Crypto.Util.number import long_to_bytes,inverse
from sympy import nextprime
from gmpy2 import iroot

def L(x,n):
    return (x-1)/n

c=
n=

#factor(n)
a = iroot(n,2)[0]
p = nextprime(a)
q = n//p
assert p*q == n

#根据解密公式，计算所需私钥对（{\lambda}，{\mu}）
Lambda=(p-1)*(q-1)
miu=inverse(Lambda,n*n)
m=(L(pow(c,Lambda,n**2),n)*miu)%n

print long_to_bytes(m)
```

 

## Homomorphic properties

### Homomorphic addition of plaintexts

设 D 为解密函数，E 为加密函数

即：$D(E(m_1, r_1)*E(m_2,r_2) \pmod {n^2}）≡ m_1+m_2 \pmod n$

#### Proof
$$
\begin{align}
C &= (g^{m_1}) * (r_1^n) * (g^{m_2}) *(r_2^n) \pmod {n^2}
\\
&= g^{(m_1+m_2)}*(r_1r_2)^n \pmod {n^2}
\end{align}
$$
首先我们可以将 $m_1+m_2$ 看作一个整体 $M$，然后由于 $r_1$、$r_2$ 是随机选的，所以 $r_1 \times r_2$ 可以看作一个整体 $R$，

故 $C = g^M * R^n \pmod {n^2}$

由于$gcd(r_1,n) = 1; gcd(r_2,n) = 1； => gcd(r_1*r_2, n) = 1$，故 $R$ 符合要求

所以 $D(C) = M ≡ m_1 + m_2 \pmod n$

### Homomorphic multiplication of plaintexts

设D为解密函数，E为加密函数

即：$D(E(m_1, r_1)^k \pmod {n^2} ≡ km_1 \pmod n$

#### Proof
$$
\begin{align}
C &= \left( g^{m_1} \times r_1^n \right) ^k \pmod {n^2}
\\
&=(g^{km_1})*(r_1^{kn} ) \pmod {n^2}
\end{align}
$$
首先我们可以将 $km_1$ 看作一个整体 $M$，然后由于 $r_1$ 是随机选的，所以 $r_1^k$ 可以看作一个整体 $R$，

故 $C = g^M * R^n \pmod {n^2}$

由于 $gcd(r_1,n) = 1 => gcd(r_1^k, n) = 1$，故 $R$ 符合要求

所以 $D(C) = M ≡ km_1 \pmod n$

## Reference

https://en.wikipedia.org/wiki/Paillier_cryptosystem

https://www.anquanke.com/post/id/204720