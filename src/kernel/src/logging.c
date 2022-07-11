#include "../inc/com.h"
#include "../inc/logging.h"
#include "../inc/stb_sprintf.h"

static const char *log[] = {
    "\033[35mOK   \033[39m", "\033[34mINFO \033[39m", "\033[31mERROR\033[39m"
};

void klog_impl(log_level_t level, char const *filename, size_t lineno, char const *format, ...)
{
    va_list args;
    char buf[512] = {0};

    stbsp_sprintf(buf, "[ %s ] \033[33m%s:%ld\033[0m ", log[level], filename, lineno);
    com_puts(buf);

    va_start(args, format);
    stbsp_vsprintf(buf, format, args);
    com_puts(buf);
    com_putc('\n');
    va_end(args);
}
