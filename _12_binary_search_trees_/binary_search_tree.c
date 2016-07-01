#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "binary_search_tree.h"

//inorder_tree_walk
uint32_t BSTree_inorder(PBSTree_t p_tree, value_t *buf) {
	uint32_t n;
	if (!p_tree) return 0;
	n = BSTree_inorder(p_tree->p_left, buf);
	uint32_t i;
	for (i = 0; i < p_tree->cnt; i++) {
		buf[n++] = p_tree->value;
	}
	return n + BSTree_inorder(p_tree->p_right, buf+n);
}

void BSTree_ini(PBSTree_t p_tree) {
	p_tree->value = 0;
	p_tree->cnt = 0;
	p_tree->p_parent = NULL;
	p_tree->p_left = NULL;
	p_tree->p_right = NULL;
}

#if 0
//insert recursive
PBSTree_t BSTree_insert(PBSTree_t p_tree, value_t value) {
	if (p_tree->cnt == 0) {
		p_tree->value = value;
		p_tree->cnt = 1;
		p_tree->p_parent = NULL;
		p_tree->p_left = NULL;
		p_tree->p_right = NULL;
		return p_tree;
	}
	if (value == p_tree->value) {
		p_tree->cnt++;
		return p_tree;
	}

	PBSTree_t *pp_child = (value < p_tree->value ? &p_tree->p_left : &p_tree->p_right);
	if (!(*pp_child)) {
		*pp_child = (PBSTree_t)malloc(sizeof(BSTree_t));
		(*pp_child)->value = value;
		(*pp_child)->cnt = 1;
		(*pp_child)->p_parent = p_tree;
		(*pp_child)->p_left = NULL;
		(*pp_child)->p_right = NULL;
		return *pp_child;
	} else {
		return BSTree_insert(*pp_child, value);
	}
}
#else 
//insert iteractive
PBSTree_t BSTree_insert(PBSTree_t p_tree, value_t value) {
	if (p_tree->cnt == 0) {
		p_tree->value = value;
		p_tree->cnt = 1;
		p_tree->p_parent = NULL;
		p_tree->p_left = NULL;
		p_tree->p_right = NULL;
		return p_tree;
	}

	PBSTree_t p_parent;
	while (p_tree && p_tree->value != value) {
		p_parent = p_tree;
		p_tree = p_tree->value > value ? p_tree->p_left : p_tree->p_right;
	}
	if (!p_tree) {
		PBSTree_t *pp_child = p_parent->value > value ? &p_parent->p_left : &p_parent->p_right;
		*pp_child = (PBSTree_t)malloc(sizeof(BSTree_t));
		(*pp_child)->value = value;
		(*pp_child)->cnt = 1;
		(*pp_child)->p_parent = p_parent;
		(*pp_child)->p_left = NULL;
		(*pp_child)->p_right = NULL;
		return *pp_child; 
	} else {
		p_tree->cnt++;
		return p_tree;
	}
}
#endif

