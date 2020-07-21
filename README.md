# TwoFourTree
Implementation of a Two-Four Tree in C++. This is a work in progress.

Last updated: April, 2020


### Working Features
* Inserting elements
* Iterating over elements


### Desired Features
This is the order in which I plan to implement and test this container

* Inserting elements (complete)
* Iterating over elements (complete)
* Removing elements (complete)
* Lookup functions  (partially complete)
* Capacity functions (complete)

Finally, I plan to write some benchmarks to compare the performance against well known data types.

### Remarks
I initially chose the internal representation of the values inside each node to be a std::array because I thought having consecutive objects close to each other would result in faster code, but I eventually realized that this is a micro-optimization with extreme overhead costs that weigh heavier than the performance boost. In addition, the templated Allocator object could be modified by the caller to manage the heap to produce similar results. I plan on changing this implementation to use a std::list instead.
 