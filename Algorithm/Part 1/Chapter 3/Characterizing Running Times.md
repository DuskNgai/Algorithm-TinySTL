# Chapter 3 Characterizing Running Times

## 3.1 $O$-notation, $\Omega$-notation, $\Theta$​-notation

$O$-notation 刻画了一个函数的渐近行为的上限。
$O$-notation characterizes an upper bound on the asymptotic behavior of a function.

$\Omega$-notation 刻画了一个函数的渐近行为的下限。
$\Omega$-notation characterizes an lower bound on the asymptotic behavior of a function.

$\Theta$-notation 刻画了一个函数的渐近行为的上下限。
$\Theta$-notation characterizes an tight bound on the asymptotic behavior of a function.

## 3.2 Asymptotic Notation: Formal Definition

**$O$-notation**

$$
O(g(n))=\{f(n)|\exist c>0,\exist n_0>0:\forall n\ge n_0,0\le f(n)\le cg(n)\}
$$
即在 $n$ 足够大的时候，$f(n)$ 一直小于等于 $cg(n)$ 。

**$\Omega$​-notation**

$$
\Omega(g(n))=\{f(n)|\exist c>0,\exist n_0>0:\forall n\ge n_0,0\le cg(n)\le f(n)\}
$$
即在 $n$ 足够大的时候，$f(n)$ 一直大于等于 $cg(n)$ 。

**$\Theta$-notation**
$$
\Theta(g(n))=\{f(n)|\exist c_1,c_2>0,\exist n_0>0,:\forall n\ge n_0,0\le c_1g(n)\le f(n)\le c_2g(n)\}
$$

即在 $n$ 足够大的时候，$f(n)$ 能夹在 $c_1g(n)$ 和 $c_2g(n)$ 之间。

$O(g(n))$ 的定义要求 $O(g(n))$ 集合中的每个函数 $f(n)$ 都是渐近非负的：即在 $n$ 足够大的时候，$f(n)$ 就必须是非负的。
The definition of $O(g(n))$ requires that every function $f(n)$ in the set $O(g(n))$ be asymptotically nonnegative: $f(n)$ must be nonnegative whenever $n$ is sufficiently large.

**Theorem 3.1**
$$
f(n)=\Theta(g(n))\Leftrightarrow f(n)=O(g(n)) \and f(n)=\Omega(g(n))
$$

我们不能说：插入排序的运行时间是 $\Theta(n^2)$。这是一个夸张的说法，因为从中省略了 "最坏情况"，我们只剩下一个涵盖所有情况的一揽子声明。这里的错误在于插入排序并不是在所有情况下都以 $\Theta(n^2)$ 的时间运行，因为正如我们所看到的，它在最佳情况下以 $\Theta(n)$ 时间运行。我们可以说，插入排序的运行时间是 $O(n^2)$。
Here is what we cannot correctly say: insertion sort’s running time is $\Theta(n^2)$. That is an overstatement because by omitting "worst-case" from the statement, we're left with a blanket statement covering all cases. The error here is that insertion sort does not run in $\Theta(n^2)$ time in all cases since, as we've seen, it runs in $\Theta(n)$ time in the best case. We can correctly say that insertion sort's running time is $O(n^2)$.

由于归并排序在所有情况下都以 $\Theta(n\log n)$ 的时间运行，我们可以直接说它的运行时间是 $\Theta(n\log n)$，而不用指明最坏情况、最好情况或其他情况。 
Since merge sort runs in $\Theta(n\log n)$ time in all cases, we can just say that its running time is $\Theta(n\log n)$ without specifying worst-case, best-case, or any other case.

**$o$-notation**

$$
o(g(n))=\{f(n)|\forall c>0,\exist n_0>0:\forall n\ge n_0,0\le f(n)<cg(n)\}
$$
$f(n)$ 是 $g(n)$ 的高阶无穷小。即在 $n$ 足够大的时候，$f(n)$ 一直小于 $cg(n)$ 。

**$\omega$-notation**

$$
\omega(g(n))=\{f(n)|\forall c>0,\exist n_0>0:\forall n\ge n_0,0\le cg(n)<f(n)\}
$$
$f(n)$ 是 $g(n)$ 的高阶无穷大。即在 $n$ 足够大的时候，$f(n)$ 一直大于 $cg(n)$ 。

**Transitivity**
$$
f(n)=\Theta(g(n))\ \and\ g(n)=\Theta(h(n))\ \Rightarrow\ f(n)=\Theta(h(n))\\
f(n)=O(g(n))\ \and\ g(n)=O(h(n))\ \Rightarrow\ f(n)=O(h(n))\\
f(n)=\Omega(g(n))\ \and\ g(n)=\Omega(h(n))\ \Rightarrow\ f(n)=\Omega(h(n))\\
f(n)=o(g(n))\ \and\ g(n)=o(h(n))\ \Rightarrow\ f(n)=o(h(n))\\
f(n)=\omega(g(n))\ \and\ g(n)=\omega(h(n))\ \Rightarrow\ f(n)=\omega(h(n))
$$

**Reflexivity**
$$
f(n)=\Theta(f(n))\\
f(n)=O(f(n))\\
f(n)=\Omega(f(n))
$$

**Symmetry**
$$
f(n)=\Theta(g(n)) \ \Leftrightarrow \ g(n)=\Theta(f(n))
$$

**Transpose symmetry**
$$
f(n)=O(g(n)) \ \Leftrightarrow \ g(n)=\Omega(f(n))\\
f(n)=o(g(n)) \ \Leftrightarrow \ g(n)=\omega(f(n))
$$

### 3.3 Standard Notations and Common Functions

$$
1<n^{\frac{1}{\ln n}}<\ln(\ln^*n)<\ln^*(\ln n)<\ln^*n<2^{\ln^*n}<\\
\ln \ln n<\sqrt{\ln n}<\ln n<2^{\sqrt{2\ln n}}<(\sqrt2)^{\ln n}<\ln^2n<\\
2^{\ln n}\le n<n\ln n\le \ln(n!)<n^2<n^3<\\
n^{\ln \ln n}\le(\ln n)^{\ln n}<4^{\ln n}<(\frac{3}{2})^n<2^n<n2^n<\\
e^n<(\ln n)!<n!<(n+1)!<2^{2^n}<2^{2^{n+1}}
$$

