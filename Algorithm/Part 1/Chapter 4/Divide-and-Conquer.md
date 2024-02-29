# Chapter 4 Divide-and-Conquer

如果一个案例涉及到函数在不同（通常较小）输入上的递归调用，它就是*递归案例*。如果一个案例不涉及递归调用，它就是一个*基本案例*。
If a case involves the recursive invocation of the function on different (usually smaller) inputs, it is a *recursive case*. If a case does not involve a recursive invocation, it is a *base case*.

如果对于每一个足够大的*阈值*常数 $n_0>0$，以下两个属性成立，那么一个递归 $T(n)$ 就是*算法的*：
A recurrence $T(n)$ is *algorithmic* if, for every sufficiently large *threshold* constant $n_0>0$, the following two properties hold:

1. 对于所有 $n<n_0$，我们有 $T(n)=\Theta(1)$。
    For all $n<n_0$, we have $T(n)=\Theta(1)$.
2. 对于所有 $n\ge n_0$，每条递归路径都在有限的递归调用次数内终止于一个定义好的基本案例。
    For all $n\ge n_0$, every path of recursion terminates in a defined base case within a finite number of recursive invocations.

## 4.1 Multiplying Square Matrices

## 4.2 Strassen's Algorithm for Matrix Multiplication

## 4.3 The Substitution Method for Solving Recurrences

Avoiding pitfalls: $O(n)+\Theta(n)\ne O(n)$, we need to compare their constants.

### Exersices

#### 4.3-1

> Use the substitution method to show that each of the following recurrences defined on the reals has the asymptotic solution specified:

> a. $T(n)=T(n-1)+n$ has solution $T(n)=O(n^2)$.

$$
\begin{align*}
T(n)&=T(n-1)+n\\
&\le c(n-1)^2+n\\
&=cn^2+(1-2c)n+c\\
&\le cn^2
\end{align*}
$$

For $c>1/2$.

> b. $T(n)=T(n/2)+\Theta(1)$ has solution $T(n)=O(\log n)$.

$$
\begin{align*}
T(n)&=T(n/2)+\Theta(1)\\
&\le c\log(n/2)+d\\
&=c\log n-c\log2+d\\
&\le c\log n
\end{align*}
$$

For $c>d/\log2$.

> c. $T(n)=2T(n/2)+n$ has solution $T(n)=\Theta(n\log n)$.

$$
\begin{align*}
T(n)&=2T(n/2)+n\\
&\le2c_1(n/2)\log(n/2)+(n/2)\\
&=c_1n\log n-c_1n\log2+(n/2)\\
&\le c_1\log n
\end{align*}
$$

For $c_1>1/(2\log2)$.
$$
\begin{align*}
T(n)&=2T(n/2)+n\\
&\ge2c_2(n/2)\log(n/2)+(n/2)\\
&=c_2n\log n-c_2n\log2+(n/2)\\
&\ge c_2\log n
\end{align*}
$$
For $0<c_2<1/(2\log2)$.

## 4.4 The Recursion-tree Method for Solving Recurrences

### Exercises

#### 4.4-1

> For each of the following recurrences, sketch its recursion tree, and guess a good asymptotic upper bound on its solution. Then use the substitution method to verify your answer.

We draw the recursion tree in our mind.

> a. $T(n)=T(n/2)+n^3$

$$
\begin{align*}
T(n)&=T(n/2)+n^3\\
&\le c(n/2)^3\log(n/2)+n^3\\
&=(c/8)n^3\log n-(c/8)n^3\log2+n^3\\
&\le cn^3\log n
\end{align*}
$$

For $c>8/\log2$.

> b. $T(n)=4T(n/3)+n$

