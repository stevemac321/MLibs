ROOTSRC=/home/comet/workspace/RTOS
LIBSRC=$(ROOTSRC)/lib
TESTSRC=$(ROOTSRC)/testsrc
INCLUDE_DIR=$(ROOTSRC)/cppinc
MACROS=-DHEAP_SIZE_BYTES=4096

all:
	g++ -std=c++14 -ggdb $(MACROS) -I$(INCLUDE_DIR) -o test $(LIBSRC)/heap.c $(TESTSRC)/*.cpp *.cpp 
rel:
	g++ -std=c++14 -O3 $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/heap.c $(TESTSRC)/*.cpp *.cpp 
clean:
	rm -f test *.o *.gch

pre:
	g++ -std=c++14 -c $(INCLUDE_DIR)/precompile.hpp -o precompile.gch
