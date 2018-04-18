CC = gcc
C_FLAGS = -std=gnu11 -Wall -Wextra -g -O0 -pedantic -I./lib/
BIN_DIR = bin/
LIBS = -lpthread
BINARY = controller
OBJECTS = bin/main.o bin/controller.o bin/command.o bin/serial.o


.PHONY: all clean

all: $(BINARY)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BINARY): $(BIN_DIR) $(OBJECTS)
	$(CC) $(C_FLAGS) $(OBJECTS) $(LIBS) -o $@

bin/%.o : src/%.c | $(BIN_DIR)
	$(CC) $(C_FLAGS) $< -c -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(BIN_DIR)
	rm -f $(BINARY)
