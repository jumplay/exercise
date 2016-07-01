#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "binary_search_tree.h"

#define N (1000000)
#define swap(a,b) do{ t=(a); (a)=(b); (b)=t; }while(0)

//////////////////////////////////////////////////////////
//#define PRINT_VALUE
//#define PRINT_TREE
//#define _dbg_
//////////////////////////////////////////////////////////

#ifndef _dbg_
#	define _bst_v_	(rand()%N)
#else
	const uint32_t dbg_a[N] = { 3, 1, 2, 4, 5 };
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
	PBSTree_t p_t;

	uint32_t r_v = 0;

	BSTree_t btree;
	PBSTree_t p_tree = &btree;
	BSTree_ini(p_tree);
	
	///////////////////////////////////////////////////////////////////
	//chect insert and inorder
	puts("--------------------------------------------");
	for (i = 0; i < N; i++) {
		uint32_t tmp = _bst_v_;
		buf_q[i] = tmp;
#		ifdef PRINT_VALUE
		printf("<%u>\n", tmp);
#		endif
		p_t = BSTree_insert(p_tree, tmp);
		if (p_t->value != tmp) {
			puts("!!!insert error");
			r_v = 1;
		}
	}
	
	memcpy(buf_tmp, buf_q, sizeof(uint32_t)*N);
	BSTree_inorder(p_tree, buf);
	qsort(buf_q, N, sizeof(uint32_t), cmp_uint32);

	if (!memcmp(buf_q, buf, sizeof(uint32_t)*N)) {
		puts("insert and inorder success!");
	}

	t = 1;
	for (i = 1; i < N; i++) {
		if (buf[i-1] != buf[i]) {
			t++;
		}
	}
	if (BSTree_size(p_tree) != t) {
		puts("!!!!BSTree_size error 1");
	}
#	ifdef PRINT_TREE
	BSTree_print(p_tree, N);
#	endif
	///////////////////////////////////////////////////////////////////
	//get tree height
	puts("--------------------------------------------");
	uint32_t b_h = BSTree_height(p_tree);
	printf("tree height: %u\n", b_h);

	///////////////////////////////////////////////////////////////////
	//check tree search
	puts("--------------------------------------------");
	for (i = 0; i < N; i++) {
		p_t = BSTree_search(p_tree, buf[i]);
		if (!p_t || p_t->value != buf[i]) {
			puts("!!!!search error 1");
			r_v = 1;
		}
	}
	p_t = BSTree_search(p_tree, N);
	if (p_t) {
		puts("!!!!search error 2");
		r_v = 1;
	}
	puts("tree search check complete");

	///////////////////////////////////////////////////////////////////
	//check minimum and maximum
	puts("--------------------------------------------");
	p_t = BSTree_minimum(p_tree);
	if (!p_t || p_t->value != buf[0]) {
		printf("!!!!minimum error: <%u, %u>\n", p_t ? p_t->value : -1, buf[0]);
		r_v = 1;
		print_buf(buf);
	} else {
		printf("minimum: %u, ", p_t->value);
	}
	p_t = BSTree_maximum(p_tree);
	if (!p_t || p_t->value != buf[N-1]) {
		puts("!!!!maximum error");
		r_v = 1;
	} else {
		printf("maximum: %u\n", p_t->value);
	}

	///////////////////////////////////////////////////////////////////
	//chect successor and predecessor
	puts("--------------------------------------------");
	for (i = 0; ; ) {
		p_t = BSTree_search(p_tree, buf[i]);	//buf has been sorted min to max
		while (buf[i] == buf[++i] && i < N);
		if (i == N) break;
		//printf("<%u, %u>\n", buf[i-1], buf[i]);
		PBSTree_t p_s;
		PBSTree_t p_x = BSTree_search(p_tree, buf[i]);
		if (p_x != (p_s=BSTree_successor(p_t)) || p_s->value != buf[i]) {
			printf("~%u, %u~\n", p_t->value, p_s->value);
			puts("!!!!successor error");
			r_v = 1;
		}
		if ((p_t != (p_s=BSTree_predecessor(p_x)) || p_s->value != buf[i-1])) {
			printf("~%u, %u~\n", p_s->value, p_x->value);
			puts("!!!!predecessor error!");
			r_v = 1;
		}
	}
	if (NULL != BSTree_successor(BSTree_maximum(p_tree))) {
			puts("!!!!successor error");
			r_v = 1;
	}
	if (NULL != BSTree_predecessor(BSTree_minimum(p_tree))) {
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

		if (p_t = BSTree_search(p_tree, value)) {
			break;
		}
	}
