# Chapter 2 Getting Started

## 2.1 Insertion Sort

当我们想对数字进行分类时，往往是因为它们是与其他数据相关的*键*，我们称之为*卫星数据*。键和卫星数据一起构成了一条*记录*。
When we want to sort numbers, it's often because they are the *keys* associated with other data, which we call *satellite data*. Together, a key and satellite data form a *record*.

```python
def InsertionSort(A, n):
    for i in range(1, n + 1):
        key = A[i]
        # Insert A[i] into the sorted subarray A[1:i - 1].
        j = i - 1
        while j > 0 and A[j] > key:
            A[j + 1] = A[j]
            j -= 1
        A[j + 1] = key
```

**循环不变式 (Loop invariants)**

初始化：循环的第一次迭代之前，它为真。
Initialization: It is true prior to the first iteration of the loop.

保持：如果循环的某次迭代之前它为真，那么下次迭代之前它仍为真。
Maintenance: If it is true before an iteration of the loop, it remains true before the next iteration.

终止：在循环终止时，不变式通常与循环终止的原因一起提供一个性质以帮助我们验证该算法的正确性。
Termination: The loop terminates, and when it terminates, the invariant--usually along with the reason that the loop terminated--gives us a useful property that helps show that the algorithm is correct.

### Exercises

#### 2.1-1

> Using Figure 2.2 as a model, illustrate the operation of `INSERTION-SORT` on an array initially containing the sequence $\langle31, 41, 59, 26, 41, 58\rangle$.

1. $\langle{\color{red}31}, 41, 59, 26, 41, 58\rangle$
2. $\langle31, {\color{red}41}, 59, 26, 41, 58\rangle$
3. $\langle31, 41, {\color{red}59}, 26, 41, 58\rangle$
4. $\langle{\color{red}26}, 31, 41, 59, 41, 58\rangle$
5. $\langle26, 31, 41, {\color{red}41}, 59, 58\rangle$
6. $\langle26, 31, 41, 41, {\color{red}58}, 59\rangle$

#### 2.1-2

> Consider the procedure `SUM-ARRAY` on the facing page. It computes the sum of the $n$ numbers in array $A[1:n]$ State a loop invariant for this procedure, and use its initialization, maintenance, and termination properties to show that the `SUM-ARRAY` procedure returns the sum of the numbers in $A[1:n]$.
>
> ```python
> def SumArray(A, n):
>        sum = 0
>        for i in range(1, n + 1):
>            sum += A[i]
>        return sum
> ```

Loop invariant: The $sum$ records the sum of elements in $A[1:i]$. 

Initialization: $sum = 0$, $i = 0$, 0 element are summed.

Maintenance: $i$ of all element are summed.

Termination: $n$ of all elements are summed, the result is record by $sum$.

#### 2.1-3

> Rewrite the `INSERTION-SORT` procedure to sort into monotonically decreasing instead of monotonically increasing order.

```python
def InsertionSort(A, n):
    for i in range(1, n + 1):
        key = A[i]
        # Insert A[i] into the sorted subarray A[1:i - 1].
        j = i - 1
        while j > 0 and A[j] < key:
            A[j + 1] = A[j]
            j -= 1
        A[j + 1] = key
```

#### 2.1-4

> Consider the searching problem:
>
> Input: A sequence of $n$ numbers $\langle a_1, a_2, \dots, a_n\rangle$ stored in array $A[1:n]$ and a value $x$.
>
> Output: An index $i$ such that $x$ equals $A[1:n]$ or the special value $NIL$ if $x$ does not appear in $A$.
>
> Write pseudocode for linear search, which scans through the array from beginning to end, looking for $x$. Using a loop invariant, prove that your algorithm is correct. Make sure that your loop invariant fulfills the three necessary properties.

```python
def LinearSearch(A, n, x):
    for i in range(1, n + 1):
        if x == A[i]:
            return i
    return None
```

Loop invariant: The index indicate that $x$ is not in $A[1:i]$.

