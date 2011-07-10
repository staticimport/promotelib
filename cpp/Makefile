GLOBAL_DEPENDENCIES = Makefile

# Compiler
CXX := $(shell which ccache 2>/dev/null) g++

# C++ Source Files
CXX_SOURCE_ROOT_DIRS = src
CXX_HEADER_FILES := $(foreach dir,$(CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.hpp"))
CXX_ALL_COMPILABLE_FILES := $(foreach dir,$(CXX_SOURCE_ROOT_DIRS),$(shell find $(dir) -name "*.cpp"))
CXX_IMPLEMENTATION_FILES := $(wildcard $(CXX_HEADER_FILES:.hpp=.cpp))

# Build Destination
OBJECTS_ROOT_DIR = bin/obj
ALL_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(CXX_ALL_COMPILABLE_FILES:.cpp=.o)))
IMPLEMENTATION_OBJECTS := $(addprefix $(OBJECTS_ROOT_DIR)/,$(subst src/,,$(CXX_IMPLEMENTATION_FILES:.cpp=.o)))
OBJECT_DIRS := $(sort $(dir $(ALL_OBJECTS)))
TARGET_LIBRARY = bin/lib/libpromote++.a

# Individual Compilation Flags
DEBUGGING_FLAGS = -g3 -ggdb
ENVIRONMENT_FLAGS = -pthread -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
INCLUDE_PATH_FLAGS = $(foreach dir,$(sort $(dir $(CXX_HEADER_FILES))),-I$(dir))
MISCELLANEOUS_FLAGS = -rdynamic -pipe
OPTIMIZATION_FLAGS = -O3 -msse3
WARNING_FLAGS = -Wall -Werror

# Combined Compilation Flags
CXX_FLAGS = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
            $(ENVIRONMENT_FLAGS) $(MISCELLANEOUS_FLAGS) \
            $(INCLUDE_PATH_FLAGS)

# Create Compilation Flags, where $1 = full path to .o file
cxx_flags_with_deps = $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS) $(DEBUGGING_FLAGS) \
                      $(ENVIRONMENT_FLAGS) $(INCLUDE_PATH_FLAGS) $(MISCELLANEOUS_FLAGS) \
                      -MMD -MF $(1:.o=.d) -MT $1


all: init compile $(TARGET_LIBRARY) install

clean:
	rm -rf bin/* include/* lib/*

compile: $(ALL_OBJECTS) | init

init:
	@mkdir -p $(OBJECT_DIRS) bin/lib

install: $(CXX_HEADER_FILES)
	@mkdir -p include lib
	cp $(CXX_HEADER_FILES) include
	cp $(TARGET_LIBRARY) lib

$(OBJECTS_ROOT_DIR)/%.o: $(GLOBAL_DEPENDENCIES)
	$(CXX) $(call cxx_flags_with_deps,$@) -c src/$*.cpp -o $@

$(TARGET_LIBRARY): $(IMPLEMENTATION_OBJECTS)
	ar rcs $(TARGET_LIBRARY) $(IMPLEMENTATION_OBJECTS)

%.d:

.PHONY: all clean clean-standard compile init install all-special clean-special

define depends_include_template
-include $(1)

endef

$(eval $(foreach dep,$(wildcard $(ALL_OBJECTS:.o=.d)),$(call depends_include_template,$(dep))))


