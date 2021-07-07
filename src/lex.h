/**
 * Provide tokens of input from a file.
 */

#pragma once

#include <stdio.h>

#define TOKEN_MAX_LENGTH 256

struct cp_lexer {

    FILE* file; /* the file this is reading from */
    int current_lineno;

};

enum token_type {

    T_IDENTIFIER,
    T_OPAREN,
    T_CPAREN,
    T_SHARP, /* '#' */
    T_ELSE,  /* every other token type because nothing else matters to us */
    T_EOF,   /* the end of the token stream */

};

static char* token_type_names[6] = {"identifier", "oparen", "cparen", "sharp", "other", "end"};

char* cp_get_token_name(int type);

struct cp_token {

    enum token_type type;
    char content[TOKEN_MAX_LENGTH];
    unsigned lineno;

};

/**
 * Initialize the lexer with the given filename.
 * Return non-zero if the file could not be found.
 */
int cp_lexer_init(struct cp_lexer *, const char * filename);

/**
 * Fill the token instance with the lexed data. Return non-zero if end of file
 * is reached and the token is meaningless.
 */
int cp_lexer_read(struct cp_lexer *, struct cp_token *);
