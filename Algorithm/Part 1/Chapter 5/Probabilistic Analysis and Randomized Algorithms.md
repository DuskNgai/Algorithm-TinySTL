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

## 5.4 Probabilistic Analysis and Further Uses of Indicator Random Variables

### 5.4.1 The birthday paradox

**Assumptions**

1. $n=365$ for number of days in a year.
2. birthdays are uniformly distributed.
3. birthdays are independent.

#### The probability that at least two of the birthdays match

Let $b_i$ be the day of the year on which person $i$'s birthday falls, where $1\le b_i\le n$. So $P(b_i=r)=1/n$, $1\le i\le k$, $1\le r\le n$.

The probability that $i$'s birthday and $j$'s birthday both fall on day $r$ is
$$
P(b_i=r,b_j=r)=P(b_i=r)P(b_j=r)=\frac{1}{n^2}
$$
The probability that they both fall on the same day is
$$
\sum_{r=1}^{n}P(b_i=r,b_j=r)=n\cdot\frac{1}{n^2}=\frac{1}{n}
$$
The probability that at least two of the birthdays match is 1 minus the probability that all the birthdays are different. The event $B_k$ that $k$ people have distinct birthdays is
$$
B_k=\bigcap_{i=1}^{k}A_i
$$
where $A_i$ is the event that person $i$'s birthday is different from person $j$'s for all $j < i$. Then $B_k=A_{k}\cap B_{k-1}$, then $P(B_k)=P(B_{k-1})P(A_k|B_{k-1})$.
$$
\begin{align*}
P(B_k)&=P(B_1)P(A_2|B_1)\cdots P(A_k|B_{k-1})\\
&=1\cdot\left(1-\frac{1}{n}\right)\left(1-\frac{2}{n}\right)\cdots\left(1-\frac{k-1}{n}\right)
\end{align*}
$$
And using $1+x\le e^x$,
$$
\begin{align*}
P(B_k)&=1\cdot\left(1-\frac{1}{n}\right)\left(1-\frac{2}{n}\right)\cdots\left(1-\frac{k-1}{n}\right)\\
&\le e^{-\sum_{i=1}^{k-1}i/n}\\
&=e^{-k(k-1)/2n}
\end{align*}
$$
To obtain the probability that at least two of the birthdays match, assume it is $p$, then
$$
\begin{align*}
P(B_k)&=e^{-k(k-1)/2n}\le(1-p)\\
k(k-1)&\ge2n\ln(1-p)\\
k&\ge\frac{1+\sqrt{8n\ln(1-p)}}{2}
\end{align*}
$$
Here $k$ is larger than the exact value.

#### The expectation of the number of pairs of individuals having the same birthday

For each pair $(i,j)$ of the $k$ people in the room, define the indicator random variable $X_{ij}$ , for $1\le i<j\le k$​, by
$$
\begin{align*}
X_{ij}&=I(\text{person $i$ and person $j$ have the same birthday})\\
&=\begin{cases}
1&\text{person $i$ and person $j$ have the same birthday}\\
0&\text{otherwise}
\end{cases}
\end{align*}
$$
By discussion above
$$
\mathbb{E}[X_{ij}]=P(\text{person $i$ and person $j$ have the same birthday})=\frac{1}{n}
$$
Letting $X$ be the random variable that counts the number of pairs of individuals having the same birthday, we have
$$
X=\sum_{i=1}^{k-1}\sum_{j=i+1}^{k}X_{ij}
$$
Then the expectation of the number of pairs of individuals having the same birthday is
$$
\mathbb{E}[X]=\mathbb{E}\left[\sum_{i=1}^{k-1}\sum_{j=i+1}^{k}X_{ij}\right]=\sum_{i=1}^{k-1}\sum_{j=i+1}^{k}\mathbb{E}[X_{ij}]=\frac{k(k-1)}{2n}
$$

### 5.4.2 Balls and bins

Consider a process in which you randomly toss identical balls into $b$ bins. The probability that a tossed ball lands in any given bin is $1/b$.

- How many balls fall in a given bin?
    If follows the binomial distribution $\mathrm{Bin}(n,1/b)$. The expectation is $n/b$.
- How many balls must you toss, on the average, until a given bin contains a ball?
    If follows the geometric distribution $\mathrm{Geom}(1/b)$. The expectation is $b$.

#### How many balls must you toss until every bin contains at least one ball?

A toss in which a ball falls into an empty bin a "hit". Let $A_i$ be the tosses after the $(i-1)$ th hit up to and including the $i$ th hit. And $n_i$ be the number of tosses in the $A_i$. Each $A_i$ follows the Bernoulli distribution $\mathrm{Bern}((b-i+1)/b)$ for hit or not hit. Then $n_i=b/(b-i+1)$ and $n_1=1$. 
$$
\begin{align*}
\mathbb{E}[n]&=\mathbb{E}\left[\sum_{i=1}^bn_i\right]=\sum_{i=1}^b\mathbb{E}[n_i]\\
&=\sum_{i=1}^b\frac{b}{b-i+1}=\sum_{i=1}^b\frac{b}{i}\\
&=b(\ln b+O(1))
\end{align*}
$$

