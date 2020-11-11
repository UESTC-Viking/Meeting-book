## Benaloh cryptosystem

#### Introduction

同态加密是一种特殊的加密方法，允许对密文进行处理得到仍然是加密的结果。即对密文直接进行处理，跟对明文进行处理后再对处理结果加密，得到的结果相同。从抽象代数的角度讲，保持了同态性。

仅满足加法同态的算法包括 Paillier 和 Benaloh 算法。

仅满足乘法同态的算法包括 RSA 和 ElGamal 算法。

#### Key generation

1.选择大素数p,q满足$r|(p-1)$,$gcd(r,(p-1)/r)=1$,$gcd(r,q-1)=1$.

2.令$n=p*q$,$\phi=(p-1)\times(q-1)$

3.选择$y\in Z_n^*$,使得$y^{\phi/r}\not\equiv1$

4.令$x=y^{\phi/r}mod n$

公钥为$y , n$,私钥为$\phi,x$

#### Encryption

加密密文$m\in Z_r$:

1.选择随机数$u\in Z_n^*$

2.令$c=E_r(m)=y^mu^rmod n$

#### Decryption

解密密文$c\in Z_n^*$:

1.计算$a=c^{\phi/r}mod n$

2.输出$m=log_x(a)$,寻找$m$满足$x^m\equiv a modn$  

   有两种方法来进行计算,当r较小时,可以考虑暴力枚举,当r较大时,可以使用大步小步算法(baby-step-giant-step)

$a=(c)^{\phi/r}\equiv (y^mu^r)^{\phi/r}\equiv(y^m)^{\phi/r}(u^r)^{\phi/r}\equiv(y^{\phi/r})^m(u)^\phi\equiv(x)^m(u)^0\equiv x^mmod n$

#### Security Analysis

Benaloh cryptosystem基于高次剩余问题(给出$n=p*q,$,不知道$p,q$,给出一个整数$d$满足$(p-1)/d$,给出$x满足x<n$)

#### Baby-Step-Giant-Step

首先,这是一种空间换时间的算法

给出一个$n$ 阶循环群$G$、该群的一个生成元 $\alpha$ 和一个元素$\beta$  。试找到一个整数$x$ 满足$\alpha^x=\beta$

$x$代换成:

$x=im+j$  

$m=\ulcorner\sqrt{n}\urcorner$

$0\leqslant i <m$

$0\leqslant j<m$

我们可以得到:

$\alpha^x=\beta$

$\alpha^{im+j}=\beta$

$\alpha^j=\beta(\alpha^{-m})^{i}$

该算法先对 $j$的不同取值计算出$\alpha^{j}$的值，然后固定一个$m$ ，并对$i$尝试不同的取值,带入上面同余式的右边，看是否与某个（已经预先算出的）同余式左边的值相匹配。











