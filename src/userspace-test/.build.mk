USERTEST = $(BUILD)/test.elf
USERTEST_SRC = $(wildcard src/userspace-test/src/*.c) $(LIBC_SRC)
BUILD_USERTEST = $(BUILD)/userspace-test
USERTEST_OBJ = $(patsubst %, $(BUILD_USERTEST)/%.o, $(USERTEST_SRC))

$(BUILD_USERTEST)/%.c.o: %.c
	@$(MKCWD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_USERTEST)/%.s.o: %.s
	@$(MKCWD)
	$(AS) -o $@ $< $(ASFLAGS)

$(USERTEST): $(USERTEST_OBJ)
	@$(MKCWD)
	$(LD) $(LDFLAGS) -o $@ $^ -T src/userspace-test/link.ld

test-bin: $(USERTEST)
	cp $(USERTEST) $(SYSROOT)/bin/test.elf
	@bash -c 'echo -e "\nMODULE_PATH=boot:///bin/test.elf\nMODULE_STRING=test\n" >> $(SYSROOT)/boot/limine.cfg'
