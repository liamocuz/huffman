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
	# ./$(EXE) 0 tests/gophers.txt output/compression/gophers.huf
	# ./$(EXE) 0 tests/frost.txt output/compression/frost.huf
	# ./$(EXE) 0 tests/bible.txt output/compression/bible.huf
	./$(EXE) 0 tests/liam.txt output/compression/liam.huf
	# ./$(EXE) 0 tests/simple.txt output/compression/simple.huf
	# ./$(EXE) 0 tests/odyssey.txt output/compression/odyssey.huf
	
	# ./$(EXE) 1 output/compression/gophers.huf output/decompression/gophers_dec.txt
	# ./$(EXE) 1 output/compression/frost.huf output/decompression/frost_dec.txt
	# ./$(EXE) 1 output/compression/bible.huf output/decompression/bible_dec.txt
	./$(EXE) 1 output/compression/liam.huf output/decompression/liam_dec.txt
	# ./$(EXE) 1 output/compression/simple.huf output/decompression/simple_dec.txt
	# ./$(EXE) 1 output/compression/odyssey.huf output/decompression/odyssey_dec.txt

	# diff tests/gophers.txt output/decompression/gophers_dec.txt
	# diff tests/frost.txt output/decompression/frost_dec.txt
	# diff tests/bible.txt output/decompression/bible_dec.txt
	diff tests/liam.txt output/decompression/liam_dec.txt
	# diff tests/simple.txt output/decompression/simple_dec.txt
	# diff tests/odyssey.txt output/decompression/odyssey_dec.txt

leaks:
	make
	leaks -atExit -- ./$(EXE) 0 tests/gophers.txt output/gophers_out.txt
	leaks -atExit -- ./$(EXE) 1 output/gophers_out.txt a.txt 

clean:
	cd build; rm *.o; rm *.d;
	rm $(EXE)
	cd output; rm *;

-include $(OBJ:.o=.d)