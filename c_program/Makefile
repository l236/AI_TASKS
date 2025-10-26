# Makefile for Contact Book

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I./include
TARGET = contact_book

# Source files
SRCS = src/main.c \
       src/contact_operations.c \
       src/file_operations.c \
       src/utils.c

# Object files
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) contacts.txt src/*.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

