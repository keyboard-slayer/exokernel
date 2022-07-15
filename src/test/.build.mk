TEST = $(BUILD)/test.elf
TEST_SRC = $(wildcard src/test/src/*.c) src/runtime/crt0.s
BUILD_TEST = $(BUILD)/test
TEST_OBJ = $(patsubst %, $(BUILD_TEST)/%.o, $(TEST_SRC))

$(BUILD_TEST)/%.c.o: %.c
	@$(MKCWD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_TEST)/%.s.o: %.s
	@$(MKCWD)
	$(AS) -o $@ $< $(ASFLAGS)

$(TEST): $(TEST_OBJ)
	@$(MKCWD)
	$(LD) $(LDFLAGS) -o $@ $^ -T src/test/link.ld

test: $(TEST)
	cp $(TEST) $(SYSROOT)/bin/test.elf
	@bash -c 'echo -e "\nMODULE_PATH=boot:///bin/test.elf\nMODULE_STRING=test\n" >> $(SYSROOT)/boot/limine.cfg'