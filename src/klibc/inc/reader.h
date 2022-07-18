#ifndef KLIBC_INC_READER_H
#define KLIBC_INC_READER_H

#include <stddef.h>
#include <stdbool.h>

typedef struct 
{
    char const *data;
    size_t size;
    size_t offset;
} reader_t;


reader_t reader_create(char const *data, size_t size);
char reader_peek(reader_t *r);
char reader_next(reader_t *r);
bool reader_is_end(reader_t *r);
void reader_skip_space(reader_t *r);
bool reader_skip_word(reader_t *r, char const *word);

#endif /* !KLIBC_INC_READER_H */
