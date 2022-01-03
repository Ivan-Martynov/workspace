#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//static_assert('z' - 'a' == 25, "alphabetic characters not contiguous");

unsigned hexatridecimal(char c) {
	if (isdigit(c)){
		return c - '0';
	} else {
		c = toupper(c);
		return isupper(c) ? 10 + abs(c - 'A') : -1;
	}
}

void print_with_format(const char format[static 1], const size_t n) {
	for (int i = 0; i < n; ++i) {
		printf(format, i);
	}
	putchar('\n');
}

void print_decimals() {
	const size_t n = 20;

	puts("Printing in a hexadecimal format:");
	print_with_format("%x ", n);
	print_with_format("%#x ", n);
	print_with_format("%X ", n);
	print_with_format("%#X ", n);
}

void test_hexatridecimal() {
	for (int i = 0; i < 256; ++i) {
		unsigned c = hexatridecimal(i);
		printf("%3d (%c) => %d\n", i, i, c);
	}
}

int main() {
	test_hexatridecimal();
	print_decimals();

	return EXIT_SUCCESS;
}
