PROJECT_TOP_DIR := $(shell pwd)
PROJECT_ROOT_DIR := $(PROJECT_TOP_DIR)/../../
PROJECT_BIN_DIR := $(PROJECT_TOP_DIR)/bin
PROJECT_SRC_DIR := $(PROJECT_TOP_DIR)/src
PROJECT_INC_DIR := $(PROJECT_TOP_DIR)/src
PROJECT_LIB_DIR := $(PROJECT_TOP_DIR)/lib
PROJECT_OBJ_DIR := $(PROJECT_TOP_DIR)/objs
MKDIR := mkdir -p  
  
CP := cp
TARGETS = x12_python.so 
  
CFLAGS :=-g -I$(PROJECT_INC_DIR) -lpthread $(PYLIB)
OFLAGS :=-g -I$(PROJECT_INC_DIR)
#CFLAGS :=$(CFLAGS) -D _OS_LINUX_ 
LDFLAG := -L$(PROJECT_LIB_DIR) 
  
src :=$(wildcard $(PROJECT_SRC_DIR)/*.c)  
dir := $(notdir $(src))  
PROJECT_OBJ := $(patsubst %.c,%.o,$(dir))  
      
PROJECT_ALL_OBJS := $(addprefix $(PROJECT_OBJ_DIR)/, $(PROJECT_OBJ))  
      
all:$(PROJECT_SRC_DIR) $(PROJECT_ALL_OBJS)  
	$(CC) $(CFLAGS) $(PROJECT_ALL_OBJS) -shared -fPIC -o $(PROJECT_BIN_DIR)/$(TARGETS)  
	$(CP) $(PROJECT_BIN_DIR)/$(TARGETS) $(PROJECT_ROOT_DIR)/bin/$(TARGETS)  
 
$(PROJECT_OBJ_DIR)/%.o:$(PROJECT_SRC_DIR)/%.c
	$(MKDIR) $(PROJECT_OBJ_DIR)
	$(CC) -c $(OFLAGS) $< -o $@
clean:  
	rm -fr $(PROJECT_OBJ_DIR)  
	rm -fr $(PROJECT_BIN_DIR)/$(TARGETS)
	rm -fr $(PROJECT_ROOT_DIR)/bin/$(TARGETS)
