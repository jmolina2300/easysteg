/**
 * Test decoding a secret message from a BMP file.
 * 
 * NOTE: This program assumes the following
 *  1. That there is actually a secret message in the BMP file
 *  2. That the secret message is a string of ASCII characters
 *  3. That the secret message is less than 1000 characters long
 * 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tinybmp.h"

/**
 * @brief 
 * 
 * Shifts in a byte of secret data (MSB first) from an array.
 * 
 *        
 */
void shift_in_byte_from_array(uint8_t *dest_byte, uint8_t *src, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}

int main(int argc, char * argv[]) 
{
    // Check arguments
    if (argc < 2) {
        printf("Usage:  ./test_bmp_decode [file name]\n\n");
        return 1;
    }
    char *inFileName = argv[1];

    // Open the  bmp file with embedded data
    BmpImage image;
    if (!bmp_read_from_file(&image, inFileName) ) {
        printf("Error reading BMP file\n");
        return 1;
    }


    // Read the data from the bmp file
    uint8_t secret_buffer[1000];
    memset(secret_buffer, 0, 1000);
    int i;
    for (i = 0; i < 1000; i++) {
        int offset = i * 8;
        shift_in_byte_from_array(&secret_buffer[i], image.data_raw, offset);
    }

    // Print the secret message
    printf("Secret message: %s\n", secret_buffer);


    
    bmp_destroy(&image);
    return 0;
}