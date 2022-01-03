#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <tgmath.h>
#include <locale.h>

void display_format_details(void) {
	const struct lconv * const lc = localeconv();
	printf("decimal point: %s\n", lc->decimal_point);
}

double my_strtod(const char s[static 1]) {
	const struct lconv * const lc = localeconv();
	const char decimal_point = lc->decimal_point[0];

	double result = 0;
	size_t base = 10;
	size_t i = 0;

	while (isspace(s[i])) {
		++i;
	}

	bool minus = false;
	switch (s[i]) {
		case '-': minus = true; // fall through intended
		case '+': ++i;
	}

	bool after_decimal = false;
	double k = 1.0 / base;
	size_t dec_point_count = 0;
	for (; i < strlen(s); ++i) {
		if (isdigit(s[i])) {
			if (after_decimal) {
				result = result + (s[i] - '0') * k;
				k *= 1.0 / base;
			} else {
				result = fma(result, base, s[i] - '0');
			}
		} else if (s[i] == decimal_point) {
			if (++dec_point_count > 1) {
				return 0;
			}
			after_decimal = true;
		}
	}

	return minus ? - result : result;
}

void test_double() {
	const char s1[] = "24832";
	const double d1 = my_strtod(s1);
	printf("%s => %f\n", s1, d1);
}

int main(int argc, char* argv[argc + 1]) {
	display_format_details();

	if (argc > 1) {
		for (size_t i = 1; i < argc; ++i) {
			const double d = my_strtod(argv[i]);
			const size_t n = strlen(argv[i]);
			printf("%s => %.12f\n", argv[i], d);
			printf("%s => %.12f\n", argv[i], strtod(argv[i], 0));
		}
	} else {
		test_double();
	}

	return EXIT_SUCCESS;
}
