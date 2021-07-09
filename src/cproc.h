/**
 * File that calls most of the code.
 */

#pragma once

#include <arguments.h>
#include <lex.h>

#define MAX_VAR_DEFINES 65536
#define MAX_FUN_DEFINES 65536
#define MAX_NESTED_IFS  65536

struct cproc_processor {

    struct cp_lexer             lexer;

/*  struct cp_var_define        var_defines [ MAX_VAR_DEFINES ];
    int                         num_var_defines;
    struct cp_fun_define        fun_defines [ MAX_FUN_DEFINES ];
    int                         num_fun_defines;
    struct active_conditionals  conditionals [ MAX_NESTED_IFS ];
    int                         num_conditionals;
 */

    /* TODO other data */

};

void cproc_init   (struct cproc_processor *, struct cp_cmd_args *);
void cproc_compile(struct cproc_processor *);
