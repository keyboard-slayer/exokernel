ECHO = $(BUILD)/echo.elf
ECHO_SRC = $(wildcard src/echo/src/*.c) $(LIBC_SRC) src/protocol/src/echo.c
BUILD_ECHO = $(BUILD)/echo
ECHO_OBJ = $(patsubst %, $(BUILD_ECHO)/%.o, $(ECHO_SRC))

$(BUILD_ECHO)/%.c.o: %.c
	@$(MKCWD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_ECHO)/%.s.o: %.s
	@$(MKCWD)
	$(AS) -o $@ $< $(ASFLAGS)

$(ECHO): $(ECHO_OBJ)
	@$(MKCWD)
	$(LD) $(LDFLAGS) -o $@ $^ -T src/echo/link.ld

echo: $(ECHO)
	cp $(ECHO) $(SYSROOT)/bin/echo.elf
	@bash -c 'echo -e "\nMODULE_PATH=boot:///bin/echo.elf\n" >> $(SYSROOT)/boot/limine.cfg'
