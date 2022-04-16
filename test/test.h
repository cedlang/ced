#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define str_assert(expect, got, name)                                          \
    if (strncmp(expect.content, got.content, expect.len)) {                    \
        fprintf(stderr,                                                        \
                "assertion `" name "` failed, expected `%.*s`, got `%.*s`\n",  \
                (int)expect.len, expect.content, (int)got.len, got.content);   \
        exit(1);                                                               \
    }

#endif