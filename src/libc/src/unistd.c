#include <unistd.h>

uint64_t syscall_impl(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
    uint64_t syscall_return;

    __asm__ volatile(
        "syscall \n"
        : "=a"(syscall_return)
        : "a"(syscall_id), "b"(arg1), "d"(arg2), "S"(arg3), "D"(arg4)
        : "memory");

    return syscall_return;
}

void syslog(const char *message)
{
    syscall(SYS_LOG, (uint64_t) message);
}
