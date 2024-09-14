/*==============================================================================
 Name        : static_tree.h
 Author      : Stephen MacKenzie
 Copyright   : Licensed under GPL version 2 (GPLv2)
==============================================================================*/
#pragma once

typedef struct node node;
typedef struct binarytree binarytree; 
typedef node* nodeptr;
typedef binarytree* treeptr;
enum traverse_order{pre, in, post};

#ifdef __cplusplus
extern "C" {
#endif

treeptr tree_alloc(int(*cmp)(const genptr v1, const genptr v2), bool fdupes);
void tree_free(treeptr pt);
void tree_add(treeptr pt, const genptr val);
genptr tree_find(treeptr pt, const genptr val);
void tree_delete_node(treeptr pt, const genptr k);
void tree_visit(treeptr pt, enum traverse_order order, void (*vis)(const genptr));

#ifdef __cplusplus
}
#endif
