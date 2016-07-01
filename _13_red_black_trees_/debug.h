#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "red_black_tree.h"

#define _xx_assert_(xx) assert(xx)

#define NIL		(&p_tree->nil)
#define ROOT	(p_tree->p_root)

#define _set_nil(p_node) (p_node = NIL)

#define _is_red(p_node) (p_node->color == red)

#define _set_left_child(parent,child)								\
			do {													\
				parent->p_left = child;								\
				child->p_parent = parent;							\
			} while (0)

#define _set_right_child(parent,child)								\
			do {													\
				parent->p_right = child;							\
				child->p_parent = parent;							\
			} while (0)

#define _set_child(parent,left,right)								\
			do {													\
				_set_left_child(parent, left);						\
				_set_right_child(parent, right);					\
			} while (0)

#define _ini_insert_node(value,p_insert,parent)						\
			do {													\
				p_insert = (PRB_node_t)malloc(sizeof(RB_node_t));	\
				p_insert->value = value;							\
				p_insert->cnt = 1;									\
				_set_nil(p_insert->p_left);							\
				_set_nil(p_insert->p_right);						\
				_set_red(p_insert);									\
				p_insert->p_parent = parent;						\
				if (value < parent->value) {						\
					_set_left_child(parent, p_insert);				\
				} else {											\
					_set_right_child(parent, p_insert);				\
				}													\
			} while (0)												

#define _change_child(parent,old_child,new_child)					\
			do {													\
				new_child->p_parent = parent;						\
				if (parent->p_left == old_child) {					\
					parent->p_left = new_child;						\
				} else {											\
					parent->p_right = new_child;					\
				}													\
			} while (0)

#define _set_red(p_node) p_node->color=red
#define _set_black(p_node) p_node->color=black

#define _set_root(p_node)											\
			do {													\
				_set_black(p_node);									\
				_set_nil(p_node->p_parent);							\
				ROOT = p_node;										\
			} while (0)

#define _reset_nil(...)												\
			do {													\
				NIL->p_parent = NIL;								\
				NIL->p_left = NIL;									\
				NIL->p_right = NIL;									\
			} while (0)

#define _swap_p(p1, p2) do { p_tmp_ = p1; p1 = p2; p2 = p_tmp_; } while (0)

#define _adopt_child(p_node)										\
			do {													\
				p_node->p_left->p_parent = p_node;					\
				p_node->p_right->p_parent = p_node;					\
			} while (0)

#define _swap_node(upper,lower)										\
			do {													\
				PRB_node_t p_tmp_;									\
				PRB_node_t upper_parent = upper->p_parent;			\
				printf("u: %u, l: %u\n", upper->value, lower->value);\
				uint32_t upper_color = upper->color;				\
				upper->color = lower->color;						\
				lower->color = upper_color;							\
				if (lower->p_parent == upper) {						\
					if (upper->p_left == lower) {					\
						upper->p_left = lower->p_left;				\
						lower->p_left = upper;						\
						_swap_p(upper->p_right, lower->p_right);	\
						_adopt_child(lower);						\
						_adopt_child(upper);						\
					} else {										\
						puts("0");\
						upper->p_right = lower->p_right;			\
						puts("1");\
						lower->p_right = upper;						\
						puts("2");\
						_swap_p(upper->p_left, lower->p_left);		\
						puts("3");\
						_adopt_child(lower);						\
						puts("4");\
						_adopt_child(upper);						\
						puts("5");\
					}												\
				} else {											\
					_swap_p(lower->p_left, upper->p_left);			\
					_swap_p(lower->p_right, upper->p_right);		\
					_swap_p(lower->p_parent, upper->p_parent);		\
					_change_child(upper->p_parent, lower, upper);	\
					_adopt_child(lower);							\
					_adopt_child(upper);							\
				}													\
				if (upper_parent == NIL) {							\
					_set_root(lower);								\
					/*_reset_nil();*/								\
					break;											\
				} else {											\
					_change_child(upper_parent, upper, lower);		\
					/*_reset_nil();*/								\
					break;											\
				}													\
			} while (0)
				
#define _rotate_left(parent,right)									\
			do {													\
				_change_child(parent->p_parent, parent, right);		\
				_set_right_child(parent, right->p_left);			\
				_set_left_child(right, parent);						\
			} while (0)

#define _rotate_right(parent,left)									\
			do {													\
				_change_child(parent->p_parent, parent, left);		\
				_set_left_child(parent, left->p_right);				\
				_set_right_child(left, parent);						\
			} while (0)

