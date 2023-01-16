# Chapter 1 The Role of Algorithms in Computing

## 1.1 Algorithms

*算法*就是任何良定义的计算过程，它以某个值或一组值作为*输入*，在有限的时间内产生某个值或一组值作为*输出*。
An *algorithm* is any well-defined computational procedure that takes some value, or set of values, as *input* and produces some value, or set of values, as *output* in a finite amount of time.

也可以把算法看成是用于求解良说明的*计算问题*的工具。
You can also view an algorithm as a tool for solving a well-specified *computational problem*.

如果对于每一个作为输入的问题实例，它都会*停止*--在有限的时间内完成计算，并输出该问题实例的正确解，那么我们称一个计算问题的算法是*正确*的。一个正确的算法可以*解决*给定的计算问题。一个不正确的算法在某些输入实例上可能根本不会停止，或者它可能以一个错误的答案停止。
An algorithm for a computational problem is *correct* if, for every problem instance provided as input, it *halts*--finishes its computing in finite time--and outputs the correct solution to the problem instance. A correct algorithm *solves* the given computational problem. An incorrect algorithm might not halt at all on some input instances, or it might halt with an incorrect answer.

### Exercises

#### 1.1-1

> Describe your own real-world example that requires sorting. Describe one that requires ûnding the shortest distance between two points.

Pass.

#### 1.1-2

> Other than speed, what other measures of efficiency might you need to consider in a real-world setting?

Power.

#### 1.1-3

> Select a data structure that you have seen, and discuss its strengths and limitations.

Pass.

#### 1.1-4

> How are the shortest-path and traveling-salesperson problems given above similar? How are they different?

Both are mean to find the shortest-path. Shortest-path can pass a node many times, while TSP are required to pass once.

#### 1.1-5

> Suggest a real-world problem in which only the best solution will do. Then come up with one in which "approximately" the best solution is good enough.

Many simulation problem.

#### 1.1-6

> Describe a real-world problem in which sometimes the entire input is available before you need to solve the problem, but other times the input is not entirely available in advance and arrives over time.

Sorting/Reinforcement learning.

## 1.2 Algorithms As A Technology

### Exercises

#### 1.2-1

> Give an example of an application that requires algorithmic content at the application level, and discuss the function of the algorithms involved.

Many simulation application.

#### 1.2-2

> Suppose that for inputs of size $n$ on a particular computer, insertion sort runs in $8n^2$ steps and merge sort runs in $64n\log n$ steps. For which values of $n$ does insertion sort beat merge sort?

$$
8n^2\le64n\log n\Rightarrow2^n\le n^8
$$

We get $2\le n\le43$.

#### 1.2-3

> What is the smallest value of $n$ such that an algorithm whose running time is $100n^2$ runs faster than an algorithm whose running time is $2^n$ on the same machine?

$$
100n^2\le2^n
$$ {0}

We get $n\ge15$.

## Problem

### 1-1 Comparison of running times

For each function $f(n)$ and time $t$ in the following table, determine the largest size $n$ of a problem that can be solved in time $t$, assuming that the algorithm to solve the problem takes $f(n)$ microseconds.

|            |     1 second     |      1 minute      |       1 hour        |          1 day          |         1 month         |         1 year          |        1 century        |
| :--------: | :--------------: | :----------------: | :-----------------: | :---------------------: | :---------------------: | :---------------------: | :---------------------: |
|  $\log n$  |    $2^{10^6}$    | $2^{6\times10^7}$  | $2^{3.6\times10^9}$ | $2^{8.64\times10^{10}}$ | $2^{2.59\times10^{12}}$ | $2^{3.15\times10^{13}}$ | $2^{3.15\times10^{15}}$ |
| $\sqrt{n}$ |    $10^{12}$     | $3.6\times10^{15}$ | $1.30\times10^{18}$ |   $7.46\times10^{21}$   |   $6.72\times10^{24}$   |   $9.95\times10^{26}$   |   $9.95\times10^{30}$   |
|    $n$     |      $10^6$      |   $6\times10^7$    |   $3.6\times10^9$   |   $8.64\times10^{10}$   |   $2.59\times10^{12}$   |   $3.15\times10^{13}$   |   $3.15\times10^{15}$   |
| $n\log n$  | $6.24\times10^4$ |  $2.8\times10^6$   |  $1.33\times10^8$   |    $2.76\times10^9$     |   $7.19\times10^{10}$   |   $7.98\times10^{11}$   |   $6.86\times10^{13}$   |
|   $n^2$    |      $10^3$      |  $7.75\times10^3$  |    $6\times10^4$    |    $2.94\times10^5$     |    $1.61\times10^6$     |    $5.62\times10^6$     |    $5.62\times10^7$     |
|   $n^3$    |      $100$       |       $394$        |       $1532$        |         $4420$          |         $13736$         |         $31593$         |        $146645$         |
|   $2^n$    |       $19$       |        $25$        |        $31$         |          $36$           |          $41$           |          $44$           |          $51$           |
|    $n!$    |       $9$        |        $11$        |        $12$         |          $13$           |          $15$           |          $16$           |          $17$           |

