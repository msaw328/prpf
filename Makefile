.DEFAULT_GOAL = all

.PHONY: all split clear

# PHONY targets below

# The default - build a single executable with subcommands
all: build build/prpf

# The alternative - build separate executables each for a different subcommand
split: build build/prpf-listen build/prpf-sequence

clear:
	@echo "[RM] build"
	@rm -rf build

# non-PHONY targets below

CFLAGS := -O3 -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -Wno-error=attributes -pedantic \
	-std=gnu11 -fno-exceptions -fno-asynchronous-unwind-tables

# List of source file names (files with main() are separate)
SRC := main.c main_help.c main_listen.c main_sequence.c \
	net.c portseq.c timers.c

# Add the src/ prefix
SRC := $(addprefix src/, $(SRC))

# 1 source file = 1 object file = 1 dependency file
OBJ := $(patsubst src/%.c,build/%.o, $(SRC))
DEP := $(patsubst src/%.c,build/%.d, $(SRC))

include $(DEP)

# Each dependency file is generated from the source file
build/%.d: src/%.c build
	@echo -e "[DEP] $(subst build/,,$@)"
	@$(CC) $(CFLAGS) -MM -MT $(patsubst src/%.c,build/%.o, $<) $< > $@

build/%.o: src/%.c
	@echo -e "[CC] $(subst build/,,$@)"
	@$(CC) $(CFLAGS) -c $< -o $@

build:
	@echo -e "[DIR] $@"
	@mkdir -p $@

build/prpf: $(OBJ)
	@echo -e "[LD] $(subst build/,,$@)"
	@$(CC) $(CFLAGS) $^ -o $@

build/prpf-listen: build/timers.o build/net.o build/portseq.o build/main_listen.o
	@echo -e "[LD] $(subst build/,,$@)"
	@$(CC) $(CFLAGS) $^ -o $@

build/prpf-sequence: build/portseq.o build/main_sequence.o
	@echo -e "[LD] $(subst build/,,$@)"
	@$(CC) $(CFLAGS) $^ -o $@
