#include <vec.h>
#include <string.h>
#include <json.h>
#include <reader.h>

static json_t json_parse_string(reader_t *r)
{
    vec_char_t string_builder;
    vec_init(&string_builder);

    while (!reader_is_end(r) && reader_next(r) != '"')
    {
        vec_push(&string_builder, reader_peek(r));
    }
    
    char const *ret = strdup(string_builder.data);
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
        vec_push(&array_builder, json_parse(r));
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
        json_t key = json_parse(r);
        reader_skip_space(r);
        
        if (reader_peek(r) != ':')
        {
            return (json_t) {
                .type = JSON_NULL
            };
        }
        
        reader_next(r);
        reader_skip_space(r);
        
        json_t value = json_parse(r);
        reader_skip_space(r);
        
        map_set(&object_builder, key, value);
        
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

json_t json_parse(reader_t *r)
{
    reader_skip_space(r);

    if (reader_peek(r) == '"')
    {
        return json_parse_string(r);
    }
    else if (reader_peek(r) == '-')
    {
        // TODO
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

    // while (!reader_is_end(&reader))
    // {
    //     switch (reader_next(&reader))
    //     {
    //         case '\"':
    //         {
    //             return json_parse_string(&reader);
    //         }
    //     }
    // }
}
