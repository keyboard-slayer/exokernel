QEMU = qemu-system-x86_64
CFLAGS += --target=x86_64-elf-none
KERNEL_SRC = $(wildcard src/x86_64/src/*.c)