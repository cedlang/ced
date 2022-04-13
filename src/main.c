#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>

void from_file(char *s) {
    FILE *f = fopen(s, "r");
    if (!f) {
        fprintf(stderr, "no such file %s\n", s);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buff = malloc(l + 1);
    int r = fread(buff, l, 1, f);
    fclose(f);
    if (r == 0) {
        fprintf(stderr, "couldn't read from %s\n", s);
        exit(1);
    }

    buff[l] = 0;
    tokchain_t toks = lex(buff);

    printf("toks.len = %ld\n", toks.len);
    for (size_t i = 0; i < toks.len; i++) {
        printf("%s -> %s\n", type_string(toks.toks[i].type),
               toks.toks[i].literal);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 2)
        from_file(argv[1]);
    else
        printf("USAGE: %s main.ced\n", argv[0]);

    return 0;
}