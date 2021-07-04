/**
 * Main file for cproc - parses command-line arguments and executes the main
 * preprocessor code.
 */
#include <stdio.h>
#include <display.h>

// To be called when -h or --help is invoked
void usage() {
	printf(usage_msg);
}

// To be called when -v or --version is invoked
void version() {
	printf(version_msg);
}

int main(int argc, char** argv) {
	version();
    return 0;
}
