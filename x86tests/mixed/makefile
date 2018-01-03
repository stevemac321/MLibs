ROOTSRC=/home/comet/workspace/RTOS
LIBSRC=$(ROOTSRC)/lib
TESTSRC=$(ROOTSRC)/testsrc
CPPINCLUDE_DIR=$(ROOTSRC)/cppinc
CINCLUDE_DIR=$(ROOTSRC)/cinc
MACROS=-DHEAP_SIZE_BYTES=4096

all:
	gcc -std=c11 -c -ggdb -I$(CINCLUDE_DIR) $(LIBSRC)/functor.c  $(LIBSRC)/algo.c
	g++ -std=c++14 -ggdb -I$(CPPINCLUDE_DIR) -I$(CINCLUDE_DIR) -o test $(TESTSRC)/precompile.cpp *.cpp *.o
rel:
	gcc -std=c11 -c -O3 $(MACROS) -I$(CINCLUDE_DIR) $(LIBSRC)/heap.c $(LIBSRC)/functor.c  
	g++ -std=c++14 -O3 $(MACROS) -I$(CPPINCLUDE_DIR) -I$(CINCLUDE_DIR) -o test $(TESTSRC)/precompile.cpp *.cpp *.o
clean:
	rm -f test *.o *.gch

pre:
	g++ -std=c++14 -c $(INCLUDE_DIR)/precompile.hpp -o precompile.gch
