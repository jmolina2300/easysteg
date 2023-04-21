#include "steg.h"




/**
 * @brief 
 * 
 * Shifts OUT a byte of secret data (MSB first) to an array of shorts.
 * - 16-bit version 
 * 
 * byte     short[]
 * 0x01 --> 0x0001 
 *        
 */
void shift_out_byte_to_array_16bit(uint16_t *dest, uint8_t src_byte, int offset) 
{
    int i;
    for (i = 0; i < 8; i++) {
        int bit = (src_byte >> (7 - i)) & 1;

        uint16_t cover_byte = dest[offset + i];
        cover_byte = (cover_byte & ~1) | bit;

        dest[offset + i] = cover_byte;
    }
}

/**
 * @brief 
 * 
 * Shifts IN a byte of secret data (MSB first) from an array of shorts.
 * - 16-bit version 
 * 
 * byte     short[]
 * 0x01 <-- 0x0001 
 *     
 */
void shift_in_byte_from_array_16bit(uint8_t *dest_byte, uint16_t *src, int offset)
{
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}


/**
 * @brief 
 * 
 * Shifts OUT a byte of secret data (MSB first) to an array.
 * - 8-bit version
 *        
 */
void shift_out_byte_to_array(uint8_t *dest, uint8_t src_byte, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = (src_byte >> (7 - i)) & 1;

        uint8_t cover_byte = dest[offset + i];
        cover_byte = (cover_byte & ~1) | bit;

        dest[offset + i] = cover_byte;
    }
}

/**
 * @brief 
 * 
 * Shifts IN a byte of secret data (MSB first) from an array.
 * - 8-bit version
 *        
 */
void shift_in_byte_from_array(uint8_t *dest_byte, uint8_t *src, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}




// Encode a message into a BMP file
int steg_encode_bmp(const char *in_file, const char *out_file, char *key, char *message, size_t message_size)
{
    // Read the BMP file
    BmpImage bmp;
    bmp_read_from_file(&bmp, in_file);


    // Check if the message is too long to fit in the image
    if (!message_fits(message_size, bmp.info.datasize, KEY_LENGTH))
    {
        bmp_destroy(&bmp);
        return 0;
    }

    // write the data to the file
    size_t i;
    for (i = 0; i < message_size; i++)
    {
        // Get the byte from the message and shift it out to the array
        uint8_t byte = message[i];
        int offset = i * 8;
        shift_out_byte_to_array(bmp.data_raw, byte, offset);
    }

    // write the bmp to the file
    if (!bmp_write_to_file(&bmp, out_file))
    {
        // Error occurred, but still need to destroy the bmp
    }
    bmp_destroy(&bmp);

    return 1;
}

// Decode a message from the bmp file and store it in the message buffer
int steg_decode_bmp(const char *in_file, const char *key, char *message, size_t max_message_size)
{

    // Read in the BMP file
    BmpImage bmp;
    if (!bmp_read_from_file(&bmp, in_file))
    {
        return 0;
    }


    // Run through the file data and fish out the message
    size_t i;
    for (i = 0; i < max_message_size; i++)
    {
        uint8_t byte = 0;
        int offset = i * 8;
        shift_in_byte_from_array(&byte, bmp.data_raw, offset);
        message[i] = byte;
    }

    // Destroy the bmp
    bmp_destroy(&bmp); 

    return 1;
}

int steg_encode_wav(const char *in_file, const char *out_file, char *key, char *message, size_t message_size)
{
    // Read the WAV file
    WaveFile wav;
    wav_read_from_file(&wav, in_file);


    // Get the length of the message and key in bits
    if (!message_fits(message_size, &wav.info.numSamples, KEY_LENGTH))
    {
        // The message is too long to fit in the WAV file
        
        wav_destroy(&wav);
        return 0;
    }

    uint16_t *sampleData = (uint16_t*)wav.data;

    // write the data to the file
    size_t i;
    for (i = 0; i < message_size; i++)
    {
        // Get the byte from the message and shift it out to the array
        uint8_t byte = message[i];
        int offset = i * 8;
        shift_out_byte_to_array_16bit(sampleData, byte, offset);
    }


    if (!wav_write_to_file(&wav, out_file))
    {
        // Error occurred, but still need to destroy the bmp
    }
    wav_destroy(&wav);

    return 1;
}


