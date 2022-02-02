SRC_DIR = src
OBJ_DIR = build
INCL_DIR = include

TESTS = bible frost gophers liam odyssey raven simple

EXE = huffman
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc
CFLAGS = -Wall -I $(INCL_DIR) -O2

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

test:
	make

	for name in $(TESTS) ; do \
		./$(EXE) 0 tests/$$name.txt output/compression/$$name.huf ; \
		./$(EXE) 1 output/compression/$$name.huf output/decompression/$${name}_dec.txt ; \
		diff tests/$$name.txt output/decompression/$${name}_dec.txt ; \
	done

leaks:
	make
	leaks -atExit -- ./$(EXE) 0 tests/gophers.txt output/compression/gophers.huf
	leaks -atExit -- ./$(EXE) 1 output/compression/gophers.huf output/decompression/gophers_dec.txt
	
	leaks -atExit -- ./$(EXE) 0 tests/bible.txt output/compression/bible.huf
	leaks -atExit -- ./$(EXE) 1 output/compression/bible.huf output/decompression/bible_dec.txt

clean:
	rm $(EXE)
	cd build; rm *.o;
	cd output/compression; rm *;
	cd output/decompression; rm *;