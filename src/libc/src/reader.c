#include <string.h>
#include <ctype.h>

#include "../inc/reader.h"

reader_t reader_create(char const *s, size_t size)
{
    return (reader_t) {
        .data = s,
        .size = size,
        .offset = 0
    };
}

char reader_peek(reader_t *r)
{
    return r->offset > r->size ? '\0' : r->data[r->offset];
}   

char reader_next(reader_t *r)
{
    r->offset++;
    return reader_peek(r);
}

bool reader_is_end(reader_t *r)
{
    return r->offset >= r->size;
}

void reader_skip_space(reader_t *r)
{
    while (!reader_is_end(r) && isspace(reader_peek(r)))
    {
        reader_next(r);
    }
}

bool reader_skip_word(reader_t *r, char const *word)
{
    size_t word_size = strlen(word);
    if (r->size < word_size)
    {
        return false;
    }
    
    for (size_t i = 0; i < word_size; i++)
    {
        if (reader_peek(r) != word[i])
        {
            return false;
        }

        reader_next(r);
    }
    
    return true;
}
