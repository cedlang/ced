#include "lexer.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

str_t read_src(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "no such file %s\n", path);
        exit(1);
    }

    off_t len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *buff = malloc(len);
    if (read(fd, buff, len) != len) {
        fprintf(stderr, "couldn't read from %s\n", path);
        exit(1);
    }

    close(fd);

    return to_str(buff);
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

        printf("%.*s -> %.*s\n", (int)type.len, type.content,
               (int)cur.literal.len, cur.literal.content);
    } while (cur.type != TOK_EOF);

    return 0;
}