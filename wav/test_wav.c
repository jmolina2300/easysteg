#include <stdio.h>
#include <stdlib.h>
#include "wavread.h"

int main(int argc, char **argv)
{
    WaveFile wf;

    // Check arguments
    if (argc < 2) {
        printf("Usage:  ./test_wav [file name]\n\n");
        return 1;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 0;
    }

    int acceptableFile = wav_read_from_file(&wf, argv[1]);
    if (acceptableFile == 0) {
        printf("Error reading file %s\n", argv[1]);
        return 0;
    }

    printf("Duration: %f\n", wf.Info.Duration);
    printf("BitsPerSample: %d\n", wf.Info.BitsPerSample);
    printf("NumChannels: %d\n", wf.Info.NumChannels);
    printf("BlockAlign: %d\n", wf.Info.BlockAlign);
    printf("NumSamples: %d\n", wf.Info.NumSamples);
    printf("SampleRate: %d\n", wf.Info.SampleRate);
    printf("DataChunkSize: %d\n", wf.Info.DataChunkSize);


    // Mess up the samples
    int16_t *samples = (int16_t *)wf.Data;
    size_t i;
    for (i = 0; i < wf.Info.NumSamples; i++) {
        samples[i] = samples[i] ^ 0xAAAA;
    }

    // Write the WAV file

    char outFileName[260];
    snprintf(outFileName, 260, "%s-2.wav", argv[1]);
    if (!wav_write_to_file(&wf, outFileName) ) {
        printf("Error writing WAV file\n");
        return 1;
    }




    fclose(infile);
    
}