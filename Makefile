CC = gcc
C_FLAGS = -std=c11 -Wall -Wextra -g -O0 -pedantic -I./lib/
BIN_DIR = bin/
BINARY = main
OBJECTS = bin/main.o bin/controller.o

all: $(BINARY)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BINARY): $(BIN_DIR) $(OBJECTS)
	$(CC) $(C_FLAGS) $(OBJECTS) -o $@

bin/%.o : src/%.c
	$(CC) $(C_FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(BIN_DIR)
	rm -f $(BINARY)
