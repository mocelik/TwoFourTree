.PHONY: all test clean

# FOLDERS
BIN_FOLDER := bin
TEST_FOLDER := test
SRC_FOLDER := src
dummy_make_dirs := $(shell mkdir -p $(BIN_FOLDER))

# EXECUTABLES
MAIN_EXE := $(BIN_FOLDER)/tft
TEST_EXE := $(BIN_FOLDER)/testtft

# Test source files
TEST_FILES := testmain.cpp test1.cpp
TEST_FILES := $(addprefix $(TEST_FOLDER)/, $(TEST_FILES))

# Main source files
SRC_FILES := main.cpp
SRC_FILES := $(addprefix $(SRC_FOLDER)/,$(SRC_FILES))

CXX = g++
CXXFLAGS = -Wall -g -std=c++17

###### TARGETS ########

all: compile_src compile_test

run:
	$(MAIN_EXE)

test:
	$(TEST_EXE)

compile_src: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(MAIN_EXE)
	
compile_test: $(TEST_FILES)
	$(CXX) $(CXXFLAGS) $(TEST_FILES) -o $(TEST_EXE)

clean:
	rm $(BIN_FOLDER)/*

