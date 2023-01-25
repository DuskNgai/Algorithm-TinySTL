# Chapter 6 Heap Sort

## 6.1 Heaps

*（二叉）堆*是一个数组，可以被看成一个完全二叉树。
The *(binary) heap* data structure is an array object that we can view as a nearly
complete binary tree.

```python
# If root is A[1].
parent = lambda x: x >> 1
left   = lambda x: x << 1
right  = lambda x: (x << 1) + 1
```

堆分为最大堆和最小堆。最大堆的每个节点中的值不小于其两个子节点的值。最小堆则相反。
There are two kinds of binary heaps: max-heaps and min-heaps. In a max-heap, the max-heap property is that for every node $i$ other than the root, the value of a node is at most the value of its parent.

把堆看作一棵树，我们把堆中一个节点的*高度*定义为从该节点到叶子的最长的简单下行路径上的边的数量，我们把堆的高度定义为根的高度。
Viewing a heap as a tree, we define the *height* of a node in a heap to be the number of edges on the longest simple downward path from the node to a leaf, and we define the height of the heap to be the height of its root.

### Exercises

#### 6.1-1

> What are the minimum and maximum numbers of elements in a heap of height $h$?

Since heap is a complete binary tree, then minimum is $2^h$, maximum is $2^{h+1}-1$. 

#### 6.1-2

> Show that an $n$-element heap has height $\lfloor\log n\rfloor$.

Rewrite $n=2^h-1+m$, and $1\le m\le 2^h$. Since
$$
\lfloor\log n\rfloor\le h<\log n<\lfloor\log(n+1)\rfloor\le h+1=\lfloor\log n\rfloor+1
$$
Then an $n$-element heap has height $\lfloor\log n\rfloor$.

#### 6.1-3

> Show that in any subtree of a max-heap, the root of the subtree contains the largest value occurring anywhere in that subtree.

Since for any node other than root in a max-heap, the value of this node is no less than its children nodes.

#### 6.1-4

> Where in a max-heap might the smallest element reside, assuming that all elements are distinct?

In the leaf nodes. Or the second half of the array.

#### 6.1-5

> At which levels in a max-heap might the $k$ th largest element reside, for $2\le k\le \lfloor n/2\rfloor$, assuming that all elements are distinct?

If the $k$ th largest element is on the simple downward path from the root, the it is $k-1$. If the elements are in descending order, then it is $\lfloor\log k\rfloor$. So it is $[\lfloor\log k\rfloor,k-1]$.

#### 6.1-6

> Is an array that is in sorted order a min-heap?

Yes. The parent node is always no less than its left or right node.

#### 6.1-7

> Is the array with values $\langle33, 19, 20, 15, 13, 10, 2, 13, 16, 12\rangle$ a max-heap?

No. $(15, 16)$ pair.

#### 6.1-8

> Show that, with the array representation for storing an $n$-element heap, the leaves are the nodes indexed by $\lfloor n/2\rfloor+1,\lfloor n/2\rfloor+2,\dots,n$.

A leaf node has no child. So the parent node of the node $n$ is $\lfloor n/2\rfloor$. Then $\lfloor n/2\rfloor+1,\lfloor n/2\rfloor+2,\dots,n$ are all leaves.

## 6.2 Maintaining the heap property

```python
def MaxHeapify(A, i):
    l = left(i)
    r = right(i)
    if l <= A.heap_size and A[l] > A[i]:
        largest = l
    else:
        largest = i
    if r <= A.heap_size and A[r] > A[largest]:
        largest = r
    if largest != i:
        swap(A[i], A[largest])
        MaxHeapify(A, largest)
```

Let $T(n)$ be the worst-case running time that the procedure takes on a subtree of size at most $n$. The running time is the $\Theta(1)$ time to fix up the relationships among the elements $A[i]$, $A[\mathrm{left}(i)]$, and $A[\mathrm{right}(i)]$. The children’s subtrees each have size at most $2n/3$.
$$
T(n)\le T(2n/3)+\Theta(1)
$$
$T(n)=O(\log n)$

