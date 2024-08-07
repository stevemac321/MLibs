/*==============================================================================
 Name        : array.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "polyarray.h"
#include "functor.h"
#include "algo.h"
#include "heap.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/**=============================================================================
 Interface:  dynarray

 Purpose:    Dynarray implements a vector or dynamic sizing array class that 
             uses the custom static heap manager.
==============================================================================*/
struct polyarray {
	genptr base;
	size_t cap;
	size_t datasize;
	size_t count;
	int (*cmp)(const genptr, const genptr);
	bool (*lessthan)(const genptr, const genptr);
	void (*swap)(genptr, genptr);
	void (*print)(const genptr);
};
/**=============================================================================
 Function:   poly_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the array class.

 Parameters: capacity: how many bytes to allocate.
	     datasize: size in bytes of each element

Returns:     Opaque array interface pointer (Pimpl idiom).

Example:     polyptr pa = poly_alloc(100, sizeof(int));
==============================================================================*/
polyptr poly_alloc(const size_t capacity, const size_t datasize,
		   int (*cmp)(const genptr, const genptr),
		   bool(*lessthan)(const genptr, const genptr),
		   void(*swap)(genptr, genptr),
		   void(*print)(const genptr))
{
	polyptr p = Heap_Malloc(sizeof(polyarray));
	assert(p);
	p->base = Heap_Malloc(capacity * datasize);
	p->cap = capacity;
	p->datasize = datasize;
	p->count = 0;
	p->cmp = cmp;
	p->lessthan = lessthan;
	p->swap = swap;
	p->print = print;

	return p;
}
/**=============================================================================
 Function:   poly_add

 Purpose:    Appends a range of bytes to the array buffer.

 Parameters: pa: array interface pointer
	     data: chuck of bytes to add
	     count: size in bytes of each element

Returns:     void

Example:     int add[] = {3,4,5};
             poly_add(add, _countof(add));
==============================================================================*/
void poly_add(polyptr pa, const genptr data, const size_t count)
{
	assert(pa && pa->count + count <= pa->cap);
	memcpy(pa->base + (pa->count * pa->datasize), data,
	       (pa->datasize * count));
	
	pa->count += count;
}

void poly_free(polyptr pa)
{
	assert(pa);
	Heap_Free(pa->base);
	Heap_Free(pa);
	pa=NULL;
}

/**=============================================================================
 Function:   poly_at

 Purpose:    Provides read/write access to individual array elements.

 Parameters: pa: array interface pointer
	     pos: element index or position in array

Returns:     pointer to element in which the user can read or write through.

Example:     int * p = poly_at(pa, 3);
             if(p)
	        *p = 99;
==============================================================================*/
iterator poly_at(polyptr pa, const size_t pos)
{
	assert(pa && pos < pa->count);
	return pa->base + (pos * pa->datasize);
}
/**=============================================================================
 Function:   poly_set

 Purpose:    copies val into specified position

 Parameters: pa: array interface pointer
             val: value to copy
	     pos: element index or position in array

Returns:     void

Example:     poly_at(pa, &i, 3);
==============================================================================*/
void poly_set(polyptr pa, const genptr val, const size_t pos)
{
	assert(pa && val && pos < pa->cap);
	genptr p = pa->base + (pos * pa->datasize);
	memcpy(p, val, pa->datasize);
}
/**=============================================================================
 Functions:   array algorithms, sort, transform, modify
 Purpose:     array wrappers to algorithms. See algo.c for algorithm docs.
==============================================================================*/
void poly_sort(polyptr pa)
{
	assert(pa);
	gensort(pa->base, pa->count, pa->datasize, pa->lessthan, pa->swap);
}
genptr poly_search(const polyptr pa, const genptr val)
{
	assert(pa);
	return gensearch(pa->base, val, 0, pa->count -1, pa->datasize, pa->cmp);
}
void poly_print(const polyptr pa)
{
	assert(pa);
	for (genptr p = pa->base; p < pa->base + (pa->count * pa->datasize);
	     p += pa->datasize)
		pa->print(p);

	printf("\n");
}
/* common factories */
polyptr poly_alloc_int(int x, const size_t cap)
{
	return poly_alloc(cap, sizeof(int), int_cmp, int_less, int_swap,
			  print_int);
}
polyptr poly_alloc_str(const char* s, const size_t cap)
{
	return poly_alloc(cap, sizeof(const char*), str_cmp, str_less,
			  str_swap, print_pstr);
}
#pragma GCC diagnostic pop