### 5.4.3 Streaks

Suppose that you flip a fair coin $n$ times. What is the longest streak of consecutive heads that you expect to see?

First prove it is $O(\log n)$.

$A_{ij}$: A streak of heads of length at least $j$ begins with the $i$ th coin flip, $1\le j\le n$, $1\le i\le n-i+1$. Then $P(A_{ij})=1/2^j$.

$L_j$: The longest streak of heads has length exactly $j$.

$L$: The length of the longest streak.
$$
\begin{align*}
\mathbb{E}[L]&=\sum_{j=0}^{n}jP(L_j)\\
&=\sum_{j=0}^{2\lceil\log n\rceil-1}jP(L_j)+\sum_{j=2\lceil\log n\rceil}^{n}jP(L_j)\\
&<\sum_{j=0}^{2\lceil\log n\rceil-1}(2\lceil\log n\rceil)P(L_j)+\sum_{j=2\lceil\log n\rceil}^{n}nP(L_j)\\
&=2\lceil\log n\rceil\sum_{j=0}^{2\lceil\log n\rceil-1}P(L_j)+n\sum_{j=2\lceil\log n\rceil}^{n}P(L_j)\\
&<2\lceil\log n\rceil+n/n\\
&=O(\log n)
\end{align*}
$$

---

Since $\sum_{j=0}^{n}P(L_j)=1$, then $\sum_{j=0}^{2\lceil\log n\rceil-1}P(L_j)\le1$.

And
$$
\begin{align*}
\sum_{j=2\lceil\log n\rceil}^{n}P(L_j)&=P\biggl(\bigcup_{i=1}^{n-2\lceil\log n\rceil+1}A_{i,2\lceil\log n\rceil}\biggr)\\
&\le\sum_{i=1}^{n-2\lceil\log n\rceil+1}P(A_{i,2\lceil\log n\rceil})\\
&=\sum_{i=1}^{n-2\lceil\log n\rceil+1}\frac{1}{2^{2\lceil\log n\rceil}}\le\sum_{i=1}^{n-2\lceil\log n\rceil+1}\frac{1}{2^{2\log n}}\\
&=\sum_{i=1}^{n-2\lceil\log n\rceil+1}\frac{1}{n^2}\le n\cdot\frac{1}{n^2}\\
&=\frac{1}{n}
\end{align*}
$$

---

Then prove it is $\Omega(\log n)$.
$$
\begin{align*}
\mathbb{E}[L]&=\sum_{j=0}^{n}jP(L_j)\\
&=\sum_{j=0}^{\lfloor(\log n)/2\rfloor-1}jP(L_j)+\sum_{j=\lfloor(\log n)/2\rfloor}^{n}jP(L_j)\\
&\ge0+\sum_{j=\lfloor(\log n)/2\rfloor}^{n}(\lfloor(\log n)/2\rfloor)P(L_j)\\
&=\lfloor(\log n)/2\rfloor\sum_{j=\lfloor(\log n)/2\rfloor}^{n}P(L_j)\\
\end{align*}
$$

---

$$
\begin{align*}
\sum_{j=\lfloor(\log n)/2\rfloor}^{n}P(L_j)&=P\biggl(\bigcup_{i=1}^{n-\lfloor(\log n)/2\rfloor+1}A_{i,\lfloor(\log n)/2\rfloor}\biggr)\\
P(A_{i,\lfloor(\log n)/2\rfloor})=\frac{1}{2^{\lfloor(\log n)/2\rfloor}}\ge\frac{1}{\sqrt{n}}
\end{align*}
$$




### Exercises

#### 5.4-1

> How many people must there be in a room before the probability that someone has the same birthday as you do is at least $1/2$? How many people must there be before the probability that at least two people have a birthday on July 4 is greater than $1/2$?

The probability that someone has the same birthday as you do is 1 minus the probability that no one has the same birthday as you do. The event $B_k$ that $k$ people have distinct birthdays as you do is
$$
B_k=\bigcap_{i=1}^{k}A_i
$$
where $A_i$ is the event that person $i$'s birthday is different from you. Since each $A_i$ is independent
$$
\begin{align*}
P(B_k)&=P(A_1)P(A_2)\cdots P(A_k)\\
&=\left(1-\frac{1}{n}\right)^k\le\frac{1}{2}\\
k&\ge\frac{1}{\log_2365-\log_2364}=253
\end{align*}
$$
Let $X$ be the event that the number of individuals whose birthday is on July 4. For $X=0$, it is the same situation above. For $X=1$, there is one whose birthday is on July 4 with probability $1/n$. And there is $k$ options, so it is
$$
\begin{align*}
P(X\ge2)&=1-P(X=1)-P(X=0)\\
&=1-\binom{k}{1}\frac{1}{n}\left(1-\frac{1}{n}\right)^{k-1}-\left(1-\frac{1}{n}\right)^k\\
&=1-\left(1-\frac{1}{n}\right)^{k-1}\frac{k+n-1}{n}>\frac{1}{2}
\end{align*}
$$
Here $k\ge613$.

