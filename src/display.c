const char* usage_msg = "Usage: cproc [FILE]... [OPTION]...\n\n\
\t--help        Display this menu\n\
\t--version     Show the version\n\
\nUpstream source: <https://github.com/adamhutchings/cproc>";

const char* version_msg = "Version: 0.0.1";

// To be called when -h or --help is invoked
void cp_usage() {
    printf("%s\n", usage_msg);
}

// To be called when -v or --version is invoked
void cp_version() {
    printf("%s\n", version_msg);
}
