#pragma once
#include <stdint.h>

typedef uint32_t value_t;

typedef struct BSTree_t_ {
	value_t				value;
	uint32_t			cnt;
	struct BSTree_t_	*p_parent;
	struct BSTree_t_	*p_left;
	struct BSTree_t_	*p_right;

} BSTree_t, *PBSTree_t;


void BSTree_ini(PBSTree_t p_tree);
PBSTree_t BSTree_insert(PBSTree_t p_tree, value_t value);
void BSTree_delete_v(PBSTree_t p_tree, value_t value);
void BSTree_delete_p(PBSTree_t p_tree);
uint32_t BSTree_inorder(PBSTree_t p_tree, value_t *buf);
uint32_t BSTree_height(PBSTree_t p_tree);
PBSTree_t BSTree_search(PBSTree_t p_tree, value_t value);
PBSTree_t BSTree_minimum(PBSTree_t p_tree);
PBSTree_t BSTree_maximum(PBSTree_t p_tree);
PBSTree_t BSTree_successor(PBSTree_t p_tree);
PBSTree_t BSTree_predecessor(PBSTree_t p_tree);
uint32_t BSTree_size(PBSTree_t p_tree);
void BSTree_print(PBSTree_t p_tree, uint32_t n);

