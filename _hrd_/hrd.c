#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hrd.h"

static uint32_t SINGLE_NUM;
static uint32_t VERTICAL_NUM;
static uint32_t HORIZONTAL_NUM;
static uint32_t BOX_NUM;

#define _check_box(p_board_,l_)												\
				(l_ % B_W + 1 < B_W &&										\
				 l_ / B_W + 1 < B_H &&										\
				 p_board_->grid[l_]           == nil &&						\
				 p_board_->grid[l_ + 1]       == nil &&						\
				 p_board_->grid[l_ + B_W]     == nil &&						\
				 p_board_->grid[l_ + B_W + 1] == nil)						

#define _check_horizontal(p_board_,l_)										\
				(l_ % B_W + 1 < B_W &&										\
				 p_board_->grid[l_]     == nil &&							\
				 p_board_->grid[l_ + 1] == nil)

#define _check_vertical(p_board_,l_)										\
				(l_ / B_W + 1 < B_H &&										\
				 p_board_->grid[l_]       == nil &&							\
				 p_board_->grid[l_ + B_W] == nil)

#define _check_single(p_board_,l_)											\
				(p_board_->grid[l_] == nil)

#define _set_box(p_board_,l_)												\
				do {														\
					p_board_->grid[l_]           = box;						\
					p_board_->grid[l_ + 1]       = box;						\
					p_board_->grid[l_ + B_W]     = box;						\
					p_board_->grid[l_ + B_W + 1] = box;						\
				} while (0)

#define _set_horizontal(p_board_,l_)										\
				do {														\
					p_board_->grid[l_]     = horizontal;					\
					p_board_->grid[l_ + 1] = horizontal;					\
				} while (0)

#define _set_vertical(p_board_,l_)											\
				do {														\
					p_board_->grid[l_]       = vertical;					\
					p_board_->grid[l_ + B_W] = vertical;					\
				} while (0)

#define _set_single(p_board_,l_)											\
				do {														\
					p_board_->grid[l_] = single;							\
				} while (0)

#define _keep_p_bd_a(...)													\
				do {														\
					if (p_bd_t != p_bd_a) {									\
						p_bd_b = p_bd_a;									\
						p_bd_a = p_bd_t;									\
					}														\
				} while (0)

void ini_grid(board_t* p_board) {
	memset(p_board, 0, sizeof(board_t));
}

void print_board(board_t* p_board) {
	uint32_t i;
	uint32_t j;
	for (i = 0; i < B_H; i++) {
		for (j = 0; j < B_W; j++) {
			printf("%d ", p_board->grid[i * B_W + j]);
		}
		printf("\n");
	}
}

static p_board_t create_boxes(p_board_t p_bd_a, p_board_t p_bd_b) {
	uint32_t i;
	uint32_t l;
	p_board_t p_a;
	p_board_t p_b;
	p_board_t p_t;

	for (i = 0; i < NN; i++) {
		p_bd_a[i].valid = 0;
		p_bd_a[i].l     = 0;
		p_bd_b[i].valid = 0;
	}

	ini_grid(p_bd_a);
	p_bd_a->valid = 1;

	for (i = 0; i < BOX_NUM; i++) {
		p_a = p_bd_a;
		for (p_b = p_bd_b; p_b->valid; p_b++) p_b->valid = 0;
		p_b = p_bd_b;
		for (; p_a->valid; p_a++) {
			for (l = p_a->l; l < GRID_NUM; l++) {
				if (_check_box(p_a, l)) {
					memcpy(p_b, p_a, sizeof(board_t));

					_set_box(p_b, l);

					p_b->l = l + 2;
					p_b++;
					_xx_assert(p_b < p_bd_b + NN);
				}
			}
			_xx_assert(p_a < p_bd_a + NN);
		}
		p_t = p_bd_a;
		p_bd_a = p_bd_b;
		p_bd_b = p_t;
	}
	
	return p_bd_a;
}

