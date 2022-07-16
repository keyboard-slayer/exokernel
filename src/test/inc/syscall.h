#ifndef TEST_INC_SYSCALL_H
#define TEST_INC_SYSCALL_H

#include <stdint.h>


#define SYS_LOG 0

uint64_t syscall_impl(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
#define __syscall(id, a1, a2, a3, a4, ...) syscall_impl(id, a1, a2, a3, a4)
#define syscall(...) __syscall(__VA_ARGS__, 0, 0, 0, 0)

#endif /* !TEST_INC_SYSCALL_H */
