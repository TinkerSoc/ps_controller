SRCDIR = src
LIBDIR = lib

CC = clang
CFLAGS = -MMD -MP -std=gnu11 -Wall -Wextra -O3 -pedantic -I./lib/
LDLIBS = -lpthread

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

.PHONY: all clean
all: controller

controller: $(OBJ)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@


-include $(DEP)

debug: CFLAGS += -g3 -O -DDEBUG
debug: all

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM) controller
