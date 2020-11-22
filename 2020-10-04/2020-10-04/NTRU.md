### NTRU

`NTRU` 加密系统的工作对象是多项式环 $ R=Z[x]/(x^N-1) $ ，并取 p 和 q 满足 $gcd(p,q)=1$。具体过程描述如下：

- **公钥 / 私钥的产生**：选取小系数多项式 $ f,g \in R $，并计算：
  $$
  F_q=f^{-1}(mod q)，F_p=f^{-1}(mod p),h=gF_q(modq)
  $$
  其中，系统的公钥是 $ h $，私钥是 $ (f,F_p) $。

- **加密**：将明文编码成一个 $ (modp) $ 的多项式m，随机取一个小系数多项式 r ，得到密文：
  $$
  c = prh + m(modq)
  $$

- **解密**：先计算 $ a = cf(mod q) $，再选取适当的 $A$ 使 $ a $ 的系数皆在区间 $[A,A+q]$内。最后 $aF_p(modp) $ 即为明文 $m$。

  **证明：**

  (1)

  
  $$
  \begin{align}
  a &\equiv f \centerdot c(modq)\\
  &\equiv f \centerdot (prh+m)(modq)\\
  &\equiv (f \centerdot prh+f \centerdot m)(modq)\\
  &\equiv (f \centerdot pr \centerdot gF_q +f \centerdot m)(modq)\\
  &\equiv (pr \centerdot g + f \centerdot m)(modq)
  \end{align}
  $$
  (2)
  $$
  \begin{align}
  a \centerdot F_p(modp) &\equiv F_p \centerdot (pr \centerdot g +f \centerdot m)(modp)\\
  &\equiv (F_p\centerdot pr \centerdot g + F-p \centerdot f \centerdot m)(modp)\\
  &\equiv m(modp)
  \end{align}
  $$

### 以格论解释NTRU

#### 格的定义

格是m维空间 $ R^m $ 的 n ($ m \geq n $) 个线性无关向量 $ \vec{b_i} $ ($ 1 \leq i \leq n $) 的所有整系数的线性组合，即$ L(B) = \{ \sum_{i=1}^n x_i \vec{b_i} :x_i \in Z , 1 \leq i \leq n \} $ 

这里 B 就是 n 个向量的集合，我们称

- 这 n 个向量是格 L 的一组基。
- 格 L 的秩为 n 。
- 格 L 的位数为 m 。

如果 $ m = n $，那么我们称这个格是满秩的。当然也可以是其它群，不是 $ R^m $ 。

#### 格、相关计算问题（了解）

一个秩为 n 的格 $ \Lambda $ 是 $ R^m $ 中的一个离散加法子群，并且以 $ R^m $ 中 n 个线性无关的向量 $ \vec{b_1},...,\vec{b_n} $ 为一组格基。令 $ \vec{B} $ 为 $ m \times n $ 矩阵 ($ \vec{b_1},...,\vec{b_n} $), 格 $ \Lambda $ 可确切表示为：
$$
\Lambda = L(\vec{B})=\vec{B} Z^n
$$
通常我们只考虑满格，即$ m=n $, 其中也称 n 为格 $ \Lambda $ 的维数。

格 $ \Lambda $ 作为一个 Z-模，其对偶格是 $ \hat{ \Lambda }=Hom( \Lambda, Z) $，更确切地，
$$
\hat{\Lambda}=\{\vec{y}\in R^n:<\vec{x},\vec{y}>\in Z,\forall \vec{x}\in \Lambda \}
$$
也是一个 n 维格。

易知，对于格 $ \Lambda $ 的另一个格基 $ \vec{B`} = (\vec{b_1`},...,\vec{b_n`}) $ ，存在一个幺模矩阵 $ U \in Z^{n \times n}$ ，使得 $ \vec{B`}=\vec{B}\vec{U} $。 给定格基  $ \vec{B}=(\vec{b_1},...\vec{b_n}) $，其基本体是下述集合：
$$
\mathcal{P}(\vec{B})=\{ \sum_{i=1}^n \alpha_i \vec{b_i} : 0 \leq \alpha_i < 1 \}
$$
其中，$ \mathcal{P}(\vec{B}) $ 还可以写成 $ R^n/\Lambda $ 其体积 $ \sqrt{det(\vec{B}\vec{B^T})} $ 通常被记为 $ det(\Lambda) $。

