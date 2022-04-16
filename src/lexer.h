#ifndef CED_LEXER_H
#define CED_LEXER_H

#include <stddef.h>

typedef enum {
    TOK_ILLEGAL,

    TOK_SEMICOLON,
    TOK_COLON,
    TOK_COMMA,

    TOK_L_PAREN,
    TOK_R_PAREN,
    TOK_L_BRACKET,
    TOK_R_BRACKET,
    TOK_L_BRACE,
    TOK_R_BRACE,

    TOK_EQUAL,
    TOK_SUM,
    TOK_SUB,
    TOK_DOT,
    TOK_STAR,

    TOK_INTEGER,
    TOK_STRING,
    TOK_CHARACTER,

    TOK_IDENTIFIER,
    TOK_EOF,
} toktype_t;

typedef struct {
    toktype_t type;
    char *literal;
} tok_t;

typedef struct {
    size_t len;
    tok_t *toks;
} tokchain_t;

tokchain_t lex(char *s);
const char *type_string(toktype_t t);

#endif
