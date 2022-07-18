#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

#include <libc/inc/json.h>

char *open_example(void)
{
    FILE *fp = fopen("./src/unittests/test.json", "r");
    assert_true(fp != NULL);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = malloc(size + 1);
    assert_true(data != NULL);

    fread(data, 1, size, fp);
    data[size] = '\0';
    fclose(fp);

    return data;
}

void test_json(__attribute__((unused)) void **state)
{
    char *data = open_example();
    json_t json = json_parse(data);
    assert_int_equal(json.type, JSON_OBJECT);

    json_t string = json_get(json, "string");
    assert_int_equal(string.type, JSON_STRING);
    assert_string_equal(string._string, "Hello, world!");

    json_t number = json_get(json, "number");
    assert_int_equal(number.type, JSON_NUMBER);
    assert_int_equal(number._number, 42);

    json_t boolean = json_get(json, "boolean");
    assert_int_equal(boolean.type, JSON_BOOL);
    assert_true(boolean._bool);

    json_t null = json_get(json, "null");
    assert_int_equal(null.type, JSON_NULL);

    json_t array = json_get(json, "array");
    assert_int_equal(array.type, JSON_ARRAY);
    assert_int_equal(array._array.length, 3);

    for (size_t i = 0; i < 3; i++)
    {
        json_t item = array._array.data[i];
        assert_int_equal(item.type, JSON_NUMBER);
        assert_int_equal(item._number, i+1);
    }

    json_t object = json_get(json, "object");
    assert_int_equal(object.type, JSON_OBJECT);
    
    json_t string2 = json_get(object, "string");
    assert_int_equal(string2.type, JSON_STRING);
    assert_string_equal(string2._string, "It works !");

    json_free(&json);
    free(data);
}
