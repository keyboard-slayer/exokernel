#ifdef __kernel__
#include <kernel/inc/logging.h>
#include <kernel/inc/utils.h>
#define printf(...) klog(NONE, __VA_ARGS__)
#define abort() halt()
#else
#include <stdio.h>
#include <stdlib.h>
#endif /* !__kernel__ */

void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function)
{
    printf("Assertion failed: %s, file %s, line %d, function %s", assertion, file, line, function);
    abort();
}

