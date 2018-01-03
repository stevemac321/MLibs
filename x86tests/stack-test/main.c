/*==============================================================================
 Name        : main.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "stack.h"
#include "heap.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#ifndef NL
#define NL printf("\n")
#endif

// test helper
void print_int_array(int *arr, const size_t count);
// test functions
void stack_test();

int main()
{
	Heap_Init();
	stack_test();
	REPORT("x86 stack-test");
}
/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
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
	stack_free(ps);
}
// often used print integer array
void print_int_array(int *arr, const size_t count)
{
	for (int *p = arr; p < arr + count; p++)
		printf("%d ", *(int *)p);

	printf("\n");
}
#pragma GCC diagnostic pop
