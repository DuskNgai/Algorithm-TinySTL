# Chapter 5 Probabilistic Analysis and Randomized Algorithms

## 5.1 The Hiring Problem

如果一个算法的行为不仅由其输入决定，而且还由一个*随机数生成器*产生的值决定，那么我们称其为*随机*算法。
We call an algorithm *randomized* if its behavior is determined not only by its input but also by values produced by a *random-number generator*.

### Exercises

#### 5.1-1

> Show that the assumption that you are always able to determine which candidate is best, in line 4 of procedure `HIRE-ASSISTANT`, implies that you know a total order on the ranks of the candidates.

Loop invariant: the $i$ records the best candidate from 1 to $i$.

#### 5.1-2

> Describe an implementation of the procedure `RANDOM(a, b)` that makes calls only to `RANDOM(0, 1)`. What is the expected running time of your procedure, as a function of $a$ and $b$?

We will first generate a random number between $0$ and $(b-a)$ and then add $a$ to get `RANDOM(a, b)`. To get this random number, we generate it bit by bit.

```python
def Random(a, b):
    limit = b - a
    n_bits = math.ceil(math.log2(limit))
    ran_num = 0
    for n in range(n_bits):
        ran_num += Random(0, 1) << n
    if ran_num > limit:
        return Random(a, b)
    else:
        return ran_num + a
```

To represent $(b-a)$, we need $\lceil\log_2(b-a)\rceil$ bits. Let $k$ be such number, then the expectation times of calling `RANDOM(a, b)` is $2^k/(b-a)$. During each call of `RANDOM(a, b)`, the times of calling `RANDOM(0, 1)` is $k$. Then the expectation times of calling `RANDOM(0, 1)` is $2^kk/(b-a)$, which is
$$
\Theta\biggl(\frac{2^{\lceil\log_2(b-a)\rceil}\lceil\log_2(b-a)\rceil}{b-a}\biggr)
$$

#### 5.1-3

> You wish to implement a program that outputs $0$ with probability $1/2$ and $1$ with probability $1/2$. At your disposal is a procedure `BIASED-RANDOM `that outputs
> either $0$ or $1$, but it outputs 1 with some probability $p$ and 0 with probability $1-p$, where $0 < p < 1$. You do not know what $p$ is. Give an algorithm that uses
> `BIASED-RANDOM` as a subroutine, and returns an unbiased answer, returning $0$ with probability $1/2$ and $1$ with probability $1/2$. What is the expected running time of your algorithm as a function of $p$?

Consider the event that generate 2 numbers with probability:
$$
\begin{align*}
P(00)&=(1-p)^2\\
P(01)&=(1-p)p\\
P(10)&=p(1-p)\\
P(11)&=p^2
\end{align*}
$$
Then the probability to generate 01 and 10 equals to each other. So we generate 2 numbers, if they are the same, then they will be discarded. Otherwith, we can return the first generated number.

```python
def Random():
    while True:
        a = BiasedRandom()
        b = BiasedRandom()
        if a != b:
            return a
```

So the expected running time is the expectation running time of generating two different numbers, which is $\Theta(1/[2p(1-p)])$.

## 5.2 Indicator Random Variables

Given a sample space $S$ and an event $A$, the indicator random variable $I(A)$ associated with event $A$ is defined as
$$
I(A)=\begin{cases}1&A\text{ occurs}\\0&A\text{ does not occur}\end{cases}
$$

### Lemma 5.1

Given a sample space $S$ and an event $A$ in the sample space $S$, let $X_A=I(A)$. Then $\mathbb{E}[X_A]=P(A)$.

#### Proof

$$
\begin{align*}
\mathbb{E}[X_A]&=\mathbb{E}[I(A)]\\
&=1\cdot P(A)+0\cdot P(\bar{A})\\
&=P(A)
\end{align*}
$$

### Exercises

#### 5.2-1

> In `HIRE-ASSISTANT`, assuming that the candidates are presented in a random order, what is the probability that you hire exactly one time? What is the probability that you hire exactly $n$ times?

The best candidate is at the first place, so it is $(n-1)!/n!=1/n$.

