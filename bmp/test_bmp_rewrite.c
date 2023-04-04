/**
 * Test rewriting of existing BMP files
 * 
 */
#include <stdio.h>
#include "tinybmp.h"



int main(int argc, char * argv[]) 
{
  // Check arguments
  if (argc < 2) {
    printf("Usage:  ./test_bmp_rewrite [file name]\n\n\n");
    return 1;
  }
  char inFileName[260];
  char outFileName[260];
  snprintf(inFileName, 260, "%s", argv[1]);
  snprintf(outFileName, 260, "%s-2.bmp", argv[1]);




  /**
   * @brief Test regular read and write functions
   * 
   */
  printf("Testing regular read and write functions\n");
  BmpImage image;
  if (!bmp_read_from_file(&image, inFileName) ) {
    printf("Error reading BMP file\n");
    return 1;
  }

  // print out a few of the bytes from data_raw
  printf("data_raw[0] = %d\n", image.data_raw[0]);
  printf("data_raw[1] = %d\n", image.data_raw[1]);
  printf("data_raw[2] = %d\n", image.data_raw[2]);
  printf("data_raw[3] = %d\n", image.data_raw[3]);
  printf("data_raw[4] = %d\n", image.data_raw[4]);

  // Write the BMP file
  if (!bmp_write_to_file(&image, outFileName) ) {
    printf("Error writing BMP file\n");
    return 1;
  }
  




  
  return 0;
}