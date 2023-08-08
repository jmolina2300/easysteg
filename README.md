
## EasySteg - Easy Digital Steganography 

  
Easysteg is a digital [steganography](https://en.wikipedia.org/wiki/Steganography) tool that can encode secret data into both image and audio files. 
The following is a list of currently accepted image and audio formats:
**Image**  
- 24-bit BMP files  

**Audio**  
-   Audio: 16-bit PCM WAV files  
<br/>
## Usage
Click **Open** to bring up a dialog to select an image or audio file to work on with.

If the file format is acceptable, information will be displayed in the Details textbox, otherwise an error
message will be displayed.

Once the file is open, you can proceed to encode or decode secret data from the file.

#### Encoding
- To encode a secret message, go to **Actions** at the top menu and select **Encode**.
- A dialog box should appear and allow you to enter any message up to a certain length. The maximum length of the message is dependent on the size of the cover file you selected.
- Enter an 8-character key to encrypt the message.
- Click OK when done.

#### Decoding
- To decode a message, go to **Actions** at the top menu and select **Decode**.
- A dialog box will appear that asks for a key to be used when decrypting the secret message.
- Enter the secret key and click OK.
- The decoded text will be displayed in a new window. If the wrong key is used, the output will appear unintelligible.
<br/>
<br/>
## Principle Of Operation
EasySteg encodes secret text into the least significant bit (LSB) of either the pixel data or sample data, for an image or audio file, respectively.

For both encoding and decoding, the secret text is processed one block at a time.

#### Encoding
- Each block of text is XOR'd with the secret key provided by the user.
- Each character of the block is then shifted out to the LSB of 8 consecutive bytes in the cover file.
- Process is repeated until entire secret message is encoded.
- The final block is padded with spaces **if the remaining bytes don't fill an entire block.**

#### Decoding
- The secret key is provided by the user.
- The cover file is read 8 bytes at a time into an input buffer.
- Each byte in the input buffer has it's LSB extracted and left-shifted into place in the output byte.
- All the output bytes are collected in an output buffer.
- The output buffer is block-XOR'd with the secret key and appended to the message display buffer.
- Process is repeated until the last block is processed **or** the current block contains a null byte.
<br/>
<br/>
## Requirements
- Windows 95 (or newer)
- Intel 386 processor (or newer)
