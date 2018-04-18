CC = gcc
C_FLAGS = -std=c99 -Wall -Wextra -g -O0 -pedantic -I./lib/
BIN_DIR = bin/
LIBS = -lpthread
BINARY = ps_controller
OBJECTS = bin/main.o bin/controller.o bin/command.o


.PHONY: all clean

all: $(BINARY) arduino

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BINARY): $(BIN_DIR) $(OBJECTS)
	$(CC) $(C_FLAGS) $(OBJECTS) $(LIBS) -o $@

bin/%.o : src/%.c | $(BIN_DIR)
	$(CC) $(C_FLAGS) $< -c -o $@

clean: clean-arduino
	rm -f $(OBJECTS)
	rm -rf $(BIN_DIR)
	rm -f $(BINARY)

include arduino.mk