Initialization: $i = 0$, 0 element not equal to $x$.

Maintenance: First $i$ element not equal to $x$.

Termination: $n$ of all elements not equal to $x$. Or the index that $A[i]$ is $x$.

#### 2.1-5

> Consider the problem of adding two $n$-bit binary integers $a$ and $b$, stored in two $n$-element arrays $A[0:n-1]$ and $B[0:n-1]$, where each element is either 0 or 1, $a=\sum_{i=0}^{n-1}A[i]\cdot2^i$, and $b=\sum_{i=0}^{n-1}B[i]\cdot2^i$. The sum $c = a + b$ of the two integers should be stored in binary form in an $(n+1)$-element array $C[0:n]$, where $c=\sum_{i=0}^{n-1}C[i]\cdot2^i$. Write a procedure `ADD-BINARY-INTEGERS` that takes as input arrays $A$ and $B$, along with the length $n$, and returns array $C$ holding the sum.

```python
def AddBinaryIntegers(A, B, n):
    C = [0] * (n + 1)
    for i in range(n):
        quotient, remainder = divmod(A[i] + B[i], 2)
        C[i] += quotient
        C[i + 1] += remainder
    return C
```

## 2.2 Analyzing Algorithms

需考虑**输入规模 (Input Size)**，**最坏情况和平均情况 (Woest-case and Average-case Analysis)**，**增长量级 (Order of Growth)**。

### Exercises

#### 2.2-1

> Express the function $n^3/1000+100n^2+100n+3$ in terms of $\Theta$-notation.

$\Theta(n^3)$

#### 2.2-2

> Consider sorting $n$ numbers stored in array $A[1:n]$ by first finding the smallest element of $A[1:n]$ and exchanging it with the element in $A[1]$. Then find the smallest element of $A[2:n]$, and exchange it with $A[2]$. Then find the smallest element of $A[3:n]$, and exchange it with $A[3]$. Continue in this manner for the first $n-1$ elements of $A$. Write pseudocode for this algorithm, which is known as selection sort. What loop invariant does this algorithm maintain? Why does it need to run for only the first $n-1$ elements, rather than for all $n$ elements? Give the worst-case running time of selection sort in $\Theta$-notation. Is the best-case running time any better?

```python
def SelectionSort(A, n):
    for i in range(1, n):
        index = i
        for j in range(i + 1, n + 1):
            if A[j] < A[index]:
                index = j
        swap(A[i], a[index])
```

Loop invariant: The sorted array $A[1:i]$ that are first $i$ smallest elements.

Initialization: $i = 0$, 0 element are sorted.

Maintenance: Since the first $i-1$ smallest elements are sorted out, then the smallest elements in $A[i:n]$ is the $i$-th smallest element in $A[1:n]$.

Termination: First $n-1$ elements are sorted out, then $A[n]$ is the largest element.

We need to check $n-1$ round. In each round, we need to find the smallest element in $n-i$ elements. Then it is $\sum_{i=1}^{n-1}n-i=n(n-1)/2$. The worst-case is the same as average and best-case, they are all $\Theta(n^2)$.

#### 2.2-3

> Consider linear search again (see Exercise 2.1-4). How many elements of the input array need to be checked on the average, assuming that the element being searched for is equally likely to be any element in the array? How about in the worst case? Using $\Theta$-notation, give the average-case and worst-case running times of linears earch. Justify your answers.

Let $X$ be the random variable of the number of the input array that is checked with probability $P[i]$. Since the element being searched for is equally likely to be any element in the array, the $P(i)=1/n$. The expectation is $E[X]=\sum_{i=1}^{n}i/n=(n+1)/2$. Then it is $\Theta(n)$.

The worst-case checks all the $n$ elements, which is $\Theta(n)$.

#### 2.2-4

> How can you modify any sorting algorithm to have a good best-case running time?

Better cache hit rate.

## 2.3 Designing Algorithms

### 2.3.1 The Divide-and-conquer Approach

分治法在每层递归的时候都有这三个步骤：

