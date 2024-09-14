/*==============================================================================
 Name        : circ_list.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "heap.h"
#include "circ_list.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
/**=============================================================================
 Interface:  circ_list

 Purpose:    circ_list is a circular linked list that uses the custom static
             heap manager.
==============================================================================*/
typedef struct node {
        struct node * next;
        genptr data;
}node;

typedef node* nodeptr;

struct circ_list {
        nodeptr head;
        size_t count;
        size_t datasize;
};
/**=============================================================================
 Function:   list_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the list class.

 Parameters: datasize: size in bytes of each element

Returns:     Opaque circ_list interface pointer (Pimpl idiom).

Example:     listptr pl = list_alloc(sizeof(int));
==============================================================================*/
listptr list_alloc(const size_t datasize)
{
        listptr pl = Heap_Malloc(sizeof(circ_list));
        assert(pl);
        pl->head = NULL;
        pl->count = 0;
        pl->datasize = datasize;

        return pl;
}
/**=============================================================================
 Function:   list_add

 Purpose:    Adds an element to the list.

 Parameters: pl: circ_list interface pointer
             data: element to add

 Returns:     void

 Example:     list_add(pl, &i);
==============================================================================*/
void list_add(listptr pl, const genptr data)
{
        assert(pl && data);
        nodeptr n = Heap_Malloc(sizeof(node));
        assert(n);
        n->next = NULL;
        n->data = Heap_Malloc(pl->datasize);
        assert(n->data);
        memcpy(n->data, data, pl->datasize);
        if(pl->head == NULL) {
                pl->head = n;
                n->next = pl->head;
        } else {
                n->next = pl->head->next;
                pl->head->next = n;
                pl->head = n;
        }
        pl->count++;
}
void list_free(listptr pl)
{
        assert(pl);
        nodeptr p = pl->head;
        nodeptr del = NULL;

        while(pl->count > 0) {
                del = p;
                p = p->next;
                Heap_Free(del->data);
                Heap_Free(del);
                pl->count--;
        }
        Heap_Free(pl);
        pl = NULL;
}
void list_delete_element(listptr pl, const genptr data)
{

}
size_t list_count(listptr pl)
{
        assert(pl);
        return pl->count;
}
void list_visit(listptr pl, void (*vis)(const genptr))
{
      assert(pl);
      nodeptr p = pl->head; 
      size_t visited = pl->count;
      while(visited > 0) {
              vis(p->data);
              p = p->next;
              visited--;
      }
}
#pragma GCC diagnostic pop
