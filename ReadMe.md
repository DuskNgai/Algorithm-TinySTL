# Algorithm & TinySTL

Simple implementations of *"Introduction to Algorithm"* by `C++` (major) and `Python` (minor). Since the whole project is build on `C++98`/`C++11` standard, it looks quite clumsy.

Some simple algorithms and data structures are implemented in [TinySTL](https://github.com/DuskNgai/TinySTL).

You may find some official `Python` code in [this link](https://mitp-content-server.mit.edu/books/content/sectbyfn/books_pres_0/11599/clrsPython.zip).

In the first round, we will go through the whole book. In the second round, we will try to solve all the questions.

## Requirement & Usage

一个支持 `C++11` 标准的编译器。我们在以下平台 + 编译器的组合上测试了我们的程序：

- `Windows` + `Visual Studio 2022`

请按照以下的流程编译程序：

```bash
git clone https://github.com/DuskNgai/Algorithm-TinySTL
cd Algorithm-TinySTL

cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Release
cmake --build ./build --parallel
```

## Ruminations on STL (C++98 Version)

STL 由六大组件（component）组成，分别是：

1. 分配器（allocator）

2. 迭代器（iterator）

3. 容器（container）

4. 算法（algorithm）

5. 仿函数（functor）

6. 接配器（adapter）

各部分组件的关系为：分配器为容器提供分配空间的接口，让容器专注于增删查改的操作上；迭代器为容器和算法提供了统一的接口；仿函数扩展了数据的解释性，扩展了算法的使用范围；接配器为容器、仿函数、迭代器提供了更为直接的接口。

分配器即是内存管理器。内存管理分为两部分：分配空间（allocation）和释放空间（deallocation），构造（construction）和析构（destruction）。分配空间后，需求者获得指向系统分配空间的指针，系统所分配的空间此时没有初始化。如果所需数据类型属于 C++ 基本数据类型，（进一步说，POD 数据类型），那么我们便可以直接复制所需数据，免去了构造的一步。同理，释放空间的时候我们也可以直接释放空间，免去析构的一步。保留构造和析构的步骤是因为非 POD 数据类型存在构造和析构函数，需要执行这些操作才能得到合适的数据。将二者分离可以保证较快的运行速度和正确的运行结果。

容器即是常见的数据结构。容器分为序列型（sequential）容器和关联型（associative）容器。序列型容器包括：可变数组（vector）、双向链表（list）单向链表（forward list）、栈（stack）、队列（queue）、双向队列（deque）、堆（heap）、优先队列（priority queue）。关联型容器包括：红黑树（red-black tree）、散列表（hashtable）和由它们实现的集合（set）、映射（map）、多集合（multiset）、多映射（multimap）。这些数据结构囊括了绝大部分的实际需求。

算法即是常见的基本算法。这些算法专门执行在容器之中，拓展了容器的增删查改的能力。这里特别讲述一下 `std::sort` 的写法。`std::sort` 为了保证稳定且快速的排序，结合了 `heap sort`、`intro sort`（`quick sort` 的变种）和 `insertion sort`。首先进行 `intro sort`，从数组头部、中间和尾部选取中间值作为 `pivot`。用分割算法把小于 `pivot` 的元素放到其左侧，大于的放在其右侧，然后递归右半部分。每次分割时候如果子数组长度小于某阈值时，则不继续分割了。这样，数组便被分割为一段一段的子数组，子数组长度不超过阈值，且在数组前部的子数组的元素不大于在数组后部的子数组的元素。这样 `insertion sort` 便可以分段排序，每一段排序绝不影响前一段排序。利用小数组的 `insertion sort` 更快的特性，`std::sort` 要比单纯 `intro sort` 更快。至于 `heap sort`，由于其稳定的 `O(nlogn)` 的时间复杂度，是用于 `intro sort` 分割次数过多时候的稳定排序时间手段。

迭代器是容器和算法之间的桥梁。由于容器的内存排布存在各种形式，为了适配，算法可能会重载多个针对不同内存排布的容器的函数。这些函数如何知道容器的内存排布，就需要从迭代器中萃取出来。

仿函数又称为函数对象。它是一个重载了 `operator()` 的结构体，可以像函数一样调用。仿函数的存在是因为类的可拓展性能。我们很容易地就可以从类的实例中获取函数参数和返回值的信息，而函数指针在 C++98 标准内不能做到这一点。

接配器是对容器、迭代器和仿函数的拓展。不过自从 `C++11` 之后，仿函数接配器就换了一个新的形式出现，与 `auto`、lambda 函数、 `std::function` 和 `std::bind` 等新特性一起使用。

## TODO List

### Introduction to Algorithm

- [x] Part 1 Foundations
    - [x] Chapter 1 The Role of Algorithms in Computing
    - [x] Chapter 2 Getting Started
    - [x] Chapter 3 Characterizing Running Times
    - [x] Chapter 4 Divide-and-Conquer
    - [x] Chapter 5 Probabilistic Analysis and Randomized Algorithms
- [ ] Part 2 Sorting and Order Statistics
    - [x] Chapter 6 Heapsort
    - [ ] Chapter 7 Quicksort
    - [ ] Chapter 8 Sorting in Linear Time
    - [ ] Chapter 9 Medians and Order Statistics
- [ ] Part 3 Data Structure
    - [ ] Chapter 10 Elementary Data Structures
    - [ ] Chapter 11 Hash Tables
    - [x] Chapter 12 Binary Search Trees
    - [x] Chapter 13 Red-Black Trees
- [ ] Part 4 Advanced Design and Analysis Techniques
    - [ ] Chapter 14 Dynamic Programming
    - [ ] Chapter 15 Greedy Algorithms
    - [ ] Chapter 16 Amortized Analysis
- [ ] Part 5 Advanced Data Structures
    - [ ] Chapter 17 Augmenting Data Structures
    - [ ] Chapter 18 B-Trees
    - [ ] Chapter 19 Data Structures for Disjoint Sets
- [ ] Part 6 Graph Algorithms
    - [ ] Chapter 20 Elementary Graph Algorithms
    - [ ] Chapter 21 Minimum Spanning Trees
    - [ ] Chapter 22 Single-Source Shortest Paths
    - [ ] Chapter 23 All-Pairs Shortest Paths
    - [ ] Chapter 24 Maximum Flow
    - [ ] Chapter 25 Matching in Bipartite Graphs
- [ ] Part 7 Selected Topics
    - [ ] Chapter 26 Parallel Algorithms
    - [ ] Chapter 27 Online Algorithms
    - [ ] Chapter 28 Matrix Operations
    - [ ] Chapter 29 Linear Programming
    - [ ] Chapter 30 Polynomial and the FFT
    - [ ] Chapter 31 Number-Theoretic Algorithms
    - [ ] Chapter 32 String Matching
    - [ ] Chapter 33 Machine-Learning Algorithms
    - [ ] Chapter 34 NP-Completeness
    - [ ] Chapter 35 Approximation Algorithms

### TinySTL

- [x] Allocator
    - [x] Primary Allocator (`stl_allocater.hpp`)
    - [x] Secodary Allocator (`stl_alloc.hpp`)
- [x] Iterator
    - [x] Iterator Categories, Iterator Associative Types (`stl_iterator.hpp`)
    - [x] Type Traits (`stl_type_traits.hpp`)
- [ ] Container
    - [x] Sequential Containers
        - [x] Vector (`stl_vector.hpp`)
        - [x] List (`stl_list.hpp`)
        - [x] Deque (`stl_deque.hpp`)
        - [x] Heap (`stl_heap.hpp`)
        - [x] Priority Queue (`stl_priority_queue.hpp`)
        - [x] Forward List (`stl_forward_list.hpp`)
    - [ ] Associative Containers
        - [x] Tree (`stl_tree.hpp`)
        - [x] Pair (`stl_pair.hpp`)
        - [x] Set (`stl_set.hpp`)
        - [x] Map (`stl_map.hpp`)
        - [x] Multiset (`stl_multiset.hpp`)
        - [x] Multimap (`stl_multimap.hpp`)
        - [ ] Hash Table (`stl_hash_table.hpp`)
        - [x] Hash Function (`stl_hash_fun.hpp`)
        - [ ] Unordered Set (`stl_unordered_set.hpp`)
        - [ ] Unordered Map (`stl_unordered_map.hpp`)
        - [ ] Unordered Multiset (`stl_unordered_multiset.hpp`)
        - [ ] Unordered Multimap (`stl_unordered_multimap.hpp`)
- [x] Algorithm
    - [x] Numeric (`stl_numeric.hpp`)
        - Accumulate, Inner Product, Partial Sum, Adjacent Difference, Power, Iota
    - [x] Algorithm
        - [x] Comparison
            - Min, Max, Median, Equal, Lexicographical Compare, Mismatch
        - [x] Memory Operations
            - Copy, Copy Backward, Fill, Fill N, Swap, Iter Swap, Swap Ranges, Reverse, Reverse Copy
        - [x] Set Operations
            - Includes, Set Union, Set Intersection, Set Difference, Set Symmetric Difference
        - [x] Search
            - Min Element, Max Element, Find, Find If, Find End, Find First Of, Adjacent Find, Count, Count If, Search, Search N, Equal Range, Lower Bound, Upper Bound, Binary Search
        - [x] Modify
            For Each, Generate, Generate N, Transform, Random Shuffle
        - [x] Replace
            - Replace, Replace If, Replace Copy, Replace Copy If, Rotate, Rotate Copy
        - [x] Remove
            - Remove, Remove If, Remove Copy, Remove Copy If, Unique Copy, Unique
        - [x] Sort
            - Partial Sort, Partial Sort Copy, Partition, Stable Partition, Sort, Nth Element, Merge, Inplace Merge
        - [x] Permutation
            - Next Permutation, Prev Permutation
- [x] Functor (`stl_function.hpp`)
    - Arithmetic
    - Relational
    - Logical
- [x] Adapter
    - [x] Container Adapter
        - [x] Stack (`stl_stack.hpp`)
        - [x] Queue (`stl_queue.hpp`)
    - [x] Iterator Adapter(`stl_iterator.hpp`)
        - Insert Iterator
        - Reverse Iterator
        - Stream Iterator
    - [x] Function Adapter (`stl_function.hpp`)
        - Negate
        - Bind
        - Compose
        - Pointer to Function
        - Pointer to Member Function
