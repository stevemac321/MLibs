/*=============================================================================
 Name        : algo.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "algo.h"
#include "functor.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

/**=============================================================================
 Function:   visit

 Purpose:    Iterates a block of bytes (usually an array), visits each 
             element, calls the functor on each element.  The element is only
	     read, it is not modified.  Typical use case is to print out each
	     element; the functor passed is aware of the element type.

 Parameters: base: contiguous block of bytes, typically an array.
	     count: length of the block in bytes, typically array len.
	     size: byte length of a chuck to be used as an array element.
             readonly: function pointer that acts upon an element.

Returns:     void

Example:     int a[] = {0,1,2,3,4};
	     visit(a, _countof(a), sizeof(int), print_integer);
==============================================================================*/
void visit(genptr base, const size_t count, const size_t size,
	   void (*readonly)(const genptr))
{
	assert(base && readonly);
	for (genptr p = base; p < (base + (count * size)); p += size)
		readonly(p);
}
/**=============================================================================
 Function:   transform

 Purpose:    Iterates 2  block of bytes (usually an array); source and 
             destination buffers.  Both the source and destination elements are
	     passed to the user provided functor.  The destination element is 
	     modified by an algorithm performed on the source element, typically.
	     However, the user can provide a functor that does whatever.
	     The buffers may overlap, in fact, most commonly, the source and
	     destination buffer are the same, although the modify function is
	     better suited for that scenario.

 Parameters: src: contiguous block of bytes to read from, typically an array.
	     dest: contiguous block of bytes to write to, typically the array
	     that gets modified.
	     count: length of the block in bytes, typically array len.
	     size: byte length of a chuck to be used as an array element.
             func: function that transforms the source element and writes the
	     result to the destination element.

Returns:     void

Example:     int src[] = {0,1,2,3,4};
	     int dest[5];
	     transform(src, dest,, _countof(src), sizeof(int), double_src);
==============================================================================*/
void transform(genptr dest, const genptr src, const size_t count,
	       const size_t size, void (*func)(const genptr, genptr))
{
	assert(src && dest && func);
	for (genptr ps = src, pd = dest; ps < src + (count * size);
	     ps += size, pd += size)
		func(ps, pd);
}
/**=============================================================================
 Function:   modify

 Purpose:    Iterates a block of bytes (usually an array). As the elements are
             visited, the user provided functor modofies the element with a 
	     user provided algorithm. 

 Parameters: base: contiguous block of bytes to read from and write back to, 
	     typically an array.
	     count: length of the block in bytes, typically array len.
	     size: byte length of a chuck to be used as an array element.
             func: function pointer that modifies each element.

Returns:     void

Example:
             int a[5];
	     modify(a, _countof(a), sizeof(int), random_int);
==============================================================================*/
void modify(genptr base, const size_t count, const size_t size,
	    void(*mod)(genptr pv))
{
	assert(base && mod);
	for (genptr p = base; p < base + (count * size); p += size)
		mod(p);
}
/**=============================================================================
 Function:   gensort

 Purpose:    sorts a contiguous block of bytes

 Parameters: base: contiguous block of bytes to sort, typically an array.
	     count: length of the block in bytes, typically array len.
	     size: byte length of a chuck to be used as an array element.
	     cmp: user defined functor that defines the sorting criteria.
	     swp: user defined functor that performs a type specify swap.  This
	     is different from qsort which uses internal storage for the swap.
	     (see functor.inl)

Returns:     void

Example:     int a[] = {9,8,7,6,5,4,3,2,1,0};
	     gensort(a, _countof(a), sizeof(int), cmp_int, swap_int);
==============================================================================*/
void gensort(genptr base, const size_t count, const size_t size,
	     bool (*cmp)(const genptr, const genptr),
	     void (*swp)(genptr, genptr))
{
	assert(base && cmp && swp);
	for (genptr key = (base + size); key < (base + (count * size));
	     key += size)
		while (key > base && cmp(key, (key - size))) {
			swp(key, (key - size));
			key -= size;
		}
}
/**=============================================================================
 Function:   gensearch

 Purpose:    searches for a value in  a contiguous block of bytes

 Parameters: base: contiguous block of bytes to search, typically an array.
	     count: length of the block in bytes, typically array len.
	     size: byte length of a chuck to be used as an array element.
	     cmp: user defined functor that defines the sorting criteria.
	     (see functor.h and c for some common functors to use.)

Returns:     pointer to found element or NULL of not found.

Example:     int a[] = {9,8,7,6,5,4,3,2,1,0};
	     int * p = gensearch(a, _countof(a), sizeof(int), cmp_int);
==============================================================================*/
genptr gensearch(genptr base, const genptr key, size_t first, size_t last,  
	         const size_t size, int (*cmp)(const genptr, const genptr))
{
	assert(base && key && cmp);
	
	while(last >= first) {
		size_t mid = (last + first) / 2;
		
		genptr cur = base + (mid * size);
		
		int ret = 0;
		if((ret =cmp(key, cur)) == 0)
			return cur;
		
		if(ret < 0)
			last = mid - 1;
		else
			first = mid + 1;

	}
	return NULL;
}
/* non-modifying algorithms for unsorted ranges */
/**=============================================================================
 Function:   count_if
 Purpose:    calculates the number of elements that meet the predicate criteria.
  ==============================================================================*/
