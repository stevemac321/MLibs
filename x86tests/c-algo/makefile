ROOTSRC=/home/comet/workspace/RTOS
LIBSRC=$(ROOTSRC)/lib
INCLUDE_FLAG=-I$(ROOTSRC)/cinc
MACROS=-DHEAP_SIZE_BYTES=4096

all:
	gcc -std=gnu11 -ggdb $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm
addr:
	gcc -std=gnu11 -ggdb -fsanitize=address $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm
rel:
	gcc -std=gnu11 -O3 $(MACROS) $(INCLUDE_FLAG) -o test $(LIBSRC)/*.c main.c -lm
clean:
	rm -f test *.o
