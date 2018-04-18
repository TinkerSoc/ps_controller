CC = gcc
C_FLAGS = -std=gnu11 -Wall -Wextra -g -O3 -pedantic -I./lib/
BIN_DIR = bin/
LIBS = -lpthread
BINARY = ps_controller
OBJECTS = bin/main.o bin/controller.o bin/serial.o

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
