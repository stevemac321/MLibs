MLibs, Copyright Stephen E. MacKenzie, licensed under GPL v2. C++14 Containers Stevemac::vector – N3797 compliant, no dynamic allocation – Works with STL algorithms

Open Address Hash Table -Double hash, no chaining or probing -Custom std::allocator – Uses the static heap manager – For STL container that takes allocator

C++14 Range-Based Algorithms Predicate and Functor based Work with STL containers -Common functors provided -Resolve via template specialization

C11 Containers -polymorphic types with _Generic factories -Pimpl Idiom – Abstract Data Types – Interface separate from implementation – Opaque Pointers -Custom Static Heap Manager -Work well STL and C algorithms Non-dynamic allocating: -array, binary tree, hashtable, circular linked list, linked list, stack

C11 Algorithms -most of the standard C++ range-based algorithms (STL) in C -Small Footprint -Predicate, Functor Pattern like Qsort and BinSearch in the CRT -Common Functors Provided

Ranged-Based Assembly Algorithms Generic Sort Bytes Transform, Modify Copy Bytes Compare Bytes

C, C++ Test Suite 7000 lines of test code 4200 lines of libraries code Fully automated build, flash, run Embedded and Desktop runs Conformance Tests Library Code not local to projects

Shared with desktop builds Automation Minimal Projects, No IDE Linux, Bash, GNU Makefiles GNU ARM Tools Python Test Harness Texane/STLink Semihosting, floating point printf

To use:

Get arm cross tools https://launchpad.net/gcc-arm-embedded get texane/stlink https://github.com/texane/stlink

Extract STLibs.tar INTO YOUR HOME FOLDER

clone into your $(HOME)_folder:
git clone https://github.com/stevemac321/MLibs.git

Extract STLibs.tar into your $(HOME) folder

If you are on FreeBSD, change make to gmake in the bfy.py

The directory structure should look like this:

gcc-arm-embedded-...

STLibs

MLibs

---cinc

---cppinc

---lib

---embtests

the suite was tested with:

gcc-arm-none-eabi-5_4-2016q3

And this is hardcoded in the bfy.py and the st32.mak files

If you download a different build, you can get away with simply 

renaming the directory name to gcc-arm-none-eabi-5_4-2016q3

rather than  edit the .mak files.

HOW TO clean, build, and run the entire suite:

From the MLibs directory:

./bfy embtests clean build flashrun

You can just clean, or just build, or just flashrun by just passing that arg.

Do debug a single test, navigate to that test, e.g. embtests/c-algorithms

To clean:

make clean

To build:

make

To Debug:

-start a second console if you are running a GUI Desktop, if console, use screen or tmux

-In one console:

st-util

(This will deploy the remote debugging server and will listen for the host gdb session)

-In the second console

cd Debug

<path to gcc-arm-none-eabi-5_4-2016q3>/bin/arm-none-eabi-gdb -tui test.elf

This will start gdb in ncurses mode, then enter the following commands

gdb> tar ext:4242 
gdb> load 
gdb> monitor semihosting enable
gdb> b main
gdb> b dummy
gdb> c

the debugger will break in main, now debug as usual.  

These programs end by running a loop, so setting a breakpoint

at dummy() so you can "kill", then "quit"








