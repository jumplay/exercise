#pragma onc

template <class T>
void m_alloc(T*&p, uint32_t n, const char* str) {
	try {
		p = new T[n];
	} catch(std::bad_alloc &bd) {
		printf("bad_alloc[%s], <%s>\n", bd.what(), str);
		exit(1);
	}
}

uint32_t top_down_run(uint32_t l, uint32_t r, const uint32_t* p, uint32_t* p_m, uint32_t* p_k, const uint32_t nr) {
	if (l == r) {
		return 0;
    } else if (p_m[l * nr + r]) {
		return p_m[l * nr + r];
    } else if (l + 1 == r) {
		p_k[l * nr + r] = l;
		return p_m[l * nr + r] = p[l] * p[l + 1] * p[l + 2];
	}

	uint32_t x;
	uint32_t min = (uint32_t)(-1);
	for (uint32_t i = l; i < r; i++) {
        uint32_t xl = top_down_run(l, i, p, p_m, p_k, nr);
        uint32_t xr = top_down_run(i + 1, r, p, p_m, p_k, nr);
		uint32_t tmp = xl + xr + p[l] * p[i + 1] * p[r + 1];
		if (tmp < min) {
			min = tmp;
			x = i;
		}
	}
	p_k[l * nr + r] = x;
	return p_m[l * nr + r] = min;
}

uint32_t* create_top_down(const uint32_t* p, const uint32_t ns) {
	uint32_t nr = ns - 1;

	uint32_t* p_m = NULL;
	m_alloc(p_m, nr * nr, "p_m");
	memset(p_m, 0, nr * nr * sizeof(uint32_t));

	uint32_t* p_k = NULL;
	m_alloc(p_k, nr * nr, "p_k");
	memset(p_k, 0, nr * nr * sizeof(uint32_t));

	uint32_t min = top_down_run(0, nr - 1, p, p_m, p_k, nr);
	printf("min: %u\n", min);

	return p_k;

}

uint32_t* create_bottom_up(const uint32_t* p, const uint32_t ns) {
	uint32_t nr = ns - 1;

	uint32_t* p_m = NULL;
	m_alloc(p_m, nr * nr, "p_m");
	memset(p_m, 0, nr * nr * sizeof(uint32_t));
	
	uint32_t* p_k = NULL;
	m_alloc(p_k, nr * nr, "p_k");
	memset(p_k, 0, nr * nr * sizeof(uint32_t));

	for (uint32_t l = 2; l <= nr; l++) {
		for (uint32_t i = 0; i < nr - l + 1; i++) {
			uint32_t j = i + l - 1;
			uint32_t min = (uint32_t)-1;
			uint32_t x;
			for (uint32_t k = i; k < j; k++) {
				uint32_t tmp = p[i] * p[k + 1] * p[j + 1] + p_m[i * nr + k] + p_m[(k + 1) * nr + j];
				if (min > tmp) {
					min = tmp;
					x = k;
				}
			}
			p_m[i * nr + j] = min;
			p_k[i * nr + j] = x;
		}
	}
	printf("min: %u\n", p_m[nr - 1]);

	return p_k;
}

void print_chain_run(uint32_t* p_k, uint32_t l, uint32_t r, uint32_t nr) {
	if (l == r) {
		printf("A%u", l);
	} else {
		uint32_t k = p_k[l * nr + r];
		printf("(");
		print_chain_run(p_k, l, k, nr);
		print_chain_run(p_k, k + 1, r, nr);
		printf(")");
	}
}

void print_chain_run2(uint32_t* p_k, uint32_t l, uint32_t r, uint32_t nr) {
	uint32_t k = p_k[l * nr + r];
	if (l == k) {
		printf("A%u", l);
	} else {
		printf("(");
		print_chain_run2(p_k, l, k, nr);
		printf(")");
	}
	if (k + 1 == r) {
		printf("A%u", r);
	} else {
		printf("(");
		print_chain_run2(p_k, k + 1, r, nr);
		printf(")");
	}
}

void print_chain(uint32_t* p_k, uint32_t nr) {
	print_chain_run(p_k, 0, nr-1, nr);
	printf("\n");
	print_chain_run2(p_k, 0, nr-1, nr);
}
