CC = clang
LD = ld.lld

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
	-std=c89						\
	-pedantic						\
	-Wpedantic						\
	-Werror							\
	-Wextra							\
	-Wall							\
	-ggdb							\
	-ffreestanding					\
	-fno-stack-protector			\
	-O0								\

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
	-enable-kvm

include src/$(ARCH)/.build.mk
include src/$(LOADER)/.build.mk
include src/kernel/.build.mk

LDFLAGS += -T$(LINK_SCRIPT)

run: sysroot
	$(QEMU) $(QEMU_FLAGS)

sysroot: $(KERNEL) LOADER
	mkdir -p $(SYSROOT)/boot
	cp $(KERNEL) $(SYSROOT)/boot