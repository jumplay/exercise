#include <stdio.h>
#include "hrd.h"

int main() {
#	if 0
	uint32_t start[20] = {
		2, 4, 4, 2,
		2, 4, 4, 2,
		2, 3, 3, 2,
		2, 1, 1, 2,
		1, 0, 0, 1,
	};
#	endif

#	if 0
	uint32_t start[20] = {
		1, 4, 4, 1,
		2, 4, 4, 2,
		2, 3, 3, 2,
		1, 3, 3, 1,
		0, 3, 3, 0,
	};
#	endif

#	if 1
	uint32_t start[20] = {
		1, 1, 1, 2,
		4, 4, 2, 2,
		4, 4, 2, 2,
		0, 3, 3, 2,
		0, 1, 3, 3,
	};
#	endif

	get_asr(start);

	return 0;
}
