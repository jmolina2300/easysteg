#ifndef STEG_H
#define STEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bmp/bmpread.h"  // BMP reading
#include "../wav/wavread.h"  // WAV reading


#define KEY_LENGTH 8



// Encode a message into a file (will select the correct function based on file type)
int steg_encode_file_raw(const char *in_file, const char *out_file, char *key, char *message);
int steg_decode_file_raw(const char *in_file, const char *key, char *message, size_t max_message_size);


// Encode a message into a BMP file
int steg_encode_bmp(const char *in_file, const char *out_file, char *key, char *message, size_t message_size);
int steg_decode_bmp(const char *in_file, const char *key, char *message, size_t max_message_size);


// Encode a message into a WAV file
int steg_encode_wav(const char *in_file, const char *out_file, char *key, char *message, size_t message_size);
int steg_decode_wav(const char *in_file, const char *key, char *message, size_t max_message_size);







#endif // STEG_H
