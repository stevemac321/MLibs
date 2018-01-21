/*==============================================================================
 Name        : list.inl
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once
#include <assert.h>

struct nodeinl {
	void *data;
	struct nodeinl *next;
};
struct listinl {
	struct nodeinl *head;
	struct nodeinl *(*ctor)(void *);
	void (*dtor)(struct nodeinl *);
};
static inline void listinl_init(struct listinl *pl, struct nodeinl *(*ctor)(void *),
		  void (*dtor)(struct nodeinl *))
{
	assert(pl && ctor && dtor);
	pl->head = NULL;
	pl->ctor = ctor;
	pl->dtor = dtor;
}
static inline void listinl_addhead(struct listinl *pl, void *data)
{
	assert(pl && data);
	struct nodeinl *pnew = pl->ctor(data);
	pnew->next = pl->head;
	pl->head = pnew;
}
static inline void listinl_destroy(struct listinl *pl)
{
	assert(pl);
	struct nodeinl *p = pl->head;
	while (p) {
		struct nodeinl *del = p;
		p = p->next;
		pl->dtor(del);
	}
}
