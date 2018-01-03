ROOTSRC=$(HOME)/MLibs
LIBSRC=$(ROOTSRC)/lib
INCLUDE_FLAG=-I$(ROOTSRC)/cinc
MACROS=-DHEAP_SIZE_BYTES=4096

dbg:
	clang -std=c11 -ggdb -Wall $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm
rel:
	clang -std=c11 -O3 $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm

anal:
	scan-build clang -std=c11 -Wall -ggdb -fsanitize=address  $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm
clean:
	rm -f test *.o
