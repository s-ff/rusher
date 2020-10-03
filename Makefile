TARGET  = rusher
SRC_DIR = src/
SOURCES = $(wildcard $(SRC_DIR)*.c)
 
CC      = gcc
STD     = -std=c11
DEBUG   = -g
CFLAGS  += -Werror -Wall -Wextra -Wpedantic $(DEBUG)


$(TARGET): $(SOURCES)
	$(CC) $(DEBUG) $(STD) $(SOURCES) -o $(TARGET)

.PHONY: clean

clean:
	$(RM) $(TARGET)
