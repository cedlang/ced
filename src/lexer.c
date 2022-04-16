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

    [TOK_EQUAL] = "=",
    [TOK_SUM] = "+",
    [TOK_SUB] = "-",
    [TOK_DOT] = ".",
    [TOK_STAR] = "*",

    [TOK_INTEGER] = "integer",
    [TOK_STRING] = "string",
    [TOK_CHARACTER] = "character",

    [TOK_IDENTIFIER] = "identifier",
    [TOK_EOF] = "eof",
};

str_t tok_type_to_str(toktype_t type) { return to_str(toktype_str[type]); }

static int is_ident(char c) { return isalnum(c) || c == '_'; }

static char cur(lexer_state_t* state) {
    return state->src.content[state->pos];
}

static str_t read_char(lexer_state_t* state) {
    state->pos++;
    return slice_str(state->src, state->pos - 1, state->pos);
}

static str_t read_integer(lexer_state_t* state) {
    size_t start = state->pos;
    while (isdigit(cur(state))) state->pos++;

    return slice_str(state->src, start, state->pos);
}

static str_t read_identifier(lexer_state_t* state) {
    size_t start = state->pos;
    while (is_ident(cur(state))) state->pos++;

    return slice_str(state->src, start, state->pos);
}

tok_t lexer_next_tok(lexer_state_t* state) {
    while (isspace(cur(state))) state->pos++;

    tok_t tok;

    if (state->pos >= state->src.len) {
        tok.type = TOK_EOF;
        tok.literal = to_str("EOF");
    } else if (is_ident(cur(state))) {
        tok.type = TOK_IDENTIFIER;
        tok.literal = read_identifier(state);
    } else if (isdigit(cur(state))) {
        tok.type = TOK_INTEGER;
        tok.literal = read_integer(state);
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
        case '=':
            tok.type = TOK_EQUAL;
            tok.literal = read_char(state);
            break;
        case '+':
            tok.type = TOK_SUM;
            tok.literal = read_char(state);
            break;
        case '-':
            tok.type = TOK_SUB;
            tok.literal = read_char(state);
            break;
        case '.':
            tok.type = TOK_DOT;
            tok.literal = read_char(state);
            break;
        case '*':
            tok.type = TOK_STAR;
            tok.literal = read_char(state);
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