### Exercises

#### 6.2-1

> Using Figure 6.2 as a model, illustrate the operation of `MAX-HEAPIFY(A, 3)` on the array $A = \langle27, 17, 3, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0\rangle$.

$\langle27, 17, {\color{red}3}, 16, 13, 10, 1, 5, 7, 12, 4, 8, 9, 0\rangle$

$\langle27, 17, {\color{red}10}, 16, 13, {\color{red}3}, 1, 5, 7, 12, 4, 8, 9, 0\rangle$

$\langle27, 17, {\color{red}10}, 16, 13, {\color{red}9}, 1, 5, 7, 12, 4, 8, {\color{red}3}, 0\rangle$

#### 6.2-2

> Show that each child of the root of an $n$-node heap is the root of a subtree containing at most $2n/3$ nodes. What is the smallest constant $\alpha$ such that each subtree has at most $\alpha n$ nodes? How does that affect the recurrence (6.1) and its solution?

Consider a heap whose height of left sub-tree is $h-1$ and right sub-tree is $h-2$. Then the number of nodes in left sub-tree is $2^{h}-1$, right sub-tree is $2^{h-1}-1$, the whole tree is $3\cdot2^{h-1}-1$. If we choose the left node, then it contains
$$
\frac{2^{h}-1}{3\cdot2^{h-1}-1}\approx\frac{2}{3}
$$
of nodes.

If the left sub-tree and the right sub-tree have the same number of nodes, then $\alpha=1/2$. The recurrence becomes $T(n)\le T(n/2)+\Theta(1)$, the solution is $O(\log n)$.

#### 6.2-3

> Starting with the procedure `MAX-HEAPIFY`, write pseudocode for the procedure `MIN-HEAPIFY(A, i)`, which performs the corresponding manipulation on a min-heap. How does the running time of `MIN-HEAPIFY` compare with that of `MAX-HEAPIFY`?

```python
def MinHeapify(A, i):
    l = left(i)
    r = right(i)
    if l <= A.heap_size and A[l] < A[i]:
        smallest = l
    else:
        smallest = i
    if r <= A.heap_size and A[r] < A[smallest]:
        smallest = r
    if smallest != i:
        swap(A[i], A[smallest])
        MaxHeapify(A, smallest)
```

The running time is the same.

#### 6.2-4

> What is the effect of calling `MAX-HEAPIFY(A, i)` when the element $A[i]$ is larger than its children?

It is a base case of the recursion, which does not satisfy line 10. It will do nothing and stop the function call recursively.

#### 6.2-5

> What is the effect of calling `MAX-HEAPIFY(A, i)` for `i > A.heap_size / 2`.

It is a base case of the recursion, which does not satisfy line 4, 5, 10. It will do nothing and stop the function call recursively.

#### 6.2-6

> The code for `MAX-HEAPIFY` is quite efficient in terms of constant factors, except possibly for the recursive call in line 10, for which some compilers might produce inefficient code. Write an efficient `MAX-HEAPIFY` that uses an iterative control construct (a loop) instead of recursion.

```python
def MaxHeapify(A, i):
    while True:
        l = left(i)
        r = right(i)
        if l <= A.heap_size and A[l] > A[i]:
            largest = l
        else:
            largest = i
        if r <= A.heap_size and A[r] > A[largest]:
            largest = r
        if largest != i:
            swap(A[i], A[largest])
            i = largest
        else:
            break
```

#### 6.2-7

> Show that the worst-case running time of `MAX-HEAPIFY` on a heap of size $n$ is $\Omega(\log n)$. (Hint: For a heap with $n$ nodes, give node values that cause `MAX-HEAPIFY` to be called recursively at every node on a simple path from the root down to a leaf.)