static p_board_t create_verticals(p_board_t p_bd_a, p_board_t p_bd_b) {
	uint32_t i;
	uint32_t l;
	p_board_t p_a;
	p_board_t p_b;
	p_board_t p_t;

	for (i = 0; i < NN; i++) {
		p_bd_a[i].l = 0;
		p_bd_b[i].valid = 0;
	}

	for (i = 0; i < VERTICAL_NUM; i++) {
		p_a = p_bd_a;
		for (p_b = p_bd_b; p_b->valid; p_b++) p_b->valid = 0;
		p_b = p_bd_b;
		for (; p_a->valid; p_a++) {
			for (l = p_a->l; l < GRID_NUM; l++) {
				if (_check_vertical(p_a, l)) {
					memcpy(p_b, p_a, sizeof(board_t));

					_set_vertical(p_b, l);

					p_b->l = l + 1;
					p_b++;
					_xx_assert(p_b < p_bd_b + NN);
				}
			}
			_xx_assert(p_a < p_bd_a + NN);
		}
		p_t = p_bd_a;
		p_bd_a = p_bd_b;
		p_bd_b = p_t;
	}
	
	return p_bd_a;
}

static p_board_t create_horizontals(p_board_t p_bd_a, p_board_t p_bd_b) {
	uint32_t i;
	uint32_t l;
	p_board_t p_a;
	p_board_t p_b;
	p_board_t p_t;

	for (i = 0; i < NN; i++) {
		p_bd_a[i].l = 0;
		p_bd_b[i].valid = 0;
	}

	for (i = 0; i < HORIZONTAL_NUM; i++) {
		p_a = p_bd_a;
		for (p_b = p_bd_b; p_b->valid; p_b++) p_b->valid = 0;
		p_b = p_bd_b;
		for (; p_a->valid; p_a++) {
			for (l = p_a->l; l < GRID_NUM; l++) {
				if (_check_horizontal(p_a, l)) {
					memcpy(p_b, p_a, sizeof(board_t));

					_set_horizontal(p_b, l);

					p_b->l = l + 2;
					p_b++;
					_xx_assert(p_b < p_bd_b + NN);
				}
			}
			_xx_assert(p_a < p_bd_a + NN);
		}
		p_t = p_bd_a;
		p_bd_a = p_bd_b;
		p_bd_b = p_t;
	}
	
	return p_bd_a;
}

static p_board_t create_singles(p_board_t p_bd_a, p_board_t p_bd_b) {
	uint32_t i;
	uint32_t l;
	p_board_t p_a;
	p_board_t p_b;
	p_board_t p_t;

	for (i = 0; i < NN; i++) {
		p_bd_a[i].l = 0;
		p_bd_b[i].valid = 0;
	}

	for (i = 0; i < SINGLE_NUM; i++) {
		p_a = p_bd_a;
		for (p_b = p_bd_b; p_b->valid; p_b++) p_b->valid = 0;
		p_b = p_bd_b;
		for (; p_a->valid; p_a++) {
			for (l = p_a->l; l < GRID_NUM; l++) {
				if (_check_single(p_a, l)) {
					memcpy(p_b, p_a, sizeof(board_t));

					_set_single(p_b, l);

					p_b->l = l + 1;
					p_b++;
					_xx_assert(p_b < p_bd_b + NN);
				}
			}
			_xx_assert(p_a < p_bd_a + NN);
		}
		p_t = p_bd_a;
		p_bd_a = p_bd_b;
		p_bd_b = p_t;
	}
	
	return p_bd_a;
}

uint32_t create_boards(p_board_t p_bd_a) {
	p_board_t p_bd_b = (p_board_t)malloc(sizeof(board_t) * NN);
	p_board_t p_bd_t;
	uint32_t i;

	p_bd_t = create_boxes(p_bd_a, p_bd_b);
	_keep_p_bd_a();

	p_bd_t = create_verticals(p_bd_a, p_bd_b);
	_keep_p_bd_a();

	p_bd_t = create_horizontals(p_bd_a, p_bd_b);
	_keep_p_bd_a();

#if 0
	p_bd_a[0].valid = 1;
	p_bd_a[1].valid = 0;
	uint32_t xxx[] = { 0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 2, 2, 2, 2, 4, 4, 2, 2, 4, 4 };
	for (i = 0; i < GRID_NUM; i++) {
		p_bd_a->grid[i] = xxx[i];
	}
#endif

	p_bd_t = create_singles(p_bd_a, p_bd_b);
	_keep_p_bd_a();

	p_board_t p_t = p_bd_a;
	for (i = 0; p_t[i].valid; i++) {
		//puts("xxxxxxxxxxxxxxxxxxxxxx");
		//print_board(p_t + i);
	}

	printf(">>total count: %u\n", i);
	if (i > NN) {
		printf("error: NN too small\n");
	}

	free(p_bd_b);

	return i;
}

