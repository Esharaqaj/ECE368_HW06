
CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wshadow -Wvla -pedantic
TARGET = a6
SRC = main.c
OBJ = main.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJ)