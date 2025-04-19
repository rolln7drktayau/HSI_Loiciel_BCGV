#==============================================================================

# Define executable name
EXECUTABLE_NAME := bcgv_api

# Define build mode (debug or release)
BUILD_MODE := release

# Define source files to compile
SOURCES := bcgv_api.c


#==============================================================================
# DIRECTORIES AND FILES
#==============================================================================

### Predefined directories
DIR_BUILD := build/
DIR_SRC := src/
DIR_LIB := bin/

### Target
TARGET := $(DIR_LIB)$(EXECUTABLE_NAME).a

### Source files
SOURCE_FILES := $(strip $(filter-out \, $(addprefix $(DIR_SRC), $(SOURCES))))

### Object file 
OBJECT_FILES := $(strip $(subst $(DIR_SRC), $(DIR_BUILD), $(addsuffix .o, $(basename $(SOURCE_FILES)))))


#==============================================================================
# COMPILER AND LINKER
#==============================================================================

### C Compiler
CC := gcc

### C standard
CSTD := -std=c99

### Extra flags to give to the C compiler
CFLAGS := $(CSTD) -W -Wall -pedantic

### Extra flags to give to the C preprocessor (e.g. -I, -D, -U ...)
CPPFLAGS := -I./include

### Build mode specific flags
DEBUG_FLAGS   := -O0 -g3
RELEASE_FLAGS := -O2 -g0

### Library static
AR := ar

### Library compiler function
RCS := rcs


#==============================================================================
# SHELL
#==============================================================================

### Commands
MKDIR := mkdir -p
RM    := rm -f
RMDIR := rm -rf


#==============================================================================
# RULES
#==============================================================================

default: build

###Verbosity
VERBOSE := $(or $(v), $(verbose))
ifeq ($(VERBOSE),)
	Q := @
else
	Q := 
endif

#-------------------------------------------------
# (Internal rule) Check directories
#-------------------------------------------------
.PHONY: __checkdirs
__checkdirs:
	$(if $(wildcard $(DIR_LIB)),,$(shell $(MKDIR) $(DIR_LIB)))
	$(if $(wildcard $(DIR_BUILD)),,$(shell $(MKDIR) $(DIR_BUILD)))

#-------------------------------------------------
# (Internal rule) Pre build operations
#-------------------------------------------------
.PHONY: __prebuild
__prebuild: __checkdirs
ifeq ($(EXECUTABLE_NAME),)
	$(error EXECUTABLE_NAME is required. Must provide an executable name)
endif
ifeq ($(filter $(BUILD_MODE), debug release),)
	$(error BUILD_MODE is invalid. Must provide a valid mode (debug or release))
endif
ifeq ($(SOURCES),)
	$(error SOURCES is required. Must provide sources files to compile)
endif

ifeq ($(BUILD_MODE),debug)
	$(eval CFLAGS += $(DEBUG_FLAGS))
else ifeq ($(BUILD_MODE),release)
	$(eval CFLAGS += $(RELEASE_FLAGS))
endif

	@echo 'Build $(TARGET) ($(BUILD_MODE))'

#-------------------------------------------------
# Build operations
#-------------------------------------------------
.PHONY: build
build: __prebuild $(TARGET)
	@echo 'Build done'

#-------------------------------------------------
# Rebuild operations
#-------------------------------------------------
.PHONY: rebuild
rebuild: clean build

#-------------------------------------------------
# Compile C source files
#-------------------------------------------------
$(DIR_BUILD)%.o: $(DIR_SRC)%.c
	@echo 'CC    $@'
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#----------------------------------------------------
# Compile object files to build the static library
#----------------------------------------------------
$(TARGET): $(OBJECT_FILES)
	@echo 'AR    $@'
	$(Q)$(AR) $(RCS) $@ $^

#-------------------------------------------------
# Clean generated files
#-------------------------------------------------
.PHONY: clean
clean:
	@echo "Clean generated files"
ifneq ($(wildcard $(TARGET)),)
	@echo "RM    $(TARGET)"
	@$(RM) $(TARGET)
endif
ifneq ($(wildcard $(OBJECT_FILES)),)
	@echo "RM    $(OBJECT_FILES)"
	@$(RM) $(OBJECT_FILES)
endif
	@echo "Clean done"

#-------------------------------------------------
# Clean entire project
#-------------------------------------------------
.PHONY: cleanall
cleanall:
	@echo "Clean entire project"
ifneq ($(wildcard $(DIR_LIB)),)
	@echo "RM    $(DIR_LIB)"
	@$(RMDIR) $(DIR_LIB)
endif
ifneq ($(wildcard $(DIR_BUILD)),)
	@echo "RM    $(DIR_BUILD)"
	@$(RMDIR) $(DIR_BUILD)
endif
	@echo "Clean done"

#-------------------------------------------------
# Project informations
#-------------------------------------------------
.PHONY: info
info:
	@echo 'Build configurations'
	@echo '-- CC: $(CC)'
	@echo '-- CFLAGS: $(CFLAGS)'
	@echo 'Files'
	@echo '-- TARGET: $(TARGET)'
	@echo 'SOURCES: $(SOURCES)'
	@echo '-- SOURCE_FILES: $(SOURCE_FILES)'
	@echo '-- OBJECT_FILES: $(OBJECT_FILES)'
