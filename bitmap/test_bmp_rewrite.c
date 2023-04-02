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




  // Create a BMP file
  BmpImage image;
  if (!bmp_read_from_file_raw(&image, inFileName) ) {
    printf("Error reading BMP file\n");
    return 1;
  }

  // do something to the data

  // Write the BMP file
  if (!bmp_write_to_file_raw(&image, outFileName) ) {
    printf("Error writing BMP file\n");
    return 1;
  }

  
  return 0;
}