CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -D__USE_MINGW_ANSI_STDIO=1 -O0 -g -Iunity -Imocks -I../src -I../src/app -I../src/driver

UNITY_SRC = unity/unity.c

TEST_SRCS = test_connect_backoff.c test_demo_printf.c
TEST_BINS = test_connect_backoff test_demo_printf

.PHONY: all clean test

all: test

test_connect_backoff: test_connect_backoff.c $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^

test_demo_printf: test_demo_printf.c $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^

test: test_connect_backoff test_demo_printf
	@echo "========================================"
	@echo "Running Unit Tests"
	@echo "========================================"
	./test_connect_backoff
	./test_demo_printf
	@echo "========================================"
	@echo "All Unit Tests Passed Successfully!"
	@echo "========================================"

clean:
	rm -f $(TEST_BINS) *.exe *.o