#### 5.4-2

> How many people must there be in a room before the probability that two people have the same birthday is at least 0.99? For that many people, what is the expected number of pairs of people who have the same birthday?

Using the same formula in this chapter
$$
\begin{align*}
P(B_k)&=1\cdot\left(1-\frac{1}{n}\right)\left(1-\frac{2}{n}\right)\cdots\left(1-\frac{k-1}{n}\right)\\
&\le e^{-k(k-1)/2n}\le(1-0.99)
\end{align*}
$$
Here $k\ge57$. The expected number of pairs of people who have the same birthday are

$$
\mathbb{E}[X]=\frac{k(k-1)}{2n}=4.37
$$

#### 5.4-3

> You toss balls into $b$ bins until some bin contains two balls. Each toss is independent, and each ball is equally likely to end up in any bin. What is the expected number of ball tosses?

The same as the birthday paradox, which is
$$
1\cdot\left(1-\frac{1}{b}\right)\left(1-\frac{2}{b}\right)\cdots\left(1-\frac{k-1}{b}\right)\le e^{-k(k-1)/2b}
$$

#### 5.4-4

> For the analysis of the birthday paradox, is it important that the birthdays be mutually independent, or is pairwise independence sufficient? Justify your answer.

Pairwise independent. If $a$ and $b$, $a$ and $c$, $b$ and $c$ are pairwise independence, then we can not infer that $a$, $b$ and $c$ are mutually independent.

#### 5.4-5

> How many people should be invited to a party in order to make it likely that there are three people with the same birthday?

For each pair $(i,j,k)$ of the $l$ people in the room, define the indicator random variable $X_{ijk}$ , for $1\le i<j<k\le l$​, by
$$
\begin{align*}
X_{ijk}&=I(\text{person $i$, $j$ and $k$ have the same birthday})\\
&=\begin{cases}
1&\text{person $i$, $j$ and $k$ have the same birthday}\\
0&\text{otherwise}
\end{cases}
\end{align*}
$$
Easy to prove that
$$
\mathbb{E}[X_{ijk}]=P(\text{person $i$, $j$ and $k$ have the same birthday})=\frac{1}{n^2}
$$
Letting $X$ be the random variable that counts the number of pairs of individuals having the same birthday, we have
$$
X=\sum_{i=1}^{l-2}\sum_{j=i+1}^{l-1}\sum_{k=j+1}^{l}X_{ijk}
$$
Then the expectation of the number of pairs of individuals having the same birthday is
$$
\mathbb{E}(X)=\sum_{i=1}^{l-2}\sum_{j=i+1}^{l-1}\sum_{k=j+1}^{l}\mathbb{E}[X_{ijk}]=\frac{l(l-1)(l-2)}{6n^2}>1
$$

Then $l\ge94$.

#### 5.4-6

> What is the probability that a $k$-string (defined on page 1179) over a set of size $n$ forms a $k$-permutation? How does this question relate to the birthday paradox?

$k$ is the number of people, $n$ is the number of days. If forms a $k$-permutation, then all the people have different birthday.

#### 5.4-7 

> You toss $n$ balls into $n$ bins, where each toss is independent and the ball is equally likely to end up in any bin. What is the expected number of empty bins? What is the expected number of bins with exactly one ball?

Let $X_i$ be the event that the $i$ th box is empty, that is, no ball will fall into this box. The probability of a ball to fall into a specific box is $1/n$, and $(n-1)/n$ for not fall into. So for $n$ balls, it is $[(n-1)/n]^n$, which means
$$
\mathbb{E}(X_i)=\left(\frac{n-1}{n}\right)^n
$$
For $n$ boxes:
$$
\mathbb{E}(X)=\sum_{i=1}^n\mathbb{E}(X_i)=n\left(\frac{n-1}{n}\right)^n
$$
Let $X_i$ be the event that the $i$ th box has exactly 1 ball. The probability of a ball to fall into a specific box is $1/n$, and $(n-1)/n$ for not fall into. So for $n$ balls, one of them will fall into and others won't, the probability is
$$
\mathbb{E}(X_i)=\binom{n}{1}\frac{1}{n}\left(\frac{n-1}{n}\right)^{n-1}
$$
For $n$ boxes:
$$
\mathbb{E}(X)=\sum_{i=1}^n\mathbb{E}(X_i)=n\left(\frac{n-1}{n}\right)^{n-1}
$$

#### 5.4-8

> Sharpen the lower bound on streak length by showing that in $n$ flips of a fair coin, the probability is at least $1-1/n$ that a streak of length $\log n-2\log\log n$ consecutive heads occurs.



