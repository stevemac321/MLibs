/*==============================================================================
 Name        : stack.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef struct stack stack;
typedef stack* stackptr;

#ifdef __cplusplus
extern "C" {
#endif

stackptr stack_alloc(const size_t datasize);
void stack_free(stackptr ps);
void stack_push(stackptr ps, const genptr data);
genptr stack_top(const stackptr ps);
void stack_pop(stackptr ps);
bool stack_isempty(const stackptr ps);

#ifdef __cplusplus
}
#endif
