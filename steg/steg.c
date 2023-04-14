#include "steg.h"




/**
 * @brief 
 * 
 * Shifts OUT a byte of secret data (MSB first) to an array.
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
 *        
 */
void shift_in_byte_from_array(uint8_t *dest_byte, uint8_t *src, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}


/**
 * @brief steg_encode_file_raw
 * 
 * Encodes a message into a file 
 * 
 * @param in_file the file to read from
 * @param out_file the file to write the encoded message to
 * @param message the secret message to encode
 * @param key  the key to use for encoding
 * @return int 
 */
int steg_encode_file_raw(const char *in_file, const char *out_file, char *message, char *key)
{
    // For now, just assume it's a BMP file
    int success = steg_encode_bmp(in_file, out_file, message, key);
    return success;
}



/**
 * @brief steg_decode_file_raw
 * 
 * Decodes a message from a file
 * 
 * @param in_file - the file to read from
 * @param out_file - the file to write the decoded message to
 * @param message - the secret message to encode
 * @param key - the key to use for encoding
 * @return int 
 */
int steg_decode_file_raw(const char *in_file, const char *key, char *message, size_t max_message_size)
{
    int success = steg_decode_bmp(in_file, key, message, max_message_size);
    return success;
}


// Encode a message into a BMP file
int steg_encode_bmp(const char *in_file, const char *out_file, char *message, char *key)
{
    // Read the BMP file
    BmpImage bmp;
    bmp_read_from_file(&bmp, in_file);

    // Get the length of the message
    size_t message_length = strlen(message);
    size_t key_length = strlen(key);

    // Get the length of the message and key in bits
    size_t message_length_bits = message_length * 8;
    size_t key_length_bits = key_length * 8;

    if (message_length_bits + key_length_bits > bmp.info.datasize)
    {
        // The message is too long to fit in the image
        bmp_destroy(&bmp);
        return 0;
    }

    // write the data to the file
    size_t i;
    for (i = 0; i < message_length; i++)
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
int steg_decode_bmp(const char *in_file,  const char *key, char *message, size_t max_message_size)
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

