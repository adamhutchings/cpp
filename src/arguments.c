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
            
            /* Check to make sure the help flag hasn't been seen before */
            if (args->general_flags & HELP_FLAG) {
                if (cp_add_error(args, ERROR_REPEATED_FLAG, arg)) {
                    return;
                }
            }

            args->general_flags |= HELP_FLAG;

        } else {

            /* No other flags exist now. */
            if (cp_add_error(args, ERROR_UNRECOGNIZED_FLAG, arg)) {
                return;
            }

        }

    }

}
