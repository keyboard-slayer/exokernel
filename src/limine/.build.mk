LINK_SCRIPT = src/limine/linker.ld
KERNEL_SRC += $(wildcard src/limine/src/*.c)
VER = 3.10.5

LOADER:
	mkdir -p $(SYSROOT)/EFI/BOOT $(SYSROOT)/boot
	wget https://github.com/limine-bootloader/limine/raw/v$(VER)-binary/BOOTX64.EFI -O $(SYSROOT)/EFI/BOOT/BOOTX64.EFI
	cp ./src/limine/limine.cfg $(SYSROOT)/boot/