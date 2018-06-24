CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c99 -c
SOURCES = $(wildcard *c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = map

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
