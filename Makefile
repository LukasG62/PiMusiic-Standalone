##  Variables

# Compiler command
CC?=gcc
# Programs to build
PROG=pimusiic pi2iserv
# Path to pc binaries
BIN_DIR?=bin

# Programs for PC
PROG_PC=$(addprefix $(BIN_DIR)/, $(PROG))

# Path to source codes
SRC_DIR=src

# Path to header files
INCLUDE_DIR=include

# Path to obj files 
OBJ_DIR=obj
OBJ_UI_DIR=$(OBJ_DIR)/ui
OBJ_EVENT_DIR=$(OBJ_DIR)/event
OBJ_DIRS=$(OBJ_DIR) $(OBJ_UI_DIR) $(OBJ_EVENT_DIR)

# Path to lib files
LIB_DIR=lib

# Compilation flags
CPDEBUGFLAGS=-g -DSESSION_DEBUG -DDATA_DEBUG -DCOMMON_DEBUG
CPFLAGS=-I$(INCLUDE_DIR) -Wall -Wextra  $(CPDEBUGFLAGS)


# Linker flags
LB_FLAG =-lncurses -lpthread -lm -lasound
LD_FLAGS =-L$(LIB_DIR)

# Unit tests
UNIT_TESTS_SRC=$(wildcard src/tests/*.c)
UNIT_TEST_BIN_DIR=bin/tests
UNIT_TESTS=$(addprefix $(UNIT_TEST_BIN_DIR)/, $(notdir $(UNIT_TESTS_SRC:.c=)))
UNIT_TESTS_OBJ_DIR=$(OBJ_DIR)/tests

## Rules
.PHONY:all clean


all:$(PROG_PC) $(UNIT_TESTS)

$(UNIT_TEST_BIN_DIR)/%: $(UNIT_TESTS_OBJ_DIR)/%.o $(LIB_DIR)/libmusic.a $(LIB_DIR)/libinet.a $(LIB_DIR)/libui.a $(LIB_DIR)/libevent.a
	@mkdir -p $(UNIT_TEST_BIN_DIR)
	@echo "LD\t$@"
	@gcc -o $@ $< -lui -lmusic -levent -linet $(LD_FLAGS) $(LB_FLAG)

# Build the executable
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_DIR)/libmusic.a $(LIB_DIR)/libinet.a $(LIB_DIR)/libui.a $(LIB_DIR)/libevent.a
	@mkdir -p $(BIN_DIR)
	@echo "LD\t$@"
	@gcc -o $@ $< -lui -lmusic -levent -linet $(LD_FLAGS) $(LB_FLAG)

# Build the object files
$(OBJ_DIR)/pimusiic.o: $(SRC_DIR)/pimusiic.c
	@mkdir -p $(OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< $(CPFLAGS)

$(OBJ_DIR)/pi2iserv.o: $(SRC_DIR)/pi2iserv.c
	@mkdir -p $(OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< $(CPFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h $(INCLUDE_DIR)/common.h
	@mkdir -p $(OBJ_DIRS)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< $(CPFLAGS)

$(UNIT_TESTS_OBJ_DIR)/%.o: src/tests/%.c
	@mkdir -p $(UNIT_TESTS_OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c $< $(CPFLAGS)

# Build the static libraries
$(LIB_DIR)/libmusic.a: $(OBJ_DIR)/mpp.o $(OBJ_DIR)/note.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/request.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(LIB_DIR)/libinet.a: $(OBJ_DIR)/data.o $(OBJ_DIR)/session.o $(OBJ_DIR)/mysyscall.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(LIB_DIR)/libui.a: $(OBJ_UI_DIR)/ui_common.o $(OBJ_UI_DIR)/ui_menu.o $(OBJ_UI_DIR)/ui_sequencer.o $(OBJ_UI_DIR)/ui_manager.o $(OBJ_UI_DIR)/ui_form.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(LIB_DIR)/libevent.a: $(OBJ_EVENT_DIR)/event.o $(OBJ_EVENT_DIR)/event_queue.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* $(LIB_DIR)/* docs/*

docs: Doxyfile
	@echo "MAN\t$@"
	@doxygen Doxyfile
##
