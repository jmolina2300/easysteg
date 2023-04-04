#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tinybmp.h"

/**
 * @brief Shifts out a byte of secret data to an array.
 * 
 * @param dest 
 * @param src 
 * @param offset 
 */
void shift_out_array(uint8_t *dest, char src, int offset) {
  int i;
  for (i = 0; i < 8; i++) {
    int bit = (src >> i) & 1;

    uint8_t cover_byte = dest[offset + i];
    cover_byte = (cover_byte & ~1) | bit;

    dest[offset + i] = cover_byte;
  }
}


int main(int argc, char * argv[]) 
{
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
  if (!bmp_read_from_file(&image, inFileName) ) {
    printf("Error reading BMP file\n");
    return 1;
  }
  
  // Get a pointer to the image data
  uint8_t *bmpData = image.data_raw;
  
  // Get a secret message from the user
  char secret[100];
  printf("Enter the secret message: ");
  fgets(secret, 100, stdin);
  int len = strlen(secret);
  
  // See if the file is big enough to hold the secret
  int cover_size = image.info.imagesize;
  if (cover_size < len * 8) {
      printf("Cover file is not large enough.\n");
      return 1;
  }

  // Shift out the secret message
  int i;
  for (i = 0; i < len - 8; i += 8) {
    shift_out_array(bmpData, secret[i], i);
  }

 
  // Write the BMP file
  if (!bmp_write_to_file(&image, outFileName) ) {
    printf("Error writing BMP file\n");
    return 1;
  }

  
  return 0;
}