/**
 * @brief steg_decode_wav
 * 
 * Decodes a message from the WAV file and stores it in the message buffer
 * 
 * 
 * @param in_file 
 * @param key 
 * @param message 
 * @param max_message_size 
 * @return int 
 */
int steg_decode_wav(const char *in_file, const char *key, char *message, size_t max_message_size)
{
    // Read in the WAV file
    WaveFile wav;
    if (!wav_read_from_file(&wav, in_file))
    {
        return 0;
    }

    size_t message_length_bits = max_message_size * 8;
    if (message_length_bits > wav.info.numSamples)
    {
        // The message you're looking for couldn't possibly fit in this file
        wav_destroy(&wav);
        return 0;
    }

    uint16_t *sampleData = (uint16_t*)wav.data;

    // Run through the file data and fish out the message
    size_t i;
    for (i = 0; i < max_message_size; i++)
    {
        uint8_t byte = 0;
        int offset = i * 8;
        shift_in_byte_from_array_16bit(&byte, sampleData, offset);
        message[i] = byte;
    }

    // Destroy the wav
    wav_destroy(&wav); 

    return 1;
}



/**
 * @brief get_padded_message
 * 
 * Pads the message with 0's to make it a multiple of KEY_LENGTH
 * 
 * @param msg 
 * @param msg_len 
 * @return char* 
 */
char *get_padded_message(char *msg, size_t msg_len)
{
    char *padded_msg = NULL;
    size_t msg_padding = KEY_LENGTH - (msg_len % KEY_LENGTH);
    size_t padded_msg_len = msg_len + msg_padding;


    padded_msg = calloc(padded_msg_len, sizeof(char));
    if (padded_msg == NULL)
    {
        return NULL;
    }

    // Copy the original message into the padded message buffer
    memcpy(padded_msg, msg, msg_len);

    return padded_msg;
}



/**
 * @brief message_fits
 * 
 * Checks to see if the message will fit in the file
 * 
 * As specified in Key Protocol V1 drawing
 * 
 * @param message_length 
 * @param proposed_file_size 
 * @param key_length 
 * @return int 
 */
int message_fits(size_t message_length, size_t file_size, size_t key_length)
{
    size_t available_space = file_size / 8;
    available_space -= available_space % key_length;

    if (message_length > available_space)
    {
        return 0;
    }
    return 1;
}


/**
 * @brief steg_encrypt_block
 * 
 * Encrypts a block of data byte-per-byte using the key
 * 
 * @param message_ptr 
 * @param key 
 * @param key_len 
 */
void steg_encrypt_block(char *message_ptr, char* key, size_t key_len)
{
    size_t i;
    for (i = 0; i < key_len; i++)
    {
        message_ptr[i] = message_ptr[i] ^ key[i];
    }
}


/**
 * @brief steg_encrypt_message
 * 
 * Encrypts an entire message buffer using the key
 * 
 * @param msg 
 * @param key 
 * @param msg_len 
 * @param key_len 
 */
void steg_encrypt_message(char *msg, char *key, size_t msg_len, size_t key_len)
{
    size_t i;
    size_t num_blocks = msg_len / key_len + 1;
    size_t message_padding = key_len - (msg_len % key_len);

    for (i = 0; i < num_blocks; i++)
    {
        char *curr_block = &msg[i * key_len];
        steg_encrypt_block(curr_block, key, key_len);
    }

}


/**
 * @brief steg_decrypt_block
 * 
 * Decrypts a block of data byte-per-byte using the key
 * 
 * @param message_ptr 
 * @param key 
 * @param key_len 
 */
void steg_decrypt_block(char *message_ptr, char* key, size_t key_len)
{
    size_t i;
    for (i = 0; i < key_len; i++)
    {
        message_ptr[i] = message_ptr[i] ^ key[i];
    }
}


/**
 * @brief steg_decrypt_message
 * 
 * Decrypts an entire message buffer using the key
 * 
 * @param msg 
 * @param key 
 * @param msg_len 
 * @param key_len 
 */

void steg_decrypt_message(char *msg, char *key, size_t msg_len, size_t key_len)
{
    size_t i;
    size_t num_blocks = msg_len / key_len + 1;
    size_t message_padding = key_len - (msg_len % key_len);

    for (i = 0; i < num_blocks; i++)
    {
        char *curr_block = &msg[i * key_len];
        steg_decrypt_block(curr_block, key, key_len);
    }

}


