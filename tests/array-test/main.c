#include "precompile.h"
#include "harness.h"
#include "algo.h"
#include "array.h"
#include "polyarray.h"
#include "functor.h"
#include "heap.h"
#include "stack.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#define SINE_ARRAY_SIZE 91

#ifndef NL
#define NL printf("\n")
#endif

// test data
// array interface tests
static int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static int grid[3][3] = {{44, 77, 11}, {2, 100, 77}, {0, 33, 9}};
static const char *strs[] = {"zero", "one", "two", "three", "four"};
static student recs[] = {{"Joe", 2.0},   {"Mary", 4.0},  {"Larry", 3.5},
			 {"Moe", 3.0},   {"Harry", 3.5}, {"Mary", 2.0},
			 {"Jill", 4.0},  {"Jerry", 2.9}, {"Mike", 3.5},
			 {"Harry", 3.0}, {"Abel", 3.5},  {"Abel", 2.0}};

// test helper
void print_int_array(int *arr, const size_t count);
void array_test();
void poly_test();

void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++);
	for (i = 0; i < 1000000; i++);
}
void dummy();
int main()
{
	Heap_Init();
	array_test();
	poly_test();
	REPORT("emb Array-Test");
	dummy();

}

/*-----------------------------------------------------------------------------
Test functions and data
-----------------------------------------------------------------------------*/
void dummy()
{
	printf("foo");
}
void array_test()
{
	TC_BEGIN(__func__);
	arrayptr pai = array_alloc(_countof(a), sizeof(int));
	array_add(pai, a, _countof(a));
	array_modify(pai, rand_int);
	array_print(pai, print_int);
	array_sort(pai, int_less, int_swap);
	array_print(pai, print_int);

	arrayptr pas = array_alloc(_countof(strs), sizeof(const char *));
	array_add(pas, strs, _countof(strs));
	array_print(pas, print_pstr);
	array_sort(pas, str_less, str_swap);
	array_print(pas, print_pstr);

	arrayptr par = array_alloc(_countof(recs), sizeof(student));
	array_add(par, recs, _countof(recs));
	array_sort(par, record_gpaless, record_swap);
	array_print(par, print_student);

	student s = {"Johnny Yuma", 1.5};
	student *p = array_at(par, 3);
	*p = s;
	array_print(par, print_student);

	array_free(pai);
	array_free(pas);
	array_free(par);

	arrayptr pag = array_alloc(_gridcountof(grid), sizeof(int));
	array_add(pag, grid, _gridcountof(grid));
	array_sort(pag, int_less, int_swap);
	array_print(pag, print_int);
	array_free(pag);

	pas = array_alloc(_countof(strs), sizeof(const char *));
	array_add(pas, strs, _countof(strs));
	array_print(pas, print_pstr);
	array_free(pas);
	PASSED(__func__, __LINE__);
}

void poly_test()
{
	TC_BEGIN(__func__);
	int a[] = {9,8,7,6,5,4,3,2,1,0};
	polyptr pa = POLY_FROM_TYPE(1, _countof(a));
	poly_add(pa, a, _countof(a));
	poly_print(pa);
	poly_sort(pa);
	poly_print(pa);
	
	for(int i=0; i < _countof(a); i++) {
		int * p = poly_search(pa, &a[i]);
		VERIFY(p);
		printf("%d ", *p);
	}
	NL;

	int notfound = 55;
	int * x = poly_search(pa, &notfound);
	VERIFY(x == NULL);

	NL;
	poly_free(pa);

	pa = POLY_FROM_TYPE((const char*)"foo", _countof(strs));
	poly_add(pa, strs, _countof(strs));
	poly_print(pa);
	poly_sort(pa);
	poly_print(pa);
	poly_free(pa);
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