**分解**原问题为若干子问题，这些子问题是原问题的规模较小的实例。
**Divide** the problem into one or more subproblems that are smaller instances of the same problem.

**解决**这些子问题，递归地求解各子问题。
**Conquer** the subproblems by solving them recursively.

**合并**这些子问题的解成原问题的解。
**Combine** the subproblem solutions to form a solution to the original problem.

```python
def Merge(A, p, q, r):
    nl = q - p + 1      # Length of A[p:q].
    nr = r - q          # Length of A[q + 1:r].
    left = [0] * nl
    right = [0] * nr
    for i in range(nl): # Copy A[p:q] to left[0:nl - 1].
        left[i] = A[p + i]
    for j in range(nr): # Copy A[q + 1:r] to right[0:nr - 1].
        right[j] = A[q + j + 1]
    i = 0               # i indexes the smallest remaining element in L
    j = 0               # j indexes the smallest remaining element in R
    k = p               # k indexes the next position in a to fill.
    # As long as each of the arrays L and R contains an unmerged element,
    # copy e smallest unmerged element back into A[p:r].
    while i < nl and j < nr:
        if left[i] <= right[j]:
            A[k] = left[i]
            i += 1
        else:
            A[k] = right[j]
            j += 1
        k += 1
    # Having gone through one of L and R entirely,
    # copy the remaining of the other array to the end of A[p:r].
    while i < nl:
        A[k] = left[i]
        i += 1
        k += 1
    while j < nr:
        A[k] = right[j]
        j += 1
        k += 1

def MergeSort(A, p, r):
    if p < r:
        q = (p + r) // 2       # Midpoint of A[p:r].
        MergeSort(A, p, q)     # Recursively sort A[p:q].
        MergeSort(A, q + 1, r) # Recursively sort A[q + 1:r].
        Merge(A, p, q, r)      # Merge the two sorted halves.

```

### 2.3.2 Analyzing Divide-and-conquer Algorithms

### Exercise

#### 2.3-1

> Using Figure 2.4 as a model, illustrate the operation of merge sort on an array initially containing the sequence $\langle3, 41, 52, 26, 38, 57, 9, 49\rangle$.

1. $\langle{\color{red}3, 41}, 52, 26, 38, 57, 9, 49\rangle$
2. $\langle{3, 41, \color{red}26, 52}, 38, 57, 9, 49\rangle$
3. $\langle{\color{red}3, 26, 41, 52}, 38, 57, 9, 49\rangle$
4. $\langle3, 26, 41, 52, {\color{red}38, 57}, 9, 49\rangle$
5. $\langle3, 26, 41, 52, 38, 57, {\color{red}9, 49}\rangle$
6. $\langle3, 26, 41, 52, {\color{red}9, 38, 57, 49}\rangle$
7. $\langle{\color{red}3, 9, 26, 38, 41, 52, 57, 49}\rangle$

#### 2.3-2

> The test in line 1 of the `MERGE-SORT `procedure reads `if p >= r` rather than  `if p == r`. If `MERGE-SORT` is called with `p > r`, then the subarray $A[p:r]$ is empty. Argue that as long as the initial call of `MERGE-SORT(A, 1, n)` has $n\ge1$, the test `if p == r` suffices to ensure that no recursive call has `p > r`.

For $p\le r$, $p,r\in\mathbb{N}$

If $p<r$, then
$$
p\le\lfloor(p+r)/2\rfloor<r\\
q+1=\lfloor(p+r)/2+1\le r
$$
If $p=r$, then
$$
p=\lfloor(p+r)/2\rfloor=r
$$

which will be tested out.

Then $p>r$ will never happen if $p\le r$ initially.

#### 2.3-3

> State a loop invariant for the `while` loop of lines 12-18 of the `MERGE` procedure. Show how to use it, along with the `while` loops of lines 20-23 and 24-27, to prove that the `MERGE` procedure is correct.

Loop invariant: The sorted subarray $A[p,k-1]$.

#### 2.3-4

