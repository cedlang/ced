#ifndef CED_STR_H
#define CED_STR_H

#include <stdint.h>
#include <string.h>

typedef struct {
    char *content;
    size_t len;
} str_t;

str_t to_str(char *null_term_str);
str_t slice_str(str_t str, size_t start, size_t end);

#endif
