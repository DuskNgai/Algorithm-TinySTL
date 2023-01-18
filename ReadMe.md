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
git submodule sync --recursive
git submodule update --init --recursive

mkdir build
cd build
cmake ..
###### If you are using Visual Studio #####
# Double click `Algorithm-TinySTL.sln`
# set one project as starting project
# make and run.
###### End if you are using Visual Studio #####

###### If you are using GCC/Clang #####
make -j
###### End if you are using GCC/Clang #####
```

## TODO List

### Introduction to Algorithm

- [x] Part 1 Foundations
    - [x] Chapter 1 The Role of Algorithms in Computing
    - [x] Chapter 2 Getting Started
    - [x] Chapter 3 Characterizing Running Times
    - [ ] Chapter 4 Divide-and-Conquer
    - [ ] Chapter 5 Probabilistic Analysis and Randomized Algorithms
- [ ] Part 2 Sorting and Order Statistics
    - [ ] Chapter 6 Heapsort
    - [ ] Chapter 7 Quicksort
    - [ ] Chapter 8 Sorting in Linear Time
    - [ ] Chapter 9 Medians and Order Statistics
- [ ] Part 3 Data Structure
    - [ ] Chapter 10 Elementary Data Structures
    - [ ] Chapter 11 Hash Tables
    - [ ] Chapter 12 Binary Search Trees
    - [ ] Chapter 13 Red-Black Trees
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
        - [ ] Tree (`stl_tree.hpp`)
        - [x] Pair (`stl_pair.hpp`)
        - [ ] Set (`stl_set.hpp`)
        - [ ] Map (`stl_map.hpp`)
        - [ ] Multiset (`stl_multiset.hpp`)
        - [ ] Multimap (`stl_multimap.hpp`)
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
