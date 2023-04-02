#include "tinybmp.h"



void bmp_create_header(BmpImage *image, int width, int height, int bits)
{
  image->header.signature[0] = 'B';
  image->header.signature[1] = 'M';
  image->header.reserved1 = 0;
  image->header.reserved2 = 0;
  image->header.offset = SIZE_BMP_HEADER + SIZE_DIB_HEADER;

  image->info.size = SIZE_DIB_HEADER;
  image->info.width = width;
  image->info.height = height;
  image->info.planes = 1;
  image->info.bits = 24;
  image->info.compression = 0;
  //image->info.imagesize = (width * height) * SIZE_PIXEL_24BIT;
  image->info.imagesize = ((((width * 24) + 31) & ~31) >> 3) * height;
  image->info.xresolution = 0;
  image->info.yresolution = 0;
  image->info.ncolors = 0;
  image->info.importantcolors = 0;

  // Now, compute the full file size 
  image->header.filesize = image->header.offset + image->info.imagesize;
}

void write_header(BmpImage *image, FILE *file)
{
  // Write the header (manually)
  fwrite( &image->header.signature, 2, 1, file);
  fwrite( &image->header.filesize, 4, 1, file);
  fwrite( &image->header.reserved1, 2, 1, file);
  fwrite( &image->header.reserved2, 2, 1, file);
  fwrite( &image->header.offset, 4, 1, file);

  // Write Dib header (manually)
  fwrite( &image->info.size, 4, 1, file);
  fwrite( &image->info.width, 4, 1, file);
  fwrite( &image->info.height, 4, 1, file);
  fwrite( &image->info.planes, 2, 1, file);
  fwrite( &image->info.bits, 2, 1, file);
  fwrite( &image->info.compression, 4, 1, file);
  fwrite( &image->info.imagesize, 4, 1, file);
  fwrite( &image->info.xresolution, 4, 1, file);
  fwrite( &image->info.yresolution, 4, 1, file);
  fwrite( &image->info.ncolors, 4, 1, file);
  fwrite( &image->info.importantcolors, 4, 1, file);

  fseek(file, image->header.offset, SEEK_SET);
}

void read_header(BmpImage *image, FILE *file)
{
  // reset the file pointer to the beginning
  fseek(file, 0, SEEK_SET);

  // Read the header (manually)
  fread( &image->header.signature, 2, 1, file);
  fread( &image->header.filesize, 4, 1, file);
  fread( &image->header.reserved1, 2, 1, file);
  fread( &image->header.reserved2, 2, 1, file);
  fread( &image->header.offset, 4, 1, file);

  // Read Dib header (manually)
  fread( &image->info.size, 4, 1, file);
  fread( &image->info.width, 4, 1, file);
  fread( &image->info.height, 4, 1, file);
  fread( &image->info.planes, 2, 1, file);
  fread( &image->info.bits, 2, 1, file);
  fread( &image->info.compression, 4, 1, file);

  //    image->info.imagesize may be bigger than expected due to padding
  fread( &image->info.imagesize, 4, 1, file);   
  fread( &image->info.xresolution, 4, 1, file);
  fread( &image->info.yresolution, 4, 1, file);
  fread( &image->info.ncolors, 4, 1, file);
  fread( &image->info.importantcolors, 4, 1, file);

  fseek(file, image->header.offset, SEEK_SET);
}



/**
 * @brief Create the pixel data portion of a BMP image
 * 
 * This function allocates memory for the pixel data and fills it with a single color.
 * 
 * @param image 
 * @return int 
 */
int bmp_create_canvas(BmpImage *image, uint32_t color)
{
  // Create one pixel to paint the entire image
  Pixel pixel;
  pixel.b = color & 0xFF;
  pixel.g = (color >> 8) & 0xFF;
  pixel.r = (color >> 16) & 0xFF;

  // Allocate memory for the pixel data
  size_t numPixels = image->info.width * image->info.height;
  

  // Paint the entire image one color
  Pixel *pixelArray = malloc(SIZE_PIXEL_24BIT * numPixels);
  if (pixelArray == NULL) {
    return 0;
  }

  // Fill the pixel array
  size_t i;
  for (i = 0; i < numPixels; i++) {
    pixelArray[i] = pixel;
  }
  image->data = pixelArray;

  return 1;
}