> Use mathematical induction to show that when $n\ge2$ is an exact power of 2, the solution of the recurrence
> $$
> T(n)=\begin{cases}2&n=2\\2T(n/2)+n&n>2\end{cases}
> $$
> is $T(n)=n\log n$.

$n=2^1$, $T(2)=2\log2=2$.

Suppose for $n=2^k$, $T(2^k)=2^k\log2^k=k2^k$ holds.

Since for $n=2^{k+1}$, $T(2^{k+1})=2T(2^k)+2^{k+1}=(k+1)2^{k+1}$ also holds, then $T(n)=n\log n$.

#### 2.3-5

> You can also think of insertion sort as a recursive algorithm. In order to sort $A[1:n]$, recursively sort the subarray $A[1:n-1]$ and then insert $A[n]$ into the sorted subarray $A[1:n-1]$. Write pseudocode for this recursive version of insertion sort. Give a recurrence for its worst-case running time.

```python
def InsertionSort(A, n):
    if n == 1:
        return
    InsertionSort(A, n - 1)
    i = n - 1
    while i > 0 and A[i] > A[n]:
        A[i + 1] = A[i]
        i -= 1
    A[i + 1] = A[n]
```

The recurrence is:
$$
T(n)=\begin{cases}\Theta(1)&n=1\\T(n-1)+\Theta(n)&n>1\end{cases}
$$
and the solution is $\Theta(n^2)$.

#### 2.3-6

> Referring back to the searching problem (see Exercise 2.1-4), observe that if the subarray being searched is already sorted, the searching algorithm can check the midpoint of the subarray against $v$ and eliminate half of the subarray from further consideration. The *binary search* algorithm repeats this procedure, halving the size of the remaining portion of the subarray each time. Write pseudocode, either iterative or recursive, for binary search. Argue that the worst-case running time of binary search is $\Theta(\log n)$.

```python
def BinarySearch(A, p, r, x):
    while p < r:
        q = (p + r) // 2
        if A[q] < x:
            p = q + 1
        elif A[q] > x:
            r = q - 1
        else:
            return q
    return None
```

Each time we will harve half the size of the subarray, so the recurrence is:
$$
T(n)=\begin{cases}\Theta(1)&n=1\\T(n/2)+\Theta(1)&n>1\end{cases}
$$
and the solution is $\Theta(\log n)$.

#### 2.3-7

> The `while` loop of lines 5-7 of the `INSERTION-SORT` procedure in Section 2.1 uses a linear search to scan (backward) through the sorted subarray `A[1:j-1]`. What if insertion sort used a binary search (see Exercise 2.3-6) instead of a linear search? Would that improve the overall worst-case running time of insertion sort to $\Theta(n\log n)$?

You can optimize the searching time, but for the searched position, we need to move the elements behind this position one by on, so we can not optimize the copy time. So the worst-case running time is $\Theta(n\log n)$.

#### 2.3-8

> Describe an algorithm that, given a set $S$ of $n$ integers and another integer $x$, determines whether $S$ contains two elements that sum to exactly $x$. Your algorithm should take $\Theta(n\log n)$ time in the worst case.

Soring ($\Theta(n\log n)$) + binary search ($n\cdot\Theta(\log n)$) = $\Theta(n\log n)$.

## Problems

### 2-1 Insertion sort on small arrays in merge sort

> Although merge sort runs in $\Theta(n\log n)$ worst-case time and insertion sort runs in $\Theta(n^2)$ worst-case time, the constant factors in insertion sort can make it faster in practice for small problem sizes on many machines. Thus it makes sense to coarsen the leaves of the recursion by using insertion sort within merge sort when subproblems become sufficiently small. Consider a modification to merge sort in which $n/k$ sublists of length $k$ are sorted using insertion sort and then merged using the standard merging mechanism, where $k$ is a value to be determined.

> a. Show that insertion sort can sort the $n/k$ sublists, each of length $k$, in $\Theta(nk)$ worst-case time.