static void find_rlt(p_board_t p_bd_a, p_board_t p_bd_b_) {
	p_board_t p_bd_b = p_bd_b_;
	uint32_t i;
	uint32_t nil_0;
	uint32_t nil_1;

	for (i = 0; i < RLT_NUM; i++) {
		p_bd_b[i].valid = 0;
	}

	for (i = 0; p_bd_a->grid[i]; i++);
	nil_0 = i;
	for (i = nil_0 + 1; p_bd_a->grid[i]; i++);
	nil_1 = i;

#	define _single_move(nil_x,op_,bd_,nb_,tp_)									\
				do {															\
					if (nil_x op_ B_W != bd_) {									\
						if (p_bd_a->grid[nil_x + nb_] == tp_) {					\
							memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
							p_bd_b->grid[nil_x] = tp_;							\
							p_bd_b->grid[nil_x + (nb_) + (nb_)] = nil;			\
							p_bd_b->valid = 1;									\
							p_bd_b++;											\
						} else if (p_bd_a->grid[nil_x + nb_] == single) {		\
							memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
							p_bd_b->grid[nil_x] = single;						\
							p_bd_b->grid[nil_x + (nb_)] = nil;					\
							p_bd_b->valid = 1;									\
							p_bd_b++;											\
						}														\
					}															\
				} while (0)

	_single_move(nil_0, %,       0,      -1, horizontal);
	_single_move(nil_0, %, B_W - 1,       1, horizontal);
	_single_move(nil_0, /,       0, 0 - B_W, vertical);
	_single_move(nil_0, /, B_H - 1,     B_W, vertical);

	_single_move(nil_1, %,       0,      -1, horizontal);
	_single_move(nil_1, %, B_W - 1,       1, horizontal);
	_single_move(nil_1, /,       0, 0 - B_W, vertical);
	_single_move(nil_1, /, B_H - 1,     B_W, vertical);

#	define _x_x_move(nil_x,op_,bd_,nb_,tp_)										\
				do {															\
					if (nil_x op_ B_W != bd_) {									\
						if (p_bd_a->grid[nil_x + (nb_)] == tp_) {				\
							memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
							p_bd_b->grid[nil_x] = tp_;							\
							p_bd_b->grid[nil_x - (nb_)] = tp_;					\
							p_bd_b->grid[nil_x + (nb_)] = nil;					\
							p_bd_b->grid[nil_x + (nb_) + (nb_)] = nil;			\
							p_bd_b->valid = 1;									\
							p_bd_b++;											\
						} else if (p_bd_a->grid[nil_x + (nb_)] == single) {		\
							memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
							p_bd_b->grid[nil_x - (nb_)] = single;				\
							p_bd_b->grid[nil_x + (nb_)] = nil;					\
							p_bd_b->valid = 1;									\
							p_bd_b++;											\
						}														\
					}															\
				} while (0)

#	define _x_y_move(op_,bd_,nb_,tp_)												\
				do {																\
					if (nil_0 op_ B_W != bd_) {										\
						uint32_t x_;												\
						uint32_t start_ = nil_0 + (nb_);							\
						uint32_t step_ = tp_ == horizontal ? 1 : B_W;				\
						uint32_t up_boundary_ = tp_ == horizontal ?					\
												start_ - (start_ % B_W) :			\
												start_ % B_W;						\
						if (p_bd_a->grid[nil_0 + (nb_)] == tp_) {					\
							for (x_ = start_; x_ != up_boundary_; x_ -= step_) {	\
								if (p_bd_a->grid[x_ - step_] != tp_) { break; }		\
							}														\
							if ((start_ - x_) / step_ % 2 == 0) {					\
								memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
								p_bd_b->grid[nil_0 + (nb_)] = nil;					\
								p_bd_b->grid[nil_1 + (nb_)] = nil;					\
								p_bd_b->grid[nil_0] = tp_;							\
								p_bd_b->grid[nil_1] = tp_;							\
								p_bd_b->valid = 1;									\
								p_bd_b++;											\
							}														\
						} else if (p_bd_a->grid[nil_0 + (nb_)] == box) {			\
							for (x_ = start_; x_ != up_boundary_; x_ -= step_) {	\
								if (p_bd_a->grid[x_ - step_] != box) { break; }		\
							}														\
							if ((start_ - x_) / step_ % 2 == 0) {					\
								memcpy(p_bd_b, p_bd_a, sizeof(board_t));			\
								p_bd_b->grid[nil_0 + (nb_) + (nb_)] = nil;			\
								p_bd_b->grid[nil_1 + (nb_) + (nb_)] = nil;			\
								p_bd_b->grid[nil_0] = box;							\
								p_bd_b->grid[nil_1] = box;							\
								p_bd_b->valid = 1;									\
								p_bd_b++;											\
							}														\
						}															\
					}																\
				} while (0)

#	define _single_move_2(nil_x,op_,bd_,nb_)										\
				do {																\
					if (nil_x op_ B_W != bd_) {										\
						if (p_bd_a->grid[nil_x + (nb_)] == single) {				\
							memcpy(p_bd_b, p_bd_a, sizeof(board_t));				\
							p_bd_b->grid[nil_x + (nb_)] = nil;						\
							p_bd_b->grid[(nil_x == nil_0 ? nil_1 : nil_0)] = single;\
							p_bd_b->valid = 1;										\
							p_bd_b++;												\
						}															\
					}																\
				} while (0)

	if (nil_0 + 1 == nil_1 && nil_1 % B_W != 0) {
		_x_x_move(nil_0, %,       0,      -1, horizontal);
		_x_x_move(nil_1, %, B_W - 1,       1, horizontal);

		_x_y_move(       /,       0, 0 - B_W, horizontal);
		_x_y_move(       /, B_H - 1,     B_W, horizontal);

		_single_move_2(nil_0, /,       0, 0 - B_W);
		_single_move_2(nil_1, /,       0, 0 - B_W);
		_single_move_2(nil_0, /, B_H - 1,     B_W);
		_single_move_2(nil_1, /, B_H - 1,     B_W);

	} else if (nil_0 + B_W == nil_1) {
		_x_x_move(nil_0, /,       0, 0 - B_W, vertical);
		_x_x_move(nil_1, /, B_H - 1,     B_W, vertical);

		_x_y_move(       %,       0,      -1, vertical);
		_x_y_move(       %, B_W - 1,       1, vertical);

		_single_move_2(nil_0, %,       0, 0 - 1);
		_single_move_2(nil_1, %,       0, 0 - 1);
		_single_move_2(nil_0, %, B_W - 1,     1);
		_single_move_2(nil_1, %, B_W - 1,     1);
	}

#	if 0
	for (i = 0; p_bd_b_[i].valid; i++) {
		uint32_t j;
		uint32_t xxx = 0;
		for (j = 0; j < GRID_NUM; j++) {
			if (p_bd_b_[i].grid[j] == single) {
				xxx++;
			}
		}
		if (xxx != 4) {
			puts("--------------");
			print_board(p_bd_a);
			puts("--------------");
			print_board(p_bd_b_ + i);
			while (1);
		}
	}
#	endif

}

