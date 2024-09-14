/*==============================================================================
 Name        : algo.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

#ifdef __cplusplus
	extern "C" {
#endif
/* non-modifying algorithms for sorted ranges  */
genptr gensearch(genptr base, const genptr key, size_t first, size_t last,  
		const size_t size, int (*cmp)(const genptr, const genptr));

/* non-modifying algorithms for unsorted ranges */
void visit(genptr base, const size_t count, const size_t size,
	   void (*readonly)(const genptr));

size_t count_if(const genptr base, const size_t count, const size_t size,
		bool(*pred)(const genptr));

bool equal(const genptr first1, const genptr first2, const size_t count1, 
	    const size_t size, int(*cmp)(const genptr, const genptr));

bool is_sorted(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr, const genptr));

bool all_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr));

bool any_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr));

bool none_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr));

/* modifying algorithms */
void gensort(genptr base, const size_t count, const size_t size,
	     bool (*cmp)(const genptr, const genptr),
	     void (*swp)(genptr, genptr));

/* ok if buffers overlap or even the same buffer writing to itself */
void transform(genptr dest, const genptr src, const size_t count,
	       const size_t size, void (*func)(const genptr, genptr));

void modify(genptr base, const size_t count, const size_t size,
	    void(*mod)(genptr pv));

void copy(genptr dest, const genptr src, const size_t count,
	  const size_t size);

void copy_backward(genptr dest, const genptr src, const size_t count,
		   const size_t size);

void merge(genptr dest, const genptr src1, const genptr src2, const size_t count,
	   const size_t size, bool (*pred)(const genptr, const genptr));

void swap_ranges(genptr first1, genptr first2, const size_t count,
		 const size_t size, void(*swap)(genptr, genptr));

void replace(genptr base, const genptr oldval, const genptr newval,
	     const size_t count, const size_t size,
	     int(*cmp)(const genptr, const genptr));

void replace_if(genptr base, const genptr newval, const size_t count,
		const size_t size, bool(*pred)(const genptr));

/* mutating algorithms */
void reverse(genptr base, const size_t count, const size_t size,
	     void(*swap)(genptr, genptr));

void reverse_copy(genptr dest, const genptr src, const size_t count,
	const size_t size);

void rotate(genptr base, const size_t count, const size_t size);

void random_shuffle(genptr base, const size_t count, const size_t size,
		    void (*swap)(genptr, genptr));

/* numeric algorithms */
int accumulate(const genptr base, const size_t count, const size_t size,
	       int(*acc)(const genptr));

int product(const genptr base, const size_t count, const size_t size,
	    int(*prod)(const genptr));

int inner_product(const genptr first1, const genptr first2, const size_t count,
		  const size_t size, int(*prod)(const genptr));

/* specializations */
void sort_int(genptr base, const size_t count);
void sort_str(genptr base, const size_t count);
genptr search_int(const genptr base, const genptr val, size_t first, size_t last);
genptr search_str(const genptr base, const genptr val, size_t first, size_t last);

#ifdef __cplusplus
	}
#endif

#define deduce_sort(base, count)                                               \
	_Generic((base), int * : sort_int, char * : sort_str)(base, count)

#define deduce_search(base, val, first, last)                                  \
	_Generic((base), int * : search_int, char * : search_str)              \
(base, val, first, last)
