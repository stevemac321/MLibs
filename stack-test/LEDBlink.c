#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "heap.h"
#include "list.h"
#include "circ_list.h"
#include "stack.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#define SINE_ARRAY_SIZE 91

#ifndef NL
#define NL printf("\n")
#endif

// test functions
void list_test();
void slist_test();
void stack_test();

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
	list_test();
	slist_test();
	stack_test();
	REPORT("emb stack-test");
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
void list_test()
{
	TC_BEGIN(__func__);
	listptr pl = list_alloc(sizeof(int));
	for(int i=0; i < 10; i++)
		list_add(pl, &i);

	list_visit(pl, print_int);
	NL;
	list_free(pl);
	PASSED(__func__, __LINE__);
}

void slist_test()
{
	TC_BEGIN(__func__);
	slistptr pl = slist_alloc(sizeof(int));
	for(int i=0; i < 10; i++)
		slist_push(pl, &i);

	slist_visit(pl, print_int);
	NL;

	while(!slist_isempty(pl)) {
		printf("%d ", *(int*)slist_top(pl));
		slist_pop(pl);
	}
	VERIFY(slist_isempty(pl));
	slist_free(pl);
}
void stack_test()
{
	TC_BEGIN(__func__);
	stackptr ps = stack_alloc(sizeof(int));
	for(int i=0; i < 10; i++)
		stack_push(ps, &i);

	while(!stack_isempty(ps)) {
		printf("%d ", *(int*)stack_top(ps));
		stack_pop(ps);
	}
	NL;
	VERIFY(stack_isempty(ps));
	slist_free(ps);
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
