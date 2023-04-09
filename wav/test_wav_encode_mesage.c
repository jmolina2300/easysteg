#include <stdio.h>
#include <stdlib.h>
#include "wavread.h"



void print_wav_info(WaveFile *wf)
{
    printf("  Duration: %f\n", wf->info.duration);
    printf("  BitsPerSample: %d\n", wf->info.bitsPerSample);
    printf("  NumChannels: %d\n", wf->info.numChannels);
    printf("  BlockAlign: %d\n", wf->info.blockAlign);
    printf("  NumSamples: %d\n", wf->info.numSamples);
    printf("  SampleRate: %d\n", wf->info.sampleRate);
    printf("  DataChunkSize: %d\n", wf->info.dataChunkSize);
    printf("\n");
}

int main(int argc, char **argv)
{
    WaveFile wf;
    char *wavFileName = NULL;

    // Check arguments
    if (argc < 2) {
        printf("Usage:  ./test_wav_encode_message [file name]\n\n");
        return 1;
    }
    wavFileName = argv[1];
    

    // Read the WAV file
    int acceptableFile = wav_read_from_file(&wf, wavFileName);
    if (acceptableFile == 0) {
        printf("Error reading file %s\n", wavFileName);
        return 0;
    }
    print_wav_info(&wf);


    // Get pointer to the sample data
    int16_t *samples = (int16_t *)wf.data;

    // Get secret message from user
    char secret[1000];
    printf("Enter the secret message: ");
    fgets(secret, 1000, stdin);
    int len = strlen(secret);

    // See if the WAV file is big enough to hold the secret
    int cover_size = wf.info.dataChunkSize;
    if (cover_size < len * 8) {
        printf("Cover file is not large enough.\n");
        return 1;
    }

    // Shift out the secret message (somehow)
    



    // Write the WAV file
    char outFileName[260];
    snprintf(outFileName, 260, "%s-2.wav", argv[1]);
    if (!wav_write_to_file(&wf, outFileName) ) {
        printf("Error writing WAV file\n");
        return 1;
    }
    wav_destroy(&wf);

}
