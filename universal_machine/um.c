#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "run_um.h"

// opens filestream and begins runnning universal machine
int main(int argc, char *argv[]) {
	if (argc > 2) {
		// display error if too many arguments
		fprintf(stderr, "Usage: %s [filename].um\n", argv[0]);
		exit(1);
	} else if (argc == 2) {
		// open file stream and run machine
		FILE *stream = fopen(argv[1], "r");
		assert(stream);
		run(stream);
		fclose(stream);
	} else {
		// run machine from standard input
		run(stdin);
	}

	return 0;
}