p_hash_t create_hash(p_board_t p_bd_a) {
	p_hash_t hash_table = (p_hash_t)malloc(sizeof(hash_t) * HASH_SIZE);
	memset(hash_table, 0, sizeof(hash_t) * HASH_SIZE);
	uint32_t i;
	uint32_t j;
	uint32_t x;
	for (i = 0; p_bd_a[i].valid; i++) {
		_hash_(p_bd_a[i].grid, x);
		hash_table[x].cnt++;
	}
	uint32_t max = 0;
	for (i = 0; i < HASH_SIZE; i++) {
		max = max > hash_table[i].cnt ? max : hash_table[i].cnt;
	}
	printf("max hash value: %u\n", max);

	for (i = 0; i < HASH_SIZE; i++) {
		uint32_t cnt;
		if (cnt = hash_table[i].cnt) {
			hash_table[i].p_idx = (uint32_t *)malloc(sizeof(uint32_t) * cnt);
			memset(hash_table[i].p_idx, 0, sizeof(uint32_t) * cnt);
		}
	}

	for (i = 0; p_bd_a[i].valid; i++) {
		_hash_(p_bd_a[i].grid, x);
		uint32_t *p_idx = hash_table[x].p_idx;
		_xx_assert(p_idx);
		while (*p_idx) p_idx++;
		*p_idx = i;
		_xx_assert(p_idx - hash_table[x].p_idx < hash_table[x].cnt);
	}

#	if 1
	uint32_t *t_h = (uint32_t *)malloc(sizeof(uint32_t) * NN);
	memset(t_h, 0, sizeof(uint32_t) * NN);
	for (i = 0; i < HASH_SIZE; i++) {
		for (j = 0; j < hash_table[i].cnt; j++) {
			t_h[hash_table[i].p_idx[j]]++;
		}
	}
	for (i = 0; i < HASH_SIZE && t_h[i] == 1; i++) {}
	printf("valid hash num: %u\n", i);
#	endif
	
	return hash_table;
}

