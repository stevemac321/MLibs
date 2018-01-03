# CPP-Algo
Generic programming C++ (vector, hashtable, algorithms)

stevemac::vector: N3797 comforming vector with a custom allocator (stevemac::allocator) that uses static (compile time) memory (by Jacob Edner, Jonathan Valvano) for systems (embedded) that do not like dynamic allocation.  Pseudo "heap" size is set in makefile

Demonstrate use of std::priority_queue using the custom allocator (via stevemac::vector).  Basically, stevemac::allocator can be used with any container that 
takes an allocator as a template parameter.

A full suit of extensive tests for stevemac::vector are built-in.  Validation tests are built in for the other algorithms.

Open addressing double hash hashtable (open_map)

Ranged base algorithms, template specialization
sort, transform, visit, for_each.

To use:  the makefile is setup for g++, but you can change it to use cl.exe for visual c++ or whatever other compiler you might be using.  

Uses for STM32 C++:
You can use the allocator.hpp as a customer allocator for std::vector (the stevemac::vector does it by default) or any standard C++ library container that takes an allocator as a template argument.  Then you can use that container with all of the standard C++ library algorithms.  This project has been tested 
on an STM32F401RE Nucleo board.