$$
\begin{align*}
T(n)&=4T\biggl(\frac{n}{3}\biggr)+n\\
&\le4\biggl[c\biggl(\frac{n}{3}\biggr)^{\log_34}-d\biggl(\frac{n}{3}\biggr)\biggr]+n\\
&=\frac{4c}{3^{\log_34}}n^{\log_34}+\biggl(1-\frac{4d}{3}\biggr)n\\
&=cn^{\log_34}+\biggl(1-\frac{4d}{3}\biggr)n\\
&\le cn^{\log_34}-dn
\end{align*}
$$

For $c>0,d>3$.

> c. $T(n)=4T(n/2)+n$

$$
\begin{align*}
T(n)&=4T(n/2)+n\\
&\le 4[c(n/2)^2-d(n/2)]+n\\
&=cn^2+(1-2d)n\\
&\le cn^2-dn
\end{align*}
$$

For $c>0,d>1$.

> d. $T(n)=3T(n-1)+1$

$$
\begin{align*}
T(n)&=3T(n-1)+1\\
&\le 3[c3^{n-1}-d]+1\\
&=c3^n-3d+1\\
&\le c3^n-d
\end{align*}
$$

For $c>0,d>1/2$.

## 4.5 The Master Method for Solving Recurrences


$$
T(n)=aT(n/b)+f(n),(a>0,b>1)
$$

我们称 $f(n)$ 为*驱动函数*，我们称这种一般形式的递归为*主递归*。
We call $f(n)$ a *driving function*, and we call a recurrence of this general form a *master recurrence*.

### Theorem 4.1 (Master theorem)

Let $a > 0$ and $b > 1$ be constants, and let $f(n)$ be a driving function that is defined and nonnegative on all sufficiently large reals. Define the recurrence $f(n)$ on $n\in\mathbb{N}$​ by
$$
T(n)=aT(n/b)+f(n)
$$
where $aT(n/b)$ actually means $a'T(\lfloor n/b\rfloor)+a''T(\lceil n/b\rceil)$ for some constants $a'\ge0$ and $a''\ge0$ satisfying $a=a'+a''$. Then the asymptotic behavior of $T(n)$ can be characterized as follows:

1. If $f(n)=O(n^{\log_ba})$, and $\exist\epsilon>0, f(n)=O(n^{\log_ba-\epsilon})\Rightarrow T(n)=\Theta(n^{\log_ba})$.
2. If $f(n)=\Theta(n^{\log_ba})$, and $\exist k\ge0,f(n)=\Theta(n^{\log_ba}\log^kn)\Rightarrow T(n)=\Theta(n^{\log_ba}\log^{k+1}n)$.
3. If $f(n)=\Omega(n^{\log_ba})$, and $\exist\epsilon>0, f(n)=\Omega(n^{\log_ba+\epsilon})\wedge\exist c<1,n_0>0,\forall n\ge n_0,af(n/b)\le cf(n)\Rightarrow T(n)=\Theta(n^{\log_ba})$.

### Exercises

#### 4.5-1

> Use the master method to give tight asymptotic bounds for the following recurrences.

> a. $T(n)=2T(n/4)+1$

$$
T(n)=\Theta(\sqrt{n})
$$

> b. $T(n)=2T(n/4)+\sqrt{n}$

$$
T(n)=\Theta(\sqrt{n}\log n)
$$

> c. $T(n)=2T(n/4)+\sqrt{n}\log^2n$

$$
T(n)=\Theta(\sqrt{n}\log^3n)
$$

> d. $T(n)=2T(n/4)+n$

$$
T(n)=\Theta(n)
$$

> e. $T(n)=2T(n/4)+n^2$

$$
T(n)=\Theta(n^2)
$$

## Problems

### 4-5 Fibonacci numbers

> This problem develops properties of the Fibonacci numbers, which are defined by recurrence (3.31) on page 69. We’ll explore the technique of generating functions to solve the Fibonacci recurrence. Define the generating function (or formal power series) $\mathcal{F}$ as
> $$
> \mathcal{F}(z)=\sum_{i=0}^{\infty}F_iz^i
> $$
> where $F_i$ is the $i$ th Fibonacci number.

