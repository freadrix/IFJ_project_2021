# author Anton Medvedev (xmedve04)
# version 0.01

CFLAGS = -std=c99 -g -Wall -pedantic -lm # -Wextra -Werror
CC = gcc
FILES = symtable.c data_stack.c scanner.c str.c gen_code.c grammar.c parser.c #exp_stack.c
MAIN_FILE = test.c #main.c
COMPILE = -o test # main

all:
	$(CC) $(CFLAGS) $(FILES) $(MAIN_FILE) $(COMPILE)
#final:
#	gcc -Wall -Wextra -g test.c symtable.c data_stack.c scanner.c str.c gen_code.c grammar.c parser.c -o test