For $n$ times, they are in ascending order, which is $1/n!$.

#### 5.2-2

> In `HIRE-ASSISTANT`, assuming that the candidates are presented in a random order, what is the probability that you hire exactly twice?

In order to hire exactly twice, we will only hire the first candidate with rank $i$ and the candidate with rank $1$. Then people between these two people should not better than the first candidate. The probability of the rank $i$ candidate appear at the first place is $1/n$. The rank $1$ person is the first person appear amoung the person that better than rank $i$ candidate, with probability $1/(n-i)$.

So it is
$$
\sum_{i=1}^{n-1}\frac{1}{n}\frac{1}{n-i}=\frac{1}{n}\sum_{i=1}^{n-1}\frac{1}{i}
$$

#### 5.2-3

> Use indicator random variables to compute the expected value of the sum of $n$ dice.

Let the $X_i$ be the number of the $i$ th dice, $X=\sum_{i=1}^nX_i$. It is easy to prove that $\mathbb{E}[X_i]=3.5$, then
$$
\begin{align*}
\mathbb{E}[X]&=\mathbb{E}\biggl[\sum_{i=1}^{n}X_i\biggr]\\
&=\sum_{i=1}^{n}\mathbb{E}[X_i]\\
&=3.5n
\end{align*}
$$

## 5.3 Randomized Algorithms

### Randomly Permuting Arrays

```python
def RandomPermute(A, n):
    for i in range(n):
        swap(A[i], A[Random(i, n)])
```

在一个有 $n$ 个元素的集合上的 *$k$-permutation* 是一个包含 $n$ 个元素中的 $k$ 个元素的序列，并且没有重复。
A *$k$-permutation* on a set of $n$ elements is a sequence containing $k$ of the $n$ elements, with no repetitions.

### Lemma 5.4

Procedure `RANDOMLY-PERMUTE` computes a uniform random permutation.

#### Proof

Just prior to the $i$ th iteration of the for loop of lines 1-2, for each possible $(i-1)$-permutation of the $n$ elements, the subarray $A[1:i-1]$ contains this $(i-1)$-permutation with probability $(n-i+1)!/n!$.

Initialization: `i = 1`, `A[1:0]` contains $0$-permutation with probability $n!/n!=1$.

Maintenance: Before $i$ th iteration, we have had a $(i-1)$-permutation in $A[1:i-1]$ with probability $(n-i+1)!/n!$. In $i$ th iteration, a new value is put into $A[i]$ with probability $1/(n-i+1)$ since we randomly choose a value from $A[i,n]$. Then the subarray $A[1:i]$ contains this $i$-permutation with probability
$$
\frac{(n-i+1)!}{n!}\frac{1}{(n-i+1)}=\frac{(n-i)!}{n!}
$$
Termination: the subarray $A[1:n]$ contains this $n$-permutation with probability $1/n!$.

### Exercises

#### 5.3-1

> Professor Marceau objects to the loop invariant used in the proof of Lemma 5.4. He questions whether it holds prior to the first iteration. He reasons that we could just as easily declare that an empty subarray contains no $0$-permutations. Therefore, the probability that an empty subarray contains a $0$-permutation should be 0, thus invalidating the loop invariant prior to the first iteration. Rewrite the procedure `RANDOMLY-PERMUTE` so that its associated loop invariant applies to a nonempty subarray prior to the first iteration, and modify the proof of Lemma 5.4 for your procedure.

```python
def RandomPermute(A, n):
    swap(A[0], A[n - 1])
    for i in range(1, n):
        swap(A[i], A[Random(i, n)])
```

#### 5.3-2

> Professor Kelp decides to write a procedure that produces at random any permutation except the identity permutation, in which every element ends up where it started. He proposes the procedure `PERMUTE-WITHOUT-IDENTITY`. Does this procedure do what Professor Kelp intends?
>
> ```python
> def PermuteWithoutIdentity(A, n):
>     for i in range(1, n):
>         swap(A[i], A[Random(i + 1, n)])
> ```
>
> 

No, $A[0]$ have no probability to stay at where it was once swapped. Then there are a lot of permutation start with $A[0]$ can not be generated, which is not an actuall random algorithm.