格 $ \Lambda $ 的逐次最小长度 $ \lambda_i(\Lambda) $ 定义为包含 i 个线性无关的格向量的以远点为球心的球的最小半径，即
$$
\lambda_i(\Lambda)=min\{r>0:dim(span(\Lambda \cap B(0,r)))\geq i\}
$$
$ i=1,2,...,n $ ,特别地，$ \lambda_1(\Lambda) $ 是格 $ \Lambda $ 中最短非零向量的长度。

#### 格上困难问题

- **最短向量问题(shortest vector problem,SVP):** 给定格 $ \Lambda $ 的一组基 $ \vec{B} $ ,寻找一个非零格向量 $ \vec{v} $ 使得 $ \parallel \vec{v} \parallel = \lambda_1(\Lambda) $ 。
- **最近向量问题(closest vector problem,CVP):** 给定格 $ \Lambda $ 的一组基 $ \vec{B} $ 和某个向量 $ \vec{t} \in R^n $，寻找一个格向量 $ \vec{v_0} $ 使得 $ \parallel \vec{v_0}-\vec{t} \parallel= min_{\vec{v}\in\Lambda} \parallel \vec{v}-\vec{t} \parallel$ 。
- $\gamma-近似最短向量问题(SVP_\gamma) $  : 给定格 $\Lambda$ 的一组基 $\vec{B}$ 和近似因子 $\gamma > 1$, 寻找一个非零格向量 $\vec{v}$ 使得  $\parallel \vec{v} \parallel \leq \gamma \centerdot \lambda_1(\Lambda)$.
- $最短线性无关向量问题 (SIVP)$: 给定格 $\Lambda$, 寻找 n 个线性无关的非零格向量 $\vec{v_i}$ 使得 $\parallel \vec{v_i} \parallel \leq \lambda_n(\Lambda)$.
-  $\gamma-近似最近向量问题(CVP_\gamma) $  : 给定格 $\Lambda$ 的一组基 $\vec{B}$、某个向量 $ \vec{t} \in R^n $ 和近似因子 $\gamma > 1$, 寻找一个非零格向量 $\vec{v}$ 使得$\parallel \vec{v}-\vec{t} \parallel \leq \gamma \centerdot dist(\vec{t},\Lambda)$ 。
- $\gamma-有界距离解码问题(BDD_\gamma) $ : 给定格 $\Lambda$ 和某个目标向量 $ \vec{t} \in R^n $ , 满足 $dist(\vec{t},\Lambda)<\gamma \centerdot \lambda_1(\Lambda)$, 寻找一个非零格向量 $\vec{v}$ , 使得 $\parallel \vec{v}-\vec{t} \parallel =  \gamma \centerdot dist(\vec{t},\Lambda) $。

#### NTRU加密的格论解释

首先，将公钥 $h$ 写成 $ h=h_0+h_1x+...+h_{N-1}x^{N-1} $，记
$$
\vec{H}=\left( \begin{array}{cc} h_0 & h_{N-1} & \cdots & h_1 \\ h_1 & h_0 & \cdots & h_2 \\ \vdots & \vdots & \ddots & \vdots \\ h_{N-1} & h_{N-2} & \cdots & h_0 \end{array} \right)
$$
并考虑 $ 2N \times 2N $ 矩阵：
$$
\vec{B_h}=\left( \begin{array}{cc} \vec{I_N} & 0 \\ \vec{H} & q\vec{I_N} \end{array} \right )
$$
设 $ \Lambda_h $ 为由 $ \vec{B_h} $ (列向量) 生成的格。设 $ f$ 和 $g$ 的系数为 $f_0,...f_{N-1}$，$g_0,...g_{N-1}$，我们很容易知道
$$
(f_0,...,f_{N-1},g_0,...,g_{N-1})^T=\vec{B_h} (f_0,...,f_{N-1},u_0,...,u_{N-1})^T
$$
其中 $u_0,...u_{N-1}$ 为多项式 $ u(x)= \frac{g(x)-f(x)h(x)}{q} mod (x^N-1) $ 的系数。这表明 $(f_0,...,f_{N-1},g_0,...,g_{N-1})^T$ 是格 $ \Lambda_h $ 中的短向量 ( f 和 g 的系数皆很小)。所以，若能计算 $ \Lambda_h $ 的 `SVP` 问题，便有可能求出密钥 $(f,F_p)$。另外，若给定公钥和密文，求解明文的问题可以归结为 $ \Lambda_h $ 的 `CVP` 问题。

**证明：**

(1)
$$
g(x)=qu(x)+f(x)h(x)(mod(x^N-1))
$$
(2)
$$
f(x)g(x)=\sum_{i=0}^{N-1}(\sum_{j=0}^{N-1}h_jf_{(N+i-j)modN}x^i)
$$
