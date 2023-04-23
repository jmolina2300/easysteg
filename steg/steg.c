#include "steg.h"


size_t compute_available_space(size_t file_size)
{
    size_t available_space = (file_size / 8);
    available_space -= available_space % KEY_LENGTH;
    return available_space;
}

/**
 * @brief 
 * 
 * Shifts OUT a byte of secret data (MSB first) to an array of shorts.
 * - 16-bit version 
 * 
 * byte     short[]
 * 0x01 --> 0x0001 
 *        
 */
void shift_out_byte_to_array_16bit(uint16_t *dest, uint8_t src_byte, int offset) 
{
    int i;
    for (i = 0; i < 8; i++) {
        int bit = (src_byte >> (7 - i)) & 1;

        uint16_t cover_byte = dest[offset + i];
        cover_byte = (cover_byte & ~1) | bit;

        dest[offset + i] = cover_byte;
    }
}

/**
 * @brief 
 * 
 * Shifts IN a byte of secret data (MSB first) from an array of shorts.
 * - 16-bit version 
 * 
 * byte     short[]
 * 0x01 <-- 0x0001 
 *     
 */
void shift_in_byte_from_array_16bit(uint8_t *dest_byte, uint16_t *src, int offset)
{
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}


/**
 * @brief 
 * 
 * Shifts OUT a byte of secret data (MSB first) to an array.
 * - 8-bit version
 *        
 */
void shift_out_byte_to_array(uint8_t *dest, uint8_t src_byte, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = (src_byte >> (7 - i)) & 1;

        uint8_t cover_byte = dest[offset + i];
        cover_byte = (cover_byte & ~1) | bit;

        dest[offset + i] = cover_byte;
    }
}

/**
 * @brief 
 * 
 * Shifts IN a byte of secret data (MSB first) from an array.
 * - 8-bit version
 *        
 */
void shift_in_byte_from_array(uint8_t *dest_byte, uint8_t *src, int offset) {
    int i;
    for (i = 0; i < 8; i++) {
        int bit = src[offset + i] & 1;
        *dest_byte |= bit << (7 - i);
    }
}




// Encode a message into a BMP file
int steg_encode_bmp(const char *in_file, const char *out_file, char *key, char *message, size_t message_size)
{
    // Read the BMP file
    BmpImage bmp;
    if (!bmp_read_from_file(&bmp, in_file))
    {
        return 0;
    }


    // Check if the message is too long to fit in the image
    if (!message_fits(message_size, bmp.info.datasize))
    {
        bmp_destroy(&bmp);
        return 0;
    }

    // Overwrite LSBs with 0
    size_t i;
    size_t num_usable_bytes = compute_available_space(bmp.info.datasize);
    for (i = 0; i < num_usable_bytes; i++) {
        size_t offset = i * 8;
        shift_out_byte_to_array(bmp.data_raw, 0, offset);
    }



    char *encrypted_message = get_encrypted_message(message, key);
    if (encrypted_message == NULL) 
    {
        bmp_destroy(&bmp);
        return 0;
    }
    size_t encrypted_message_len = strlen(encrypted_message);

    // write the data to the file
    for (i = 0; i < encrypted_message_len; i++)
    {
        // Get the byte from the message and shift it out to the array
        uint8_t byte = encrypted_message[i];
        int offset = i * 8;
        shift_out_byte_to_array(bmp.data_raw, byte, offset);
    }

    // write the bmp to the file
    if (!bmp_write_to_file(&bmp, out_file))
    {
        // Error occurred, but still need to destroy the bmp
    }
    free(encrypted_message);
    bmp_destroy(&bmp);
    return 1;
}

// Decode a message from the bmp file and store it in the message buffer
int steg_decode_bmp(const char *in_file, const char *key, char *buffer, size_t buffer_size)
{

    // Read in the BMP file
    BmpImage bmp;
    if (!bmp_read_from_file(&bmp, in_file))
    {
        return 0;
    }

    // Compute number of possible bytes to read
    size_t num_bytes_to_read;
    size_t max_msg_len = compute_available_space(bmp.info.datasize);
    if (buffer_size > max_msg_len) {
        num_bytes_to_read = max_msg_len;
    } else {
        num_bytes_to_read = buffer_size;
    }

    // Intialize the buffer to the contents of the key repeatedly
    size_t i;
    //for (i = 0; i < buffer_size; i++) {
    //    buffer[i] = key[i % KEY_LENGTH];
    //}

    
    // Run through the file data and fish out the message
    for (i = 0; i < num_bytes_to_read; i++)
    {
        uint8_t byte = 0;
        int offset = i * 8;
        shift_in_byte_from_array(&byte, bmp.data_raw, offset);
        buffer[i] = byte;
    }


    
    // Decrypt all blocks 
    size_t num_blocks = num_bytes_to_read / KEY_LENGTH;
    for (i = 0; i < num_blocks; i++) {
        char *curr_block = buffer + (i * KEY_LENGTH);
        steg_decrypt_block(curr_block, key, KEY_LENGTH);
    }

    // Destroy the bmp
    bmp_destroy(&bmp); 
    return 1;
}

