/**
 * Test rewriting of existing BMP files
 * 
 */
#include <stdio.h>
#include "bmpread.h"



int main(int argc, char * argv[]) 
{
	// Check arguments
	if (argc < 2) {
		printf("Usage:  ./test_bmp [file name]\n\n\n");
		return 1;
	}
	char inFileName[260];
	char outFileName[260];
	snprintf(inFileName, 260, "%s", argv[1]);
	snprintf(outFileName, 260, "%s-out.bmp", argv[1]);

	char read_buffer[512 + 1];

	// Using the non-heap functions
	BmpImage in_image;
	bmp_open_read(&in_image, inFileName);

	// Initialize the output image with the same header
	BmpImage out_image;
	bmp_copy_header(&out_image, &in_image);
	bmp_open_write(&out_image, outFileName);

	// display the header contents of both to make sure they are the same
	printf("Input Image Header:\n\n");
	printf("  Signature: %c%c\n"
		   "  File Size: %d\n"
		   "  Header Offset: %d\n"
		   "  Info Size: %d\n"
		   "  Width: %d\n"
		   "  Height: %d\n"
		   "  Planes: %d\n"
		   "  Bits: %d\n"
		   "  Data Size: %d\n",
		   in_image.header.signature[0], in_image.header.signature[1],
		   in_image.header.filesize,
		   in_image.header.offset,
		   in_image.info.size,
		   in_image.info.width,
		   in_image.info.height,
		   in_image.info.planes,
		   in_image.info.bits,
		   in_image.info.datasize);
	
	printf("\n\nOutput Image Header:\n\n");
	printf("  Signature: %c%c\n"
		   "  File Size: %d\n"
		   "  Header Offset: %d\n"
		   "  Info Size: %d\n"
		   "  Width: %d\n"
		   "  Height: %d\n"
		   "  Planes: %d\n"
		   "  Bits: %d\n"
		   "  Data Size: %d\n",
		   out_image.header.signature[0], out_image.header.signature[1],
		   out_image.header.filesize,
		   out_image.header.offset,
		   out_image.info.size,
		   out_image.info.width,
		   out_image.info.height,
		   out_image.info.planes,
		   out_image.info.bits,
		   out_image.info.datasize);

	// read in the contents to a buffer and write them back out to another file
	printf("\n");
	printf("Copying data from %s to %s...\n", inFileName, outFileName);
	size_t total_bytes_read = 0;
	size_t total_bytes_written = 0;
	size_t bytes_read = 0;
	size_t bytes_written = 0;
	while (total_bytes_read < in_image.info.datasize) {
		bytes_read = bmp_read(&in_image, read_buffer, 1, 512);
		total_bytes_read += bytes_read;

		bytes_written = bmp_write(&out_image, read_buffer, 1, bytes_read);
		total_bytes_written += bytes_written;
	}

	if (total_bytes_read != total_bytes_written) {
		printf("FAILURE: total_bytes_read (%u) != total_bytes_written (%u)\n", 
		(unsigned int) total_bytes_read, (unsigned int)total_bytes_written);	
	}

	printf("SUCCESS: total_bytes_read (%u) == total_bytes_written (%u)\n", 
		(unsigned int) total_bytes_read, (unsigned int)total_bytes_written);



	
	bmp_close(&in_image);
	bmp_close(&out_image);

    return 0;
}