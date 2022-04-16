#include "str.h"

str_t to_str(char *null_term_str) {
    str_t ret;
    ret.content = null_term_str;
    ret.len = strlen(null_term_str);

    return ret;
}

str_t slice_str(str_t str, size_t start, size_t end) {
    str_t ret;
    ret.content = str.content + start;
    ret.len = end - start;

    return ret;
}