int steg_encode_wav(const char *in_file, const char *out_file, char *key, char *message, size_t message_size)
{
    // Read the WAV file
    WaveFile wav;
    if (!wav_read_from_file(&wav, in_file))
    {
        return 0;
    }

    // Check if the message is too long to fit in the image
    if (!message_fits(message_size, wav.info.numSamples))
    {
        wav_destroy(&wav);
        return 0;
    }

    uint16_t *sampleData = (uint16_t*)wav.data; 

    // Overwrite LSBs with 0
    size_t i;
    size_t num_usable_bytes = compute_available_space(wav.info.numSamples);
    for (i = 0; i < num_usable_bytes; i++)
    {
        int offset = i * 8;
        shift_out_byte_to_array_16bit(sampleData, 0, offset);
    }

    // Create the encrypted message
    char *encrypted_message = get_encrypted_message(message, key);
    if (encrypted_message == NULL) 
    {
        wav_destroy(&wav);
        return 0;
    }
    size_t encrypted_message_len = strlen(encrypted_message);



    // write the data to the file
    for (i = 0; i < encrypted_message_len; i++)
    {
        uint8_t byte = encrypted_message[i];
        int offset = i * 8;
        shift_out_byte_to_array_16bit(sampleData, byte, offset);
    }

    // write the wav to the file
    if (!wav_write_to_file(&wav, out_file))
    {
        // Error occurred, but still need to destroy the wav
    }
    free(encrypted_message);
    wav_destroy(&wav);
    return 1;
}


/**
 * @brief steg_decode_wav
 * 
 * Decodes a message from the WAV file and stores it in the message buffer
 * 
 * 
 * @param in_file 
 * @param key 
 * @param message 
 * @param buffer_size 
 * @return int 
 */
int steg_decode_wav(const char *in_file, const char *key, char *buffer, size_t buffer_size)
{
    // Read in the WAV file
    WaveFile wav;
    if (!wav_read_from_file(&wav, in_file))
    {
        return 0;
    }

    // Compute number of possible bytes to read
    size_t num_bytes_to_read;
    size_t max_msg_len = compute_available_space(wav.info.numSamples);
    if (buffer_size > max_msg_len) {
        num_bytes_to_read = max_msg_len;
    } else {
        num_bytes_to_read = buffer_size;
    }

    uint16_t *sampleData = (uint16_t*)wav.data;

    // Run through the file data and fish out the message
    size_t i;
    for (i = 0; i < num_bytes_to_read; i++)
    {
        uint8_t byte = 0;
        int offset = i * 8;
        shift_in_byte_from_array_16bit(&byte, sampleData, offset);
        buffer[i] = byte;
    }

    // Decrypt all blocks
    size_t num_blocks = num_bytes_to_read / KEY_LENGTH;
    for (i = 0; i < num_blocks; i++) {
        char *curr_block = buffer + (i * KEY_LENGTH);
        steg_decrypt_block(curr_block, key, KEY_LENGTH);
    }

    // Destroy the wav
    wav_destroy(&wav); 
    return 1;
}



/**
 * @brief get_padded_message
 * 
 * Pads the message with 0's to make it a multiple of KEY_LENGTH
 * 
 * @param msg 
 * @param msg_len 
 * @return char* 
 */
char *get_padded_message(const char *msg)
{
    char *padded_msg = NULL;
    size_t msg_len = strlen(msg);
    size_t msg_padding = KEY_LENGTH - (msg_len % KEY_LENGTH);
    size_t padded_msg_len = msg_len + msg_padding;


    padded_msg = (char*)calloc(padded_msg_len + 1, sizeof(char));
    if (padded_msg == NULL)
    {
        return NULL;
    }

    // Copy the original message into the padded message buffer
    memcpy(padded_msg, msg, msg_len);
    size_t i;
    for(i =  msg_len; i < padded_msg_len; i++) {
        padded_msg[i] = PADDING_CHARACTER;
    }

    padded_msg[padded_msg_len] = '\0';
    return padded_msg;
}



/**
 * @brief message_fits
 * 
 * Checks to see if the message will fit in the file
 * 
 * As specified in Key Protocol V1 drawing
 * 
 * @param message_length 
 * @param proposed_file_size 
 * @param key_length 
 * @return int 
 */
int message_fits(size_t message_length, size_t file_size)
{
    size_t available_space = file_size / 8;
    available_space -= available_space % KEY_LENGTH;

    if (message_length > available_space)
    {
        return 0;
    }
    return 1;
}





char *get_encrypted_message(char *original_msg, char *key)
{
    char *new_message = get_padded_message(original_msg);
    if (new_message == NULL) {
        return NULL;
    }

    size_t new_msg_len = strlen(new_message);
    size_t num_blocks = new_msg_len / KEY_LENGTH;
    size_t i;
    for (i = 0 ; i < num_blocks; i++) {
        char *curr_block = new_message + (i * KEY_LENGTH);
        steg_encrypt_block(curr_block, key, KEY_LENGTH);
    }

    return new_message;
}





/**
 * @brief steg_encrypt_block
 * 
 * Encrypts a block of data byte-per-byte using the key
 * 
 * @param message_ptr 
 * @param key 
 * @param key_len 
 */
void steg_encrypt_block(char *message_ptr, const char* key, size_t key_len)
{
    size_t i;
    for (i = 0; i < key_len; i++)
    {
        // Make sure the current character doesn't match one in the key
        message_ptr[i] = message_ptr[i] ^ key[i];
        if (message_ptr[i] == '\0') {
            message_ptr[i] = message_ptr[i] ^ key[i];
        }
    }
}


/**
 * @brief steg_decrypt_block
 * 
 * Decrypts a block of data byte-per-byte using the key
 * 
 * @param message_ptr 
 * @param key 
 * @param key_len 
 */
void steg_decrypt_block(char *message_ptr, const char * key, size_t key_len)
{
    size_t i;
    for (i = 0; i < key_len && (message_ptr[i] != 0) ; i++)
    {
        // Make sure the current character doesn't match one in the key
        message_ptr[i] = message_ptr[i] ^ key[i];
        if (message_ptr[i] == '\0') {
            message_ptr[i] = message_ptr[i] ^ key[i];
        }
    }
}



