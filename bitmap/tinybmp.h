#ifndef TINYBMP_H
#define TINYBMP_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE_BMP_HEADER 14
#define SIZE_DIB_HEADER 40
#define SIZE_PIXEL_24BIT 3


// BMP header structure
typedef struct {
  uint8_t   signature[2];
  uint32_t  filesize;
  uint16_t  reserved1;
  uint16_t  reserved2;
  uint32_t  offset;
} BmpFileHeader;

// DIB header structure
typedef struct {
  uint32_t  size;
  uint32_t  width;
  uint32_t  height;
  uint16_t  planes;
  uint16_t  bits;
  uint32_t  compression;
  uint32_t  imagesize;
  uint32_t  xresolution;
  uint32_t  yresolution;
  uint32_t  ncolors;
  uint32_t  importantcolors; 
} DibHeader;

// BMP pixel structure
typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} Pixel;

// BMP complete file structure
typedef struct {
  BmpFileHeader  header;
  DibHeader      info;
  Pixel         *data;
  uint8_t       *data_raw;
}  BmpImage;


void bmp_create_header(BmpImage *image, int width, int height, int bits);

int bmp_create_canvas(BmpImage *image, uint32_t color);

int bmp_write_to_file(BmpImage *image, const char *filename);
int bmp_write_to_file_raw(BmpImage *image, const char *filename);

int bmp_read_from_file(BmpImage *image, const char *filename);
int bmp_read_from_file_raw(BmpImage *image, const char *filename);

void bmp_destroy_image(BmpImage *image);




#endif