PRB_node_t _debug_RBTree_insert(PRB_tree_t p_tree, value_t value) {
	PRB_node_t p_insert = ROOT;

	//the first node insert to the tree
	if (p_insert == NIL) {
		ROOT = (PRB_node_t)malloc(sizeof(RB_node_t));
		ROOT->value = value;
		ROOT->cnt = 1;
		_set_nil(ROOT->p_parent);
		_set_nil(ROOT->p_left);
		_set_nil(ROOT->p_right);
		_set_black(ROOT);
		return ROOT;
	}

	PRB_node_t p_p;
	while (p_insert != NIL && p_insert->value != value) {
		p_p = p_insert;
		p_insert = value < p_insert->value ? p_insert->p_left : p_insert->p_right;
	}	//after this while loop, p_p mustn't be NIL

	//the insert value already in the tree, just add the `cnt' of the corresponding node.
	if (p_insert != NIL) {
		p_insert->cnt++;
		return p_insert;
	}

	//create one node, whose child nodes are `NIL' and color is red
	_ini_insert_node(value, p_insert, p_p);

	while (_is_red(p_p)) {
		//p_p is not the node of the tree, because its color is black
		PRB_node_t p_pp = p_p->p_parent; //p_pp is not `NIL'
		PRB_node_t p_ppp = p_pp->p_parent; 

		if (p_p == p_pp->p_left) {
			if (_is_red(p_pp->p_right)) {
				_set_black(p_p);
				_set_black(p_pp->p_right);
				if (p_ppp == NIL) {	//is this case p_pp is the root node, and already be black
					_xx_assert_(p_pp == ROOT);
					return p_pp;
				}
				_set_red(p_pp);
				p_insert = p_pp;
				p_p = p_ppp;
				continue;
			}
			if (p_insert == p_p->p_left) {
				_set_left_child(p_pp, p_p->p_right);
				_set_right_child(p_p, p_pp);
				_set_black(p_insert);
				if (p_ppp == NIL) {
					_set_root(p_p);
					return ROOT;
				}
				_change_child(p_ppp, p_pp, p_p);
				p_insert = p_p;
				p_p = p_ppp;
				continue;
			} else {
				_set_right_child(p_p, p_insert->p_left);
				_set_left_child(p_pp, p_insert->p_right);
				_set_child(p_insert, p_p, p_pp);
				_set_black(p_p);
				if (p_ppp == NIL) {
					_set_root(p_insert);
					return ROOT;
				}
				_change_child(p_ppp, p_pp, p_insert);
				p_p = p_ppp;
				continue;
			}
		} else {
			if (_is_red(p_pp->p_left)) {
				_set_black(p_p);
				_set_black(p_pp->p_left);
				if (p_ppp == NIL) {	//is this case p_pp is the root node, and already be black
					_xx_assert_(p_pp == ROOT);
					return ROOT;
				}
				_set_red(p_pp);
				p_insert = p_pp;
				p_p = p_ppp;
				continue;
			}
			if (p_insert == p_p->p_right) {
				_set_right_child(p_pp, p_p->p_left);
				_set_left_child(p_p, p_pp);
				_set_black(p_insert);
				if (p_ppp == NIL) {
					_set_root(p_p);
					return ROOT;
				}
				_change_child(p_ppp, p_pp, p_p);
				p_insert = p_p;
				p_p = p_ppp;
				continue;
			} else {
				_set_right_child(p_pp, p_insert->p_left);
				_set_left_child(p_p, p_insert->p_right);
				_set_child(p_insert, p_pp, p_p);
				_set_black(p_p);
				if (p_ppp == NIL) {
					_set_root(p_insert);
					return ROOT;
				}
				_change_child(p_ppp, p_pp, p_insert);
				p_p = p_ppp;
				continue;
			}
		}
	}
	return p_p;
}	

PRB_node_t _debug_RBTree_search(PRB_tree_t p_tree, PRB_node_t p_node, value_t value) {
	puts("qqqqqqqqqqqqqqqqqqqqq");
	RBTree_print(p_tree, NULL, 10);
	if (!p_node) p_node = ROOT;
	if (p_node == NIL) return NULL;
	while (p_node != NIL && p_node->value != value) {
		p_node = value < p_node->value ? p_node->p_left : p_node->p_right;
	}
	puts("qqqqqqqqqqqqqqqqqqqqq");
	RBTree_print(p_tree, NULL, 10);
	return p_node == NIL ? NULL : p_node;
}

