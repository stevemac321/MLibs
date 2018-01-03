/*==============================================================================
 Name        : main.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "heap.h"
#include "circ_list.h"
#include "list.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#ifndef NL
#define NL printf("\n")
#endif

// test helper
void print_int_array(int *arr, const size_t count);

// test functions

void list_test();
void slist_test();

int main()
{
	Heap_Init();
	list_test();
	slist_test();
	NL;
	REPORT("x86 list-test");
}
/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/

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

// often used print integer array
void print_int_array(int *arr, const size_t count)
{
	for (int *p = arr; p < arr + count; p++)
		printf("%d ", *(int *)p);

	printf("\n");
}
#pragma GCC diagnostic pop
