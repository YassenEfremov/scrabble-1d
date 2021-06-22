# Macros

# Compiler and passed flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -I.

# Header and object files
DEPS = ../libs/trie.h ./game_logic/game_logic.h ./game_settings/game_settings.h ./dictionary_handling/dict_to_trie.h ./dictionary_handling/add_word_to_dict.h

_OBJ = main.o jWrite.o	# raw file names
OBJ = $(patsubst %,obj/%,$(_OBJ))	# Add the obj directory path to each .o file


# ============================================================================================== #
# Rules

# To build the .o files you need the .c version of them and all of the headers

# Command for main.c
obj/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# # Command for jWrite.c
# obj/jWrite.o: ../libs/jWrite.c
# 	$(CC) -c -o $@ $< $(CFLAGS)


# To build the main executable you need all of the object files
main: $(OBJ)
	$(CC) -o $@ ./obj/main.o ./obj/jWrite.o $(CFLAGS)
