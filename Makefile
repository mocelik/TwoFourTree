.PHONY: all test clean

BIN_FOLDER := bin
MAIN_EXE := $(BIN_FOLDER)/tft
TEST_EXE := $(BIN_FOLDER)/testtft

dummy_make_dirs := $(shell mkdir -p $(BIN_FOLDER))

all:
	g++ src/main.cpp -o $(MAIN_EXE)
	g++ test/testmain.cpp -o $(TEST_EXE)

run:
	$(MAIN_EXE)

test:
	$(TEST_EXE)

clean:
	rm $(BIN_FOLDER)/*

