CC=gcc
BINARY_NAME=eleg
eleg:
	@$(CC) parsearg.c utils.c main.c -o $(BINARY_NAME)
