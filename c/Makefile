GLOBAL_DEPENDENCIES = Makefile

# Compiler
CC := $(shell which ccache 2>/dev/null) gcc

# C Source Files
CC_SOURCE_ROOT_DIRS = src
CC_HEADER_FILES := $(foreach dir,$(CC_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.h"))
CC_ALL_COMPILABLE_FILES := $(foreach dir,$(CC_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.c"))
CC_IMPLEMENTATION_FILES := $(wildcard $(CC_HEADER_FILES:.h=.c))

# Build Destination
OBJECTS_ROOT_DIR = bin/obj
ALL_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(CC_ALL_COMPILABLE_FILES:.c=.o)))
IMPLEMENTATION_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(CC_IMPLEMENTATION_FILES:.c=.o)))
OBJECT_DIRS := $(sort $(dir $(ALL_OBJECTS)))
TARGET_LIBRARY = bin/lib/libpromote.a

# Individual Compilation Flags
DEBUGGING_FLAGS = -g3 -ggdb
ENVIRONMENT_FLAGS = -pthread -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
INCLUDE_PATH_FLAGS = $(foreach dir,$(sort $(dir $(CC_HEADER_FILES))),-I$(dir))
MISCELLANEOUS_FLAGS = -pipe -std=gnu99
#OPTIMIZATION_FLAGS = -Ofast -mtune=corei7 -march=corei7 -funroll-loops
OPTIMIZATION_FLAGS = -Ofast -funroll-loops
WARNING_FLAGS = -Wall -Werror

# Combined Compilation Flags
CC_FLAGS = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
            $(ENVIRONMENT_FLAGS) $(MISCELLANEOUS_FLAGS) \
            $(INCLUDE_PATH_FLAGS)

# Create Compilation Flags, where $1 = full path to .o file
CC_flags_with_deps = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
                      $(ENVIRONMENT_FLAGS) $(INCLUDE_PATH_FLAGS) $(MISCELLANEOUS_FLAGS) \
                      -MMD -MF $(1:.o=.d) -MT $1


all: init compile $(TARGET_LIBRARY) install

clean:
	rm -rf bin/* lib/* include/*

compile: $(ALL_OBJECTS) | init

init:
	@mkdir -p $(OBJECT_DIRS) bin/lib

install: $(CC_HEADER_FILES) $(TARGET_LIBRARY)
	@mkdir -p include lib
	@cp $(CC_HEADER_FILES) include
	@cp $(TARGET_LIBRARY) lib

$(OBJECTS_ROOT_DIR)/%.o: $(GLOBAL_DEPENDENCIES) | init
	$(CC) $(call CC_flags_with_deps,$@) -c src/$*.c -o $@

$(TARGET_LIBRARY): $(IMPLEMENTATION_OBJECTS)
	ar rcs $(TARGET_LIBRARY) $(IMPLEMENTATION_OBJECTS)

%.d:

.PHONY: all clean clean-standard compile init all-special clean-special

define depends_include_template
-include $(1)

endef

$(eval $(foreach dep,$(wildcard $(ALL_OBJECTS:.o=.d)),$(call depends_include_template,$(dep))))


