/*==============================================================================
 Name        : list.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef struct slist slist;
typedef slist* slistptr;

#ifdef __cplusplus
extern "C" {
#endif

slistptr slist_alloc(const size_t datasize);
void slist_push(slistptr pl, const genptr data);
void slist_free(slistptr pl);
genptr slist_top(slistptr pl);
void slist_pop(slistptr pl);
bool slist_isempty(slistptr pl);
void slist_visit(slistptr, void (*vis)(const genptr));

#ifdef __cplusplus
}
#endif