In a worse case, the value will be swapped from the root node to a leaf node. Since the height of the heap is $\lfloor\log n\rfloor$, then it has worse-case time $\Omega(\log n)$.

## 6.3 Building a heap

```python
def BuildMaxHeap(A, n):
    A.heap_size = n
    for i in range(n / 2, 0, -1):
        MaxHeapify(A, i)
```

循环不变式：节点 $i+1,i+2,\dots,n$ 都是对应最大堆的根。
Loop invariant: each node $i+1,i+2,\dots,n$ is the root of a max-heap.

初始化：在循环的第一次迭代之前，$i=\lfloor n/2\rfloor$。节点 $\lfloor n/2\rfloor+1,\lfloor n/2\rfloor+2,\dots,n$ 都是叶子，也是对应最大堆的根。
Initialization: Prior to the first iteration of the loop, $i=\lfloor n/2\rfloor$. Each node $\lfloor n/2\rfloor+1,\lfloor n/2\rfloor+2,\dots,n$ is a leaf and is thus the root of a trivial max-heap.

维护：节点 $i$ 的子节点编号比 $i$ 大。这正是调用 `MAX-HEAPIFY(A, i)` 使节点 $i$ 成为最大堆根所需的条件。
Maintenance: the children of node $i$ are numbered higher than $i$. This is precisely the condition required for the call `MAX-HEAPIFY(A, i)` to make node $i$ a max-heap root.

Termination: $i=0$.

An n-element heap has height $\lfloor\log n\rfloor$, and at most $\lceil n/2^{h+1}\rceil$ nodes of any height $h$.
含 $n$ 个元素的堆高度为 $\lfloor\log n\rfloor$，高度为 $h$ 的节点最多有 $\lceil n/2^{h+1}\rceil$ 个。
$$
\sum_{h=0}^{\lfloor\log n\rfloor}\left\lceil\frac{n}{2^{h+1}}\right\rceil O(h)=\sum_{h=0}^{\lfloor\log n\rfloor}\left\lceil\frac{n}{2^{h+1}}\right\rceil ch=
cn\sum_{h=0}^{\lfloor\log n\rfloor}\left\lceil\frac{h}{2^{h+1}}\right\rceil\le cn\sum_{h=0}^{\infty}\left\lceil\frac{h}{2^{h+1}}\right\rceil=O(n)
$$
Hence, we can build a max-heap from an unordered array in linear time.
故建堆的时间复杂度为 $O(n)$。

### Exercises

#### 6.3-1

> Using Figure 6.3 as a model, illustrate the operation of `BUILD-MAX-HEAP` on the array $A=\langle5,3,17,10,84,19,6,22,9\rangle$.

$\langle5,3,17,{\color{red}22},84,19,6,{\color{red}10},9\rangle$

$\langle5,3,{\color{red}19},22,84,{\color{red}17},6,10,9\rangle$

$\langle5,{\color{red}84},19,22,{\color{red}3},17,6,10,9\rangle$

$\langle{\color{red}84},{\color{red}22},19,{\color{red}10},3,17,6,{\color{red}5},9\rangle$

#### 6.3-2

> Show that $\lceil n/2^{h+1}\rceil\ge1/2$ for $0\le h\le\lfloor\log n\rfloor$.

$$
\lceil n/2^{\lfloor\log n\rfloor+1}\rceil\ge\lceil n/2^{\log n+1}\rceil=\lceil (n/2^{\log n})/2\rceil=1\ge1/2
$$

#### 6.3-3

> Why does the loop index $i$ in line 2 of `BUILD-MAX-HEAP` decrease from $\lfloor n/2\rfloor$ to 1 rather than increase from 1 to $\lfloor n/2\rfloor$?

It may not be a max-heap. If start from node 1, values in 2 and 3 may not be the maximum value of the whole heap.

#### 6.3-4

