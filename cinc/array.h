/*==============================================================================
 Name        : array.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef void* iterator;
typedef struct dynarray dynarray;
typedef dynarray* arrayptr;

#ifdef __cplusplus
extern "C" {
#endif
arrayptr array_alloc(const size_t capacity, const size_t datasize);
arrayptr array_attach(const genptr base, const size_t capacity,
			const size_t datasize);

void array_free(arrayptr pa);
void array_detach(arrayptr pa);

void array_add(arrayptr pa, const genptr data, const size_t count);
void array_insert(arrayptr pa, iterator first, iterator last);

/* algorithms */
void array_print(const arrayptr pa, void (*prnt)(const genptr));

void array_sort(arrayptr pa, bool (*cmp)(const genptr, const genptr),
				   void (*swap)(genptr, genptr));

genptr array_search(arrayptr pa, const genptr val,
		    int(*com)(const genptr, const genptr));

void array_transform(arrayptr dest, const arrayptr src,
		     void (*func)(const genptr, genptr));

void array_modify(arrayptr pa, void (*mod)(genptr));

iterator array_at(arrayptr pa, const size_t pos);
void array_set(arrayptr pa, const genptr val, const size_t pos);

#ifdef __cplusplus
}
#endif

