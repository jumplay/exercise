#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "red_black_tree.h"
#include "debug.h"
#include "common.h"

#define N (1000000)
#define swap(a,b) do{ t=(a); (a)=(b); (b)=t; }while(0)

//////////////////////////////////////////////////////////
//#define PRINT_VALUE
//#define PRINT_TREE
//#define _dbg_
//////////////////////////////////////////////////////////

#ifndef _dbg_
#	define _bst_v_	(((0u+rand())<<16 + rand()) % N)
#else
	const uint32_t dbg_a[N] = { 2, 6, 6, 2, 8, 4, 0, 0, 4, 3 };
#	define _bst_v_		(dbg_a[i])
#	define DELETE_VALUE	0
#endif

void print_buf(uint32_t *buf) {
	uint32_t i;
	for (i = 0; i < N; i++) {
		printf(">%u\n", buf[i]);
	}
}

int cmp_uint32(const void *a, const void *b) {
	if (*(uint32_t*)a < *(uint32_t*)b) return -1;
	if (*(uint32_t*)a > *(uint32_t*)b) return 1;
	if (*(uint32_t*)a == *(uint32_t*)b) return 0;
}

int main() {
	srand((uint32_t)time(0));
	uint32_t *buf_q = (uint32_t*)malloc(sizeof(uint32_t)*N);
	uint32_t *buf_tmp = (uint32_t*)malloc(sizeof(uint32_t)*N);
	uint32_t *buf = (uint32_t*)malloc(sizeof(uint32_t)*N);
	uint32_t *buf_ro = (uint32_t*)malloc(sizeof(uint32_t)*N);
	uint32_t i;
	uint32_t t;
	uint32_t bh;

	uint32_t r_v = 0;

	RB_tree_t tree;
	PRB_tree_t p_tree = &tree;
	RBTree_ini(p_tree);

	PRB_node_t p_n;
	
	///////////////////////////////////////////////////////////////////
	//chect insert and inorder
	puts("--------------------------------------------");
	for (i = 0; i < N; i++) {
		uint32_t tmp = _bst_v_;
		buf_q[i] = tmp;
#		ifdef PRINT_VALUE
		printf("<%u>\n", tmp);
#		endif

		p_n = RBTree_insert(p_tree, tmp);

	}

#	ifdef PRINT_TREE
	RBTree_print(p_tree, NULL, N);
#	endif
	
	memcpy(buf_tmp, buf_q, sizeof(uint32_t)*N);
	if (N != RBTree_inorder(p_tree, NULL, buf)) {
		puts("!!!!inorder error");
		r_v = 1;
	}
	qsort(buf_q, N, sizeof(uint32_t), cmp_uint32);

	if (!memcmp(buf_q, buf, sizeof(uint32_t)*N)) {
		puts("insert and inorder success!");
	} else {
		puts("!!!!insert or inorder error!");
		r_v = 1;
	}

	t = 1;
	for (i = 1; i < N; i++) {
		if (buf[i-1] != buf[i]) {
			t++;
		}
	}
	if (RBTree_size(p_tree, NULL) != t) {
		puts("!!!!RBTree_size error 1");
		r_v = 1;
	}
	///////////////////////////////////////////////////////////////////
	//get tree height
	puts("--------------------------------------------");
	uint32_t b_h = RBTree_height(p_tree, NULL);
	printf("tree height: %u\n", b_h);
	b_h = RBTree_black_height(p_tree, NULL);
	printf("tree black height: %u\n", b_h);

	///////////////////////////////////////////////////////////////////
	//check tree search
	puts("--------------------------------------------");
	for (i = 0; i < N; i++) {
		p_n = RBTree_search(p_tree, NULL, buf[i]);
		if (!p_n || p_n->value != buf[i]) {
			puts("!!!!search error 1");
			r_v = 1;
		}
	}
	p_n = RBTree_search(p_tree, NULL, N);
	if (p_n) {
		puts("!!!!search error 2");
		r_v = 1;
	}
	puts("tree search check complete");

	///////////////////////////////////////////////////////////////////
	//check minimum and maximum
	puts("--------------------------------------------");
	p_n = RBTree_minimum(p_tree, NULL);
	if (!p_n || p_n->value != buf[0]) {
		printf("!!!!minimum error: <%u, %u>\n", p_n ? p_n->value : -1, buf[0]);
		r_v = 1;
		print_buf(buf);
	} else {
		printf("minimum: %u, ", p_n->value);
	}
	p_n = RBTree_maximum(p_tree, NULL);
	if (!p_n || p_n->value != buf[N-1]) {
		puts("!!!!maximum error");
		r_v = 1;
	} else {
		printf("maximum: %u\n", p_n->value);
	}

	///////////////////////////////////////////////////////////////////
	//chect successor and predecessor
	puts("--------------------------------------------");
	for (i = 0; ; ) {
		p_n = RBTree_search(p_tree, NULL, buf[i]);	//buf has been sorted min to max
		while (buf[i] == buf[++i] && i < N);
		if (i == N) break;
		//printf("<%u, %u>\n", buf[i-1], buf[i]);
		PRB_node_t p_s;
		PRB_node_t p_x = RBTree_search(p_tree, NULL, buf[i]);
		//printf("p_n: %u, %u; p_x: %u, %u\n", p_n->value, p_n, p_x->value, p_x);
		if (p_x != (p_s=RBTree_successor(p_tree, p_n)) || p_s->value != buf[i]) {
			printf("~%u, %u~\n", p_n->value, p_s->value);
			puts("!!!!successor error");
			r_v = 1;
		}
		if ((p_n != (p_s=RBTree_predecessor(p_tree, p_x)) || p_s->value != buf[i-1])) {
			printf("~%u, %u~\n", p_s->value, p_x->value);
			puts("!!!!predecessor error!");
			r_v = 1;
		}
	}
	if (NULL != RBTree_successor(p_tree, RBTree_maximum(p_tree, NULL))) {
			puts("!!!!successor error");
			r_v = 1;
	}
	if (NULL != RBTree_predecessor(p_tree, RBTree_minimum(p_tree, NULL))) {
			puts("!!!!predecessor error");
			r_v = 1;
	}
	puts("successor and predecessor complete");

	///////////////////////////////////////////////////////////////////
	//chect delete
	puts("--------------------------------------------");
	//<begin> random delete_p
	uint32_t value;
	while (1) {
#		ifndef _dbg_
		value = rand() % N;
#		else
		value = DELETE_VALUE;
#		endif

		if (p_n = RBTree_search(p_tree, NULL, value)) {
			break;
		}

	}
#	ifdef _dbg_
	printf("delete value: %u\n", value);
#	endif

	RBTree_delete_p(p_tree, p_n);

	RBTree_inorder(p_tree, NULL, buf);
	uint32_t *b1 = buf;
	uint32_t *b2 = buf_q;
	for (i = 0; i < N; i++) {
		if (*b1 == *b2) {
			b1++;
			b2++;
		} else if (*b2 == value) {
			b2++;
		} else {
			break;
		}
	}
	if (i != N || ((p_n = RBTree_search(p_tree, NULL, value)) && p_n->cnt)) {
		puts("!!!!delete p error");
		r_v = 1;
		printf("delete value: %u\n", value);
		puts("buf:");
		//print_buf(buf);
		puts("buf_q:");
		//print_buf(buf_q);
	}
	
	_xx_assert_(RBTree_black_height(p_tree, NULL) || !RBTree_size(p_tree, NULL));
	
	//<end> random delete_p
	//<begin> delete_p all inorder 1
	for (i = 0; i < N; i++) {
		if (p_n = RBTree_search(p_tree, NULL, i)) {
#			ifdef _dbg_
			printf(">>>%u\n", i);
#			endif
			RBTree_delete_p(p_tree, p_n);
			_xx_assert_(RBTree_black_height(p_tree, NULL) || !RBTree_size(p_tree, NULL));
			//printf("i: %u, bh: %u\n", i, RBTree_black_height(p_tree, NULL));
#			ifdef _dbg_
			if (p_n = RBTree_search(p_tree, NULL, i)) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				RBTree_inorder(p_tree, NULL, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (RBTree_size(p_tree, NULL)) {
		puts("!!!!delete_p all error 1");
		r_v = 1;
	}
	//printf(">>>tree size: %u\n", (RBTree_size(p_tree, NULL)));

	//<end> delete_p all inorder 1
	//<begin> delete_p all inorder 2
	for (i = 0; i < N; i++) {
		RBTree_insert(p_tree, buf_tmp[i]);
	}
	RBTree_inorder(p_tree, NULL, buf);
	if (memcmp(buf, buf_q, sizeof(uint32_t)*N)) {
		puts("!!!!insert error");
		r_v = 1;
	}
	for (i = N; i--; ) {
		if (p_n = RBTree_search(p_tree, NULL, i)) {
#			ifdef _dbg_
			printf(">>>%u\n", i);
#			endif
			RBTree_delete_p(p_tree, p_n);
			_xx_assert_(RBTree_black_height(p_tree, NULL) || !RBTree_size(p_tree, NULL));
			//printf("i: %u, bh: %u\n", i, RBTree_black_height(p_tree, NULL));
#			ifdef _dbg_
			if (p_n = RBTree_search(p_tree, NULL, i)) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				RBTree_inorder(p_tree, NULL, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (RBTree_size(p_tree, NULL)) {
		puts("!!!!delete_p all error 2");
		r_v = 1;
	}
	//printf(">>>tree size: %u\n", (RBTree_size(p_tree, NULL)));

	//<end> delete_p all inorder 2
	//<begin> delete_p all randum

	for (i = 0; i < N; i++) {
		RBTree_insert(p_tree, buf_tmp[i]);
	}
	RBTree_inorder(p_tree, NULL, buf);
	if (memcmp(buf, buf_q, sizeof(uint32_t)*N)) {
		puts("!!!!insert error");
		r_v = 1;
	}
	for (i = 0; i < N; i++) {
		buf_ro[i] = i;
	}
	for (i = 0; i < N; i++) {
		uint32_t tmp = _bst_v_;
		swap(buf_ro[i], tmp);
	}
	
	//RBTree_print(p_tree, NULL, N);

	for (i = 0; i < N; i++) {
		if (p_n = RBTree_search(p_tree, NULL, i)) {
			uint32_t tmp = buf_ro[i];
#			ifdef _dbg_
			printf(">>>%u\n", tmp);
#			endif
			RBTree_delete_p(p_tree, p_n);
			_xx_assert_(RBTree_black_height(p_tree, NULL) || !RBTree_size(p_tree, NULL));
			//printf("i: %u, bh: %u\n", i, RBTree_black_height(p_tree, NULL));
#			ifdef _dbg_
			if ((p_n = RBTree_search(p_tree, NULL, tmp)) && p_n->cnt) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				RBTree_inorder(p_tree, NULL, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (RBTree_size(p_tree, NULL)) {
		puts("!!!!delete_p all error 3");
		r_v = 1;
	}

	//<end> delete_p all randun
	puts("delete check complete");

	free(buf_q);
	free(buf);
	free(buf_tmp);
	free(buf_ro);
	return r_v;
}

