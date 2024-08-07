/*==============================================================================
 Name        : static_tree.c
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#include "precompile.h"
#include "static_tree.h"
#include "heap.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
/**=============================================================================
 Interface:  binarytree

 Purpose:    binarytree uses the custom static heap manager that can support 
             duplicate entries.
==============================================================================*/
struct node {
	genptr data;
	nodeptr left;
	nodeptr right;
	nodeptr parent;
	int count;
};

struct binarytree {
	nodeptr root;
	int(*cmp)(const genptr v1, const genptr v2);
	int count;
	bool dupes_allowed;
};

/* private */
nodeptr node_alloc(const genptr val, nodeptr parent);
nodeptr tree_dupes_add(nodeptr p, const genptr val, 
	int(*cmp)(const genptr v1, const genptr v2), nodeptr parent);

nodeptr tree_nodupes_add(nodeptr p, const genptr val, 
	int(*cmp)(const genptr v1, const genptr v2), nodeptr parent);

void tree_postorder_free(nodeptr p);

nodeptr tree_inner_find(node *p, const genptr k, 
	int(*cmp)(const genptr v1, const genptr v2));

void tree_preorder_visit(nodeptr p, void(*vis)(const genptr));
void tree_inorder_visit(nodeptr p, void(*vis)(const genptr));
void tree_postorder_visit(nodeptr p, void(*vis)(const genptr));
/*-----------------------------------------------------------------------------
implementation
-----------------------------------------------------------------------------*/
/**=============================================================================
 Function:   tree_alloc

 Purpose:    Allocates from the custom static heap manager and returns an 
             opaque interface pointer to the binarytree  class.

 Parameters: cmp: user provided compare function
             fdupes: flag to indicate whether duplicates are allowed.

Returns:     Opaque binarytree interface pointer (Pimpl idiom).

Example:     treeptr pt = tree_alloc(cmp_int, true);
==============================================================================*/
treeptr tree_alloc(int(*cmp)(const genptr v1, const genptr v2), bool fdupes)
{
	assert(cmp);
	treeptr p = Heap_Malloc(sizeof(binarytree));
	assert(p);
	p->cmp = cmp;
	p->dupes_allowed = fdupes;
	p->root = NULL;
	p->count = 0;
	return p;
}

nodeptr node_alloc(const genptr val, nodeptr parent)
{
	assert(val);
	nodeptr p = Heap_Malloc(sizeof(node));
	assert(p);

	p->data = val;
	p->count = 1;
	p->left = NULL;
	p->right = NULL;
	p->parent = parent;
	return p;
}

void tree_free(treeptr pt)
{
	assert(pt);
        tree_postorder_free(pt->root);
       	Heap_Free(pt);
       	pt = NULL;
}
/**=============================================================================
 Function:   tree_add

 Purpose:    Inserts an element into tree

 Parameters: pt: binarytree interface pointer.
             val: value to insert.

 Returns:     void

 Example:     treeptr pt = tree_add(pt, &val);
==============================================================================*/
void tree_add(treeptr pt, const genptr val)
{
	assert(pt && val);
	if(pt->dupes_allowed)
		pt->root = tree_dupes_add(pt->root, val, pt->cmp, NULL);
	else
		pt->root = tree_nodupes_add(pt->root, val, pt->cmp, NULL);

	pt->count++;
}
/**=============================================================================
 Function:   tree_find

 Purpose:    Searches for an element in the tree

 Parameters: pt: binarytree interface pointer.
             val: value to search for.

 Returns:     pointer to found element or NULL if not found.

 Example:     int * p  = tree_find(pt, &val);
==============================================================================*/
genptr tree_find(treeptr pt, const genptr val)
{
        assert(pt && val);
	nodeptr p = tree_inner_find(pt->root, val, pt->cmp);
	return (p != NULL) ? p->data : NULL;
}
/**=============================================================================
 Function:   tree_visit

 Purpose:    Traverses the tree and executes the user provided functor upon
             the element.

 Parameters: pt: binarytree interface pointer.
             traverse_order: pre, in, or postorder traversal.

 Returns:     void

 Example:     tree_visit(pt, print_node);
==============================================================================*/
void tree_visit(treeptr pt, enum traverse_order order, void (*vis)(const genptr))
{
	assert(pt && vis);
	if(order == pre) 
		tree_preorder_visit(pt->root, vis);
	else if(order == in)
		tree_inorder_visit(pt->root, vis);
	else
		tree_postorder_visit(pt->root, vis);

	printf("\nTree Total: %d\n", pt->count);
}

