Static vector
==============


# Purpose / Use cases

Real-time applications need to avoid memory allocation during runtime.
Therefore, a memory static vector is provided which only allocates memory
on construction.

\snippet core/apex_containers/test/apex_containers/test_static_vector_snippets.cpp Basic Static Vector

`apex::static_vector` provides the functionality of `std::vector`, with the following exceptions:

* Constructors which provide an additional size argument
* Copying, which is not possible as it requires memory allocation

Container adapters which work with `std::vector` also work with `apex::static_vector`, such as
priority queue and stack.


## Priority Queue

A suitable factory function for a static priority looks similar to the following:

```
include <queue>
template<class T, class Comp=std::less<T>>
decltype(auto) make_static_priority_queue(const typename apex::static_vector<T>::size_type size) {
  return std::priority_queue<T, apex::static_vector<T>, Comp>(
      Comp(), apex::static_vector<T>(size));
}
```


## Stack

A suitable factory function for a static stack looks similar to the following:

```
#include <stack>
template<class T>
decltype(auto) make_stack(const typename apex::static_vector<T>::size_type size) {
  std::stack<T, apex::static_vector<T>>(apex::static_vector<T>(size));
}
```


# Design

The design consists of two parts, an allocator and an adapter for `std::vector`.


## Allocator

A single-use allocator, `single_allocator`, is provided which only allows a single fixed size
allocation. Allocations violating this constraint will throw an exception.


## Vector adapter

The adapter `static_vector` is provided which wraps a `std::vector` and ensures that the
`single_allocator` constraints are not violated by reserving all provided memory in the constructor,
and disabling the `shrink_to_fit()` function.

Further, no copy constructor is provided as this would allocate memory.


# Related issues

- #2416: Initial implementation
- #2508: apex::static_vector minor improvements
- #2472: `containers design` does not follow the documentation style guide
