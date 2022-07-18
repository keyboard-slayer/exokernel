#include <stb_sprintf.h>
#include <stdarg.h>
#include <unistd.h>

int printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buf[512] = {0};
    stbsp_vsprintf(buf, format, args);
    syslog(buf);

    va_end(args);

    return 0;
}

__attribute__((noreturn)) void abort(void)
{
    printf("Abort\n");
    for (;;);
}
