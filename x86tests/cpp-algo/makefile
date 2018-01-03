ROOTSRC=$(HOME)/MLibs
LIBSRC=$(ROOTSRC)/lib
TESTSRC=$(ROOTSRC)/testsrc
INCLUDE_DIR=$(ROOTSRC)/cppinc
MACROS=-DHEAP_SIZE_BYTES=4096

dbg:
	clang++ -std=c++14 -Wall -ggdb $(MACROS) -I$(INCLUDE_DIR) -o test $(LIBSRC)/heap.c $(TESTSRC)/*.cpp *.cpp 

anal:
	scan-build clang++ -std=c++14 -Wall -ggdb -fsanitize=address $(MACROS) -I$(INCLUDE_DIR) -o test $(LIBSRC)/heap.c $(TESTSRC)/*.cpp *.cpp 
rel:
	clang++ -std=c++14 -O3 $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/heap.c $(TESTSRC)/*.cpp *.cpp 
clean:
	rm -f test *.o *.gch

