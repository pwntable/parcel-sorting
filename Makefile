CC=gcc
CFLAGS=-Wall -Wextra -g -I./include

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
EXEC=parcel_system

# Unit tests source files (excludes main.c to prevent duplicate main definitions)
TEST_SRC=tests/test_suite.c src/address.c src/database.c src/login.c src/output.c src/parcel_list.c src/search.c src/sorting.c src/status.c src/validation.c
TEST_EXEC=test_suite

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC) $(TEST_EXEC)

run: $(EXEC)
	./$(EXEC)
