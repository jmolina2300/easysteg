
# Compiler flags
CFLAGS = -g -Wall -O0 -std=c99


# Linker flags
LDFLAGS = 


SOURCES = bmpread.c test_bmp.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = test_bmp



$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)



.PHONY: clean


clean:
	@rm -f $(TARGET) $(OBJECTS) core
