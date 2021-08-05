SRC_DIR = src
OBJ_DIR = build

EXE = huffman
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CPPFLAGS = -Iinclude -MMD -MP
CFLAGS = -Wall

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(EXE) $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -c $< -o $@

run:
	make
	./$(EXE) 0 tests/gophers.txt output/gophers_out.txt
	./$(EXE) 0 tests/frost.txt output/frost_out.txt
	# ./$(EXE) 0 tests/bible.txt output/bible_out.txt
	./$(EXE) 0 tests/liam.txt output/liam_out.txt
	./$(EXE) 0 tests/simple.txt output/simple_out.txt
	./$(EXE) 0 tests/odyssey.txt output/odyssey_out.txt

leaks:
	leaks -atExit -- ./$(EXE) 0 tests/gophers.txt output/gophers_out.txt

clean:
	cd build; rm *.o; rm *.d;
	rm $(EXE)
	cd output; rm *;

-include $(OBJ:.o=.d)