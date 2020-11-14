CC=gcc
FLAGS=-Wall -pedantic
INC=-Isrc/
CFLAGS=$(FLAGS) -c -g --std=c99 $(INC)
LFLAGS=$(FLAGS) -lncurses
DIR_GUARD=@mkdir -p $(@D)
CFG=release

SOURCES=$(shell find src/ -type f -name "*.c")
OBJECTS=$(patsubst src/%.c,obj/$(CFG)/%.o,$(SOURCES))
DEPS=$(patsubst src/%.c,deps/%.d,$(SOURCES))

# Main targets
.PHONY: all clean clean_all

all: bin/$(CFG)/main

GTAGS: $(SOURCES)
	gtags

clean:
	rm -rf obj/$(CFG)/* bin/$(CFG)/* src/*.gch GTAGS GPATH GRTAGS

clean_all:
	rm -rf bin/* obj/* deps/*

# --- Compile Rule
obj/$(CFG)/%.o: src/%.c
	$(DIR_GUARD)
	$(CC) $(CFLAGS) $< -o $@

# --- Link Rule
bin/$(CFG)/main: $(OBJECTS)
	$(DIR_GUARD)
	$(CC) $(OBJECTS) $(LFLAGS) -o bin/$(CFG)/main

# --- Dependency Rule
#deps/%.d: src/%.c
#	$(DIR_GUARD)
#	$(CC) $(CFLAGS) -MM $< | sed -e 's/~\(.*\)\.o:/\1.d \1.o:/' > $@
