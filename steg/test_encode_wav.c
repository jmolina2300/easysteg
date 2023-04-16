#include "steg.h"
#include <stdio.h>


int main(int argc, char **argv)
{
    // Check if the user provided the correct number of arguments
    if (argc != 3)
    {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return -1;
    }

    char *in_file = argv[1];
    char *out_file = argv[2];

    char message[1000];
    char key[1000];
    // Get the message from the user
    printf("Enter the message to encode: ");
    fgets(message, 1000, stdin);

    // Get the key from the user
    printf("Enter the key to use: ");
    fgets(key, 1000, stdin);

    

    // Encode the message into the file
    if (steg_encode_wav(in_file, out_file, key, message, 1000))
    {
        printf("SUCCESS: encoded message into WAV file %s\n", out_file);
    }
    else
    {
        printf("ERROR: failed to encode message into WAV file %s\n", out_file);
        return -1;
    }
    printf("Encoded message: %s\n", message);


    // Decode the message from the file
    if (steg_decode_wav(out_file, key, message, 1000))
    {
        printf("SUCCESS: decoded message from WAV file %s\n", out_file);
    }
    else
    {
        // Checksum didnt match or something
        printf("ERROR: failed to decode message from WAV file %s\n", out_file);
        return -1;
    }
    printf("Decoded message: %s\n", message);


    return 0;

}