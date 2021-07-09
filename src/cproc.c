#include "cproc.h"

#include <stdio.h>  /* printf */
#include <string.h> /* memset */

void cproc_init   (struct cproc_processor * processor, struct cp_cmd_args * args) {
    memset(processor, 0, sizeof *processor);
    /* TODO - error reporting */
    cp_lexer_init(&processor->lexer, args->filename);
}

void cproc_compile(struct cproc_processor * processor) {

    struct cp_token           token;

    while ( cp_lexer_read(&processor->lexer, &token) == 0 ) {
        printf("%d:%-16s %s\n", token.type, cp_get_token_name(token.type), token.content);
    }

}
