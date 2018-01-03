/*==============================================================================
 Name        : main.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "algo.h"
#include "functor.h"
#include "static_tree.h"
#include "heap.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#ifndef NL
#define NL printf("\n")
#endif

// test helper
void print_int_array(int *arr, const size_t count);
// test functions
void tree_test();
void tree_find_delete_test();

int main()
{
	Heap_Init();
	tree_test();
	tree_find_delete_test();
	REPORT("x86 tree-test");
}
/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
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
	VERIFY(f == NULL);
	tree_visit(pt, in, print_int);
	tree_free(pt);
	PASSED(__func__, __LINE__);
}

// often used print integer array
void print_int_array(int *arr, const size_t count)
{
	for (int *p = arr; p < arr + count; p++)
		printf("%d ", *(int *)p);

	printf("\n");
}
#pragma GCC diagnostic pop
