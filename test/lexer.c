#include "../src/lexer.h"
#include "../src/str.h"
#include "test.h"

int main() {
    str_t src = to_str("sample(123);");
    tok_t expect[] = {{TOK_IDENTIFIER, to_str("sample")},
                      {TOK_L_PAREN, to_str("(")},
                      {TOK_INTEGER, to_str("123")},
                      {TOK_R_PAREN, to_str(")")},
                      {TOK_SEMICOLON, to_str(";")}};
    lexer_state_t state = create_lexer_state(src);

    tok_t cur;
    for (int i = 0;; i++) {
        cur = lexer_next_tok(&state);
        if (cur.type == TOK_EOF) break;

        str_assert(tok_type_to_str(expect[i].type), tok_type_to_str(cur.type), "type");
        str_assert(expect[i].literal, cur.literal, "literal");
    }
    return 0;
}
