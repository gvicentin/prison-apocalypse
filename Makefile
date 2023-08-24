CC         = gcc
CFLAGS     = -Wall -Wextra -std=c11 -g
LDFLAGS    = -lraylib -lm -ldl -lpthread

SRCS_DIR   = src
ASSETS_DIR = assets

BUILD_DIR   = build
OBJS_DIR    = build/objects
TESTBIN_DIR = build/tests
BINARY      = build/prison-apocalypse

RAYLIB_DIR = deps/raylib/src

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
SOURCES	 := $(shell find $(SRCS_DIR) -name '*.c' -not -name '*_test.c')
OBJECTS  := $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SOURCES))
TESTSRCS := $(shell find $(SRCS_DIR) -name '*_test.c')
TESTS    := $(patsubst $(SRCS_DIR)/%.c, $(TESTBIN_DIR)/%.test, $(TESTSRCS))

.PHONY: all clean

all: compile compile-tests

compile-deps:
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

compile: $(OBJS_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -L$(RAYLIB_DIR) $(LDFLAGS) -Wl,-rpath=$(ROOT_DIR)$(RAYLIB_DIR) -o $(BINARY)

compile-tests: $(BULIDDIR) $(TESTS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) -DDEBUG -DASSETS_PATH=\"$(ROOT_DIR)assets\" -I$(RAYLIB_DIR) -c $< -o $@ 

$(TESTBIN_DIR)/%.test: $(SRCS_DIR)/%.c $(TESTBIN_DIR)
	$(CC) $(CFLAGS) -I$(SRCS_DIR) $< -o $(basename $@)

$(OBJS_DIR):
	mkdir -p $@

$(TESTBIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