> a. Show that $\mathcal{F}(z)=z+z\mathcal{F}(z)+z^2\mathcal{F}(z)$.

$$
\begin{align*}
\mathcal{F}(z)&=F_0+F_1z+\sum_{i=2}^{\infty}F_iz^i\\
&=F_0+F_1z+\sum_{i=2}^{\infty}(F_{i-2}+F_{i-1})z^i\\
&=F_0+F_1z+z^2\sum_{i=0}^{\infty}(F_i+F_{i+1})z^i\\
&=F_0+F_1z+z\sum_{i=1}^{\infty}F_iz^i+z^2\sum_{i=0}^{\infty}F_iz^i\\
&=F_1z+z\sum_{i=0}^{\infty}F_iz^i+z^2\sum_{i=0}^{\infty}F_iz^i\\
&=z+z\mathcal{F}(z)+z^2\mathcal{F}(z)\\
\end{align*}
$$

> b. Show that
> $$
> \begin{align*}
> \mathcal{F}(z)&=\frac{z}{1-z-z^2}\\
> &=\frac{z}{(1-\phi z)(1-\hat\phi z)}\\
> &=\frac{1}{\sqrt{5}}\biggl(\frac{1}{1-\phi z}-\frac{1}{1-\hat\phi z}\biggr)
> \end{align*}
> $$
> where $\phi$ is the golden ratio, and $\hat\phi$ is its conjugate.

$$
\begin{align*}
\mathcal{F}(z)&=z+z\mathcal{F}(z)+z^2\mathcal{F}(z)\\
\mathcal{F}(z)&=\frac{z}{1-z-z^2}
\end{align*}
$$

And the rest are just factorization.

> c. Show that
> $$
> \mathcal{F}(z)=\sum_{i=0}^{\infty}\frac{1}{\sqrt{5}}(\phi^i-\hat\phi^i)z^i
> $$
> You may use without proof the generating-function version of equation (A.7) on page 1142, $\sum_{k=0}^{\infty}x^k=1/(1-x)$. Because this equation involves a generating function, $x$ is a formal variable, not a real-valued variable, so that you don't have to worry about convergence of the summation or about the requirement in equation (A.7) that $|x|<1$, which doesn’t make sense here.

$$
\begin{align*}
\mathcal{F}(z)&=\frac{1}{\sqrt{5}}\biggl(\frac{1}{1-\phi z}-\frac{1}{1-\hat\phi z}\biggr)\\
&=\frac{1}{\sqrt{5}}\biggl(\sum_{i=0}^{\infty}(\phi z)^i-\sum_{i=0}^{\infty}(\hat\phi z)^i\biggr)\\
&=\frac{1}{\sqrt{5}}\sum_{i=0}^{\infty}(\phi^i-\hat\phi^i)z^i
\end{align*}
$$

> d. Use part (c) to prove that $F_i=\phi^i/\sqrt{5}$ for $i > 0$, rounded to the nearest integer. (Hint: Observe that $|\hat\phi|<1$.)

$$
\begin{align*}
\mathcal{F}(z)&=\sum_{i=0}^{\infty}\frac{\phi^i-\hat\phi^i}{\sqrt{5}}z^i\\
&=\sum_{i=0}^{\infty}F_iz^i
\end{align*}
$$

$F_1=(5+\sqrt{5})/10\approx1$. For $i>1$, $|\hat\phi^i|<0.5$. Then $\phi^i/\sqrt{5}$ get dominate.

> e. Prove that $F_{i+2}\ge\phi^i$ for $i\ge0$.

$$
\begin{align*}
F_{i+2}&=F_{i+1}+F_i\\
&\ge(\phi^{i+1}+\phi^i)/\sqrt{5}\\
&=\phi^i\frac{3+\sqrt{5}}{2\sqrt{5}}\ge\phi^i
\end{align*}
$$

