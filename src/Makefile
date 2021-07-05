# Macros

# TO SEE ALL WARNINGS RUN > make -B

# Compiler and passed flags
CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -I.

# Header and object files
DEPS = \
	trie.h \
	game_logic.h \
	game_settings.h \
	dict_handling.h \
	jWrite.h \
	jRead.h

_OBJ = \
	main.o \
	game_logic.o \
	game_settings.o \
	dict_handling.o \
	trie.o \
	jWrite.o \
	jRead.o

OBJ = $(patsubst %,obj/%,$(_OBJ))	# Add the obj directory path to each .o file


# ============================================================================================== #
# Rules

# To build the .o files you need the .c version of them and all of the headers
obj/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# To build the main executable you need all of the object files
scrabble: $(OBJ)
	$(CC) -o $@ ./obj/main.o ./obj/game_logic.o ./obj/game_settings.o ./obj/dict_handling.o ./obj/trie.o ./obj/jWrite.o ./obj/jRead.o $(CFLAGS)