size_t count_if(const genptr base, const size_t count, const size_t size,
		bool(*pred)(const genptr))
{
	assert(base && pred);
	size_t accum=0;
	for(genptr p=base; p < (base + (count * size)); p += size)
		if(pred(p))
			accum++;

	return accum;
}
/**=============================================================================
 Function:   equal
 Purpose:    determines if two ranges are equal up to first1's count, NOTE uses
             strcmp semantics: 0, < 0, > 0.
  ==============================================================================*/
bool equal(const genptr first1, const genptr first2, const size_t count,
	   const size_t size, int(*cmp)(const genptr, const genptr))
{
	assert(first1 && first2 && cmp);
	for(genptr p1=first1, p2=first2; p1 < (first1 + (count*size));
	    p1 += size, p2 += size)
		if(cmp(p1,p2) != 0)
			return false;

	return true;
}
/**=============================================================================
 Function:   is_sorted
 Purpose:    determines if a range meets the predicate's sorting criteria.
  ==============================================================================*/
bool is_sorted(const genptr base, const size_t count, const size_t size,
	       bool(*pred)(const genptr, const genptr))
{
	assert(base && pred);
	for(genptr prev=base, cur=base + size; cur < (base + (count * size));
	    cur += size, prev += size)
		if(!pred(prev, cur))
			return false;
		
	return true;
}
/**=============================================================================
 Function:   all_of
 Purpose:    returns true if all elements meet predicate criteria.
  ==============================================================================*/
bool all_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr))
{
	assert(base && pred);
	for(genptr p=base; p < (base + (count * size)); p += size)
		if(!pred(p))
			return false;

	return true;
}
/**=============================================================================
 Function:   any_of
 Purpose:    returns true if any element meets predicate criteria.
  ==============================================================================*/
bool any_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr))
{
	assert(base && pred);
	for(genptr p=base; p < (base + (count * size)); p += size)
		if(pred(p))
			return true;

	return false;
}
/**=============================================================================
 Function:   none_of
 Purpose:    returns true if no elements meet predicate criteria.
  ==============================================================================*/
bool none_of(const genptr base, const size_t count, const size_t size,
	   bool(*pred)(const genptr))
{
	assert(base && pred);
	for(genptr p=base; p < (base + (count * size)); p += size)
		if(pred(p))
			return false;

	return true;
}

/* modifying algorithms */
/**=============================================================================
 Function:   copy
 Purpose:    copy src to dest.  overlapping buffers undefined.
  ==============================================================================*/
void copy(genptr dest, const genptr src, const size_t count, const size_t size)
{
	assert(src && dest);
	memcpy(dest, src, (count * size));
}

/**=============================================================================
 Function:   reverse_copy
  ==============================================================================*/
void reverse_copy(genptr dest, const genptr src, const size_t count,
		   const size_t size)
{
	assert(src && dest);
	genptr ps = src;
	genptr pd = dest + ((count -1) * size);
	while(ps < src + (count * size)) {
		memcpy(pd, ps, size);
		ps += size;
		pd -= size;
	}
}
/**=============================================================================
 Function:   merge
 Purpose:    merge two sorted ranges into one sorted range beginning at dest.
  ==============================================================================*/
void merge(genptr dest, const genptr src1, const genptr src2, const size_t count,
	   const size_t size, bool(*pred)(const genptr, const genptr))
{
	assert(dest && src1 && src2 && pred);
	for(genptr pd = dest, ps1 = src1, ps2 = src2;
	    pd < dest + (count * size); pd += size) {
		if(pred(ps1, ps2)) {
			memcpy(pd, ps1, size);
			ps1 += size;
		}else {
			memcpy(pd, ps2, size);
			ps2 += size;
		}
	}
}
/**=============================================================================
 Function:   swap_ranges
  ==============================================================================*/
