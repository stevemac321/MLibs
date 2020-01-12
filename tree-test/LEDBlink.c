#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "static_tree.h"
#include "heap.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#define SINE_ARRAY_SIZE 91

#ifndef NL
#define NL printf("\n")
#endif


// test helper
void print_int_array(int *arr, const size_t count);
// test functions
void tree_test();
void tree_find_delete_test();

void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++);
	for (i = 0; i < 1000000; i++);
}
void dummy();
int main()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Heap_Init();
	tree_test();
	tree_find_delete_test();
	REPORT("emb tree-test");
	dummy();
	
	for (;;) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		Delay();
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		Delay();
	}
}

/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
void dummy()
{
	printf("foo");
}
void tree_test()
{
	TC_BEGIN(__func__);
	int t2[30];
	modify(t2, _countof(t2), sizeof(int), rand_int);
	treeptr pt = tree_alloc(int_cmp, true);
	if(pt) {
		for(size_t i=0; i < _countof(t2); i++)
			tree_add(pt, &t2[i]);

		tree_visit(pt, in, print_int);
		tree_free(pt);
		NL;
	}
	
	
	modify(t2, _countof(t2), sizeof(int), rand_int);
	pt = tree_alloc(int_cmp, false);
	VERIFY(pt);
	for(size_t i=0; i < _countof(t2); i++)
		tree_add(pt, &t2[i]);

	tree_visit(pt, in, print_int);
	tree_free(pt);
	NL;
	PASSED(__func__, __LINE__);
}
void tree_find_delete_test()
{
	TC_BEGIN(__func__);
	int easy[] = {6, 3, 44, 1, 30};
	treeptr pt = tree_alloc(int_cmp, false);
	VERIFY(pt);
	for(size_t i = 0; i < _countof(easy); i++)
		tree_add(pt, &easy[i]);
	
	tree_visit(pt, in, print_int);
	int val = 44;
	int* f = tree_find(pt, &val);
	VERIFY(f);
	printf("should have fount it: %d\n", *f);
	tree_delete_node(pt, f);
	tree_visit(pt, in, print_int);
	
	f = tree_find(pt, &val);
	//VERIFY(f == NULL);
	tree_visit(pt, in, print_int);
	tree_free(pt);
}

// often used print integer array
void print_int_array(int *arr, const size_t count)
{
	for (int *p = arr; p < arr + count; p++)
		printf("%d ", *(int *)p);

	printf("\n");
}
void PrintByte(char c)
{
  USART_SendData(USART2, c);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
          ; /* do nothing until RESET state*/
}

size_t __write(int handle, const unsigned char* data, size_t byteCount)
{
    size_t nChars = 0;

    // handle == -1 means "flush all streams". Not implemented here.
    if (handle == -1 || byteCount == 0)
        return 0;

    // 1 - stdout; 2 - stderr. 
    if (handle != 1 && handle != 2)
        return -1;

    while (byteCount-- > 0) {
        char ch = (char)(*data);
        PrintByte(ch);
        if (ch == '\n') {
            PrintByte('\r');
        }
       ++data;
       ++nChars;
    }
    return nChars;

} // __write
#pragma GCC diagnostic pop
