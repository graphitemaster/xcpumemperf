TARGET = xcpumemperf
LIBS = -lm -lpthread -lrt
CC = gcc
CFLAGS = -Wall -Wextra -Wno-format-truncation -O2

.PHONY: default all clean plot

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

plot:
	gnuplot plot.gp

clean:
	-rm -f *.o
	-rm -f $(TARGET)
