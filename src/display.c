const char* usage_msg = "Usage: cproc [FILE]... [OPTION]...";
const char* version_msg = "Version: 0.0.1";

// To be called when -h or --help is invoked
void usage() {
	printf(usage_msg);
}

// To be called when -v or --version is invoked
void version() {
	printf(version_msg);
}
