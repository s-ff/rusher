TARGET  = rusher
SRC_DIR = src/
SOURCES = $(wildcard $(SRC_DIR)*.c)
 
CC      = clang 
STD     = -std=c11
DEBUG   = -g -DDEBUG
CFLAGS  += -Werror -Wall -Wextra -Wpedantic $(DEBUG)


$(TARGET): $(SOURCES)
	$(CC) $(DEBUG) $(STD) $(SOURCES) -o $(TARGET)

.PHONY: clean

clean:
	$(RM) $(TARGET)