> Show that there are at most $\lceil n/2^{h+1}\rceil$ nodes of height $h$ in any $n$-element heap.

Prove by induction.

$h=0$. Since the second half of the array are all leaf nodes, $\lceil n/2\rceil$ holds.

$h=1$. The parents of all the leaves, which are $\lceil\lceil n/2\rceil/2\rceil\ge\lceil n/2^2\rceil$.

Suppose for $h=m$, the relation holds.

Then for $h=m+1$, $\lceil\lceil n/2^m\rceil/2\rceil\ge\lceil n/2^{m+1}\rceil$ holds.

## 6.4 The heap sort algorithm

```python
def HeapSort(A, n):
    BuildMaxHeap(A, n)
    for i in range(n, 1, -1):
        swap(A[1], A[n])
        A.heap_size -= 1
        MaxHeapify(A, 1)
```

取出时间复杂度为 $O(n)$，维护 $n-1$ 次，每次 $O(\log n)$ 的时间，故时间复杂度为 $O(n\log n)$。The `HEAPSORT` procedure takes $O(n\log n)$ time, since the call to `BUILD-MAX-HEAP` takes $O(n)$ time and each of the $n-1$ calls to `MAX-HEAPIFY` takes $O(\log n)$ time.

### Exercises

#### 6.4-1

> Using Figure 6.4 as a model, illustrate the operation of `HEAPSORT` on the array $A=\langle5, 13, 2, 25, 7, 17, 20, 8, 4\rangle$.

$\langle5, 13, 2, 25, 7, 17, 20, 8, 4\rangle$

$\langle{\color{red}25}, {\color{red}13}, {\color{red}20}, {\color{red}8}, 7, {\color{red}17}, {\color{red}2}, {\color{red}5}, 4\rangle$

$\langle{\color{red}20}, {\color{red}13}, {\color{red}17}, 8, 7, {\color{red}4}, 2, 5, {\color{red}25}\rangle$

$\langle{\color{red}17}, 13, {\color{red}5}, 8, 7, 4, 2, {\color{red}20}, 25\rangle$

$\langle{\color{red}13}, {\color{red}8}, 5, {\color{red}2}, 7, 4, {\color{red}17}, 20, 25\rangle$

$\langle{\color{red}8}, {\color{red}7}, 5, 2, {\color{red}4}, {\color{red}13}, 17, 20, 25\rangle$

$\langle{\color{red}7}, {\color{red}4}, 5, 2, {\color{red}8}, 13, 17, 20, 25\rangle$

$\langle{\color{red}5}, {\color{red}4}, 2, {\color{red}7}, 8, 13, 17, 20, 25\rangle$

$\langle{\color{red}4}, {\color{red}2}, {\color{red}5}, 7, 8, 13, 17, 20, 25\rangle$

$\langle{\color{red}2}, {\color{red}4}, 5, 7, 8, 13, 17, 20, 25\rangle$

#### 6.4-2

> Argue the correctness of `HEAPSORT` using the following loop invariant:
>
> At the start of each iteration of the `for` loop of lines 2-5, the subarray $A[1:i]$ is a max-heap containing the $i$ smallest elements of $A[1:n]$, and the subarray $A[i+1:n]$ contains the $n-i$ largest elements of $A[1:n]$, sorted.

Initialization: $i=n$, $A[1:n]$ is a max-heap, no element is sorted.

Maintenance: In each iteration, the heap pops the current maximum, which is the $i$ th maximum element in $A[1:n]$ to $A[i]$. And the heap will maintain itself. So the subarray $A[1:i]$ is a max-heap containing the $i$ smallest elements of $A[1:n]$, and the subarray $A[i+1:n]$ contains the $n-i$ largest elements of $A[1:n]$, sorted.

Termination: $i=1$, $A[1]$ is the smallest element and $A[2:n]$ are sorted. Then $A[1:n]$ is sorted.

#### 6.4-3

