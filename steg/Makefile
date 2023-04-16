CC=gcc -Wall
CFLAGS= -g


all: test_encode test_encode_wav


test_encode:
	$(CC) $(CFLAGS) -o test_encode.x test_encode.c steg.c ../wav/wavread.c ../bmp/bmpread.c

test_encode_wav:
	$(CC) $(CFLAGS) -o test_encode_wav.x test_encode_wav.c steg.c ../wav/wavread.c ../bmp/bmpread.c



clean:
	rm -fR *.o
	rm -fR *.x

