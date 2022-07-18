#include <unistd.h>
#include <stdio.h>

int _start(void)
{
    pid_t pid = getpid();
    printf("Current pid: %d\n", pid);

    for (;;);
    __builtin_unreachable();
}
