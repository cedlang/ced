#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

str_t read_src(char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "no such file %s\n", path);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buff = malloc(l + 1);
    int r = fread(buff, l, 1, f);
    fclose(f);
    if (r == 0) {
        fprintf(stderr, "couldn't read from %s\n", path);
        exit(1);
    }

    buff[l] = 0;

    str_t src = to_str(buff);

    return src;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s main.ced\n", argv[0]);
        return 1;
    }

    str_t src = read_src(argv[1]);
    lexer_state_t state = create_lexer_state(src);

    tok_t cur;
    do {
        cur = lexer_next_tok(&state);
        str_t type = tok_type_to_str(cur.type);

        printf("%.*s -> %.*s\n", (int)type.len, type.content, (int)cur.literal.len, cur.literal.content);
    } while (cur.type != TOK_EOF);

    return 0;
}