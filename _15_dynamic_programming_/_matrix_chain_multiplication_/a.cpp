#include <stdio.h>
#include <stdlib.h>
#include <stdInt.h>
#include <time.h>
#include <new>
#include <string.h>

template <class T>
void alloc_buf(T*&p, uint32_t n, const char* str) {
	try {
		p = new T[n];
	} catch(std::bad_alloc &bd) {
		printf("bad_alloc, <%s>\n", str);
		exit(1);
	}
}

uint32_t sub_run(uint32_t l, uint32_t r, const uint32_t* p, uint32_t* p_m, const uint32_t nr) {
	//printf("<%u, %u>\n", l, r);
	if (l == r) {
        printf("<%u, %u>\n", l, r);
        printf("\t>1\n");
        return 1;
    }
	if (p_m[l * nr + r]) {
        uint32_t x = p_m[l * nr + r];
        printf("<%u, %u>\n", l, r);
        printf("\t>%u\n", x);
        return x;
    }
	if (l + 1 == r) {
		uint32_t x = p_m[l * nr + r] = p[l] * p[l + 1] * p[l + 2];
        printf("<%u, %u>\n", l, r);
        printf("\t>%u\n", x);
        return x;
	}

	uint32_t k;
	uint32_t min = (uint32_t)(-1);
	for (uint32_t i = l; i < r; i++) {
        uint32_t xl = sub_run(l, i, p, p_m, nr);
        uint32_t xr = sub_run(i + 1, r, p, p_m, nr);
		uint32_t tmp = p[l] * xl * p[i + 1] * xr * p[r + 1];
        if (tmp == 0) {
            printf("<l/i/r> [%u/%u/%u]\n", l, i, r);
            printf("<xl/xr> [%u/%u]\n", xl, xr);
            uint64_t x64 = 1;
            x64 = x64*p[l]*xl*p[i+1]*xr*p[r+1];
            uint32_t x32l = (uint32_t)x64;
            uint32_t x32u = x64>>32;
            printf("<x32l/x32u> [%u/%u]\n", x32l, x32u);
        }
		if (tmp < min) {
			min = tmp;
			k = i;
		}
	}
	uint32_t x = p_m[l * nr + r] = min;
    printf("<%u, %u>\n", l, r);
    printf("\t>%u\n", x);
    return x;
}

void create_a(const uint32_t* p, const uint32_t ns) {
	uint32_t* p_m = NULL;
	uint32_t nr = ns - 1;
	alloc_buf(p_m, nr * nr, "p_m");
	memset(p_m, 0, nr * nr * sizeof(uint32_t));
	//uint32_t min = sub_run(0, nr - 1, p, p_m, nr);
	uint32_t min = sub_run(2, 8, p, p_m, nr);
	printf("min: %u\n", min);
}

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
		alloc_buf(p, ns, "p");
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
		alloc_buf(p, ns, "p");
		for (uint32_t i = 0; fgets(buf, 10, p_f); i++) {
			p[i] = atoi(buf);
		}
	}
	if(p_f) {
		fclose(p_f);
		p_f = NULL;
	}
	
    printf("ns: %u\n", ns);
	create_a(p, ns);


	return 0;
}
