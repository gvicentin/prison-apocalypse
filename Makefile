CC         = gcc
CFLAGS     = -Wall -Wextra -std=c11 -g
LDFLAGS    = -lraylib -lm -ldl -lpthread
SRCS_DIR   = src
ASSETS_DIR = assets
BUILD_DIR  = build
OBJS_DIR   = build/objects
BINARY     = build/prison-apocalypse

# Dependencies
RAYLIB_DIR        = deps/raylib/src
RAYLIB_SUBMOD_DIR = deps/raylib/src/external

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
SOURCES	 := $(shell find $(SRCS_DIR) -name '*.c')
OBJECTS  := $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SOURCES))

.PHONY: clean

compile-deps:
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

compile: $(OBJS_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINARY)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
