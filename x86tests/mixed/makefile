ROOTSRC=$(HOME)/MLibs
LIBSRC=$(ROOTSRC)/lib
TESTSRC=$(ROOTSRC)/testsrc
CPPINCLUDE_DIR=$(ROOTSRC)/cppinc
CINCLUDE_DIR=$(ROOTSRC)/cinc
MACROS=-DHEAP_SIZE_BYTES=4096

gdb:
	clang -std=c11 -c -ggdb -I$(CINCLUDE_DIR) $(LIBSRC)/functor.c  $(LIBSRC)/algo.c
	clang++ -std=c++14 -ggdb -I$(CPPINCLUDE_DIR) -I$(CINCLUDE_DIR) -o test $(TESTSRC)/precompile.cpp *.cpp *.o
anal:
	scan-build clang -std=c11 -c -ggdb -fsanitize=address -I$(CINCLUDE_DIR) $(LIBSRC)/functor.c  $(LIBSRC)/algo.c
	scan-build clang++ -std=c++14 -ggdb -fsanitize=address -I$(CPPINCLUDE_DIR) -I$(CINCLUDE_DIR) -o test $(TESTSRC)/precompile.cpp *.cpp *.o
rel:
	clang -std=c11 -c -O3 $(MACROS) -I$(CINCLUDE_DIR) $(LIBSRC)/heap.c $(LIBSRC)/functor.c  
	clang++ -std=c++14 -O3 $(MACROS) -I$(CPPINCLUDE_DIR) -I$(CINCLUDE_DIR) -o test $(TESTSRC)/precompile.cpp *.cpp *.o
clean:
	rm -f test *.o *.gch

