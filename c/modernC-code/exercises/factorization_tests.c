#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

size_t show_factor(const size_t k, const size_t i) {
	if (k > 0) {
		if (k > 1) {
			printf(" * %zu^%zu", i, k);
		} else {
			printf(" * %zu", i);
		}
	}

	return 0;
}

bool isPrime(const size_t n) {
	if (n < 0) {
		return isPrime(-n);
	}

	if (n == 2 || n == 3) {
		return true;
	}

	if (n < 2 || n % 2 == 0 || n % 3 == 0) {
		return false;
	}

	for (long int i = 5; i * i <= n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}

	return true;
}

void prime_factorization(size_t n) {
	printf("%zu = 1", n);

	size_t k = 0;

	while (n % 2 == 0) {
		n >>= 1;
		++k;
	}

	k = show_factor(k, 2);

	while (n % 3 == 0) {
		n /= 3;
		++k;
	}

	k = show_factor(k, 3);

	for (size_t i = 5; i <= n; i += 6) {
		while (n % i == 0) {
			n /= i;
			++k;
		}

		k = show_factor(k, i);

		const size_t j = i + 2;

		while (n % j == 0) {
			n /= j;
			++k;
		}

		k = show_factor(k, j);
	}

	printf("\n");
}

void test_factorization() {
	const size_t n = 13;
	prime_factorization(n);
}

int main(int argc, char* argv[argc + 1]) {

	if (argc > 1) {
		for (size_t i = 1; i < argc; ++i) {
			prime_factorization(atoi(argv[i]));
		}
	} else {
		test_factorization();
	}

	return EXIT_SUCCESS;
}
