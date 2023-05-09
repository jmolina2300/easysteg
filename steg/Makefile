CC=gcc -Wall
CFLAGS= -g


all: test_bmp test_wav


test_bmp:
	$(CC) $(CFLAGS) -o test_bmp.x test_bmp.c steg.c ../wav/wavread.c ../bmp/bmpread.c

test_wav:
	$(CC) $(CFLAGS) -o test_wav.x test_wav.c steg.c ../wav/wavread.c ../bmp/bmpread.c



clean:
	rm -fR *.o
	rm -fR *.x