$$
\Theta(k^2)\cdot n/k=\Theta(nk)
$$

> b. Show how to merge the sublists in $\Theta(n\log(n/k))$ worst-case time.

For the merge, the recurrence is
$$
T(n)=\begin{cases}c_1k&n\le k\\2T(n/2)+c_2n&n>k\end{cases}
$$
Then the height of the recursion tree is $\log(n/k)+1$, each takes $\Theta(n)$ time, so it is $\Theta(n\log(n/k))$ worst-case time.

> c. Given that the modified algorithm runs in $\Theta(nk+n\log(n/k))$ worst-case time, what is the largest value of $k$ as a function of $n$ for which the modified algorithm has the same running time as standard merge sort, in terms of $\Theta$-notation?

Since $\Theta(nk+n\log(n/k))=\Theta(n\log n)$, and
$$
\Theta(nk+n\log n-n\log k)
$$
Then $k=\Theta(\log n)$.

### 2-2 Correctness of bubblesort

> Bubblesort is a popular, but inefficient, sorting algorithm. It works by repeatedly swapping adjacent elements that are out of order. The procedure `BUBBLESORT` sorts array $A[1:n]$.
>
> ```python
> def BubbleSort(A, n):
>     for i in range(1, n):
>         for j in range(n, i, -1):
>             if A[j] < A[j - 1]:
>                 swap(A[j], A[j-1])
> ```

> a. Let $A'$ denote the array $A$ after `BUBBLESORT(A, n)` is executed. To prove that `BUBBLESORT` is correct, you need to prove that it terminates and that
> $$
> A'[1]\le A'[2]\le\cdots\le A'[n]
> $$
> In order to show that `BUBBLESORT` actually sorts, what else do you need to prove?

$A'$ contains all the elements in $A$.

> b. State precisely a loop invariant for the `for` loop in lines 2-4, and prove that this loop invariant holds. Your proof should use the structure of the loop-invariant proof presented in this chapter.

Loop invariant: The $j$ record the smallest element in $A[j:n]$.

Initialization: $j=n$, $A[n]$ is currently the smallest element.

Maintenance: If $A[j-1]$ is larger than $A[j]$, then it will be swap to $j$'s position, and in next iteration, $j$ becomes $j-1$, which is still point to the smallest element. If smaller, no swapping and $j-1$ is already the smallest.

Termination: $j=i+1$, elements in $A[i+2:n]$ are all larger than $A[i+1]$. So the smaller one in $A[i]$ and $A[i+1]$ will be the final element in $i$'s position.

> c. Using the termination condition of the loop invariant proved in part (b), state a loop invariant `for` the for loop in lines 1-4 that allows you to prove inequality. Your proof should use the structure of the loop-invariant proof presented in this chapter.

Loop invariant: The sorted array $A[1:i]$ that are first $i$ smallest elements.

Initialization: $i = 0$, 0 element are sorted.

Maintenance: Since the first $i-1$ smallest elements are sorted out, in the inner loop, there exists the smallest elements in the remaining unsorted elements, and it will be poped out to $A[i]$.

Termination: First $n-1$ elements are sorted out, then $A[n]$ is the largest element.

> d. What is the worst-case running time of `BUBBLESORT`? How does it compare with the running time of `INSERTION-SORT`?

The reserved order array will be the worst-case. It is $\Theta(n^2)$.

### 2-3 Correctness of Horner's rule

> You are given the coefficents $a_0,a_1,\dots,a_n$ of a polynomial
> $$
> P(x)=\sum_{k=0}^{n}a_kx^k
> $$
> and you want to evaluate this polynomial for a given value of $x$. Horner's rule says to evaluate the polynomial according to this parenthesization:
> $$
> P(x)=a_0+x(a_1+x(a_2+\dots+x(a_{n-1}+xa_n)\cdots))
> $$
> The procedure `HORNER` implements Horner's rule to evaluate $P(x)$, given the coefficients $a_0,a_1,\dots,a_n$ in an array $A[0:n]$ and the value of $x$.
>
> ```python
> def Horner(A, n, x):
>     p = 0
>     for i in range(n, -1, -1):
>         p = A[i] + x * p
>     return p
> ```

