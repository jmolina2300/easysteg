CC=gcc -Wall
CFLAGS= -g

all: test_bmp_create test_bmp_rewrite

bmpread.o: bmpread.c
	$(CC) $(CFLAGS) -c bmpread.c

test_bmp_create: test_bmp_create.o bmpread.o
	$(CC) $(CFLAGS) -o test_bmp_create.x test_bmp_create.o bmpread.o

test_bmp_rewrite: test_bmp_rewrite.o bmpread.o
	$(CC) $(CFLAGS) -o test_bmp_rewrite.x test_bmp_rewrite.o bmpread.o


clean:
	rm -fR *.o
	rm -fR *.x
