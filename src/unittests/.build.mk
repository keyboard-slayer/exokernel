UNITTEST = $(BUILD)/unittest.elf
UNITTEST_BUILD = $(BUILD)/unittest
UNITTEST_CC ?= gcc

UNITTEST_CFLAGS = 				\
	-std=c2x					\
	-Wall						\
	-Werror						\
	-Werror						\
	-Wextra						\
	-ggdb						\
	-fno-stack-protector		\
	-Isrc/						\
	-fsanitize=address			\
	-fsanitize=undefined		\
	-lcmocka

UNITTEST_SRC = $(wildcard src/unittests/*.c)			\
			   src/klibc/src/map.c 					    \
			   src/klibc/src/vec.c 						\
			   src/klibc/src/reader.c					\
			   src/klibc/src/json.c

$(UNITTEST): $(UNITTEST_SRC)
	@$(MKCWD)
	$(UNITTEST_CC) $^ -o $@ $(UNITTEST_CFLAGS)
