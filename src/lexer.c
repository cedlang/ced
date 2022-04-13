#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

const char *toktype_str[] = {
    [ILLEGAL] = "illegal",

    [SEMICOLON] = ";",
    [COLON] = ":",
    [COMMA] = ",",

    [L_PAREN] = "(",
    [R_PAREN] = ")",
    [L_BRACKET] = "[",
    [R_BRACKET] = "]",
    [L_BRACE] = "{",
    [R_BRACE] = "}",

    [EQUAL] = "=",
    [SUM] = "+",
    [SUB] = "-",
    [DOT] = ".",
    [STAR] = "*",

    [INTEGER] = "integer",
    [STRING] = "string",
    [CHARACTER] = "character",

    [IDENTIFIER] = "identifier",
};
static size_t pos = 0;

const char *type_string(toktype_t t) { return toktype_str[t]; }

static int is_space(char c) { return isspace(c); }
static int is_alnum(char c) { return isalnum(c) || c == '_'; }
static int is_digit(char c) { return c >= '0' && '9' >= c; }

static size_t len(int (*c)(char), char *s) {
    size_t i = 0;
    while ((*c)(s[i])) {
        i++;
    }
    return i;
}

static char *literal_char(char c) {
    char *s = malloc(2 * sizeof(char));
    s[0] = c;
    s[1] = 0;
    return s;
}

static char *read_char(char *s) { return literal_char(s[pos++]); }

static char *read_number(char *s) {
    size_t n_len = len(is_digit, s);

    char *buf = malloc(n_len * sizeof(char) + 1);
    memcpy(buf, s, n_len);
    buf[n_len] = 0;

    pos += n_len;
    return buf;
}

static char *read_identifier(char *s) {
    size_t id_len = len(is_alnum, s);

    char *buf = malloc(id_len * sizeof(char) + 1);
    memcpy(buf, s, id_len);
    buf[id_len] = 0;

    pos += id_len;
    return buf;
}

static tok_t read_token(char *s_) {
    while (is_space(s_[pos]))
        pos++;

    tok_t tok = {0};
    char *s = &s_[pos];

    switch (s_[pos]) {
    case ';':
        tok.type = SEMICOLON;
        tok.literal = read_char(s_);
        break;
    case ':':
        tok.type = COLON;
        tok.literal = read_char(s_);
        break;
    case ',':
        tok.type = COMMA;
        tok.literal = read_char(s_);
        break;
    case '(':
        tok.type = L_PAREN;
        tok.literal = read_char(s_);
        break;
    case ')':
        tok.type = R_PAREN;
        tok.literal = read_char(s_);
        break;
    case '[':
        tok.type = L_BRACKET;
        tok.literal = read_char(s_);
        break;
    case ']':
        tok.type = R_BRACKET;
        tok.literal = read_char(s_);
        break;
    case '{':
        tok.type = L_BRACE;
        tok.literal = read_char(s_);
        break;
    case '}':
        tok.type = R_BRACE;
        tok.literal = read_char(s_);
        break;
    case '=':
        tok.type = EQUAL;
        tok.literal = read_char(s_);
        break;
    case '+':
        tok.type = SUM;
        tok.literal = read_char(s_);
        break;
    case '-':
        tok.type = SUB;
        tok.literal = read_char(s_);
        break;
    case '.':
        tok.type = DOT;
        tok.literal = read_char(s_);
        break;
    case '*':
        tok.type = STAR;
        tok.literal = read_char(s_);
        break;
    case '0' ... '9':
        tok.type = INTEGER;
        tok.literal = read_number(s);
        break;
    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '_':
        tok.type = IDENTIFIER;
        tok.literal = read_identifier(s);
        break;
    case 0:
        tok.type = EOF;
        break;
    default:
        tok.type = ILLEGAL;
        tok.literal = read_char(s_);
        break;
    }
    return tok;
}

tokchain_t lex(char *s) {
    tokchain_t toks = {0};

    size_t slen = strlen(s);
    size_t alloc = 0;

    if (slen == 0) return toks;

    while (pos < slen) {
        tok_t tok = read_token(s);

        if ((int)tok.type == EOF) break;

        toks.len++;
        if (toks.len >= alloc) {
            alloc = toks.len * 2;
            toks.toks = realloc(toks.toks, alloc * sizeof(tok_t));
        }
        toks.toks[toks.len - 1] = tok;
    }
    return toks;
}