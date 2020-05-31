# ECC 加密原理简介

椭圆曲线方程：$y^{2}+a_1 x y+a_3 y=x^{3}+a_2 x^{2}+a_4 x+a_6$

并不是所有的椭圆曲线都适合于加密，最为常用的方程如下:
$$
y^{2}=x^{3}+a_4 x+a_6 \pmod q
$$
其中，$4 a_1^{3}+27 a_2^{2} \bmod p \neq 0$.

在线加法运算演示：[Elliptic Curve point addition (ℝ)](https://cdn.rawgit.com/andreacorbellini/ecc/920b29a/interactive/reals-add.html)

更详尽的介绍：[Elliptic Curve Cryptography: a gentle introduction](https://andrea.corbellini.name/2015/05/17/elliptic-curve-cryptography-a-gentle-introduction/)

### 密钥生成

用户A先选择一条椭圆曲线$E_q (a,b)$ ，然后选择其上的一个生成元G，假设其阶为n，之后再选择一个正整数 $n_a$作为密钥，计算 $P_a=n_aG$。

其中，$E_q(a,b), q,G$ 都会被公开。

公钥为$P_a$，私钥为$n_a $。

### 加密

用户B在向用户A发送消息m，这里假设消息m已经被编码为椭圆曲线上的点，其加密步骤如下

1. 查询 $ \left( E_q(a,b), q, P_a,G \right)$ 。
1. 在 $(1,q-1)$ 的区间内选择随机数 k 。
1.  $C_1 = k·G = (x_1,y_1)$ 。
1. $C_2 = m + k·P_a =(x_2,y_2)$ 
   如果为O，则从第二步重新开始。
1. 将 $(C_1,C_2)$ 发送给A。

### 解密

解密步骤如下

1. 利用私钥计算点 $n_a·C_1=n_akG=kP_a=(x_2,y_2)$。
1. 计算消息 $m=C_1-n_A · C_2$。

