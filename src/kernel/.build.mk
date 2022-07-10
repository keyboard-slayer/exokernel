KERNEL = $(BUILD)/kernel.elf

KERNEL_CFLAGS = 					\
	$(CFLAGS)						\
	-fno-pic						\
	-fno-pie						\
	-mno-red-zone					\
	-mno-sse						\
	-mno-sse2						\
	-fno-zero-initialized-in-bss	\
	-mcmodel=kernel					\
	-m64

KERNEL_LDFLAGS =					\
	$(LDFLAGS)						\
	-z max-page-size=0x1000

KERNEL_SRC += 						\
	$(wildcard src/kernel/src/*.c)			

KERNEL_OBJ := $(patsubst %, $(BUILD)/%.o, $(KERNEL_SRC))
DEPENDENCIES += $(KERNEL_OBJ:.o=.d)

$(BUILD)/%.c.o: %.c
	@$(MKCWD)
	$(CC) -c -o $@ $< $(KERNEL_CFLAGS)

$(KERNEL): $(KERNEL_OBJ)
	@$(MKCWD)
	$(LD) -o $@ $^ $(KERNEL_LDFLAGS)
