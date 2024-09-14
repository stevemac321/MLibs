/*==============================================================================
 Name        : list.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "heap.h"
#include "list.h"
/**=============================================================================
 Interface:  slist

 Purpose:    slist is a singlar linked list that uses the custom static
             heap manager.
==============================================================================*/
typedef struct snode {
        struct snode * next;
        genptr data;
}snode;

typedef snode* snodeptr;

struct slist {
        snodeptr head;
        size_t datasize;
};
/**=============================================================================
 Function:   slist_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the list class.

 Parameters: datasize: size in bytes of each element

Returns:     Opaque slist interface pointer (Pimpl idiom).

Example:     slistptr pl = list_alloc(sizeof(int));
==============================================================================*/
slistptr slist_alloc(const size_t datasize)
{
        slistptr pl = Heap_Malloc(sizeof(slist));
        assert(pl);
        pl->head = NULL;
        pl->datasize = datasize;
        return pl;
}
/**=============================================================================
 Function:   slist_push

 Purpose:    Adds an element to the head of the list.

 Parameters: pl: slist interface pointer
             data: element to add

 Returns:     void

 Example:     slist_push(pl, &i);
==============================================================================*/
void slist_push(slistptr pl, const genptr data)
{
        assert(pl && data);
        snodeptr n = Heap_Malloc(sizeof(snode));
        assert(n);
        n->next = NULL;
        n->data = Heap_Malloc(pl->datasize);
        assert(n->data);
        memcpy(n->data, data, pl->datasize);
        
        if(pl->head == NULL) 
                pl->head = n;
        else {
                n->next = pl->head;
                pl->head = n;
        }
}
void slist_free(slistptr pl)
{
        assert(pl);
        snodeptr p = pl->head;
        
        while(p) {
                snodeptr del = p;
                p = p->next;      
                Heap_Free(del->data);
                Heap_Free(del);
        }
        Heap_Free(pl);
        pl = NULL;
}
/**=============================================================================
 Functions:   slist_top, slist_pop

 Purpose:    This pattern is similar to std::stack.  slist_top returns the 
             element at the head of the list.  slist_pop removes the head of the
	     list.  

 Parameters: pl: slist interface pointer

 Returns:     slist_top returns the head element, slist_top returns void

 Example:     int * p = slist_top(pl);
              printf("%d ", *p);
	      slist_pop(pl);  
==============================================================================*/
genptr slist_top(slistptr pl)
{
        assert(pl && pl->head);
        return pl->head->data;
}
void slist_pop(slistptr pl)
{
        assert(pl);
        snodeptr p = pl->head;
        snodeptr del = NULL;
        if(p) {
                del = p;
                p = p->next;
                Heap_Free(del->data);
                Heap_Free(del);
                del = NULL;
        }
        pl->head = p;
}
bool slist_isempty(slistptr pl)
{
        assert(pl);
        return pl->head == NULL;
}
void slist_visit(slistptr pl, void (*vis)(const genptr data))
{
        assert(pl);
        snodeptr p = pl->head;
        while(p) {
                vis(p->data);
                p = p->next;
        }
}
