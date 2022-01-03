#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

enum { buf_max = 32, };

int main(int argc, char* argv[argc + 1]) {
	int ret = EXIT_FAILURE;

	char buffer[buf_max] = { 0 };

	bool show_lines = false;
	size_t start_index = 1;

	if (argc > 0 && strcmp(argv[1], "-n") == 0) {
		show_lines = true;
		start_index = 2;
	}

	for (size_t i = start_index; i < argc; ++i) {
		FILE* instream = fopen(argv[i], "r");

		if (instream) {
			size_t k = 1;
			bool show_now = true;
			while (fgets(buffer, buf_max, instream)) {
				if (show_lines) {
					if (show_now) {
						fprintf(stdout, "%zu: ", k);
						++k;
					}

					show_now = strchr(buffer, '\n');
				}
				fputs(buffer, stdout);
			}

			fclose(instream);

			ret = EXIT_SUCCESS;
		} else {
			fprintf(stderr, "Could not open %s: ", argv[i]);
			perror(0);
			errno = 0;
		}
	}

	return ret;
}
