#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static char *toktype_str[] = {
    [TOK_ILLEGAL] = "illegal",

    [TOK_SEMICOLON] = ";",
    [TOK_COLON] = ":",
    [TOK_COMMA] = ",",

    [TOK_L_PAREN] = "(",
    [TOK_R_PAREN] = ")",
    [TOK_L_BRACKET] = "[",
    [TOK_R_BRACKET] = "]",
    [TOK_L_BRACE] = "{",
    [TOK_R_BRACE] = "}",
    [TOK_L_ANGLE] = "<",
    [TOK_R_ANGLE] = ">",

    [TOK_EQUAL] = "=",
    [TOK_SUM] = "+",
    [TOK_SUB] = "-",
    [TOK_DIV] = "/",
    [TOK_REM] = "%",
    [TOK_DOT] = ".",
    [TOK_STAR] = "*",

    [TOK_INC] = "++",
    [TOK_DEC] = "--",

    [TOK_LOGICAL_AND] = "&",
    [TOK_LOGICAL_OR] = "|",
    [TOK_BIT_AND] = "&&",
    [TOK_BIT_OR] = "||",

    [TOK_INTEGER] = "integer",
    [TOK_STRING] = "string",
    [TOK_CHARACTER] = "character",

    [TOK_IDENTIFIER] = "identifier",
    [TOK_EOF] = "eof",
};

str_t tok_type_to_str(toktype_t type) { return to_str(toktype_str[type]); }

static int is_ident(char c) { return isalnum(c) || c == '_'; }

static char cur(lexer_state_t *state) { return state->src.content[state->pos]; }

static str_t read_char(lexer_state_t *state) {
    state->pos++;
    return slice_str(state->src, state->pos - 1, state->pos);
}

static int is_base_digit(char c, int base) {
    /* assert: base != 0 */

    const char *alphanum_set = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char *set = strchr(alphanum_set, toupper(c));
    if (set == NULL) {
        return 0;
    } else {
        /* assert: (ptrdiff_t)set - (ptrdiff_t)alphanum_set is not bigger than
         * strlen(alphanum_set) */
        /* check that the length is not bigger than the base */
        int num = (int)((ptrdiff_t)set - (ptrdiff_t)alphanum_set);
        if (num > base) {
            return 0;
        }
    }
    return 1;
}

static str_t read_integer(lexer_state_t *state) {
    size_t start = state->pos;
    char decimal_pt = 0;
    int base = 10;

    /* base specifier */
    if (cur(state) == '0') {
        state->pos++;
        switch (cur(state)) {
        case 'b':
            base = 2;
            break;
        case 'o':
            base = 8;
            break;
        case 'x':
            base = 16;
            break;
        default:
            break;
        }
        state->pos++;
    }

    while (cur(state) != '\0') {
        if (is_base_digit(cur(state), base)) {
            state->pos++;
        } else if (cur(state) == '.') {
            if (base != 10) {
                /* throw error: float as hex? */
                abort();
            }

            if (decimal_pt) {
                /* throw error: already has decimal point */
                abort();
            }
            state->pos++;
            decimal_pt = 1;
        } else if (cur(state) == '\'') /* comma separators*/ {
            state->pos++;
        } else {
            /* throw error: invalid character(?) */
            break;
        }
    }

    /* type specifier */
    switch (cur(state)) {
    case 'f': /* float */
    case 'u': /* unsigned */
    case 'l': /* long */
        state->pos++;
        break;
    }

    return slice_str(state->src, start, state->pos);
}

static str_t read_quoted(lexer_state_t *state, char quote_sym) {
    if (cur(state) == quote_sym) {
        state->pos++;
    } else {
        /* throw error: expected quote sym */
        abort();
    }

    /* quote itself will not be part of the sliced string, so start = quote+1 */
    size_t start = state->pos;
    while (cur(state) != '\0' && cur(state) != quote_sym) {
        if (cur(state) == '\\') {
            state->pos++;
            /* todo: do your interpreting of shit here (\n, \r, \t, \b, \x1b,
             * etc) */
            if (cur(state) == '\0') {
                /* throw error: expected character after \ */
                abort();
            } else if (cur(state) == quote_sym) {
                state->pos++;
            }
        } else {
            state->pos++;
        }
    }

    if (cur(state) != quote_sym) {
        /* throw error: expected enclosing quote character */
        abort();
    }
    state->pos++;
    return slice_str(state->src, start, state->pos - 1);
}

