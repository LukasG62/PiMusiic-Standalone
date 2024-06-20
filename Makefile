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
# Path to lib files
LIB_DIR=lib
# Compilation flags
CPFLAGS =-I$(INCLUDE_DIR)
# Linker flags
LB_FLAG =-lncurses -lpthread -lm -lasound
LD_FLAGS =-L$(LIB_DIR)


## Rules
.PHONY:all clean docs


all:$(PROG_PC) docs

$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB_DIR)/libmusic.a $(LIB_DIR)/libinet.a $(LIB_DIR)/libui.a
	@mkdir -p $(BIN_DIR)
	@echo "LD\t$@"
	@gcc -o $@ $< -I$(INCLUDE_DIR) -lui -lmusic -linet $(LD_FLAGS) $(LB_FLAG) 

$(OBJ_DIR)/pimusiic.o: $(SRC_DIR)/pimusiic.c
	@mkdir -p $(OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR)

$(OBJ_DIR)/pi2iserv.o: $(SRC_DIR)/pi2iserv.c
	@mkdir -p $(OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR)

$(LIB_DIR)/libmusic.a: $(OBJ_DIR)/mpp.o $(OBJ_DIR)/note.o $(OBJ_DIR)/sound.o $(OBJ_DIR)/request.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(LIB_DIR)/libinet.a: $(OBJ_DIR)/data.o $(OBJ_DIR)/session.o $(OBJ_DIR)/mysyscall.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(LIB_DIR)/libui.a: $(OBJ_DIR)/ui_common.o $(OBJ_DIR)/ui_menu.o $(OBJ_DIR)/ui_sequencer.o $(OBJ_DIR)/ui_manager.o
	@mkdir -p $(LIB_DIR)
	@echo "AR\t$@"
	@ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h $(INCLUDE_DIR)/common.h
	@mkdir -p $(OBJ_DIR)
	@echo "CC\t$@"
	@gcc -o $@ -c  $< -I$(INCLUDE_DIR) -DSESSION_DEBUG -DDATA_DEBUG -DCOMMON_DEBUG

# Clean rule
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* $(LIB_DIR)/* docs/*

docs: Doxyfile
	@echo "MAN\t$@"
	@doxygen Doxyfile 

##
