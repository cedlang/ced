#ifndef CED_LEXER_H
#define CED_LEXER_H

#include <stddef.h>
#include "str.h"

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
    str_t src;
    size_t pos;
} lexer_state_t;

typedef struct {
    toktype_t type;
    str_t literal;
} tok_t;

lexer_state_t create_lexer_state(str_t src);
tok_t lexer_next_tok(lexer_state_t* state);

str_t tok_type_to_str(toktype_t type);

#endif
