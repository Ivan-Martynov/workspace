#include <stdio.h>
#include <stdlib.h>

double my_strtod(const char s[static 1]) {
}

void test_double() {
	const char s1[] = "2";
	const double d1 = my_strtod(s1);
}

int main() {
	return EXIT_SUCCESS;
}
