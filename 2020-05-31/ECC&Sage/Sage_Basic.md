[TOC]

## 入门指南

### 简介&资料

官网：http://www.sagemath.org/index.html
遇到问题查文档：http://doc.sagemath.org/html/en/index.html 或 http://www.sagemath.org/search.html
Sage 入门参考（中文译）http://www.sagemath.org/zh/
另外此文件同目录下有参考指南（`Sage快速参考.pdf  Sage快速参考_基本数论.pdf  Sage快速参考_微积分.pdf  Sage快速参考_抽象代数.pdf`）

Sagemath 可以用来研究初等和高等，纯粹和应用数学。 这包括了大量的数学知识，包括基础代数、微积分、初等到非常高级的数论、密码学、数值计算、交换代数、群论、组合学、图论、精确线性代数等等。 

## 基本运算

### v.s. Python

#### 运算符

```sage
2^3
# 8
```

#### 内置函数

不用`math.sin`：

```
sin(5.135)
# -0.912021158525540
sin(pi/3)
# 1/2*sqrt(3)
```

有些数学表达式返回“精确”的值，而不是近似的数值结果。

#### 精确数值结果

要得到一个近似的数值解，使用函数 `n` 或者方法 `n` 
（两者的全名都是 `numerical_approx`, 并且函数 `N` 和 `n` 是一样的） 

可选参数 `prec` 和 `digits` ，前者指定结果的二进制位数， 即bit数，后者指定结果的十进制位数。默认精度是53 bit。

```
exp(2)
n(exp(2))
N(n(exp(2)), digits = 5)
sin(10).n()
sin(10).n(digits=10)
```

#### 动态类型变量

一个给定的变量可以接受任何Python类型的值。

#### 获取帮助

有内置文档（下载的时候可以看到），交互编程时加个 `?` 即可；

“Tab补全”功能和 Linux shell 很像

## 基本的代数和微积分

### 解方程

`solve` 函数用于解方程。要使用它，先要指定变量， 然后将方程（或方程组）以及要求解的变量作为参数传给 `solve`.

```
sage: x = var('x')
sage: solve(x^2 + 3*x + 2, x)
[x == -2, x == -1]
```