void delete_hash(p_hash_t hash_table) {
	uint32_t i;
	for (i = 0; i < HASH_SIZE; i++) {
		if (hash_table[i].p_idx) {
			free(hash_table[i].p_idx);
			hash_table[i].p_idx = NULL;
		}
	}
	free(hash_table);
}

p_node_t build_rlt(p_board_t p_bd_a, uint32_t count, p_hash_t hash_table) {
	uint32_t i;
	uint32_t j;

	p_board_t p_bd_b = (p_board_t)malloc(sizeof(board_t) * RLT_NUM);
	
#	if 0
	uint32_t xxx[] = {
		0, 4, 4, 2,
		0, 4, 4, 2,
		2, 1, 2, 2,
		2, 1, 2, 2,
		3, 3, 1, 1,
	};
	p_bd_a = (p_board_t)malloc(sizeof(board_t)*2);
	p_bd_a[0].valid = 1;
	p_bd_a[1].valid = 0;
	for (i = 0; i < GRID_NUM; i++) {
		p_bd_a->grid[i] = xxx[i];
	}

	print_board(p_bd_a);
	find_rlt(p_bd_a, p_bd_b);

	for (i = 0; p_bd_b[i].valid; i++) {
		printf(">> %u\n", i);
		print_board(p_bd_b + i);
	}
#	endif

	p_node_t p_node = (p_node_t)malloc(sizeof(node_t) * count);

	for (i = 0; p_bd_a[i].valid; i++) {
#		if 0
		printf(">i: %u----------------\n", i);
		print_board(p_bd_a + i);
#		endif

		uint32_t x;
		p_node[i].idx = i;
		find_rlt(&p_bd_a[i], p_bd_b);
		for (j = 0; p_bd_b[j].valid; j++) {
#			if 0
				printf("--i: %u, j: %u\n", i, j);
				print_board(p_bd_b + j);
#			endif
			_hash_(p_bd_b[j].grid, x);
			uint32_t *p_idx = hash_table[x].p_idx;
			_xx_assert(p_idx);
			uint32_t k;
			for (k = 0; ; k++) {
#			if 0
				printf("---k: %u--\n", k);
				print_board(p_bd_a + p_idx[k]);
#			endif
				_xx_assert(k < hash_table[x].cnt);
				_xx_assert(p_idx[k] < count);
				if (memcmp(p_bd_a + p_idx[k], p_bd_b + j, sizeof(piece_t) * GRID_NUM) == 0) {
					break;
				}
			}
			p_node[i].p_idx[j] = p_idx[k];
			p_node[i].cnt++;
		}
	}


	return p_node;
}

uint32_t get_final_step(uint32_t *buf_a, uint32_t *buf_b, uint32_t s_idx, p_node_t p_node, uint32_t *f_idx_, uint32_t *uf_idx_) {
	p_node[s_idx].is_used = 1;
	uint32_t i;
	uint32_t j;
	uint32_t c_a = 1;
	uint32_t c_b;
	uint32_t f_idx;
	uint32_t uf_idx;
	uint32_t find = 0;
	uint32_t stp_cnt;
	buf_a[0] = s_idx;
	if (p_node[s_idx].is_end) {
		*f_idx_ = s_idx;
		*uf_idx_ = s_idx;
		return 0;
	}
	
	for (stp_cnt = 1; ; stp_cnt++) {
		//printf(">step: %u, c_a: %u\n", stp_cnt, c_a);
		c_b = 0;
		if (!c_a) {
			printf("no answer\n");
			break;
		}
		for (i = 0; i < c_a; i++) {
			uint32_t idx = buf_a[i];
			uint32_t *p_idx = p_node[idx].p_idx;
			uint32_t cnt = p_node[idx].cnt;
			for (j = 0; j < cnt; j++) {
				if (p_node[p_idx[j]].is_used) { continue; }
				if (p_node[p_idx[j]].is_end) {
					uf_idx = idx;
					f_idx = p_idx[j];
					find = 1;
					break;
				}
				p_node[p_idx[j]].is_used = 1;
				buf_b[c_b++] = p_idx[j];
				if (c_b > BUF_SIZE) {
					puts("error: buf_size too small");
					f_idx = s_idx;
					find = 1;
					break;
				}
			}
			if (find) { break; }
		}
		if (find) { break; }
		c_a = c_b;
		uint32_t *buf_t = buf_a;
		buf_a = buf_b;
		buf_b = buf_t;
	}

	*f_idx_ = f_idx;
	*uf_idx_ = uf_idx;

	return stp_cnt;
}

