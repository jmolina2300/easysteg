#include "steg.h"
#include <stdio.h>



void get_custom_message(char *message, char *key)
{
    printf("Enter a message to encode: ");
    scanf("%s", message);

    printf("Enter a key: ");
    scanf("%s", key);
}

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


    snprintf(message, 1000, "Hello world! This is my message!");
    snprintf(key, 1000, "applepie");

    // Encode the message into the file
    if (steg_encode_bmp(in_file, out_file, key, message, strlen(message)))
    {
        printf("SUCCESS: encoded message into file %s\n", out_file);
    }
    else
    {
        printf("ERROR: failed to encode message into file %s\n", out_file);
        return -1;
    }
    printf("Encoded message: %s\n\n", message);


    // Decode the message from the file
    if (steg_decode_bmp(out_file, key, message, 1000))
    {
        printf("SUCCESS: decoded message from file %s\n", out_file);
    }
    else
    {
        // Checksum didnt match or something
        printf("ERROR: failed to decode message from file %s\n", out_file);
        return -1;
    }
    printf("Decoded message: %s\n", message);


    return 0;

}