static str_t read_identifier(lexer_state_t *state) {
    size_t start = state->pos;
    while (is_ident(cur(state)))
        state->pos++;

    return slice_str(state->src, start, state->pos);
}

tok_t lexer_next_tok(lexer_state_t *state) {
    while (isspace(cur(state)))
        state->pos++;

    tok_t tok;

    if (state->pos >= state->src.len) {
        tok.type = TOK_EOF;
        tok.literal = to_str("EOF");
    } else if (isdigit(cur(state))) {
        tok.type = TOK_INTEGER;
        tok.literal = read_integer(state);
    } else if (is_ident(cur(state))) {
        tok.type = TOK_IDENTIFIER;
        tok.literal = read_identifier(state);
    } else {
        switch (cur(state)) {
        case ';':
            tok.type = TOK_SEMICOLON;
            tok.literal = read_char(state);
            break;
        case ':':
            tok.type = TOK_COLON;
            tok.literal = read_char(state);
            break;
        case ',':
            tok.type = TOK_COMMA;
            tok.literal = read_char(state);
            break;
        case '(':
            tok.type = TOK_L_PAREN;
            tok.literal = read_char(state);
            break;
        case ')':
            tok.type = TOK_R_PAREN;
            tok.literal = read_char(state);
            break;
        case '[':
            tok.type = TOK_L_BRACKET;
            tok.literal = read_char(state);
            break;
        case ']':
            tok.type = TOK_R_BRACKET;
            tok.literal = read_char(state);
            break;
        case '{':
            tok.type = TOK_L_BRACE;
            tok.literal = read_char(state);
            break;
        case '}':
            tok.type = TOK_R_BRACE;
            tok.literal = read_char(state);
            break;
        case '<':
            tok.type = TOK_L_ANGLE;
            tok.literal = read_char(state);
            break;
        case '>':
            tok.type = TOK_R_ANGLE;
            tok.literal = read_char(state);
            break;
        case '=':
            tok.type = TOK_EQUAL;
            tok.literal = read_char(state);
            break;
        case '+':
            tok.type = TOK_SUM;
            tok.literal = read_char(state);
            if (cur(state) == '+') {
                tok.type = TOK_INC;
            }
            break;
        case '-':
            tok.type = TOK_SUB;
            tok.literal = read_char(state);
            if (cur(state) == '-') {
                tok.type = TOK_DEC;
            }
            break;
        case '.':
            tok.type = TOK_DOT;
            tok.literal = read_char(state);
            break;
        case '*':
            tok.type = TOK_STAR;
            tok.literal = read_char(state);
            break;
        case '"':
            tok.type = TOK_STRING;
            tok.literal = read_quoted(state, '"');
            break;
        case '\'':
            tok.type = TOK_CHARACTER;
            tok.literal = read_quoted(state, '\'');
            break;
        case '/':
            tok.type = TOK_DIV;
            tok.literal = read_char(state);
            if (cur(state) == '/') {
                while (cur(state) != '\0' && cur(state) != '\n') {
                    state->pos++;
                }

                if (cur(state) == '\n') {
                    state->pos++;
                }
            }
            break;
        case '%':
            tok.type = TOK_REM;
            tok.literal = read_char(state);
            break;
        case '&':
            tok.type = TOK_BIT_AND;
            tok.literal = read_char(state);
            if (cur(state) == '&') {
                tok.type = TOK_LOGICAL_AND;
            }
            break;
        case '|':
            tok.type = TOK_BIT_OR;
            tok.literal = read_char(state);
            if (cur(state) == '|') {
                tok.type = TOK_LOGICAL_OR;
            }
            break;
        default:
            tok.type = TOK_ILLEGAL;
            tok.literal = read_char(state);
            break;
        }
    }

    return tok;
}

lexer_state_t create_lexer_state(str_t src) {
    lexer_state_t state;
    state.src = src;
    state.pos = 0;

    return state;
}
