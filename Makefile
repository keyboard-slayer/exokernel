CC = clang
LD = ld.lld
AS = nasm

MKCWD=mkdir -p $(@D)
BUILD = ./build

LOADER ?= limine
ARCH ?= x86_64
BIOS ?= /usr/share/ovmf/x64/OVMF.fd
SYSROOT ?= ./sysroot

LINK_SCRIPT = src/linker.ld

CFLAGS =							\
	-nostdlib						\
	-fno-builtin					\
	-std=c99						\
	-pedantic						\
	-Wpedantic						\
	-Werror							\
	-Wextra							\
	-Wall							\
	-ggdb							\
	-ffreestanding					\
	-fno-stack-protector			\
	-O0								\

ASFLAGS =							\
	-F dwarf						\
	-g

LDFLAGS = 							\
	-static							\

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
include src/kernel/.build.mk

LDFLAGS += -T$(LINK_SCRIPT)

run: sysroot
	$(QEMU) $(QEMU_FLAGS)

sysroot: $(KERNEL) $(LOADER_FILE)
	mkdir -p $(SYSROOT)/boot
	cp $(KERNEL) $(SYSROOT)/boot

clean:
	rm -r $(BUILD)
	rm -r $(SYSROOT)

.PHONY: run clean
.DEFAULT_GOAL := sysroot