#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define BUF_SIZE (1<<30)

#define _xx_assert(x)	assert(x)

#define B_W				(4)
#define B_H				(5)
#define GRID_NUM		(B_W * B_H)

//#define SINGLE_NUM		(4)
//#define VERTICAL_NUM	(4)
//#define HORIZONTAL_NUM	(1)
//#define BOX_NUM			(1)

#define NN	(1<<18)

#define RLT_NUM	(4 + 4 + 4 + 4)

#define HASH_SIZE (1<<20)
#define _hash_(grid_,x_)																		\
			do {																				\
				const uint32_t select_10_for_hash[10] = { 0, 3, 5, 7, 8, 10, 13, 15, 16, 18 };	\
				x_ = 0;																			\
				uint32_t i_;																	\
				for (i_ = 0; i_ < 10; i_++) {													\
					x_ <<= 2;																	\
					x_ = x_ | (grid_[select_10_for_hash[i_]] & 3);							\
				}																				\
			} while (0)

typedef enum {
	nil,
	single,
	vertical,
	horizontal,
	box,
} piece_t;

typedef struct board_t_{
	piece_t grid[GRID_NUM];
	uint32_t l;
	uint32_t valid;
} board_t, *p_board_t;

typedef struct hash_t_ {
	uint32_t	*p_idx;
	uint32_t	cnt;
} hash_t, *p_hash_t;

typedef struct node_t_{
	uint32_t	idx;
	uint32_t	p_idx[RLT_NUM];
	uint32_t	cnt;
	uint32_t	is_end;
	uint32_t	is_used;
} node_t, *p_node_t;

void ini_grid(p_board_t p_board);
void print_board(p_board_t p_board);
uint32_t create_boards();
p_node_t build_rlt(p_board_t p_bd_a, uint32_t count, p_hash_t hash_table);
p_hash_t create_hash(p_board_t p_bd_a);
void delete_hash(p_hash_t hash_table);
uint32_t get_final_step(uint32_t *buf_a, uint32_t *buf_b, uint32_t s_idx, p_node_t p_node, uint32_t *f_idx_, uint32_t *uf_idx_);
void get_asr(uint32_t *start);
