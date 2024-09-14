/*==============================================================================
 Name        : open_table.c 
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

enum insert_state { INSERTED, EXISTS, TABLEFULL };

typedef struct open_table open_table;
typedef open_table* tableptr;
typedef void* genptr;

#ifdef __cplusplus
extern "C" {
#endif

tableptr open_table_init(const size_t tbsize, const size_t datasize,
			size_t (*h1)(const genptr),
			size_t (*h2)(const genptr),
			int (*compare)(const genptr, const genptr),
                        void(*alloc_insert)(tableptr, size_t, genptr));

void open_table_destroy(tableptr);
enum insert_state open_table_insert(tableptr, genptr);
bool open_table_remove_entry(tableptr, const genptr);
genptr open_table_lookup(const tableptr, const genptr);
size_t open_table_resize(tableptr, const size_t);
void open_table_visit(const tableptr, void (*visit)(const genptr));
size_t open_table_size(const tableptr);
size_t open_table_entries(const tableptr);
bool open_table_resize_needed(const tableptr);
size_t strhash(const char* value);
size_t inthash(const int* value);
void table_report(const tableptr table);
void table_int_alloc(tableptr table, size_t idx, genptr value);
void table_str_alloc(tableptr table, size_t idx, genptr value);
#ifdef __cplusplus
}
#endif