PRB_node_t _debug_RBTree_delete_p(PRB_tree_t p_tree, PRB_node_t p_node) {
	if (!p_node || p_node == NIL) return NULL;
	
	uint32_t choose_left = p_node->value & 1;
	uint32_t delete_red;
	PRB_node_t p_p;
	PRB_node_t p_pp;
	PRB_node_t p_sibling;
	PRB_node_t p_tmp;

	if (p_node->p_left == NIL && p_node->p_right == NIL) {
		if (p_node == ROOT) {
			free(p_node);
			_set_nil(ROOT);
			return NULL;
		}
	} else {
		if (p_node->p_right == NIL || (p_node->p_left != NIL && choose_left)) {
			p_tmp = p_node->p_left;
			while (p_tmp->p_right != NIL) { p_tmp = p_tmp->p_right; }
		} else {
			p_tmp = p_node->p_right;
			while (p_tmp->p_left != NIL) { p_tmp = p_tmp->p_left; }
		}
		_swap_node(p_node, p_tmp); // swich the pointer's position, but another way, is just swiching ther pointer's value
	}

	RBTree_print(p_tree, NULL, 10);

	p_p = p_node->p_parent;
	p_pp = p_p->p_parent;
	p_sibling = p_node == p_p->p_left ? p_p->p_right : p_p->p_left;
	p_tmp = p_node;
	p_node = p_node->p_left != NIL ? p_node->p_left : p_node->p_right;	// p_node->p_left and p_node->p_right can be both NIL, so this line may make p_node be NIL, and in this case, the next line will make NIL's parent be p_p, this is significant and we will not `_reset_nil()'

	_change_child(p_p, p_tmp, p_node);

	delete_red = p_tmp->color == red;
	free(p_tmp);

	if (delete_red) {
		return NULL;
	}

	if (_is_red(p_node)) {							// p_node: red
		_set_black(p_node);
		return NULL;
	}

	// the sub tree's bh need to increase 1, whose root is `p_node'
	while (1) {
		if (_is_red(p_p)) {							// p_node: black, p_p: red, p_sibling: black
			if (p_node == p_p->p_left) {
				if (_is_red(p_sibling->p_left)) {
					p_tmp = p_sibling->p_left;
					_rotate_right(p_sibling, p_tmp);
					_xx_assert_(p_tmp == p_p->p_right);
					_rotate_left(p_p, p_tmp);
					return NULL;
				} else {
					_rotate_left(p_p, p_sibling);
					return NULL;
				}
			} else {
				if (_is_red(p_sibling->p_right)) {
					p_tmp = p_sibling->p_right;
					_rotate_left(p_sibling, p_tmp);
					_xx_assert_(p_tmp == p_p->p_left);
					_rotate_right(p_p, p_tmp);
					return NULL;
				} else {
					_rotate_right(p_p, p_sibling);
					return NULL;
				}
			}
		} else if (_is_red(p_sibling)) {			// p_node: black, p_p: black, p_sibling: red
			if (p_node == p_p->p_left) {
				_rotate_left(p_p, p_sibling);
				if (p_pp == NIL) { _set_root(p_sibling); }
				_set_black(p_sibling);
				_set_red(p_p);
				p_pp = p_sibling;
				p_node = p_p->p_left;
				p_sibling = p_p->p_right;
			} else {
				_rotate_right(p_p, p_sibling);
				if (p_pp == NIL) { _set_root(p_sibling); }
				_set_black(p_sibling);
				_set_red(p_p);
				p_pp = p_sibling;
				p_node = p_p->p_right;
				p_sibling = p_p->p_left;
			}
		} else {									// p_node: black, p_p: black, p_sibling: black
			if (p_node == p_p->p_left) {
				if (_is_red(p_sibling->p_left)) {
					p_tmp = p_sibling->p_left;
					_rotate_right(p_sibling, p_tmp);
					_rotate_left(p_p, p_tmp);
					if (p_pp == NIL) { _set_root(p_tmp); }
					_set_black(p_tmp);
					return NULL;
				} else {
					_rotate_left(p_p, p_sibling);
				}
			} else {
				if (_is_red(p_sibling->p_right)) {
					p_tmp = p_sibling->p_right;
					_rotate_left(p_sibling, p_tmp);
					_rotate_right(p_p, p_tmp);
					if (p_pp == NIL) { _set_root(p_tmp); }
					_set_black(p_tmp);
					return NULL;
				} else {
					_rotate_right(p_p, p_sibling);
				}
			}
			_set_red(p_p);
			if (p_pp == NIL) {
				_set_root(p_sibling);
				return NULL;
			}
			p_node = p_sibling;
			p_p = p_pp;
			p_pp = p_p->p_parent;
			_xx_assert_(p_node->p_parent == p_p);
			_xx_assert_(p_node == p_p->p_left || p_node == p_p->p_right);
			p_sibling = p_node == p_p->p_left ? p_p->p_right : p_p->p_left;
		}
	}
}

#undef _xx_assert_
#undef NIL
#undef ROOT
#undef _set_nil
#undef _is_red
#undef _set_left_child
#undef _set_right_child
#undef _set_child
#undef _ini_insert_node
#undef _change_child
#undef _set_red
#undef _set_black
#undef _set_root
#undef _reset_nil
#undef _swap_node
#undef _swap_p
#undef _adopt_child
#undef _rotate_left
#undef _rotate_right
