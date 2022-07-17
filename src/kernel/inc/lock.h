#ifndef KERNEL_INC_LOCK_H
#define KERNEL_INC_LOCK_H

void unlock_interrupt(void);
void lock_interrupt(void);

#define DECLARE_LOCK(name) volatile int name ## Locked
#define LOCK(name) \
	lock_interrupt(); \
	while (!__sync_bool_compare_and_swap(& name ## Locked, 0, 1)); \
	__sync_synchronize();
#define UNLOCK(name) \
	unlock_interrupt(); \
	__sync_synchronize(); \
	name ## Locked = 0;

#endif /* !KERNEL_INC_LOCK_H */
