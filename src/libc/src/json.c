#include <string.h>
#include <ctype.h>

#include <stdio.h>

#include "../inc/json.h"
#include "../inc/reader.h"

static json_t json_parse_string(reader_t *r)
{
    vec_char_t string_builder;
    vec_init(&string_builder);

    while (!reader_is_end(r) && reader_next(r) != '"')
    {
        vec_push(&string_builder, reader_peek(r));
    }

    reader_next(r);

    char *ret = strndup(string_builder.data, string_builder.length);

    vec_free(&string_builder);

    return (json_t) {
        .type = JSON_STRING,
        ._string = ret
    };
}

static json_t json_parse_array(reader_t *r)
{
    json_vec_t array_builder;
    vec_init(&array_builder);

    reader_next(r);
    reader_skip_space(r);
    
    while (!reader_is_end(r) && reader_peek(r) != ']')
    {
        vec_push(&array_builder, json_parse_reader(r));
        reader_skip_space(r);
        
        if (reader_peek(r) == ',')
        {
            reader_next(r);
            reader_skip_space(r);
        }
    }
    
    reader_next(r);
    reader_skip_space(r);
    
    json_t ret = (json_t) {
        .type = JSON_ARRAY,
        ._array = array_builder
    };
    
    return ret;
}

static json_t json_parse_object(reader_t *r)
{
    json_obj_t object_builder;
    map_init(&object_builder);
    
    reader_next(r);
    reader_skip_space(r);
    
    while (!reader_is_end(r) && reader_peek(r) != '}')
    {
        json_t key = json_parse_reader(r);

        reader_skip_space(r);
        
        if (reader_peek(r) != ':')
        {
            return (json_t) {
                .type = JSON_ERROR,
                ._string = strdup("Expected ':'")
            };
        }
        
        reader_next(r);
        reader_skip_space(r);
        
        json_t value = json_parse_reader(r);
        reader_skip_space(r);

        if (key.type != JSON_STRING)
        {
            return (json_t) {
                .type = JSON_ERROR,
                ._string = strdup("Expected string key")
            };
        }
        
        map_set(&object_builder, key._string, value);
        free(key._string);
        
        if (reader_peek(r) == ',')
        {
            reader_next(r);
            reader_skip_space(r);
        }
    }
    
    reader_next(r);
    reader_skip_space(r);

    json_t ret = (json_t) {
        .type = JSON_OBJECT,
        ._object = object_builder
    };
    
    return ret;
}

static json_t json_parse_number(reader_t *r)
{
    vec_char_t number_builder;
    vec_init(&number_builder);
    
    while (!reader_is_end(r) && (isdigit(reader_peek(r))))
    {
        vec_push(&number_builder, reader_peek(r));
        reader_next(r);
    }
    
    char *tmp = strndup(number_builder.data, number_builder.length);

    long ret = atol(tmp);

    free(tmp);
    vec_free(&number_builder);

    return (json_t) {
        .type = JSON_NUMBER,
        ._number = ret
    };
}

json_t json_get(json_t json, char const *key)
{
    if (json.type != JSON_OBJECT)
    {
        return (json_t) {
            .type = JSON_ERROR,
            ._string = strdup("json_get: json is not an object")
        };
    }

    json_t *value = map_get(&json._object, key);

    if (value == NULL)
    {
        return (json_t) {
            .type = JSON_ERROR,
            ._string = strdup("json_get: key not found")
        };
    }

    return *value;
}

json_t json_parse_reader(reader_t *r)
{
    reader_skip_space(r);

    if (reader_peek(r) == '"')
    {
        return json_parse_string(r);
    }
    else if (reader_peek(r) == '-' || isdigit(reader_peek(r)))
    {
        return json_parse_number(r);
    }
    else if(reader_skip_word(r, "true"))
    {
        return (json_t) {
            .type = JSON_BOOL,
            ._bool = true
        };
    }
    else if(reader_skip_word(r, "false"))
    {
        return (json_t) {
            .type = JSON_BOOL,
            ._bool = false
        };
    }
    else if(reader_skip_word(r, "null"))
    {
        return (json_t) {
            .type = JSON_NULL
        };
    }
    else if(reader_peek(r) == '[')
    {
        return json_parse_array(r);
    }
    else if(reader_peek(r) == '{')
    {
        return json_parse_object(r);
    }

    return (json_t) {
        .type = JSON_ERROR,
        ._string = strdup("json_parse_reader: invalid json")
    };
}

json_t json_parse(char const *str)
{
    reader_t r = reader_create(str, strlen(str));
    return json_parse_reader(&r);
}

void json_free(json_t *j)
{
    if (j->type == JSON_STRING || j->type == JSON_ERROR)
    {
        free(j->_string);
    }
    else if (j->type == JSON_ARRAY)
    {
        json_vec_t *vec = &j->_array;
        for (size_t i = 0; i < vec->length; i++)
        {
            json_free(&vec->data[i]);
        }
        free(vec->data);
    }
    else if(j->type == JSON_OBJECT)
    {
        const char *key;
        map_iter_t iter = map_iter(&j->_object);

        while ((key = map_next(&j->_object, &iter)))
        {
            json_free((json_t *)map_get(&j->_object, key));
        }

        map_deinit(&j->_object);
    }
}
