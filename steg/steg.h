#ifndef STEG_H
#define STEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bmp/bmpread.h"  // BMP reading
#include "../wav/wavread.h"  // WAV reading


#define PADDING_CHARACTER ' '
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



// Encrypt a single block of the message up to KEY_LENGTH bytes
void steg_encrypt_block(char *message_ptr, const char *key, size_t key_len);
void steg_decrypt_block(char *message_ptr, const char *key, size_t key_len);

int message_fits(size_t message_length, size_t proposed_file_size);
size_t compute_available_space(size_t file_size);

// Create an encrypted version of the plaintext 
char *get_encrypted_message(char *original_msg, char *key);


// Encode (Destination, Source, Length)
void encode_one_block(char *dest, const char *src, size_t src_len);
void decode_one_block(char *dest, const char *src, size_t src_len);


void copy_one_block(char *buffer, const char *src, size_t block_len);
void copy_one_chunk(char *dest, const char *src, size_t chunk_len);





#endif // STEG_H