void get_asr(uint32_t *start) {
	uint32_t i;
	SINGLE_NUM = 0;
	VERTICAL_NUM = 0;
	HORIZONTAL_NUM = 0;
	BOX_NUM = 0;

	for (i = 0; i < GRID_NUM; i++) {
		if (start[i] == single) { SINGLE_NUM++; }
		if (start[i] == vertical) { VERTICAL_NUM++; }
		if (start[i] == horizontal) { HORIZONTAL_NUM++; }
		if (start[i] == box) { BOX_NUM++; }
	}
	SINGLE_NUM /= 1;
	VERTICAL_NUM /= 2;
	HORIZONTAL_NUM /= 2;
	BOX_NUM /= 4;

	p_board_t p_bd_a = (p_board_t)malloc(sizeof(board_t) * NN);

	uint32_t count = create_boards(p_bd_a);

	p_hash_t hash_table = create_hash(p_bd_a);
	p_node_t p_node = build_rlt(p_bd_a, count, hash_table);

	for (i = 0; i < count; i++) {
		if (p_node[i].cnt == 0) {
			puts("------------");
			print_board(p_bd_a + p_node[i].idx);
		}
	}

	uint32_t end_cnt = 0;
	for (i = 0; i < count; i++) {
		p_node[i].is_end = 0;
		p_node[i].is_used = 0;
		if (p_bd_a[i].grid[13] == box && p_bd_a[i].grid[14] == box && p_bd_a[i].grid[17] == box) {
			_xx_assert(p_bd_a[i].grid[18] == box);
			p_node[i].is_end = 1;
			end_cnt++;
		}
	}
	printf("end count: %u\n", end_cnt);

	uint32_t x;
	_hash_(start, x);
	uint32_t s_idx;
	for (i = 0; i < hash_table[x].cnt; i++) {
		if (memcmp(start, p_bd_a[hash_table[x].p_idx[i]].grid, sizeof(uint32_t) * GRID_NUM) == 0) {
			s_idx = hash_table[x].p_idx[i];
			break;
		}
	}
	puts("-----start-----");
	print_board(p_bd_a + s_idx);
	uint32_t *buf_a = (uint32_t *)malloc(sizeof(uint32_t) * BUF_SIZE);
	uint32_t *buf_b = (uint32_t *)malloc(sizeof(uint32_t) * BUF_SIZE);

	uint32_t f_idx;
	uint32_t uf_idx;
	uint32_t stp_cnt;

	stp_cnt = get_final_step(buf_a, buf_b, s_idx, p_node, &f_idx, &uf_idx);

	printf("steps count: %u\n", stp_cnt);
	//print_board(&p_bd_a[f_idx]);

	uint32_t *p_asr = (uint32_t*)malloc(sizeof(uint32_t) * stp_cnt);
	for (i = stp_cnt; i-- != 0; ) {
		p_asr[i] = f_idx;
		uint32_t j;
		for (j = 0; j < count; j++) {
			p_node[j].is_used = 0;
			p_node[j].is_end= 0;
		}
		p_node[s_idx].is_used = 1;
		p_node[uf_idx].is_end = 1;
		if (i != get_final_step(buf_a, buf_b, s_idx, p_node, &f_idx, &uf_idx)) {
			printf("error!");
		}
	}
	for (i = 0; i < stp_cnt; i++) {
		printf("step <%u> -----------\n", i + 1);
		print_board(p_bd_a + p_asr[i]);
	}

	delete_hash(hash_table);
	free(p_bd_a);
	free(p_node);
	free(buf_a);
	free(buf_b);
	free(p_asr);
}
