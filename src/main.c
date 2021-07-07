/**
 * Main file for cproc - parses command-line arguments and executes the main
 * preprocessor code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arguments.h>
#include <display.h>
#include <lex.h>

int main(int argc, char** argv) {
    struct cp_cmd_args args;
    struct cp_lexer lexer;
    struct cp_token token;
    int error_iterator;

    cp_parse_cmd_args(&args, argc, argv);

    for (error_iterator = 0; error_iterator < args.error_count; ++error_iterator) {

        printf(
            "%s: error with argument %s: %s\n",
            argv[0], /* the name of the program */
            argv[args.errors[error_iterator].argument], /* the argument causing the error */

            /* TODO - improve later */
            args.errors[error_iterator].error == ERROR_REPEATED_FLAG
            ? "repeated flag" : "unrecognized flag"

        );

    }

    if (args.general_flags & HELP_FLAG) {
        cp_usage();
        exit(0);
    }

    if (args.general_flags & VERSION_FLAG) {
        cp_version();
        exit(0);
    }

    cp_lexer_init(&lexer, argv[1]);
    while ( cp_lexer_read(&lexer, &token) == 0 ) {
        printf("%d:%s\t\t %s\n", token.type, cp_get_token_name(token.type), token.content);
    }

    return 0;

}
