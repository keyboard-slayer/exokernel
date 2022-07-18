#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "tests.h"

int main(void)
{
    struct CMUnitTest const tests[] =
    {
        cmocka_unit_test(test_json),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}