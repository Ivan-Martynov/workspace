#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

size_t fib_v1(const size_t n) {
	if (n < 3) {
		return 1;
	}

	return fib_v1(n - 2) + fib_v1(n - 1);
}

size_t fib_v2_rec(const size_t n, size_t cache[n]) {
	const size_t m = n - 1;

	if (cache[m] == 0) {
		cache[m] = fib_v2_rec(m, cache) + fib_v2_rec(m - 1, cache);
	}

	return cache[m];
}

size_t fib_v2(const size_t n) {
	if (n < 3) {
		return 1;
	}

	size_t cache[n];
	cache[0] = 1;
	cache[1] = 1;
	for (size_t i = 2; i < n; ++i) {
		cache[i] = 0;
	}

	return fib_v2_rec(n, cache);
}

void fib_v3_rec(const size_t n, size_t buffer[2]) {
	if (n > 2) {
		const size_t next = buffer[0] + buffer[1];
		buffer[1] = buffer[0];
		buffer[0] = next;

		fib_v3_rec(n - 1, buffer);
	}
}

size_t fib_v3(const size_t n) {
	size_t buffer[] = {1, 1, };

	fib_v3_rec(n, buffer);

	return buffer[0];
}

void test_fibonacci() {
	const size_t arr[] = {1, 2, 4, 8, 12, 35};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
		const size_t result = fib_v3(arr[i]);
		printf("Fibonacci for %zu is %zu, distance from ULONG_MAX = %zu\n", arr[i], result, (ULONG_MAX - result));
	}
}

int main() {
	printf("max = %zu\n", ULONG_MAX);
	test_fibonacci();

	return EXIT_SUCCESS;
}
