SRCDIR = src
LIBDIR = lib
OBJDIR = bin

CC ?= clang
CFLAGS += -MMD -MP -std=gnu11 -Wall -Wextra -O3 -pedantic -I./lib/
LDLIBS += -lpthread

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.c=.o))
DEP = $(OBJ:.o=.d)

.PHONY: all clean
all: controller

controller: $(OBJ)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir $@

-include $(DEP)

debug: CFLAGS += -g3 -O -DDEBUG
debug: all

clean:
	$(RM) -r $(OBJDIR)
	$(RM) controller
