SRCDIR = src
LIBDIR = lib
OBJDIR = bin

export BOARD_TAG=mega
export BOARD_SUB=atmega2560

CC ?= clang
CFLAGS += -MMD -MP -std=c11 -Wall -Wextra -O3 -pedantic -I./lib/ -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE
LDLIBS += -lpthread

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: all clean arduino
all: controller arduino

controller: bin/main.o bin/controller.o bin/serial.o bin/command.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/%.o: CFLAGS += -MF $(@:.o=.d)
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(OBJDIR):
	mkdir $@

-include $(wildcard $(OBJDIR)/*.d)

debug: CFLAGS += -g3 -O -DDEBUG
debug: all

arduino:
	$(MAKE) -C arduino

clean-controller:
	$(RM) -r $(OBJDIR)
	$(RM) controller

clean: clean-controller
	$(RM) -r $(OBJDIR)
	$(RM) controller
	-$(MAKE) -C arduino clean
