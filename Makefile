
CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGET = rush

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)