#	ifdef _dbg_
	printf("delete value: %u\n", value);
#	endif
	BSTree_delete_p(p_t);
	BSTree_inorder(p_tree, buf);
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
	if (i != N || ((p_t = BSTree_search(p_tree, value)) && p_t->cnt)) {
		puts("!!!!delete p error");
		r_v = 1;
		printf("delete value: %u\n", value);
		puts("buf:");
		print_buf(buf);
		puts("buf_q:");
		print_buf(buf_q);
	}
	//<end> random delete_p
	//<begin> delete_p all inorder 1
	for (i = 0; i < N; i++) {
		if (p_t = BSTree_search(p_tree, i)) {
#			ifdef _dbg_
			printf(">>>%u\n", i);
#			endif
			BSTree_delete_p(p_t);
#			ifdef _dbg_
			if ((p_t = BSTree_search(p_tree, i)) && p_t->cnt) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				BSTree_inorder(p_tree, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (p_tree->cnt || p_tree->p_left || p_tree->p_right) {
		puts("!!!!delete_p all error 1");
		r_v = 1;
	}
	//<end> delete_p all inorder 1
	//<begin> delete_p all inorder 2
	for (i = 0; i < N; i++) {
		BSTree_insert(p_tree, buf_tmp[i]);
	}
	BSTree_inorder(p_tree, buf);
	if (memcmp(buf, buf_q, sizeof(uint32_t)*N)) {
		puts("!!!!insert error");
		r_v = 1;
	}
	for (i = N; i--; ) {
		if (p_t = BSTree_search(p_tree, i)) {
#			ifdef _dbg_
			printf(">>>%u\n", i);
#			endif
			BSTree_delete_p(p_t);
#			ifdef _dbg_
			if ((p_t = BSTree_search(p_tree, i)) && p_t->cnt) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				BSTree_inorder(p_tree, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (p_tree->cnt || p_tree->p_left || p_tree->p_right) {
		puts("!!!!delete_p all error 2");
		r_v = 1;
	}
	//<end> delete_p all inorder 2
	//<begin> delete_p all randum
	for (i = 0; i < N; i++) {
		BSTree_insert(p_tree, buf_tmp[i]);
	}
	BSTree_inorder(p_tree, buf);
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
	for (i = 0; i < N; i++) {
		if (p_t = BSTree_search(p_tree, i)) {
			uint32_t tmp = buf_ro[i];
#			ifdef _dbg_
			printf(">>>%u\n", tmp);
#			endif
			BSTree_delete_p(p_t);
#			ifdef _dbg_
			if ((p_t = BSTree_search(p_tree, tmp)) && p_t->cnt) {
				puts("-----------");
				printf(">>>>%u, %u\n", p_tree->p_left, p_tree->p_right);
				uint32_t *bufxxx = (uint32_t*)malloc(sizeof(uint32_t)*N);
				memset(bufxxx, -1, sizeof(uint32_t)*N);
				BSTree_inorder(p_tree, bufxxx);
				print_buf(bufxxx);
				free(bufxxx);
			}
#			endif
		}
	}
	if (p_tree->cnt || p_tree->p_left || p_tree->p_right) {
		puts("!!!!delete_p all error 3");
		r_v = 1;
	}

	if (BSTree_size(p_tree) != 1) {
		puts("!!!!BSTree_size error 2");
	}
	//<end> delete_p all randun
	puts("delete check complete");

	free(buf_q);
	free(buf);
	return r_v;
}

