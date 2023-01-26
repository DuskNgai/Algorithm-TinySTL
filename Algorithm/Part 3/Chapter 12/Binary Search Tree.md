# Chapter 12 Binary Search Tree

## 12.1 What is a binary search tree?

性质：左子树节点的值不大于右子树节点的值。
The keys in a binary search tree are always stored in such a way as to satisfy the binary-search-tree property: Let $x$ be a node in a binary search tree. If $y$ is a node in the left subtree of $x$, then $y.key \le x.key$. If $y$ is a node in the right subtree of $x$, then $y.key \ge x.key$.

```python
def InorderTreeWalk(x):
    if x is not None:
        InorderTreeWalk(x.left)
        print(x.key)
        InorderTreeWalk(x.right)
```

### Theorem 12.1

如果 $x$ 是一个有 $n$ 个结点子树的根，中序遍历需要 $\Theta(n)$ 的时间。
If $x$ is the root of an $n$-node subtree, then the call `INORDER-TREE-WALK(x)` takes $\Theta(n)$ time.

#### proof:

Since `INORDER-TREE-WALK` visits all $n$ nodes of the subtree, we have $T(n)=\Omega(n)$.
用 $T(n)$ 表示中序遍历时间，$n$ 个结点需要 $\Omega(n)$ 的时间。

For $n > 0$, suppose that `INORDER-TREE-WALK` is called on a node $x$ whose left subtree has $k$ nodes and whose right subtree has $n-k-1$ nodes.
$$
\begin{align*}
T(n)&=T(k)+T(n-k-1)+d\\
&\le((c+d)k+c)+((c+d)(n-k-1)+c)+d\\
&=(c+d)n+c=O(n)
\end{align*}
$$
$T(n)=\Theta(n)$。

### Exercises

#### 12.1-1

> For the set $\{1,4,5,10,16,17,21\}$ of keys, draw binary search trees of heights 2, 3, 4, 5, and 6.

Pass.

#### 12.1-2

> What is the difference between the binary-search-tree property and the min-heap property? Can the min-heap property be used to print out the keys of an $n$-node tree in sorted order in $O(n)$ time? Show how, or explain why not.

Difference: BST guarantees that the left node is always no greater than right node; min-heap guarantees that the top node is always the minimum.

No. Its property does not guarantee it has a way to be traversed in sorted order. We can not find the smallest element in neighbor node subtree.

#### 12.1-4

> Give recursive algorithms that perform preorder and postorder tree walks in $\Theta(n)$ time on a tree of $n$ nodes.

```python
def PreorderTreeWalk(x):
    if x is not None:
        print(x.key)
        PreTreeWalk(x.left)
        PreTreeWalk(x.right)

def PostorderTreeWalk(x):
    if x is not None:
        PostTreeWalk(x.left)
        PostTreeWalk(x.right)
        print(x.key)
```

## 12.2 Querying a binary search tree

```python
def TreeSearch(x, k):
    if x is not None or k == x.key:
        return x
    if k < x.key:
        return TreeSearch(x.left, k)
    else:
        return TreeSearch(x.right, k)

def IterativeTreeSearch(x, k):
    while x is not None and k != x.key:
        if k < x.key:
            x = x.left
        else:
            x = x.right
    return x

def TreeMinimum(x):
    while x.left is not None:
        x = x.left
    return x

def TreeMaximum(x):
    while x.right is not None:
        x = x.right
    return x

def TreeSuccessor(x):
    if x.right is not None:
        return TreeMinimum(x.right) # leftmost node in right subtree
    else: # find the lowest ancestor of x whose left child is an ancestor of x
        y = x.parent
        while y is not None and x == y.right:
            x, y = y, y.parent
        return y
```

### Theorem 12.2

The dynamic-set operations `SEARCH`, `MINIMUM`, `MAXIMUM`, `SUCCESSOR`, and `PREDECESSOR` can be implemented so that each one runs in $O(h)$ time on a binary
search tree of height $h$.
在一棵高度为 $h$ 的二叉搜索树上，查找，最值，前后继的时间复杂度是 $O(h)$。

### Exercises

#### 12.2-2

> Write recursive versions of `TREE-MINIMUM` and `TREE-MAXIMUM`.

```python
def TreeMinimum(x):
    if x.left is not None:
        return TreeMinimum(x.left)
    return x

def TreeMaximum(x):
    if x.right is not None:
        return TreeMinimum(x.right)
    return x
```

## 12.3 Insertion and deletion

