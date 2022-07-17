CC = clang
LD = ld.lld
AS = nasm

MKCWD=mkdir -p $(@D)
BUILD = ./build

LOADER ?= limine
ARCH ?= x86_64
BIOS ?= /usr/share/ovmf/x64/OVMF.fd
SYSROOT ?= ./sysroot
SCHED_QUANTUM ?= 8

CFLAGS =							\
	-nostdlib						\
	-std=c17						\
	-fbuiltin						\
	-pedantic						\
	-Wpedantic						\
	-Werror							\
	-Wextra							\
	-Wall							\
	-ggdb							\
	-fno-stack-protector			\
	-O0								\
	-D__$(ARCH)__

ASFLAGS =							\
	-F dwarf						\
	-g

LDFLAGS = 							\
	-static							\
	-nostdlib						\

QEMU_FLAGS =												\
	-no-shutdown											\
	-no-reboot												\
	-serial mon:stdio										\
	-m 256M													\
	-smp 4													\
	-bios $(BIOS)											\
	-drive file=fat:rw:sysroot,media=disk,format=raw		\
	-enable-kvm												\
	
include src/$(ARCH)/.build.mk
include src/$(LOADER)/.build.mk
include src/klibc/.build.mk
include src/kernel/.build.mk
include src/test/.build.mk

run: sysroot
	$(QEMU) $(QEMU_FLAGS)

sysroot: $(KERNEL) $(LOADER_FILE)
	mkdir -p $(SYSROOT)/boot $(SYSROOT)/bin
	cp $(KERNEL) $(SYSROOT)/boot

clean:
	rm -r $(BUILD)
	rm -r $(SYSROOT)

.PHONY: run clean
.DEFAULT_GOAL := sysroot