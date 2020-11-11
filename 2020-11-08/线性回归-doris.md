线性回归线性回归是利用数理统计中回归分析，来确定两种或两种以上变量间相互依赖的定量关系的一种统计分析方法，运用十分广泛

通常的表达形式:$y=W^T*x+\varepsilon$

$\varepsilon\in N(0,\sigma^2)$

$L(\omega)=\sum_{i=1}^{N}\lVert{w_ix_i-y_i}\rVert^2=\sum_{i=1}^{N}({w_ix_i-y_i})^2=(w^Tx^T-y^T)(xw-y)$

$=w^Tx^Txw-y^Txw-w^Tx^Ty+y^Ty$

目标:$\widehat{\omega}=argminL(\omega)$

求导过程:令$\frac{\partial L(\omega)}{\partial\omega}=2x^Tx\omega-2x^Ty=0$

$\therefore x^Tx\omega-x^Ty=0$

$解得:\omega=(x^Tx)^{-1}x^Ty$

矩阵求导:

类型                   标量y(scalar)    向量Y(vector)    矩阵**Y**(matrix)

标量x(scalar)           $\frac{\partial{y}}{\partial{x}}$                       $\frac{\partial{Y}}{\partial{x}}$                      $\frac{\partial\textbf{Y}}{\partial{x}}$  

向量X(vector)         $\frac{\partial{y}}{\partial{X}}$                       $\frac{\partial{Y}}{\partial{X}}$  

矩阵**X**(matrix)        $\frac{\partial{y}}{\partial\textbf{X}}$ 

常用矩阵公式求导:

$\frac{\partial{\textbf{x}^TA\textbf{x}}}{\partial\textbf{x}}=(A+A^T)\textbf{x}$

$\frac{\partial{\beta^T\textbf{x}}}{\partial{\textbf{x}}}=\beta$

$$过拟合 \begin{cases}  \text{1.加数据}\\ \text{2.特征选择/特征提取(Principle Component Analysis)}\\ \text{3.正则化} \end{cases}$$

正则化框架:$\mathop{argmin}\limits_{\omega}[{\mathop{L(\omega)}\limits_{loss}+\mathop{\lambda P(\omega)}\limits_{penalty}]}$

$L1:Lasso$  $p(\omega)=||\omega||$

$L2:Ridge$  $p(\omega)=||\omega||^2={\omega}^T{\omega}$

$L(\omega)=\sum_{i=1}^{N}\lVert{\omega_ix_i-y_i}\rVert^2+\lambda \omega^T\omega=\sum_{i=1}^{N}({\omega_ix_i-y_i})^2+\lambda \omega^T\omega=(\omega^Tx^T-y^T)(x\omega-y)+\lambda \omega^T\omega$

$=\omega^T(x^Tx+\lambda I)\omega-y^Tx\omega-\omega^Tx^Ty+y^Ty$

目标:$\widehat{\omega}=argminL(\omega)$

求导过程:令$\frac{\partial L(\omega)}{\partial\omega}=2(x^Tx+\lambda I)\omega-2x^Ty=0$

$\therefore (x^Tx+\lambda I)\omega-x^Ty=0$

$解得:\widehat{\omega}=(x^Tx+\lambda I)^{-1}x^Ty$





























