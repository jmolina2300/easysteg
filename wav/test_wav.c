#include <stdio.h>
#include <stdlib.h>
#include "wavread.h"

int main(int argc, char **argv)
{
    WaveInfo wi;
    FILE *infile = fopen(argv[1], "rb");
    if (infile == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 0;
    }

    int acceptableFile = read_pcm_wav_info(infile, argv[1] , &wi);    
    if (!acceptableFile) {
        printf("Error reading file %s\n", argv[1]);
        return 0;
    }

    // Print out the WAV info
    printf("Duration: %f\n", wi.Duration);
    printf("BitsPerSample: %d\n", wi.BitsPerSample);
    printf("NumChannels: %d\n", wi.NumChannels);
    printf("BlockAlign: %d\n", wi.BlockAlign);
    printf("NumSamples: %d\n", wi.NumSamples);
    printf("SampleRate: %d\n", wi.SampleRate);
    printf("DataChunkSize: %u\n", wi.DataChunkSize);



    fclose(infile);
    
}