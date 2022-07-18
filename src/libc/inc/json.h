#ifndef LIBC_INC_JSON_H
#define LIBC_INC_JSON_H

#include <stdbool.h>

#include "vec.h"
#include "map.h"
#include "reader.h"

typedef enum 
{
    JSON_ERROR,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_NULL,
} json_type;

typedef struct json json_t;

typedef vec(json_t) json_vec_t;
typedef map_t(json_t) json_obj_t;

struct json
{
    json_type type;

    union 
    {
        json_vec_t _array;
        json_obj_t _object;
        char *_string;
        long _number;
        bool _bool;
    };
};

json_t json_parse(char const *str);
json_t json_parse_reader(reader_t *r);
json_t json_get(json_t const j, char const *key);
void json_free(json_t *j);

#endif /* !LIBC_INC_JSON_H */
