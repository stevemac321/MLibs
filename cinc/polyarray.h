/*==============================================================================
 Name        : polyarray.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef void* iterator;
typedef struct polyarray polyarray;
typedef polyarray* polyptr;
#ifdef __cplusplus
extern "C" {
#endif

polyptr poly_alloc(const size_t capacity, const size_t datasize,
		   int (*cmp)(const genptr, const genptr),
		   bool(*lessthan)(const genptr, const genptr),
		   void(*swap)(genptr, genptr),
		   void(*print)(const genptr));
void poly_free(polyptr pa);
void poly_add(polyptr pa, const genptr data, const size_t count);
iterator poly_at(polyptr pa, const size_t pos);
void poly_set(polyptr pa, const genptr val, const size_t pos);
/* algorithms */
void poly_print(const polyptr pa);
void poly_sort(polyptr pa);
genptr poly_search(const polyptr pa, const genptr val);

/* common factories */
polyptr poly_alloc_int(int x,const size_t cap);
polyptr poly_alloc_str(const char* s, const size_t cap);

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
#define POLY_FROM_TYPE(type, cap)		\
	_Generic((type),			\
		 int: poly_alloc_int,		\
		 const char*: poly_alloc_str	\
)(type, cap)
#endif
