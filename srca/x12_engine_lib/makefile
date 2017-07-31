PROJECT_TOP_DIR := $(shell pwd)
PROJECT_ROOT_DIR := $(PROJECT_TOP_DIR)/../../
PROJECT_BIN_DIR := $(PROJECT_TOP_DIR)/bin
PROJECT_SRC_DIR := $(PROJECT_TOP_DIR)/src
PROJECT_INC_DIR := $(PROJECT_TOP_DIR)/src
PROJECT_LIB_INC_DIR := $(PROJECT_TOP_DIR)/inc
PROJECT_LIB_DIR := $(PROJECT_TOP_DIR)/lib
PROJECT_OBJ_DIR := $(PROJECT_TOP_DIR)/objs
MKDIR := mkdir -p  
  
CP := cp
FD := find *.h
TARGETS = libx12.a 
TARGETSSO = x12.so
  
CFLAGS :=-g -I$(PROJECT_INC_DIR) -I$(PROJECT_LIB_INC_DIR) -lpthread -ldl -llua $(PYLIB) $(NLLIB)
OFLAGS :=-g -I$(PROJECT_INC_DIR) -I$(PROJECT_LIB_INC_DIR)  $(PFLAGS) $(NLLIB)
LDFLAG := -L$(PROJECT_LIB_DIR) -L$(PROJECT_ROOT_DIR)
  
src :=$(wildcard $(PROJECT_SRC_DIR)/*.c)  
dir := $(notdir $(src))  
PROJECT_OBJ := $(patsubst %.c,%.o,$(dir) )  
      
PROJECT_ALL_OBJS := $(addprefix $(PROJECT_OBJ_DIR)/, $(PROJECT_OBJ))  
      
USR_H_FILE := $(shell find $(PROJECT_INC_DIR) -name '*.h' -exec basename {} \;)

$(warning $(USR_H_FILE))

all:$(PROJECT_SRC_DIR) $(PROJECT_ALL_OBJS)  
	ar  -rc  $(PROJECT_BIN_DIR)/$(TARGETS)  $(PROJECT_ALL_OBJS) 
	$(CC) $(CFLAGS) $(PROJECT_ALL_OBJS) -shared -fPIC -o $(PROJECT_BIN_DIR)/$(TARGETSSO) 
	$(CP) $(PROJECT_BIN_DIR)/$(TARGETS) $(PROJECT_ROOT_DIR)/lib/$(TARGETS) 
	$(CP) $(PROJECT_BIN_DIR)/$(TARGETSSO) $(PROJECT_ROOT_DIR)/bin/$(TARGETSSO) 
	@for n in $(USR_H_FILE); \
	do \
	$(CP) $(PROJECT_INC_DIR)/$$n $(PROJECT_ROOT_DIR)/inc/$$n; \
	done 

$(PROJECT_OBJ_DIR)/%.o:$(PROJECT_SRC_DIR)/%.c
	$(MKDIR) $(PROJECT_OBJ_DIR)
	$(CC) -c $(OFLAGS) $< -o $@  
clean:  
	rm -fr $(PROJECT_OBJ_DIR)  
	rm -fr $(PROJECT_BIN_DIR)/$(TARGETS)
	rm -fr $(PROJECT_BIN_DIR)/$(TARGETSSO)
	rm -fr $(PROJECT_ROOT_DIR)/lib/$(TARGETS)
	rm -fr $(PROJECT_ROOT_DIR)/bin/$(TARGETSSO)
	@for n in $(USR_H_FILE); \
	do \
	rm -fr $(PROJECT_ROOT_DIR)/inc/$$n; \
	done 
