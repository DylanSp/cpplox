# cargo-culted from https://www.partow.net/programming/makefile/index.html

CXX := g++
CXXFLAGS := -pedantic-errors -Wall -Wextra --std=c++17

BUILD := ./build
OBJ_DIR := $(BUILD)/objects
APP_DIR := $(BUILD)/apps

TARGET := cpplox

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
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET)

-include $(DEPENDENCIES)


.PHONY: all build clean debug release lox_tests

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

# -DDEBUG makes sure DEBUG macro is defined
debug: CXXFLAGS += -g -DDEBUG
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

lox_tests: debug
	dart tool/bin/test.dart clox --interpreter $(APP_DIR)/$(TARGET)
