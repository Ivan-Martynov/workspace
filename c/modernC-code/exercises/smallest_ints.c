#include <stdio.h>
#include <limits.h>
#include <stdint.h>

void test_types() {
	int s = -(short)32768;
	unsigned int i = -1U;
	unsigned long int il = -1UL;
	unsigned long long int ill = -1ULL;
	printf("%i\n", s);
	//printf("%hd\n", -(short)32768);
	//printf("%d\n", -(int)2147483647);
	//printf("%d\n", -(int)0x80000000);
	printf("unsigned int -1U => %u\n", i);
	printf("unsigned int -1UL => %lu\n", il);
	printf("unsigned int -1ULL => %llu\n", ill);
}

void test_logical() {
	if (-1 < 0) {
		printf("-1 < 0 is true, shouldn't be problematic\n");
	} else {
		printf("-1 < 0 should have been fine, but something is wrong\n");
	}

	if (-1L < 0) {
		printf("-1L < 0 is true, shouldn't be problematic\n");
	} else {
		printf("-1L < 0 should have been fine, but something is wrong\n");
	}

	if (!(-1U < 0U)) {
		printf("-1U < 0U is false, shouldn't be problematic, because the same signedness is in place\n");
	} else {
		printf("-1U < 0U should have been fine, but something is wrong\n");
	}

	if (!(-1 < 0U)) {
		printf("-1 < 0U is false, dangerous, mixed signedness!\n");
	} else {
		printf("-1 < 0U should have been wrong, but something went south\n");
	}

	if (!(-1U < 0)) {
		printf("-1U < 0 is false, dangerous, mixed signedness!\n");
	} else {
		printf("-1U < 0 should have been wrong, but something went south\n");
	}

	if (-1L < 0U) {
		printf("-1L < 0U: depends (now it true), dangerous, same or mixed signedness!\n");
	} else {
		printf("-1L < 0U: depends (now it false), dangerous, same or mixed signedness!\n");
	}

	if (-1LL < 0UL) {
		printf("-1LL < 0UL: depends (now it true), dangerous, same or mixed signedness!\n");
	} else {
		printf("-1LL < 0UL: depends (now it false), dangerous, same or mixed signedness!\n");
	}
}

int main() {
	test_types();
	test_logical();

	return 0;
}