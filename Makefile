.PHONY: all test clean
.DEFAULT_GOAL := all

# FLAGS
CXX := clang++
CXXFLAGS := -Wall -g -std=c++17
CXXFLAGS += -MMD

# FOLDERS
BIN_FOLDER := bin
TEST_FOLDER := test
SRC_FOLDER := src
OBJ_FOLDER := obj
dummy_make_dirs := $(shell mkdir -p $(BIN_FOLDER) $(OBJ_FOLDER))

# EXECUTABLES
MAIN_EXE := $(BIN_FOLDER)/tft
TEST_EXE := $(BIN_FOLDER)/testtft

# Test source files
TEST_FILES := $(wildcard $(TEST_FOLDER)/*.cpp)
OBJ_FILES := $(patsubst $(TEST_FOLDER)/%.cpp,$(OBJ_FOLDER)/%.o,$(TEST_FILES))

# Main source files
SRC_FILES :=  $(wildcard $(SRC_FOLDER)/*.cpp)
SOBJ_FILES := $(patsubst $(SRC_FOLDER)/%.cpp,$(OBJ_FOLDER)/%.o,$(SRC_FILES))

# Dependencies
DEPENDS :=  $(patsubst $(TEST_FOLDER)/%.cpp,$(OBJ_FOLDER)/%.d,$(TEST_FILES))
DEPENDS +=  $(patsubst $(SRC_FOLDER)/%.cpp,$(OBJ_FOLDER)/%.d,$(SRC_FILES))

###### TARGETS ########

all: build_src build_test

run:
	$(MAIN_EXE)

test:
	$(TEST_EXE)

build_src: $(SOBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(MAIN_EXE) $^

build_test: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TEST_EXE) $^

-include $(DEPENDS)

$(OBJ_FOLDER)/%.o: $(TEST_FOLDER)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm $(BIN_FOLDER)/* $(OBJ_FOLDER)/*