void swap_ranges(genptr first1, genptr first2, const size_t count,
		 const size_t size, void(*swap)(genptr, genptr))
{
	assert(first1 && first2 && swap);
	for(genptr p1=first1, p2=first2; p1 < first1 + (count * size);
	    p1 += size, p2 += size)
		swap(p1, p2);
}
/**=============================================================================
 Function:   replace
 Purpose:    replace occurances of oldval with newval
 Parameter of Note: cmp: this compare function has strcmp semantics (0, < 0, > 0)
  ==============================================================================*/
void replace(genptr base, const genptr oldval, const genptr newval,
	     const size_t count, const size_t size,
	     int(*cmp)(const genptr, const genptr))
{
	assert(base && oldval && newval && cmp);
	for(genptr p=base; p < base + (count * size); p += size)
		if(cmp(p, oldval)==0)
			memcpy(p, newval, size);
}
/**=============================================================================
 Function:   replace_if
 Purpose:    replace occurances of oldval with newval if the predicate criteria
             is met.
 Parameter of Note: cmp: this compare function has strcmp semantics (0, < 0, > 0)
  ==============================================================================*/
void replace_if(genptr base, const genptr newval, const size_t count,
		const size_t size, bool(*pred)(const genptr))
{
	assert(base && newval && pred);
	for(genptr p=base; p < base + (count * size); p += size)
		if(pred(p))
			memcpy(p, newval, size);
}
/* mutating algorithms */
/**=============================================================================
 Function:   reverse
  ==============================================================================*/
void reverse(genptr base, const size_t count, const size_t size,
	     void(*swap)(genptr, genptr))
{
	assert(base && swap);
	for(genptr pl = base, pr = base + ((count-1) * size); pr > pl;
	    pl += size, pr -= size) 
		swap(pl, pr);
}
/**=============================================================================
 Function:   copy_backward
 Purpose:    preserves the order of the elements, it simply copies from and to 
	     starting at the end of the buffers.  Good for overlapping buffers.
  ==============================================================================*/
void copy_backward(genptr dest, const genptr src, const size_t count, 
		   const size_t size)
{
	assert(dest && src);
	memmove(dest, src, count * size);
}
void rotate(genptr base, const size_t count, const size_t size)
{
}
/**=============================================================================
 Function:   random_shuffle
  ==============================================================================*/
void random_shuffle(genptr base, const size_t count, const size_t size,
		    void(*swap)(genptr, genptr))
{
	assert(base && swap);
	int modr = count-1;
	for(genptr p = base + ((count-1) * size); p >= base; p -= size) {
		int r = (modr > 0) ? (rand() % modr) : 0;
		swap(p, base + (r * size));
		modr--;
	}
}

/* numeric algorithms */
/**=============================================================================
 Function:   accumulate
 Purpose:    calculates the total of numberic elements in a range.
  ==============================================================================*/
int accumulate(const genptr base, const size_t count, const size_t size,
	       int(*acc)(const genptr))
{
	assert(base);
	int accum = 0;
	for(genptr p=base; p < (base + (count * size)); p += size)
		accum += acc(p);
	
	return accum;
}
/**=============================================================================
 Function:   product
 Purpose:    calculates the product of elements in a range.
  ==============================================================================*/
int product(const genptr base, const size_t count, const size_t size,
	       int(*prod)(const genptr))
{
	assert(base);
	int result = 1;
	for(genptr p=base; p < (base + (count * size)); p += size)
		result *= prod(p);
	
	return result;
}

/**=============================================================================
 Function:   inner_product
 Purpose:    calculates the sum of products of two ranges (adds the sums).
 ==============================================================================*/
int inner_product(const genptr first1, const genptr first2, const size_t count,
		  const size_t size, int(*prod)(const genptr))
{
	assert(first1 && first2 && prod);
	int sum1=1;
	int sum2=1;

	for(genptr p1=first1, p2=first2; p1 < (first1 + (count * size));
	    p1 += size, p2 += size){
		sum1 *= prod(p1);
		sum2 *= prod(p2);
	}
	
	return sum1 + sum2;
}

/* common specializations */
void sort_int(genptr base, const size_t count)
{
	gensort(base, count, sizeof(int), int_less, int_swap);
}
void sort_str(genptr base, const size_t count)
{
	gensort(base, count, sizeof(int), str_less, str_swap);
}
genptr search_int(const genptr base, const genptr val, size_t first, size_t last)
{
	return gensearch(base, val, first, last, sizeof(int), int_cmp);
}

genptr search_str(const genptr base, const genptr val, size_t first, size_t last)
{
	return gensearch(base, val, first, last, sizeof(const char*), str_cmp);
}

#pragma GCC diagnostic pop
