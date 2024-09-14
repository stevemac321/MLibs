/*==============================================================================
 Name        : array.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "array.h"
#include "algo.h"
#include "heap.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/**=============================================================================
 Interface:  dynarray

 Purpose:    Dynarray implements a vector or dynamic sizing array class that 
             uses the custom static heap manager.
==============================================================================*/
struct dynarray {
	genptr base;
	size_t cap;
	size_t datasize;
	size_t count;
};
/**=============================================================================
 Function:   array_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the array class.

 Parameters: capacity: how many bytes to allocate.
	     datasize: size in bytes of each element

Returns:     Opaque array interface pointer (Pimpl idiom).

Example:     arrayptr pa = array_alloc(100, sizeof(int));
==============================================================================*/
arrayptr array_alloc(const size_t capacity, const size_t datasize)
{
	arrayptr p = Heap_Malloc(sizeof(dynarray));
	assert(p);
	p->base = Heap_Malloc(capacity * datasize);
	p->cap = capacity;
	p->datasize = datasize;
	p->count = 0;

	return p;
}
arrayptr array_attach(const genptr base, const size_t capacity,
			const size_t datasize)
{
	arrayptr p = Heap_Malloc(sizeof(dynarray));
	assert(p);
	p->base = base;
	p->cap = capacity;
	p->datasize = datasize;
	p->count = 0;

	return p;
}

/**=============================================================================
 Function:   array_add

 Purpose:    Appends a range of bytes to the array buffer.

 Parameters: pa: array interface pointer
	     data: chuck of bytes to add
	     count: size in bytes of each element

Returns:     void

Example:     int add[] = {3,4,5};
             array_add(add, _countof(add));
==============================================================================*/
void array_add(arrayptr pa, const genptr data, const size_t count)
{
	assert(pa && pa->count + count <= pa->cap);
	memcpy(pa->base + (pa->count * pa->datasize), data,
	       (pa->datasize * count));
	
	pa->count += count;
}

void array_free(arrayptr pa)
{
	assert(pa);
	Heap_Free(pa->base);
	Heap_Free(pa);
	pa=NULL;
}
void array_detach(arrayptr pa)
{
	assert(pa);
	Heap_Free(pa);
	pa=NULL;
}

void array_print(arrayptr pa, void (*prnt)(const genptr v))
{
	assert(pa && prnt);
	for (genptr p = pa->base; p < pa->base + (pa->count * pa->datasize);
	     p += pa->datasize)
		prnt(p);

	printf("\n");
}
/**=============================================================================
 Function:   array_at

 Purpose:    Provides read/write access to individual array elements.

 Parameters: pa: array interface pointer
	     pos: element index or position in array

Returns:     pointer to element in which the user can read or write through.

Example:     int * p = array_at(pa, 3);
             if(p)
	        *p = 99;
==============================================================================*/
iterator array_at(arrayptr pa, const size_t pos)
{
	assert(pa && pos < pa->count);
	return pa->base + (pos * pa->datasize);
}
/**=============================================================================
 Function:   array_set

 Purpose:    copies val into specified position

 Parameters: pa: array interface pointer
             val: value to copy
	     pos: element index or position in array

Returns:     void

Example:     array_at(pa, &i, 3);
==============================================================================*/
void array_set(arrayptr pa, const genptr val, const size_t pos)
{
	assert(pa && val && pos < pa->cap);
	genptr p = pa->base + (pos * pa->datasize);
	memcpy(p, val, pa->datasize);
}
/**=============================================================================
 Functions:   array algorithms, sort, transform, modify
 Purpose:     array wrappers to algorithms. See algo.c for algorithm docs.
==============================================================================*/
void array_sort(arrayptr pa, bool (*cmp)(const genptr v1, const genptr v2),
		void (*swap)(genptr, genptr))
{
	assert(pa && cmp);
	gensort(pa->base, pa->count, pa->datasize, cmp, swap);
}
genptr array_search(const arrayptr pa, const genptr val,
		    int(*cmp)(const genptr, const genptr))
{
	assert(pa);
	return gensearch(pa->base, val, 0, pa->count -1, pa->datasize, cmp);
}
void array_transform(arrayptr dest, const arrayptr src,
		     void (*func)(const genptr s, genptr d))
{
	assert(src && dest && func);
	transform(dest->base, src->base, dest->count, dest->datasize, func);
}
void array_modify(arrayptr pa, void (*mod)(genptr p))
{
	assert(pa && mod);
	modify(pa->base, pa->count, pa->datasize, mod);
}

#pragma GCC diagnostic pop
