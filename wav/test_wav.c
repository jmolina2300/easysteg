/**
 * Test reading in and writing out a WAV file without modifications.
 * 
 * The contents of both WAV files should be the same (diff returns 0).
 * 
 */
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
    char newFileName[260];
    char *fileName = NULL;

    // Check arguments
    if (argc < 2) {
        printf("Usage:  ./test_wav [file name]\n\n");
        return 1;
    }
    fileName = argv[1];
    snprintf(newFileName, 260, "%s-2.wav", fileName);


    // 1. Read in the WAV file
    printf("Opening file %s...\n", fileName);
    int acceptableFile = wav_read_from_file(&wf, fileName);
    if (acceptableFile == 0) {
        printf("Error reading file %s\n", fileName);
        return 0;
    }
    print_wav_info(&wf);

    // 2. Write the data to a new file
    printf("Writing file %s-2.wav...\n\n", argv[1]);
    if (!wav_write_to_file(&wf, newFileName) ) {
        printf("Error writing WAV file\n");
        return 1;
    }

    wav_destroy(&wf);



    // 3. Read in the NEW file and display info
    printf("Opening file %s...\n", newFileName);
    acceptableFile = wav_read_from_file(&wf, newFileName);
    if (acceptableFile == 0) {
        printf("Error reading file %s\n", newFileName);
        return 0;
    }
    print_wav_info(&wf);

    wav_destroy(&wf);
}