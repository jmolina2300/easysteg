/**
 * Test creation of large BMP files
 * 
 */
#include <stdio.h>
#include "tinybmp.h"

#define START_SIZE 4000
#define END_SIZE 5000

int main(int argc, char * argv[]) 
{

    // Create a BMP file and write it to a file
    BmpImage image;
    int color = 0x00ffff;

    // Test that we can create images of varying sizes
    size_t size;
    for (size = START_SIZE; size < END_SIZE; size++) {
        bmp_create_header(&image, size, size, 24);
        if (!bmp_create_canvas(&image, color)) {
            printf("FAILURE: Failed to create canvas of color %06x and size %zu\n", color, size);
            return 1;
        }

        char fileName[32];
        sprintf(fileName, "test_size_%d.bmp", (int)size);
        if (!bmp_write_to_file(&image, fileName)) {
            printf("FAILURE: Failed to write BMP of size %zu\n", size);
            return 1;
        }

        printf("SUCCESS: Created BMP of size %zu\n"
               "  file size: %d\n"
               "  numPixels: %d\n"
               "  datasize: %d\n"
               , size, image.header.filesize, image.info.imagesize, image.info.imagesize * 3);
        
        bmp_destroy_image(&image);
    } 

    // Remove the files
    for (size = START_SIZE; size < END_SIZE; size++) {
        char fileName[32];
        sprintf(fileName, "test_size_%d.bmp", (int)size);
        remove(fileName);
    }

    return 0;
}