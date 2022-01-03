#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void test_time() {
	time_t result = time(NULL);

	if (result != (time_t)(-1)) {
		printf("Seconds: %jd\n", (intmax_t)result);
	}
}

int main() {
	test_time();

	return EXIT_SUCCESS;
}
