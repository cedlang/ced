#ifndef __CED_H__
#define __CED_H__

#include <stddef.h>

typedef enum {
    // internal
    ILLEGAL,

    // delimiters
    SEMICOLON,
    COLON,
    COMMA,

    //
    L_PAREN,
    R_PAREN,
    L_BRACKET,
    R_BRACKET,
    L_BRACE,
    R_BRACE,

    // operators
    EQUAL,
    SUM,
    SUB,
    DOT,
    STAR,

    // data types
    INTEGER,
    STRING,
    CHARACTER,

    // identifier
    IDENTIFIER,
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
