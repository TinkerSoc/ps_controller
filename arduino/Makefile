INCLUDES = -I../lib/
CXXFLAGS += $(INCLUDES) -Wall -pedantic
CFLAGS += $(INCLUDES) -Wall -pedantic

-include board.mk
BOARD_TAG ?= uno

EXT_SRCS = command.c

OBJDIR=build

EXT_OBJS = $(patsubst %.c, $(OBJDIR)/%.c.o, $(EXT_SRCS))

OTHER_LIBS += $(EXT_OBJS)
COMMON_DEPS += $(EXT_OBJS)

include $(ARDMK_DIR)/Arduino.mk

$(EXT_OBJS): $(OBJDIR)/%.c.o: ../src/%.c | $(OBJDIR)
	@$(MKDIR) $(dir $@)
	$(CC) -MMD -c $(CPPFLAGS) $(CFLAGS) $< -o $@

check-syntax:
	$(CXX) -c -include Arduino.h -x c++ $(CXXFLAGS) $(CPPFLAGS) -fsyntax-only $(CHK_SOURCES)