void BSTree_delete_p(PBSTree_t p_tree) {
	if (!p_tree) return;
#if 0
	//plan A
	//change the pointer's value and keep the tree's structure
	if (!p_tree->p_left && !p_tree->p_right) {
		if(!p_tree->p_parent) {
			p_tree->cnt = 0;
			return;
		} else {
			PBSTree_t *pp_t;
			pp_t = p_tree->p_parent->p_left == p_tree ? &p_tree->p_parent->p_left : &p_tree->p_parent->p_right;
			free(*pp_t);
			*pp_t = NULL;
			return;
		}
	} else {
		PBSTree_t p_t;
		PBSTree_t *pp_t;
		if (!p_tree->p_left || (p_tree->p_right && p_tree->value & 1)) {
			p_t = BSTree_minimum(p_tree->p_right);
			p_tree->value = p_t->value;
			p_tree->cnt = p_t->cnt;
			pp_t = p_t->p_parent->p_left == p_t ? &p_t->p_parent->p_left : &p_t->p_parent->p_right;
			(*pp_t) = p_t->p_right;
			if (p_t->p_right) {
				p_t->p_right->p_parent = p_t->p_parent;
			}
			free(p_t);
			return;
		} else {
			p_t = BSTree_maximum(p_tree->p_left);
			p_tree->value = p_t->value;
			p_tree->cnt = p_t->cnt;
			pp_t = p_t->p_parent->p_left == p_t ? &p_t->p_parent->p_left : &p_t->p_parent->p_right;
			(*pp_t) = p_t->p_left;
			if (p_t->p_left) {
				p_t->p_left->p_parent = p_t->p_parent;
			}
			free(p_t);
			return;
		}
	}
#else
	//plan B (refer to the pseudocode in "Introduction to Algorithms")
	//change the tree's structure, but don't change pointers' value
	//In this case, it is more reasonable to use a structure, different from `BSTree_t', to keep the tree's root. Here I take a compromise method, that keeping the root pointer but change its value when the delete pointor is root. It may seem meaningless and boring, but it's just a test
	if (!p_tree->p_parent) {
		//copy from plan A
		PBSTree_t p_t;
		PBSTree_t *pp_t;
		if (!p_tree->p_left && !p_tree->p_right) {
			p_tree->cnt = 0;
			return;
		} else if (!p_tree->p_left || (p_tree->p_right && p_tree->value & 1)) {
			p_t = BSTree_minimum(p_tree->p_right);
			p_tree->value = p_t->value;
			p_tree->cnt = p_t->cnt;
			pp_t = p_t->p_parent->p_left == p_t ? &p_t->p_parent->p_left : &p_t->p_parent->p_right;
			(*pp_t) = p_t->p_right;
			if (p_t->p_right) {
				p_t->p_right->p_parent = p_t->p_parent;
			}
			free(p_t);
			return;
		} else {
			p_t = BSTree_maximum(p_tree->p_left);
			p_tree->value = p_t->value;
			p_tree->cnt = p_t->cnt;
			pp_t = p_t->p_parent->p_left == p_t ? &p_t->p_parent->p_left : &p_t->p_parent->p_right;
			(*pp_t) = p_t->p_left;
			if (p_t->p_left) {
				p_t->p_left->p_parent = p_t->p_parent;
			}
			free(p_t);
			return;
		}
	}

	if (!p_tree->p_left) {
		if (p_tree == p_tree->p_parent->p_left) {
			p_tree->p_parent->p_left = p_tree->p_right;
		} else {
			p_tree->p_parent->p_right = p_tree->p_right;
		}
		if (p_tree->p_right) {
			p_tree->p_right->p_parent = p_tree->p_parent;
		}
		free(p_tree);
		return;
	} else if (!p_tree->p_right) {
		if (p_tree == p_tree->p_parent->p_left) {
			p_tree->p_parent->p_left = p_tree->p_left;
		} else {
			p_tree->p_parent->p_right = p_tree->p_left;
		}
		p_tree->p_left->p_parent = p_tree->p_parent;
		free(p_tree);
		return;
	} else if (p_tree->value & 1) {
		PBSTree_t p_t = BSTree_maximum(p_tree->p_left);
		if (p_t == p_tree->p_left) {
			p_t->p_right = p_tree->p_right;
			p_tree->p_right->p_parent = p_t;
			p_t->p_parent = p_tree->p_parent;
			if (p_tree == p_tree->p_parent->p_left) {
				p_tree->p_parent->p_left = p_t;
			} else {
				p_tree->p_parent->p_right = p_t;
			}
			free(p_tree);
			return;
		} else {
			if (p_t->p_left) {
				p_t->p_left->p_parent = p_t->p_parent;
			}
			p_t->p_parent->p_right = p_t->p_left;
			p_t->p_left = p_tree->p_left;
			p_tree->p_left->p_parent = p_t;
			p_t->p_right = p_tree->p_right;
			p_tree->p_right->p_parent = p_t;
			p_t->p_parent = p_tree->p_parent;
			if (p_tree == p_tree->p_parent->p_left) {
				p_tree->p_parent->p_left = p_t;
			} else {
				p_tree->p_parent->p_right = p_t;
			}
			free(p_tree);
			return;
		}
	} else {
		PBSTree_t p_t = BSTree_minimum(p_tree->p_right);
		if (p_t == p_tree->p_right) {
			p_t->p_left = p_tree->p_left;
			p_tree->p_left->p_parent = p_t;
			p_t->p_parent = p_tree->p_parent;
			if (p_tree == p_tree->p_parent->p_left) {
				p_tree->p_parent->p_left = p_t;
			} else {
				p_tree->p_parent->p_right = p_t;
			}
			free(p_tree);
			return;
		} else {
			if (p_t->p_right) {
				p_t->p_right->p_parent = p_t->p_parent;
			}
			p_t->p_parent->p_left = p_t->p_right;
			p_t->p_right = p_tree->p_right;
			p_tree->p_right->p_parent = p_t;
			p_t->p_left = p_tree->p_left;
			p_tree->p_left->p_parent = p_t;
			p_t->p_parent = p_tree->p_parent;
			if (p_tree == p_tree->p_parent->p_left) {
				p_tree->p_parent->p_left = p_t;
			} else {
				p_tree->p_parent->p_right = p_t;
			}
			free(p_tree);
			return;
		}
	}
#endif
}

