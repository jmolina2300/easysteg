#include <stdio.h>
#include "tinybmp.h"

int main(int argc, char * argv[]) 
{
    char *firstFileName = "generated_image.bmp";
    char *secondFileName = "generated_image2.bmp";


    // Create a BMP file and write it to a file
    BmpImage image;
    int color = 0x00ffff;
    bmp_create_header(&image, 32, 32, 24);
    if (!bmp_create_canvas(&image, color)) {
        printf("FAILURE: Failed to create canvas of color %x\n", color);
        return 1;
    }
    if (!bmp_write_to_file(&image, firstFileName)) {
        printf("FAILURE: Failed to write BMP file\n");
        return 1;
    }
    bmp_destroy_image(&image);
    printf("SUCCESS: Created BMP file\n");



    // Read the BMP file and write it to a new file
    BmpImage image2;
    if (!bmp_read_from_file(&image2, firstFileName)) {
        printf("FAILURE: Failed to read BMP file\n");
        return 1;
    }
    if (!bmp_write_to_file(&image2, secondFileName)) {
        printf("FAILURE: Failed to write BMP file\n");
        return 1;
    }
    bmp_destroy_image(&image2);

    printf("SUCCESS: Read BMP file\n");

    // the output of this write should be the same as original bmp

    return 0;
}