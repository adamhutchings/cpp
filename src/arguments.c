#include "arguments.h"

#include <stdlib.h>
#include <string.h>

/**
 * Internal - add an error to the list. Return 1 if max has been reached.
 */
int cp_add_error(struct cp_cmd_args * args, enum error_kind error, int argument) {
    
    if (args->error_count >= MAX_ERRORS) return 1;

    args->errors[args->error_count++] = (struct reported_error) {
        argument, error
    };

    return 0;

}

int cp_check_extra_args(struct cp_cmd_args * args, int *arg, int flag) {
    /* Check to make sure a flag hasn't been seen before */
    if (args->general_flags & flag) {
        return cp_add_error(args, ERROR_REPEATED_FLAG, *arg);
    }
    return 0;
}

void cp_parse_cmd_args(struct cp_cmd_args * args, int argc, char** argv) {

    int arg;

    /* Make sure the struct doesn't contain garbage */
    memset(args, 0, sizeof *args);

    /* If no arguments, print help message anyway. */
    if (argc == 1) {
        args->general_flags |= HELP_FLAG;
        return;
    }

    /* Start at 1 so we skip over the name of the program. */
    for (arg = 1; arg < argc; ++arg) {
    
        if (!strcmp(argv[arg], "--help")) {
            if (cp_check_extra_args(&args, &arg, HELP_FLAG)) { return; }

            args->general_flags |= HELP_FLAG;

        } else if (!strcmp(argv[arg], "--version")) {
            if (cp_check_extra_args(&args, &arg, VERSION_FLAG)) { return; }

            args->general_flags |= VERSION_FLAG;

        } else { args->filename = argv[arg]; }

    }

}
