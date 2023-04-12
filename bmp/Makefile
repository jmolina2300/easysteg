CC=gcc -Wall
CFLAGS= -g

all: test_bmp_create test_bmp_rewrite

tinybmp.o: tinybmp.c
	$(CC) $(CFLAGS) -c tinybmp.c

test_bmp_create: test_bmp_create.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_create.x test_bmp_create.o tinybmp.o

test_bmp_rewrite: test_bmp_rewrite.o tinybmp.o
	$(CC) $(CFLAGS) -o test_bmp_rewrite.x test_bmp_rewrite.o tinybmp.o


clean:
	rm -fR *.o
	rm -fR *.x
