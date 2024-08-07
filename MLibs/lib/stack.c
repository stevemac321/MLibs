/*==============================================================================
 Name        : stack.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "heap.h"
#include "stack.h"
#include "list.h"
/**=============================================================================
 Interface:  stack

 Purpose:    stack is a LIFO pattern that internally uses a singlar linked list 
             that uses the custom static heap manager.  It uses the top/pop
	     pattern similar to std::stack.
==============================================================================*/
typedef struct stacknode {
        struct stacknode * next;
        genptr data;
}stacknode;

typedef stacknode* stacknodeptr;

struct stack {
        stacknodeptr head;
        size_t datasize;
};
/**=============================================================================
 Function:   stack_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the stack class.

 Parameters: datasize: size in bytes of each element

 Returns:     Opaque slist interface pointer (Pimpl idiom).

 Example:     stackptr ps = stack_alloc(sizeof(int));
==============================================================================*/
stackptr stack_alloc(const size_t datasize)
{
        stackptr ps = Heap_Malloc(sizeof(stack));
        assert(ps);
        ps->head = NULL;
        ps->datasize = datasize;
        return ps;
}
void stack_free(stackptr ps)
{
        assert(ps);
        stacknodeptr p = ps->head;
        
        while(p) {
                stacknodeptr del = p;
                p = p->next;      
                Heap_Free(del->data);
                Heap_Free(del);
        }
        Heap_Free(ps);
        ps = NULL;
}
/**=============================================================================
 Function:   stack_push

 Purpose:    Adds an element to the head of the list.

 Parameters: ps: stack interface pointer
             data: element to add

 Returns:     void

 Example:     stack_push(ps, &i);
==============================================================================*/
void stack_push(stackptr ps, const genptr data)
{
        assert(ps && data);
        stacknodeptr n = Heap_Malloc(sizeof(stacknode));
        assert(n);
        n->next = NULL;
        n->data = Heap_Malloc(ps->datasize);
        assert(n->data);
        memcpy(n->data, data, ps->datasize);
        
        if(ps->head == NULL) 
                ps->head = n;
        else {
                n->next = ps->head;
                ps->head = n;
        }
}
/**=============================================================================
 Functions:   stack_top, stack_pop

 Purpose:    This pattern is similar to std::stack.  slist_top returns the 
             element at the head of the list.  slist_pop removes the head of the
	     list.  

 Parameters: ps: stack interface pointer

 Returns:     stack_top returns the head element, stack_top returns void

 Example:     int * p = stack_top(pl);
              printf("%d ", *p);
	      stack_pop(pl);  
==============================================================================*/
genptr stack_top(const stackptr ps)
{
         assert(ps && ps->head);
         return ps->head->data;
}
void stack_pop(stackptr ps)
{
        assert(ps);
        stacknodeptr p = ps->head;
        stacknodeptr del = NULL;
        if(p) {
                del = p;
                p = p->next;
                Heap_Free(del->data);
                Heap_Free(del);
                del = NULL;
        }
        ps->head = p;
}
bool stack_isempty(const stackptr ps)
{
        assert(ps);
        return ps->head == NULL;
}
