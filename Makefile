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
	./$(EXE) 0 tests/gophers.txt output/gophers_out.txt

leaks:
	leaks -atExit -- ./$(EXE) 0 tests/gophers.txt output/gophers_out.txt

clean:
	cd build; rm *.o; rm *.d;
	rm $(EXE)

-include $(OBJ:.o=.d)