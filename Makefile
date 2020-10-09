TARGET  = rusher
SRC_DIR = src/
SOURCES = $(wildcard $(SRC_DIR)*.c $(SRC_DIR)*/*.c)
 
CC      = clang 
STD     = -std=c11
DEBUG   = -g -DDEBUG
CFLAGS  += -Werror -Wall -Wextra -Wpedantic # $(DEBUG)
LDFLAGS = -pthread -lcurl


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(STD) $(LDFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: clean

clean:
	$(RM) $(TARGET)