> a. In terms of $\Theta$-notation, what is the running time of this procedure?

The recurrence is:
$$
T(n)=\begin{cases}\Theta(1)&n=1\\T(n-1)+\Theta(1)&n>1\end{cases}
$$
and the solution is $\Theta(n)$.

> b. Write pseudocode to implement the naive polynomial-evaluation algorithm that computes each term of the polynomial from scratch. What is the running time of this algorithm? How does it compare with `HORNER`?

```python
def NaiveHorner(A, n, x):
    p = 0
    for i in range(n + 1):
        xi = 1
        for j in range(i):
            xi *= x
        p += A[i] * xi
    return p
```

Obviously it is $\Theta(n^2)$, which is slower.

> c. Consider the following loop invariant for the procedure `HORNER`:
>
> At the start of each iteration of the for loop of lines 2-3,
> $$
> p=\sum_{k=0}^{n-(i+1)}A[k+i+1]\cdot x^k
> $$
> Interpret a summation with no terms as equaling 0. Following the structure of the loop-invariant proof presented in this chapter, use this loop invariant to show that, at termination, $p=\sum_{k=0}^nA[k]\cdot x^k$.

Initialization: $p=0$ is obvious.

Maintenace: 
$$
\begin{align*}
p&=A[i]+x\cdot\Biggl[\sum_{k=0}^{n-(i+1)}A[k+i+1]\cdot x^k\Biggr]\\
&=A[i]+\Biggl[\sum_{k=0}^{n-(i+1)}A[k+i+1]\cdot x^{k+1}\Biggr]\\
&=A[i]+\Biggl[\sum_{k=1}^{n-i}A[k+i]\cdot x^k\Biggr]\\
&=\sum_{k=0}^{n-i}A[k+i]\cdot x^k
\end{align*}
$$
Termination: $i=0$, the same equation.

### 2-4 Inversions

> Let $A[1:n]$ be an array of $n$ distinct numbers. If $i < j$ and $A[i]>A[j]$, then the pair $(i,j)$ is called an inversion of $A$.

> a. List the five inversions of the array $\langle2,3,8,6,1\rangle$.

(1, 5), (2, 5), (3, 4), (3, 5), (4, 5).

> b. What array with elements from the set $\{1,2,\dots,n\}$ has the most inversions? How many does it have?

$\langle n,n-1,\dots,1\rangle$, which has $n(n-1)/2$ inversions.

> c. What is the relationship between the running time of insertion sort and the number of inversions in the input array? Justify your answer.

The number of swapping equals to the number of inversions. The inner loop of the pseudo code of insertion sort swaps the number of inversions end with $i$ times. So the summation must be the number of inversions of $A$.

> d. Give an algorithm that determines the number of inversions in any permutation on $n$ elements in $\Theta(n\log n)$ worst-case time. (Hint: Modify merge sort.)

Shown in pseudo code below:

```python
def Count(A, p, q, r):
    nl = q - p + 1
    nr = r - q
    left = [0] * nl
    right = [0] * nr
    for i in range(nl):
        left[i] = A[p + i]
    for j in range(nr):
        right[j] = A[q + j + 1]
    i = 0
    j = 0
    k = p
    count = 0
    while i < nl and j < nr:
        if left[i] <= right[j]:
            A[k] = left[i]
            i += 1
        else:
            count += nl - i + 1
            A[k] = right[j]
            j += 1
        k += 1
    while i < nl:
        count += nr
        A[k] = left[i]
        i += 1
        k += 1
    while j < nr:
        A[k] = right[j]
        j += 1
        k += 1
    return count

def CountInversions(A, p, r):
    count = 0
    if p < r:
        q = (p + r) // 2
        count += Count(A, p, q)
        count += Count(A, q + 1, r)
        count += CountInversions(A, p, q, r)
    return count

```

