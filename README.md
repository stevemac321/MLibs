MLibs, Copyright Stephen E. MacKenzie, licensed under GPL v2. C++14 Containers Stevemac::vector – N3797 compliant, no dynamic allocation – Works with STL algorithms

Open Address Hash Table -Double hash, no chaining or probing -Custom std::allocator – Uses the static heap manager – For STL container that takes allocator

C++14 Range-Based Algorithms Predicate and Functor based Work with STL containers -Common functors provided -Resolve via template specialization

C11 Containers -polymorphic types with _Generic factories -Pimpl Idiom – Abstract Data Types – Interface separate from implementation – Opaque Pointers -Custom Static Heap Manager -Work well STL and C algorithms Non-dynamic allocating: -array, binary tree, hashtable, circular linked list, linked list, stack

C11 Algorithms -most of the standard C++ range-based algorithms (STL) in C -Small Footprint -Predicate, Functor Pattern like Qsort and BinSearch in the CRT -Common Functors Provided

Ranged-Based Assembly Algorithms Generic Sort Bytes Transform, Modify Copy Bytes Compare Bytes

C, C++ Test Suite 7000 lines of test code 4200 lines of libraries code Fully automated build, flash, run Embedded and Desktop runs Conformance Tests Library Code not local to projects

Shared with desktop builds Automation Minimal Projects, No IDE Linux, Bash, GNU Makefiles GNU ARM Tools Python Test Harness Texane/STLink Semihosting, floating point printf

To use:

Get arm cross tools https://developer.arm.com/open-source/gnu-toolchain/gnu-rm: Copy to your $(HOME) directory

Get texane/stlink https://github.com/texane/stlink  (follow the compiling manual:

https://github.com/texane/stlink/blob/master/doc/compiling.md ).

Get libusb as follows: sudo apt-get install libusb-1.0-0-dev

Extract STLibs.tar INTO YOUR HOME FOLDER

clone into your $(HOME)_folder:
git clone https://github.com/stevemac321MLibs.git

Extract STLibs.tar into your the MLibs folder.

If you are on FreeBSD, change make to gmake in the bfy.py

The directory structure should look like this:

$(HOME)/MLIBs/gcc-arm-embedded-...
$(HOME)/MLIBs/STLibs
$(HOME)/MLibs
$(HOME)/MLIBS/cinc
$(HOME)/MLIBS/cppinc
$(HOME)/MLIBS/lib
$(HOME)/MLIBS/embtests

NOTE: x86tests are NOT part of the suite.  They for testing using more analysis tools on x64 or x86.
$(HOME)MLIBS/x86tests

NOTE: the suite was orginally tested with:
gcc-arm-none-eabi-5_4-2016q3 (but we are using version 9 these days).
HOWEVER, rename this directory to gcc-arm.
So that it matches the makefiles, plus it is a long name and a hassle when
debugging.

HOW TO clean, build, and run the entire suite:

You will need to give ./bfy.py executable permissions:  chmod +x bfy.py

(If you are running on FreeBSD, you can build and clean as normal user,
but you will need to run as superuser (root) to debug or flashrun.)

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
<path to gcc-arm/bin/arm-none-eabi-gdb -tui test.elf
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
