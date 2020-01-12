#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "static_tree.h"
#include "heap.h"
#include "open_table.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#define SINE_ARRAY_SIZE 91
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef NL
#define NL printf("\n")
#endif

// test helper
void print_int_array(int *arr, const size_t count);

// test functions
void table_test1();
void table_test2();

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
	table_test1();
	table_test2();
	REPORT("emb hashtable-test");
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
void table_test1()
{
	TC_BEGIN(__func__);
	tableptr table = open_table_init(97, sizeof(int), inthash, inthash, int_cmp, table_int_alloc);
	VERIFY(table);
	PASSED(__func__, __LINE__);

	int a[97];
	int dupes = 0;
	modify(a, _countof(a), sizeof(int), rand_int);
	print_int_array(a, _countof(a));
	for (int i = 0; i < _countof(a); i++) {
		if (EXISTS == open_table_insert(table, &a[i]))
			dupes++;
	}

	open_table_visit(table, print_int);
	NL;
	table_report(table);
	printf("dupes: %d\n", dupes);
	open_table_destroy(table);
	PASSED(__func__, __LINE__);
}
void table_test2()
{
	TC_BEGIN(__func__);
	tableptr table = open_table_init(97, sizeof(const char*), strhash, strhash, table_str_cmp, table_str_alloc);
	VERIFY(table);
	PASSED(__func__, __LINE__);
	
	char *a[] = {"John",  "Mary",  "Joseph", "Abraham",
		     "Obama", "Satan", "Larry"};
	size_t len = _countof(a);

	for (int i = 0; i < len; i++)
		open_table_insert(table, a[i]);

	open_table_visit(table, print_str);
	NL;
	char *p = open_table_lookup(table, a[3]);
	if (p)
		printf("found: %s\n", p);

	open_table_remove_entry(table, a[3]);
	open_table_visit(table, print_str);
	NL;
	open_table_destroy(table);
	PASSED(__func__, __LINE__);
}
void sine_table_test()
{
	TC_BEGIN(__func__);
	tableptr table = open_table_init(SINE_ARRAY_SIZE * 2, sizeof(int), 
						inthash, inthash, int_cmp, table_int_alloc);
	VERIFY(table);
	
	int compare[SINE_ARRAY_SIZE];
	float sf=0;
	int res=0;
	/* this loop taken from Zhu textbook */
	for(int i=0; i < SINE_ARRAY_SIZE; i++) {
		sf = sin(M_PI * i / 180);
		res = (1 + sf) * 2048;
		if(res == 0x1000)
			res = 0xFFF;
		
		compare[i] = res;
		open_table_insert(table, &res);
	}
	printf("contexts: \n");
	open_table_visit(table, print_hex);
	NL;
	printf("compare: \n");
	for(int i=0; i < SINE_ARRAY_SIZE; i++)
		printf("%X ", compare[i]);
	NL;
	for(int i=0; i < SINE_ARRAY_SIZE; i++) {
		int *p = open_table_lookup(table, &compare[i]);
		VERIFY(p != NULL);
	}

	open_table_destroy(table);
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
