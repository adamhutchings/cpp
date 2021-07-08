#include "lex.h"

#include <ctype.h>
#include <string.h>

int cp_lexer_init(struct cp_lexer * lexer, const char * filename) {

    memset(lexer, 0, sizeof *lexer);

    lexer->file = fopen(filename, "r");
    if (lexer->file == NULL) {
        return 1;
    }

    /* Remember that line numbers start at 1, not zero. */
    lexer->current_lineno = 1;

    return 0;

}

/* INTERNAL USE FUNCTIONS */

/* SKIP IRRELEVANT THINGS */
static int                  cp_lexer_skip_whitespace(struct cp_lexer *);
static int                  cp_lexer_skip_comment   (struct cp_lexer *);
static int                  cp_lexer_skip_all       (struct cp_lexer *);
static enum token_type      cp_lexer_get_ttype      (int);
static int                  cp_lexer_getc           (struct cp_lexer *);

/* End function decls */

char* cp_get_token_name(int type) {
	return token_type_names[type];
}

/**
 * This is necessary as an internal wrapper because we have to account for
 * line splices (lines being continued with backslashes), trigraphs, etc.
 * This function only performs line splices on input.
 */
static int cp_lexer_getc(struct cp_lexer * lexer) {
    int c = getc(lexer->file);
    if (c == '\\') {
        int n = getc(lexer->file);
        if (n == '\n') {
            return getc(lexer->file);
        } else {
            ungetc(n, lexer->file);
        }
    }
    if (c == '\n') {
        ++lexer->current_lineno;
    }
    return c;
}

int cp_lexer_read(struct cp_lexer * lexer, struct cp_token * token) {

    int firstchar;
    int buf_index = 0; /* where in the character buffer we put tokens */

    if (feof(lexer->file)) return 1;

    memset(token, 0, sizeof *token);

    cp_lexer_skip_all(lexer); /* All whitespace is now skipped. */

    firstchar = cp_lexer_getc(lexer);

    token->type = cp_lexer_get_ttype(firstchar);

    do {
        token->content[buf_index++] = firstchar;
        firstchar = cp_lexer_getc(lexer);
    } while (isdigit(firstchar) || cp_lexer_get_ttype(firstchar) == T_IDENTIFIER);

    /* We've read one character too far. */
    ungetc(firstchar, lexer->file);

    return 0;

}

static enum token_type cp_lexer_get_ttype(int begin) {

    /* Single character matches */
    if (begin == '(') {
        return T_OPAREN;
    }
    if (begin == ')') {
        return T_CPAREN;
    }
    if (begin == '#') {
        return T_SHARP;
    }
    if (begin == -1) {
        return T_EOF;
    }
    if (isalpha(begin)) {
        return T_IDENTIFIER;
    }

    return T_ELSE;

}

/**
 * Returns the number of characters skipped.
 */
static int cp_lexer_skip_whitespace(struct cp_lexer * lexer) {

    char c;
    int skipped = 0;

    for ( ; ; ++skipped) {

        c = cp_lexer_getc(lexer);
        if (!isspace(c)) {
            /* We've reached the end - put back and return. */
            ungetc(c, lexer->file);
            return skipped;
        }

    }

}

#define CP_LINE_COMMENT 1 /* // a comment like this */

/* A comment
like this */
#define CP_BLOCK_COMMENT 2

/**
 * Returns the number of characters skipped, or a negative number in case of
 * open comment.
 */
static int cp_lexer_skip_comment(struct cp_lexer * lexer) {

    char initial, second;
    int mode; /* what kind of comment it is */
    int counted = 0; /* how many characters. */

    /* Initial must be a forward slash in all cases. */
    initial = cp_lexer_getc(lexer);
    if (initial != '/') {
        ungetc(initial, lexer->file);
        return 0;
    }

    second = cp_lexer_getc(lexer);
    switch (second) {
        case '/':
            mode = CP_LINE_COMMENT;
            break;
        case '*':
            mode = CP_BLOCK_COMMENT;
            break;
        default:
            /* Not a comment. Unlex all characters. */
            ungetc(second,  lexer->file);
            ungetc(initial, lexer->file);
            return 0;
    }

    /* Two characters so far. */
    counted = 2;

    if (mode == CP_LINE_COMMENT) {

        /* Go forward until we reach a newline. */
        while (cp_lexer_getc(lexer) != '\n')
            if (feof(lexer->file)) return -1;

    } else {

        /* I learned this trick by perusing GCC source code - because comments
         * tend to have '*'s sprinkled throughout, it's better to check for end
         * of comment whenever you encounter a '/' instead.
         */

        /* Initialize 'cur' to 0 so that when last gets assigned to it we're OK. */
        char last, cur = '\0';

        for (;; ++counted) {

            last = cur;
            cur = cp_lexer_getc(lexer);

            if (cur == '/' && last == '*') break;

            if (feof(lexer->file))
                return -1;

        }

    }

    return counted;

}

/**
 * Return the number of characters skipped in total, or negative if error.
 */
int cp_lexer_skip_all(struct cp_lexer * lexer) {

    int sum_total = 0;

    for (;;) {

        int whitespace_skipped, comment_skipped, total_skipped;

        whitespace_skipped = cp_lexer_skip_whitespace(lexer);
        comment_skipped    = cp_lexer_skip_comment   (lexer);
        total_skipped      = whitespace_skipped + comment_skipped;

        if (whitespace_skipped < 0 || comment_skipped < 0) return -1; /* error */

        sum_total += total_skipped;

        if (total_skipped == 0) return sum_total;

    }

}
