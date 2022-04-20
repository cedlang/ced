#include "../src/lexer.h"
#include "../src/str.h"
#include "test.h"

int main() {
    str_t src = to_str("sample(123);\ntest lol\n\n*-  ++ =\n,\n");
    tok_t expect[] = {{TOK_IDENTIFIER, to_str("sample"), 1, 1},
                      {TOK_L_PAREN, to_str("("), 1, 7},
                      {TOK_INTEGER, to_str("123"), 1, 8},
                      {TOK_R_PAREN, to_str(")"), 1, 11},
                      {TOK_SEMICOLON, to_str(";"), 1, 12},
                      {TOK_IDENTIFIER, to_str("test"), 2, 1},
                      {TOK_IDENTIFIER, to_str("lol"), 2, 6},
                      {TOK_STAR, to_str("*"), 4, 1},
                      {TOK_SUB, to_str("-"), 4, 2},
                      {TOK_SUM, to_str("+"), 4, 5},
                      {TOK_SUM, to_str("+"), 4, 6},
                      {TOK_EQUAL, to_str("="), 4, 8},
                      {TOK_COMMA, to_str(","), 5, 1}};

    lexer_state_t state = create_lexer_state(src);

    tok_t cur;
    for (int i = 0;; i++) {
        cur = lexer_next_tok(&state);
        if (cur.type == TOK_EOF) break;

        str_assert(tok_type_to_str(expect[i].type), tok_type_to_str(cur.type), "type");
        str_assert(expect[i].literal, cur.literal, "literal");

        int_assert(expect[i].line, cur.line, "line");
        int_assert(expect[i].column, cur.column, "column");
    }
    return 0;
}
