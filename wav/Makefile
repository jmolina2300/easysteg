CC=gcc -Wall
CFLAGS= -g

all: test_wav test_wav_encode_message

wavread.o: wavread.c
	$(CC) $(CFLAGS) -c wavread.c

test_wav.o: test_wav.c
	$(CC) $(CFLAGS) -c test_wav.c

test_wav: test_wav.o wavread.o
	$(CC) $(CFLAGS) -o test_wav.x test_wav.o wavread.o




test_wav_encode_message: 
	$(CC) $(CFLAGS) -o test_wav_encode_message.x test_wav_encode_mesage.c wavread.c


clean:
	rm -fR *.o
	rm -fR *.x