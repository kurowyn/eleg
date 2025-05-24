CC=gcc
BINARY_NAME=eleg
eleg:
	@$(CC) utils.c main.c -o $(BINARY_NAME)
