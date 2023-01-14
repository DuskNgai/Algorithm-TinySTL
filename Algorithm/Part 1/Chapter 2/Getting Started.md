# Chapter 2 Getting Started

## 2.1 Insertion Sort

当我们想对数字进行分类时，往往是因为它们是与其他数据相关的*键*，我们称之为*卫星数据*。键和卫星数据一起构成了一条*记录*。
When we want to sort numbers, it's often because they are the *keys* associated with other data, which we call *satellite dat*a. Together, a key and satellite data form a *record*.

```python
def InsertionSort(a, n):
    for i in range(1, n):
        key = a[i]
        # Insert a[i] into the sorted subarray a[1:i - 1].
        j = i - 1
        while j >= 0 and a[j] > key:
            a[j + 1] = a[j]
            j -= 1
        a[j + 1] = key
```

**循环不变式 (Loop invariants)**

初始化：循环的第一次迭代之前，它为真。
Initialization: It is true prior to the first iteration of the loop.

保持：如果循环的某次迭代之前它为真，那么下次迭代之前它仍为真。
Maintenance: If it is true before an iteration of the loop, it remains true before the next iteration.

终止：在循环终止时，不变式通常与循环终止的原因一起提供一个性质以帮助我们验证该算法的正确性。
Termination: The loop terminates, and when it terminates, the invariant--usually along with the reason that the loop terminated--gives us a useful property that helps show that the algorithm is correct.

## 2.2 Analyzing Algorithms

需考虑**输入规模 (Input Size)**，**最坏情况和平均情况 (Woest-case and Average-case Analysis)**，**增长量级 (Order of Growth)**。

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
def Merge(a, p, q, r):
    nl = q - p + 1      # Length of a[p:q].
    nr = r - q          # Length of a[q + 1:r].
    left = [0] * nl
    right = [0] * nr
    for i in range(nl): # Copy a[p:q] to left[0:nl - 1].
        left[i] = a[p + i]
    for j in range(nr): # Copy a[q + 1:r] to right[0:nr - 1].
        right[j] = a[q + j + 1]
    i = 0               # i indexes the smallest remaining element in L
    j = 0               # j indexes the smallest remaining element in R
    k = p               # k indexes the next position in a to fill.
    # As long as each of the arrays L and R contains an unmerged element,
    # copy e smallest unmerged element back into a[p:r].
    while i < nl and j < nr:
        if left[i] <= right[j]:
            a[k] = left[i]
            i += 1
        else:
            a[k] = right[j]
            j += 1
        k += 1
    # Having gone through one of L and R entirely,
    # copy the remaining of the other array to the end of a[p:r].
    while i < nl:
        a[k] = left[i]
        i += 1
        k += 1
    while j < nr:
        a[k] = right[j]
        j += 1
        k += 1

def MergeSort(a, p, r):
    if p < r:
        q = (p + r) // 2       # Midpoint of a[p:r].
        MergeSort(a, p, q)     # Recursively sort a[p:q].
        MergeSort(a, q + 1, r) # Recursively sort a[q + 1:r].
        Merge(a, p, q, r)      # Merge the two sorted halves.

```

### 2.3.2 Analyzing Divide-and-conquer Algorithms
