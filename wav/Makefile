CC=gcc -Wall
CFLAGS= -g

all: test_wav 

wavread.o: wavread.c
	$(CC) $(CFLAGS) -c wavread.c

test_wav: test_wav.o wavread.o
	$(CC) $(CFLAGS) -o test_wav.x test_wav.o wavread.o

clean:
	rm -fR *.o
	rm -fR *.x
