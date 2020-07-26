# TwoFourTree
Implementation of a Two-Four Tree in C++. A Two-Four Tree (aka two-three-four tree) is a data structure that resembles a balanced binary tree. This is done by allowing each node to contain up to 3 keys. Each node must contain either no children or n+1 children where n is the number of keys. Every leaf in the tree has the same depth.

The main algorithms (underflow and overflow) are complete but the project is still ongoing to fix known overheads and to create a user interface to play around in.

Last updated: July, 2020

### Getting Started
Currently, running the program does not demonstrate much. I plan on adding some demos to show how the tree splits and merges as elements are added and removed. In the meantime, you may run the tests by executing  ```make``` followed by ```make test```. Alternatively, the test executable may be invoked directly to pass in catch2 arguments. For exmaple, to see the test names and test times you may run ```./bin/testtft -d yes```

### Code Organization
The directory structure is as follows:
```
├── Makefile
├── README.md
├── src
│   ├── ...
│   ├── main.cpp
│   ├── [node|iter|debug]_operations.ipp
│   └── tftree.hpp
└── test
    ├── catch.hpp
    ├── test_[insert|erase|accessories ...] .cpp
    └── testmain.cpp
```
Two targets are built using the makefile: one for the main executable and one for testing. The files to create the main executable are in the src directory, whereas the files for the test executable are in the test directory (although they also include tftree.hpp). The test uses the catch2 framework. The main algorithms are located in tftree.hpp and the \*.ipp implementation files.

Upon building (```make```), folders named ```bin``` and ```obj``` will be created. They correspond to the executable binaries and the object/dependency files respectively.

### Future Plans
Although the main code is complete and working, I am aware of some extreme overhead and some bad design decisions that I made. I would like to fix these. I would also like to add new features to the main executable, such as showing a demonstration of how the tree splits and merges, and comparing the performance to known data types (mainly std::set). I also have not implemented the code to handle custom Compare and Allocator templates, but plan on doing that soon. Allowing a custom Allocator will also allow me to compare the memory usage with the std::set.

#### Upcoming Fixes
 * I split the work into three classes ```TwoFourTree```, ```Iterator``` and ```Node```. Although having a separate class for the iterator was necessary and a good choice, I now think the Node should have been a POD struct instead of a class on its own. The reason is that the methods I use inside the node do not have access to the Tree member variables (i.e. the begin and end iterators, comparator and allocator objects, as well as the reference to the root node). This resulted in overhead that could have been avoided. I plan on fixing this.
  * The Key is internally stored by value in an std::array. I chose arrays over lists because I knew the size of the container is not going to change. Although I still agree with that, the array creates too many objects and does not delete them until the entire array has to be deleted. I now believe the best choice is to use an array of unique pointers instead of an array of values.

#### Upcoming Features
 * I would like to have some code to benchmark the program, whether this benchmark is part of the test or the main executable. I want to benchmark the time and memory usages to compare it with a std::set. Ideally, I would benchmark it using different statistical distributions (uniform, binomial and bimodal).
 * I would like to have a user-interactive demo where values are added and removed so the user can watch how the tree adapts and balances itself.
 