> What is the running time of `HEAPSORT` on an array $A$ of length $n$ that is already sorted in increasing order? How about if the array is already sorted in decreasing order?

Sorted: $O(n\log n)$.

Reversed: $O(n\log n)$.

#### 6.4-4

> Show that the worst-case running time of `HEAPSORT` is $\Omega(n\log n)$.

Each `MaxHeapify` takes $\Omega(\log k)$ times where $k$ is current heap size. Add them up
$$
\sum_{i=2}^{n}\log k=\log n!\approx n\log n
$$

## 6.5 Priority queues

```python
def MaxHeapMaximum(A):
    if A.heap_size < 1:
        raise RuntimeError
    return A[1]

def MaxHeapExtractMax(A):
    max = MaxHeapMaximum(A)
    A[1] = A[A.heap_size]
    A.heap_size -= 1
    MaxHeapify(A, 1)
    return max

def MaxHeapIncreaseKey(A, x, k):
    x.key = k
    while i > 1 and A[parent(i)].key < A[i].key:
        swpa(A[i], A[parent(i)])
        i = parent(i)

def MaxHeapInsert(A, x, n):
    if A.heap_size == n:
        raise RuntimeError
    A.heap_size += 1
    k = x.key
    x.key = None
    A[A.heap_size] = x
    MaxHeapIncreaseKey(A, x, k)
```

### Exercises

## Problems

### 6.3 Young tableaus

> An $m\times n$ Young tableau is an $m\times n$ matrix such that the entries of each row are in sorted order from left to right and the entries of each column are in sorted order from top to bottom. Some of the entries of a Young tableau may be $\infty$, which we treat as non-existent elements. Thus, a Young tableau can be used to hold $r\le mn$ finite numbers.

> a. Draw a $4\times4$ Young tableau containing the elements $\{9,16,3,2,4,8,5,14,12\}$.

$$
\begin{bmatrix}2&3&5&14\\4&8&9&16\\12&\infty&\infty&\infty\\\infty&\infty&\infty&\infty
\end{bmatrix}
$$

> b. Argue that an $m\times n$ Young tableau $Y$ is empty if $Y[1,1]=\infty$. Argue that $Y$ is full (contains $mn$ elements) if $Y[m,n]<\infty$.

Since $Y[1,1]=\infty$, then all the elements in the first row is $\infty$. Thus making all the elements in the table to be $\infty$.

If $Y[m,n]<\infty$, then all the elements in the last row are less than $\infty$, then all the elements in the table are less than $\infty$.

> c. Give an algorithm to implement `EXTRACT-MIN` on a nonempty $m\times n$ Young tableau that runs in $O(m+n)$ time. Your algorithm should use a recursive subroutine that solves an $m\times n$ problem by recursively solving either an $(m-1)\times n$ or an $m\times(n-1)$ subproblem. (Hint: Think about `MAX-HEAPIFY`.) Explain why your implementation of `EXTRACT-MIN` runs in $O(m+n)$ time.

Using a min-heap-like Young tableau. After popping the smallest element, we start from top left, each time if we have not touched the boundary, we exchange the elements below or right which is the smallest among the two and move. We terminate until the element is larger than its below and right elements.
$$
T(m,n)=\begin{cases}T(m-1,n)+\Theta(1)\\T(m,n-1)+\Theta(1)\end{cases}
$$
$T(m,n)=O(m+n)$.

> d. Show how to insert a new element into a non-full $m\times n$ Young tableau in $O(m+n)$ time.

Similar procedure above.

> e. Using no other sorting method as a subroutine, show how to use an $n\times n$ Young tableau to sort $n^2$ numbers in $O(n^3)$ time.

Since moving an element takes $O(n)$ time, moving $n^2$ elements takes $O(n^3)$ time.

> f. Give an $O(m+n)$-time algorithm to determine whether a given number is
> stored in a given $m\times n$ Young tableau.  

Similar procedure in c.
