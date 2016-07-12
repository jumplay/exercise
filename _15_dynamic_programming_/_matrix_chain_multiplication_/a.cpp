#include <stdio.h>
#include <stdlib.h>
#include <stdInt.h>
#include <time.h>
#include <new>
#include <string.h>
#include "matrix_chain.h"


int main() {

	uint32_t ns = 0;
	uint32_t *p = NULL;
	
	char *ss = NULL;
	
	char buf[1024];
	FILE *p_f = fopen("1.txt", "r");
	if (p_f == NULL) {
		FILE *p_f = fopen("1.txt", "w");
		printf("Inputs ns:\n");
		fgets(buf, 10, stdin);
		ns = atoi(buf);
		fprintf(p_f, "%u\n", ns);
		srand((uint32_t)time(NULL));
		m_alloc(p, ns, "p");
		for (uint32_t i = 0; i < ns; i++) {
			uint32_t x = rand() % 100;
			while (x < 10) {
				x = rand() % 100;
			}
			printf("x: %u\n", x);
			p[i] = x;
			fprintf(p_f, "%u\n", p[i]);
		}
	} else {
		fgets(buf, 10, p_f);
		ns = atoi(buf);
		m_alloc(p, ns, "p");
		uint32_t i = 0;
		for (; fgets(buf, 10, p_f) && i < ns; i++) {
			p[i] = atoi(buf);
		}
		if (i != ns) {
			printf("error: i != ns\n");
			return 0;
		}
	}
	if(p_f) {
		fclose(p_f);
		p_f = NULL;
	}
	
    printf("ns: %u\n", ns);
	uint32_t* p1 = create_top_down(p, ns);
	uint32_t* p2 = create_bottom_up(p, ns);

	for (uint32_t i = 0; i < (ns - 1) * (ns - 1); i ++) {
		if (p1[i] != p2[i]) {
			printf("<%u, %u>\n", i / (ns - 1), i % (ns - 1));
			printf("\t[%u, %u]\n", p1[i], p2[i]);
		}
	}

	print_chain(p1, ns - 1);

	return 0;
}
