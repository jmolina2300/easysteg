CC=gcc -Wall
CFLAGS= -g

all: test_bmp_create test_bmp_size test_bmp_rewrite test_bmp_encode

tinybmp.o: tinybmp.c
	$(CC) $(CFLAGS) -c tinybmp.c

test_bmp_create: test_bmp_create.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_create.x test_bmp_create.o tinybmp.o

test_bmp_size: test_bmp_size.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_size.x test_bmp_size.o tinybmp.o

test_bmp_rewrite: test_bmp_rewrite.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_rewrite.x test_bmp_rewrite.o tinybmp.o

test_bmp_encode: test_bmp_encode.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_encode.x test_bmp_encode.o tinybmp.o

clean:
	rm -fR *.o
	rm -fR *.x
