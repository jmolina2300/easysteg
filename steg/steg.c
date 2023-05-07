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



int __steg_encode_bmp_old(const char *in_file, const char *out_file, char *key, char *message, size_t message_size)
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


    // Get the encrypted version of the message
    char *encrypted_message = get_encrypted_message(message, key);
    if (encrypted_message == NULL) 
    {
        bmp_destroy(&bmp);
        return 0;
    }
    size_t encrypted_message_len = strlen(encrypted_message);

    // write the message to the bmp data array
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

void copy_remaining_bmp_data(BmpImage *in_image, BmpImage *out_image)
{
    const int ZERO_BLOCK_SIZE = 1024;
    char zero_buffer[ZERO_BLOCK_SIZE];
    char bmp_buffer[ZERO_BLOCK_SIZE * 8];
    size_t bytes_read = bmp_read(in_image, bmp_buffer, 1, sizeof(bmp_buffer));
    memset(zero_buffer, 0, ZERO_BLOCK_SIZE);
    while (bytes_read > 0)
    {
        encode_one_block(bmp_buffer, zero_buffer, sizeof(zero_buffer));
        bmp_write(out_image, bmp_buffer, 1, bytes_read);
        bytes_read = bmp_read(in_image, bmp_buffer, 1, sizeof(bmp_buffer));
    }
}


// Encode a message into a BMP file
int steg_encode_bmp(const char *in_file, const char *out_file, char *key, char *msg, size_t msg_len)
{   
    BmpImage in_image;
    BmpImage out_image;

    const int BMP_CHUNK_SIZE = KEY_LENGTH * 8;

    char msg_buffer[KEY_LENGTH + 1];
    char bmp_buffer[BMP_CHUNK_SIZE + 1];

    if (!bmp_open_read(&in_image, in_file)) {
        return 0;
    }
    bmp_copy_header(&out_image, &in_image);
    if (!bmp_open_write(&out_image, out_file)) {
        bmp_close(&in_image);
        return 0;
    }

    // Check if the message is too long to fit in the image
    if (!message_fits(msg_len, in_image.info.datasize))
    {
        bmp_close(&in_image);
        bmp_close(&out_image);
        return 0;
    }
    // The entire message can definitely fit, with possible padding at the end

    // Compute the number of blocks we need to write
    size_t num_blocks_in_msg;
    if ((msg_len % KEY_LENGTH) != 0) {
        num_blocks_in_msg = (msg_len / KEY_LENGTH) + 1;
    } else {
        num_blocks_in_msg = (msg_len / KEY_LENGTH);
    }
    size_t i;
    for (i = 0; i < num_blocks_in_msg; i++)
    {
        char *current_msg_block = msg + (i * KEY_LENGTH);
        copy_one_block_of_message(msg_buffer, current_msg_block);

        // Read in a chunk of pixel data to encode into
        bmp_read(&in_image, bmp_buffer, 1, BMP_CHUNK_SIZE);

        // Encrypt the message block
        steg_encrypt_block(msg_buffer,key, KEY_LENGTH);

        // Encode the message block into the bmp buffer
        encode_one_block(bmp_buffer, msg_buffer, KEY_LENGTH);

        // Write the chunk of pixel data to the output file
        bmp_write(&out_image, bmp_buffer, 1, BMP_CHUNK_SIZE);
    }

    copy_remaining_bmp_data(&in_image, &out_image);

    bmp_close(&in_image);
    bmp_close(&out_image);
    return 1;
}


int contains_null(const char *block)
{
    size_t i = 0;
    while (block[i] != '\0' && i < KEY_LENGTH)
        i++;
    return (i != KEY_LENGTH);
}



// Decode a message from the bmp file and store it in the message buffer
int steg_decode_bmp(const char *in_file, const char *key, char *buffer, size_t buffer_size)
{
    BmpImage in_image;

    const int BMP_CHUNK_SIZE = KEY_LENGTH * 8;

    char msg_buffer[KEY_LENGTH + 1];
    char bmp_buffer[BMP_CHUNK_SIZE + 1];

    if (!bmp_open_read(&in_image, in_file)) {
        return 0;
    }

    // Compute number of possible bytes to read
    size_t num_bytes_to_read;
    size_t max_msg_len = compute_available_space(in_image.info.datasize);
    if (buffer_size > max_msg_len) {
        num_bytes_to_read = max_msg_len;
    } else {
        num_bytes_to_read = buffer_size;
    }


    char *buffer_ptr = buffer;  // Our sliding window into the output buffer
    int reached_end = 0;
    size_t num_blocks = num_bytes_to_read / KEY_LENGTH;
    size_t block_num = 0;
    while (!reached_end) {

        // Read in a chunk of pixel data to decode
        bmp_read(&in_image, bmp_buffer, 1, BMP_CHUNK_SIZE);

        // Decode the chunk of pixel data into the message buffer
        decode_one_block(msg_buffer, bmp_buffer, KEY_LENGTH);
        steg_decrypt_block(msg_buffer,key,KEY_LENGTH);


        // write the decoded piece of message to the output buffer
        copy_one_block_of_message(buffer_ptr, msg_buffer);
        buffer_ptr += KEY_LENGTH;

        reached_end = (block_num >= num_blocks) || (contains_null(msg_buffer));
        block_num++;
    }


    bmp_close(&in_image);
    return 1;
}

void encode_one_block(char *dest, const char *src, size_t src_len) 
{
    // shift data into the destination
    size_t i;
    for (i = 0; i < src_len ; i++) {
        uint8_t byte_out = (uint8_t)src[i];
        shift_out_byte_to_array((uint8_t*)dest, byte_out, i * 8);
    }
}

void decode_one_block(char *dest, const char *src, size_t src_len)
{
    // shift data out of the source
    size_t i;
    for (i = 0; i < src_len ; i++) {
        uint8_t byte_in = 0;
        shift_in_byte_from_array(&byte_in, (uint8_t*)src, i * 8);
        dest[i] = byte_in;
    }
}

void copy_one_block_of_message(char *buffer, const char *src) {
    size_t i;
    size_t bytes_to_copy = strlen(src);
    if (bytes_to_copy > KEY_LENGTH) {
        bytes_to_copy = KEY_LENGTH;
    }
    for (i = 0; i < bytes_to_copy; i++) {
        if (src[i] == '\0') {
            break;
        }
        buffer[i] = src[i];
    }

    // add the padding bytes if needed (ie, bytes_to_copy < KEY_LENGTH)
    while (i < KEY_LENGTH) {
        buffer[i] = PADDING_CHARACTER;
        i++;
    }
}


int __steg_decode_bmp_old(const char *in_file, const char *key, char *buffer, size_t buffer_size)
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


    // Run through the file data and fish out the message
    size_t i;
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
 * @param buffer
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
 * Pads the message to make it a multiple of KEY_LENGTH
 * 
 * @param msg
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



