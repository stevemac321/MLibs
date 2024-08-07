/*==============================================================================
 Name        : circ_list.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef struct circ_list circ_list;
typedef circ_list* listptr;

#ifdef __cplusplus
extern "C" {
#endif

listptr list_alloc(const size_t datasize);
void list_add(listptr pl, const genptr data);
void list_free(listptr pl);
void list_delete_element(listptr pl, const genptr data);
void list_visit(listptr pl, void (*vis)(const genptr));
size_t list_count(const listptr pl);
#ifdef __cplusplus
}
#endif
