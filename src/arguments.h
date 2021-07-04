/**
 * This provides a utility to parse command-line arguments into one structure.
 */

#pragma once

#include <stdbool.h> /* bool */

/**
 * the number of errors accepted at max befeore argparsing stops and reports all
 * errors so far
 */
#define MAX_ERRORS 8

enum general_flags {

    HELP_FLAG = 0x01,

};

enum error_kind {

    ERROR_REPEATED_FLAG,
    ERROR_UNRECOGNIZED_FLAG,

};

struct cp_cmd_args {

    /* Flags (whether to help or not, etc) */
    unsigned general_flags;

    /**
     * All of the errors reported.
     */
    struct reported_error {
        int argument; /* which error the argument was reported at */
        enum error_kind error; /* what error it was */
    } errors [MAX_ERRORS];

    /* How many errors */
    unsigned error_count;

    /* More to come later */

};

/**
 * Parse command args
 * @param args the structure to fill
 * @param argc the argument count
 * @param argv the arguments
 */
void cp_parse_cmd_args(struct cp_cmd_args * args, int argc, char** argv);
