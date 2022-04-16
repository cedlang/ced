#ifndef CED_LEXER_H
#define CED_LEXER_H

#include "str.h"
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
    TOK_L_ANGLE,
    TOK_R_ANGLE,

    TOK_EQUAL,
    TOK_SUM,
    TOK_SUB,
    TOK_DIV,
    TOK_REM,
    TOK_DOT,
    TOK_STAR,

    TOK_INC,
    TOK_DEC,

    TOK_LOGICAL_AND,
    TOK_LOGICAL_OR,
    TOK_BIT_AND,
    TOK_BIT_OR,

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
tok_t lexer_next_tok(lexer_state_t *state);

str_t tok_type_to_str(toktype_t type);

#endif
