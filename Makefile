 # Implementace překladače imperativního jazyka IFJ21.
 # Makefile
 # Anton Medvedev (xmedve04)

CFLAGS = -std=c99 -g -Wall -pedantic -lm -Wextra -Werror
CC = gcc
FILES = symtable.c data_stack.c scanner.c str.c parser.c gen_code.c expr_handle.c exp_stack.c
MAIN_FILE = main.c
COMPILE = -o
OUTPUT_FILE = main
SCRIPT = is_it_ok.sh
SCRIPT_DIR = test_dir
PACK_NAME = xmedve04.tgz

.PHONY: all pack check clean

all:
	$(CC) $(CFLAGS) $(FILES) $(MAIN_FILE) $(COMPILE) $(OUTPUT_FILE)

pack:
	tar -czf $(PACK_NAME) *.h *.c Makefile rozdeleni dokumentace.pdf

check:
	chmod +x $(SCRIPT)
	./$(SCRIPT) $(PACK_NAME) $(SCRIPT_DIR)

clean:
	rm -rf $(OUTPUT_FILE) $(PACK_NAME) $(SCRIPT_DIR)