/**
 * @brief Write a BMP image to a file
 * 
 * @param image 
 * @param filename 
 * @return int 
 */
int bmp_write_to_file(BmpImage *image, const char *filename)
{
  // Check if this image has been initialized (not an exhaustive check)
  if (image->header.filesize == 0) {
      return 0;
  }
  if (image->data == NULL) {
      return 0;
  }

  FILE *file = fopen(filename, "wb");
  if (!file) {
    // Could not open file for writing
    return 0;
  }

  write_header(image, file);


  // Write the pixel data
  size_t numPixels = image->info.width * image->info.height;
  fwrite(image->data, SIZE_PIXEL_24BIT, numPixels, file);


  // Close the file
  fclose(file);
  return 1;
}

/**
 * @brief Write a BMP raw data to a file
 * 
 * @param image 
 * @param filename 
 * @return int 
 */
int bmp_write_to_file_raw(BmpImage *image, const char *filename)
{
  // Check if this image has been initialized (not an exhaustive check)
  if (image->header.filesize == 0) {
      return 0;
  }
  if (image->data_raw == NULL) {
      return 0;
  }

  FILE *file = fopen(filename, "wb");
  if (!file) {
    // Could not open file for writing
    return 0;
  }
  

  write_header(image, file);

  // Write the pixel data
  size_t numBytes = image->info.imagesize;
  fwrite(image->data_raw, 1, numBytes, file);


  // Close the file
  fclose(file);
  return 1;
}




/**
 * @brief Check if the BMP header is valid
 * 
 * @param image 
 * @return int 
 */
int is_valid_bmp_header(BmpImage *image)
{
  if (image->header.signature[0] != 'B' || image->header.signature[1] != 'M') {
      // Invalid signature
      return 0;
  }
  if (image->info.size != SIZE_DIB_HEADER) {
      // Invalid DIB header size  
      return 0;
  }
  if (image->info.bits != 24) {
      // Unsupported bits per pixel (only 24 is supported)
      return 0;
  }
  if (image->info.compression != 0) {
      // Invalid compression
      return 0;
  }
  if (image->info.imagesize <= 0) {
      // Invalid image size
      return 0;
  }
  return 1;
}

/**
 * @brief Read a BMP image from a file
 * 
 * @param image 
 * @param filename 
 * @return int 
 */
int bmp_read_from_file(BmpImage *image, const char *filename)
{   
  FILE *file = fopen(filename, "rb");
  if (!file) {
      return 0;
  }

  // Check if the file is even long enough to contain the header section
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  if (fileSize < SIZE_BMP_HEADER + SIZE_DIB_HEADER) {
      fclose(file);
      return 0;
  }

  // Read the header
  read_header(image, file);
  if (!is_valid_bmp_header(image)) {
      fclose(file);
      return 0;
  }

  // Read the pixel data
  size_t numPixels = image->info.width * image->info.height;
  image->data = malloc(SIZE_PIXEL_24BIT * numPixels);
  if (image->data == NULL) {
      fclose(file);
      return 0;
  }
  fread(image->data, SIZE_PIXEL_24BIT, numPixels, file);

  // Close the file
  fclose(file);       
  return 1;
}

/**
 * @brief Read BMP raw data 
 * 
 * @param image 
 * @param filename 
 * @return int 
 */
int bmp_read_from_file_raw(BmpImage *image, const char *filename)
{   
  FILE *file = fopen(filename, "rb");
  if (!file) {
      return 0;
  }

  // Check if the file is even long enough to contain the header section
  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);
  if (fileSize < SIZE_BMP_HEADER + SIZE_DIB_HEADER) {
      fclose(file);
      return 0;
  }

  // Read the header
  read_header(image, file);
  if (!is_valid_bmp_header(image)) {
      fclose(file);
      return 0;
  }
  

  // Read the raw data
  size_t numDataBytes = image->header.filesize - (SIZE_BMP_HEADER + SIZE_DIB_HEADER);
  image->data_raw = malloc(numDataBytes);
  if (image->data_raw == NULL) {
      fclose(file);
      return 0;
  }
  fread(image->data_raw, 1, numDataBytes, file);

  // Close the file
  fclose(file);
  return 1;
}


void bmp_destroy_image(BmpImage *image)
{
  free(image->data);
  image->data = NULL;
}


