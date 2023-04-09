/**
 * Test encoding a secret message into a BMP file.
 * 
 * 
 * Future plan:
 * 
 * 1. Before writing the secret message, include some kind of header
 *    with information about the secret message, such as the length
 *    and a checksum to make sure the message was decoded correctly.
 * 
 * 2. (MAYBE) use a more sophisticated encoding scheme
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tinybmp.h"


/**
 * @brief 
 * 
 * Shifts out a byte of secret data (MSB first) to an array.
 * 
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

int main(int argc, char * argv[]) {
    // Check arguments
    if (argc < 2) {
        printf("Usage:  ./test_bmp_encode [file name]\n\n");
        return 1;
    }
    char inFileName[260];
    char outFileName[260];
    snprintf(inFileName, 260, "%s", argv[1]);
    snprintf(outFileName, 260, "%s-encoded.bmp", argv[1]);

    // Create a BMP file
    BmpImage image;
    if (!bmp_read_from_file( &image, inFileName)) {
        printf("Error reading BMP file\n");
        return 1;
    }

    // Get a secret message from the user
    char secret[1000];
    printf("Enter secret message: ");
    fgets(secret, 1000, stdin);
    int len = strlen(secret);

    // See if the file is big enough to hold the secret
    int cover_size = image.info.datasize;
    if (cover_size < len * 8) {
        printf("Cover file is not large enough to hold that message.\n");
        return 1;
    }

    // Shift out the secret message
    int i;
    for (i = 0; i < len; i++) {
        uint8_t byte = secret[i];
        int offset = i * 8;
        shift_out_byte_to_array(image.data_raw, byte, offset);
    }

    // Write the BMP file
    if (!bmp_write_to_file( & image, outFileName)) {
        printf("Error writing BMP file\n");
        return 1;
    }

    bmp_destroy(&image);
    return 0;
}