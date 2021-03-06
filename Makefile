# cargo-culted from https://www.partow.net/programming/makefile/index.html

CXX := clang++-8
CXXFLAGS := -pedantic-errors -Wall -Wextra --std=c++17

BUILD := ./build
OBJ_DIR := $(BUILD)/objects
APP_DIR := $(BUILD)/apps

TARGET := main

SRC := \
  $(wildcard src/*.cpp)

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^

-include $(DEPENDENCIES)


.PHONY: all build clean debug release asan lox_tests run_debug run_release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

# -DDEBUG defines DEBUG macro
debug: CXXFLAGS += -g -DDEBUG -DDEBUG_TRACE_EXECUTION -DDEBUG_PRINT_CODE
debug: all

release: CXXFLAGS += -O3
release: all

asan: CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
asan: release

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

lox_tests: clean debug
	dart tool/bin/test.dart clox --interpreter $(APP_DIR)/$(TARGET)

run_debug: clean debug
	$(APP_DIR)/$(TARGET)

run_release: clean release
	$(APP_DIR)/$(TARGET)