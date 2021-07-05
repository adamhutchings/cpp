#include "lex.h"

#include <ctype.h>

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
int cp_lexer_skip_whitespace(struct cp_lexer *);
int cp_lexer_skip_comment   (struct cp_lexer *);
int cp_lexer_skip_all       (struct cp_lexer *);

/* End function decls */

int cp_lexer_read(struct cp_lexer * lexer, struct cp_token * token) {

    char firstchar;

    memset(token, 0, sizeof *token);

    cp_lexer_skip_all(lexer); /* All whitespace is now skipped. */

    firstchar = getc(lexer->file);

}

/**
 * Returns the number of characters skipped.
 */
static int cp_lexer_skip_whitespace(struct cp_lexer * lexer) {

    char c;
    int skipped = 0;

    for ( ; ; ++skipped) {

        c = getc(lexer->file);
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
    initial = getc(lexer->file);
    if (initial != '/') {
        ungetc(initial, lexer->file);
        return 0;
    }

    second = getc(lexer->file);
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
        while (getc(lexer->file) != '\n')
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
            cur = getc(lexer->file);

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