void BSTree_delete_v(PBSTree_t p_tree, value_t value) {
	;
}

uint32_t BSTree_height(PBSTree_t p_tree) {
	if (!p_tree) return 0;
	uint32_t l_h = BSTree_height(p_tree->p_left);
	uint32_t r_h = BSTree_height(p_tree->p_right);
	return 1 + (l_h > r_h ? l_h : r_h);
}

PBSTree_t BSTree_search(PBSTree_t p_tree, value_t value) {
	while (p_tree && p_tree->cnt && p_tree->value != value) {
		p_tree = p_tree->value > value ? p_tree->p_left : p_tree->p_right;
	}

	return p_tree;
}

PBSTree_t BSTree_minimum(PBSTree_t p_tree) {
	if (!p_tree) {
		return NULL;
	}
	while (p_tree->p_left) {
		p_tree = p_tree->p_left;
	}
	return p_tree;
}

PBSTree_t BSTree_maximum(PBSTree_t p_tree) {
	if (!p_tree) {
		return NULL;
	}
	while (p_tree->p_right) {
		p_tree = p_tree->p_right;
	}
	return p_tree;
}

PBSTree_t BSTree_successor(PBSTree_t p_tree) {
	if (!p_tree) {
		return NULL;
	}

	//if p_tree has right child tree, p_tree's successor must be the minimum of p_tree's right child tree
	if (p_tree->p_right) {
		return BSTree_minimum(p_tree->p_right);
	}
	
	//if p_tree has no right child tree, p_tree's successor should be p_tree's lowest ancestor tree in which p_tree is a left descendant
	PBSTree_t p_tmp = p_tree->p_parent;
	while (p_tmp && p_tmp->p_right == p_tree) {
		p_tree = p_tmp;
		p_tmp = p_tree->p_parent;
	}
	return p_tmp; 
}

PBSTree_t BSTree_predecessor(PBSTree_t p_tree) {
	if(!p_tree) {
		return NULL;
	}

	//if p_tree has left child tree, p_tree's predecessor must be the maximum of p_tree's left child tree
	if (p_tree->p_left) {
		return BSTree_maximum(p_tree->p_left);
	}

	//if p_tree has no left child tree, p_tree's predecessor should be p_tree's lowest ancestor tree in which p_tree is a right descendant
	PBSTree_t p_tmp = p_tree->p_parent;
	while (p_tmp && p_tmp->p_left == p_tree) {
		p_tree = p_tmp;
		p_tmp = p_tree->p_parent;
	}
	return p_tmp;
}

uint32_t BSTree_size(PBSTree_t p_tree) {
	if (!p_tree) {
		return 0;
	}
	return 1 + BSTree_size(p_tree->p_left) + BSTree_size(p_tree->p_right);

}

//<begin print tree>
uint32_t digit_wide(uint32_t digit) {
	uint32_t w = 1;
	while (digit /= 10) {
		w++;
	}
	return w;
}
void print_one_digit(uint32_t digit, uint32_t wide, uint32_t x) {
	uint32_t d_w = digit_wide(digit);
	uint32_t gap = wide/2 - d_w/2;
	uint32_t i;
	for (i = 0; i < gap; i++) printf(" ");
	if (x) {
		printf("%u", digit);
	} else {
		printf("#");
	}
	for (i += d_w; i < wide; i++) printf(" ");
	return;
}
void print_void_node(uint32_t wide, uint32_t h) {
	wide >>= h;
	uint32_t i;
	for (i = 0; i < (1<<(h-1)); i++ ) {
		print_one_digit(0, wide, 0);
	}
}
void bstree_print_one_layer(PBSTree_t p_tree, uint32_t h, uint32_t w) {
	if (!p_tree) return;
	if (!h) {
		print_one_digit(p_tree->value, w, 1);
		return;
	}
	if (!p_tree->p_left) {
		print_void_node(w, h);
	} else {
		bstree_print_one_layer(p_tree->p_left, h-1, w/2);
	}
	if (!p_tree->p_right) {
		print_void_node(w, h);
	} else {
		bstree_print_one_layer(p_tree->p_right, h-1, w/2);
	}
	
}
void BSTree_print(PBSTree_t p_tree, uint32_t h_1) {
	if (!h_1) return;
	uint32_t h = BSTree_height(p_tree);
	h = h > h_1 ? h_1 : h;
	uint32_t dw = digit_wide(BSTree_maximum(p_tree)->value) + 2;
	uint32_t l_w = dw * (1<<(h-1));
	uint32_t i;
	for (i = 0; i < h; i++) {
		bstree_print_one_layer(p_tree, i, l_w);
		printf("\n");
	}
}
//<end print tree>
