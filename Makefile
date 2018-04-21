SRCDIR = src
LIBDIR = lib
OBJDIR = bin

export BOARD_TAG=mega
export BOARD_SUB=atmega2560

CC ?= clang
CFLAGS += -MMD -MP -std=c11 -Wall -Wextra -O3 -pedantic -I./lib/ -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE
LDLIBS += -lpthread

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

.PHONY: all clean arduino
all: controller arduino

controller: $(OBJ)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir $@

-include $(DEP)

debug: CFLAGS += -g3 -O -DDEBUG
debug: all

arduino:
	$(MAKE) -C arduino

clean:
	$(RM) -r $(OBJDIR)
	$(RM) controller
	$(MAKE) -C arduino clean
