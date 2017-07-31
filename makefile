export CC := gcc
#export PFLAGS := -D _OS_LINUX_ 
export PYLIB := -L/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/config -ldl -framework CoreFoundation -lpython2.7
export NLLIB := -I/usr/include/libnl3

PROJECT_ROOT_DIR := $(shell pwd)
PROJECT_INC_DIR := $(PROJECT_ROOT_DIR)/src
PROJECT_INC_SRC := $(PROJECT_ROOT_DIR)/src

#USR_SUB_DIR := $(PROJECT_INC_SRC)/x12_test_app $(PROJECT_INC_SRC)/x12_test_pyapi $(PROJECT_INC_SRC)/x12_test_so
USR_SUB_DIR := $(shell ls $(PROJECT_INC_SRC))


default:usr
      
usr:
	@for n in $(USR_SUB_DIR); \
	do \
	$(MAKE) -C $(PROJECT_INC_SRC)/$$n; \
	done 
	
clean:
	@for n in $(USR_SUB_DIR); \
	do \
	$(MAKE) -C $(PROJECT_INC_SRC)/$$n clean; \
	done