nodeptr tree_nodupes_add(nodeptr p, const genptr val, 
	int(*cmp)(const genptr v1, const genptr v2), nodeptr parent)
{
	assert(val && cmp);
	int cond=0;
	if(p == NULL) 
		p = node_alloc(val, parent);
	else if((cond = cmp(val, p->data)) < 0)
		p->left = tree_nodupes_add(p->left, val, cmp, p);
	else if( cond > 0)
		p->right = tree_nodupes_add(p->right, val, cmp, p);
	else
		p->count++;
		
	return p;
}
nodeptr tree_dupes_add(nodeptr p, const genptr val, 
	int(*cmp)(const genptr v1, const genptr v2), nodeptr parent)
{
	assert(val && cmp);
	if(p == NULL) 
		p = node_alloc(val, parent);
	else if(cmp(val, p->data) < 0)
		p->left = tree_dupes_add(p->left, val, cmp, p);
	else 
		p->right = tree_dupes_add(p->right, val, cmp, p);
		
	return p;
}
void tree_inorder_visit(nodeptr p, void(*vis)(const genptr))
{
	assert(vis);
	if(p) {
		tree_inorder_visit(p->left, vis);
		vis(p->data);
		tree_inorder_visit(p->right, vis);
	}
}
void tree_preorder_visit(nodeptr p, void(*vis)(const genptr))
{
	assert(vis);
	if(p) {
		vis(p->data);
		tree_inorder_visit(p->left, vis);
		tree_inorder_visit(p->right, vis);
	}
}
void tree_postorder_visit(nodeptr p, void(*vis)(const genptr))
{
	assert(vis);
	if(p) {
		tree_inorder_visit(p->left, vis);
		tree_inorder_visit(p->right, vis);
		vis(p->data);
	}
}
void tree_postorder_free(nodeptr p)
{
	if(p) {
		tree_postorder_free(p->left);
		tree_postorder_free(p->right);
		Heap_Free(p);
		p = NULL;
	}
}
/**=============================================================================
 Function:   tree_delete_node

 Purpose:    Searches for an element and if found, deletes it and fixes up tree.

 Parameters: pt: binarytree interface pointer.
             val: value to search/delete.

 Returns:    void

 Example:     tree_delete_node(pt, &val);
==============================================================================*/
/* I got the basic idea from Kruse, Tondo, and Leung, I think */
void tree_delete_node(treeptr pt, const genptr k)
{
	nodeptr x = tree_inner_find(pt->root, k, pt->cmp);
	if (x == NULL)
		return;

	nodeptr parent = NULL;
	nodeptr xsucc = NULL;

	// if the node to be deleted has two children
	// not using node->_parent for the _root case, fixup later
	if (x->left != NULL && x->right != NULL) {
		parent = x;
		xsucc = x->right;

		while (xsucc->left != NULL) {
			parent = xsucc;
			xsucc = xsucc->left;
		}

		x = xsucc;
	}
	// fixup parent if not initialized from previous block
	if (parent == NULL && x != pt->root)
		parent = x->parent;

	// if the node to be deleted has no child
	if (x->left == NULL && x->right == NULL)
		if (x == pt->root)
			pt->root = NULL;
		else if (parent->right == x)
			parent->right = NULL;
		else
			parent->left = NULL;

	// has right child only. 
	else if (x->right != NULL)  
		if (x == pt->root)
			pt->root = x->right;
		else if (parent->left == x)
			parent->left = x->right;
		else
			parent->right = x->right;

	// has left child only
	else
		if (x == pt->root)
			pt->root = x->left;
		else if (parent->left == x)
			parent->left = x->left;
		else
			parent->right = x->left;

	Heap_Free(x);
	x = NULL;
}

nodeptr tree_inner_find(nodeptr p, const genptr k, 
	int(*cmp)(const genptr v1, const genptr v2))
{
	if (p == NULL)
		return NULL;

	int comp = 0;
	if ((comp = cmp(k, p->data)) == 0)
		return p;

	if (comp < 0)
        	tree_inner_find(p->left, k, cmp);
	else
	        tree_inner_find(p->right, k, cmp);

        return p;
}
#pragma GCC diagnostic pop
