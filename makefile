CC = gcc
GAME_NAME = main

# flags given to compiler
CFLAGS = -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer -lm -g

# gives each xyz.c file in SRC a corresponding xyz.o file
SRC_DIR := $(wildcard src/*.c)
OBJ_DIR := $(patsubst src/%.c,obj/%.o,$(SRC_DIR))



# ===== c-commands *gulps* ===== 

# if no arguements are passed to make, the first one (game) is run by default
game: $(OBJ_DIR)
	$(CC) obj/* $(CFLAGS) -o $(GAME_NAME)

# removes all object files
clean:
	rm -rf obj/*

# removes executables and object files
purge:
	rm -rf obj/*
	rm -f $(GAME_NAME)

# this uhhh... uhmmm......
# compiles all .c files in SRC into .o files
obj/%.o: src/%.c 
	@mkdir -p obj/
	$(CC) $(CFLAGS) -c $< -o $@
