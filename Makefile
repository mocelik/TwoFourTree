.PHONY: all run test clean
.DEFAULT_GOAL := all

# FLAGS
CXX := clang++
CXXFLAGS := -Wall -Wextra -g -std=c++17

# FOLDERS
BIN_DIR := ./bin
TEST_DIR := ./test
SRC_DIR := ./src
OBJ_DIR := ./obj
INC := -I$(SRC_DIR)
CXXFLAGS += $(INC)
dummy_make_dirs := $(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# EXECUTABLES
MAIN_EXE := $(BIN_DIR)/tft
TEST_EXE := $(BIN_DIR)/testtft

# Main source files
SRC_FILES :=  $(wildcard $(SRC_DIR)/*.cpp)
SOBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Test source files
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(TEST_FILES))

# Dependencies
DEPENDS :=  $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(TEST_FILES))
DEPENDS +=  $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC_FILES))

###### TARGETS ########

all: $(MAIN_EXE) $(TEST_EXE)

run:
	$(MAIN_EXE)

test:
	$(TEST_EXE)

$(MAIN_EXE): $(SOBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(MAIN_EXE) $^

$(TEST_EXE): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TEST_EXE) $^

-include $(DEPENDS)

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

clean:
	$(RM) -rv $(BIN_DIR) $(OBJ_DIR)
