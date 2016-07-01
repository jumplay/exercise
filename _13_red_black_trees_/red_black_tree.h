#pragma once

#include <stdint.h>

typedef uint32_t value_t;

typedef struct RB_node_t_ {
	value_t				value;	//Here is a simple case, that the data itself if the keyword, maybe, in more complicated case, we should use a pointer to that points keyword and data
	uint32_t			cnt;	//Use this member to recode the data that has the same keyword
	struct RB_node_t_	*p_parent;
	struct RB_node_t_	*p_left;
	struct RB_node_t_	*p_right;
	enum { red, black }	color;
} RB_node_t, *PRB_node_t;

typedef struct RB_tree_t_{
	PRB_node_t p_root;
	RB_node_t nil;	//Seting leaf nodes and root's parent node to be nil does help to reduce judgements of conditions and make the codes of insert and delete to be more uniform. Use nil, we needn't care about if a node's parent or child was NULL, just need care about the color of the node's parent or child.
} RB_tree_t, *PRB_tree_t;

void RBTree_ini(PRB_tree_t p_tree);
PRB_node_t RBTree_insert(PRB_tree_t p_tree, value_t value);
uint32_t RBTree_height(PRB_tree_t p_tree, PRB_node_t p_node);
uint32_t RBTree_black_height(PRB_tree_t p_tree, PRB_node_t p_node);
PRB_node_t RBTree_minimum(PRB_tree_t p_tree, PRB_node_t p_node);
PRB_node_t RBTree_maximum(PRB_tree_t p_tree, PRB_node_t p_node);
uint32_t RBTree_inorder(PRB_tree_t p_tree, PRB_node_t p_node, value_t *buf);
uint32_t RBTree_size(PRB_tree_t p_tree, PRB_node_t p_node);
PRB_node_t RBTree_search(PRB_tree_t p_tree, PRB_node_t p_node, value_t value);
PRB_node_t RBTree_successor(PRB_tree_t p_tree, PRB_node_t p_node);
PRB_node_t RBTree_predecessor(PRB_tree_t p_tree, PRB_node_t p_node);
PRB_node_t RBTree_delete_p(PRB_tree_t p_tree, PRB_node_t p_node);
void RBTree_print(PRB_tree_t p_tree, PRB_node_t p_node, uint32_t h_1);