```python
def TreeInsert(T, z):
    x = T.root            # node being compared with z
    y = None              # y will be parent of z
    while x is not None:  # descend until reaching a leaf
        y = x
        if z.key < x.key:
            x = x.left
        else:
            x = x.right
    z.parent = y          # found the location -- insert z with parent y
    if y is None:
        T.root = z        # tree T was empty
    elif z.key < y.key:
        y.left = z
    else:
        y.right = z

def Transplant(T, u, v):
    if u.parent is None:     # `u` is root.
        T.root = v
    elif u == u.parent.left:
        u.parent.left = v
    else:
        u.parent.right = v
    if v is not None:
        v.parent = u.parent

def TreeDelete(T, z):
    if z.left is None:
        Transplant(T, z, z.right)     # replace z by its right child
    elif z.right is None:
        Transplant(T, z, z.left)      # replace z by its left child
    else:
        y = TreeMinimum(z.right)      # y is z's successor
        if y != z.right:              # is y farther down the tree?
            Transplant(T, y, y.right) # replace y by its right child
            y.right = z.right         # z's right child becomes y's right child
            y.right.parent = y
        Transplant(T, z, y)           # replace z by its successor y
        y.left = z.left               # and give z's left child to y, which had no left child
        y.left.parent = y
```

### Theorem 12.3

插入删除操作都是 $O(h)$。
The dynamic-set operations `INSERT` and `DELETE `can be implemented so that each one runs in $O(h)$ time on a binary search tree of height $h$.

### Exercises

## Problems

### 12-4 Number of different binary trees

> Let $b_n$ denote the number of different binary trees with $n$ nodes. In this problem, you will find a formula for $b_n$, as well as an asymptotic estimate.

> a. Show that $b_0=1$ and that, for $n\ge1$
> $$
> b_n=\sum_{k=0}^{n-1}b_kb_{n-1-k}
> $$

$n=0$, 0 node tree has only 1 kind of tree.

$n=1$, 1 node tree has only 1 kind of tree.

For $n\ge1$, given a root node, it has one left and one right subtree. If left subtree has $k$ nodes, then the right subtree has $n-1-k$ nodes.

> b. Referring to Problem 4-5 on page 121 for the definition of a generating function, let $B(x)$ be the generating function
> $$
> B(x)=\sum_{n=0}^{\infty}b_nx^n
> $$
> Show that $B(x)=xB^2(x)+1$, and hence one way to express $B(x)$ in closed form is
> $$
> B(x)=\frac{1}{2x}(1-\sqrt{1-4x})
> $$

$$
\begin{align*}
B^2(x)&=\sum_{n=0}^{\infty}\sum_{k=0}^{n}b_kb_{n-k}x^n\\
xB^2(x)&=\sum_{n=0}^{\infty}\sum_{k=0}^{n}b_kb_{n-k}x^{n+1}=\sum_{n=1}^{\infty}\sum_{k=0}^{n-1}b_kb_{n-1-k}x^n\\
&=\sum_{n=1}^{\infty}b_nx^n\\
B(x)&=\sum_{n=0}^{\infty}b_nx^n=\sum_{n=1}^{\infty}b_nx^n+1=xB^2(x)+1
\end{align*}
$$

Solving the quadratic equation
$$
B(x)=\frac{1\pm\sqrt{1-4x}}{2x}
$$
The one way to express $B(x)$ in closed form is $B(x)=\frac{1}{2x}(1-\sqrt{1-4x})$.

> c. Show that
> $$
> b_n=\frac{1}{n+1}\binom{2n}{n}
> $$
> (the $n$ th Catalan number) by using the Taylor expansion of $\sqrt{1-4x}$ around $x=0$.

$$
\begin{align*}
f(x)&=1+\sum_{n=1}^{\infty}(-1)^{n}\cdot4^n\cdot\frac{(-1)^{n-1}(2n-3)!!}{2^n}x^n\\
&=1-\sum_{n=1}^{\infty}2^n\frac{(2n-3)!!}{n!}x^n\\
\end{align*}
$$

Then
$$
\begin{align*}
B(x)&=\frac{1}{2}\sum_{n=1}^{\infty}\frac{2^n(2n-3)!!}{(n+1)!}x^{n-1}\\
&=\sum_{n=0}^{\infty}\frac{2^{n}(2n-1)!!}{(n+1)!}x^{n}\\
&=\sum_{n=0}^{\infty}\frac{(2n)!}{(n+1)!n!}x^{n}\\
&=\sum_{n=0}^{\infty}\frac{1}{(n+1)}\binom{2n}{n}x^{n}\\
\end{align*}
$$
Then $b_n=\frac{1}{n+1}\binom{2n}{n}$.

> d. Show that
> $$
> b_n=\frac{4^n}{\sqrt{\pi}n^{3/2}}(1+O(1/n))
> $$

$$
\begin{align*}
b_n&=\frac{1}{n+1}\frac{(2n)!}{n!n!}\\
&=\frac{1}{n+1}\frac{(2n)^{2n}\sqrt{4\pi n}(1+O(1/n))/e^{2n}}{n^{2n}(2\pi n)(1+O(1/n))/e^{2n}}\\
&\approx\frac{1}{n+1}\frac{4^n}{\sqrt{\pi n}}\\
&\approx\frac{4^n}{\sqrt{\pi}n^{3/2}}(1+O(1/n))
\end{align*}
$$

