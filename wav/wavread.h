#ifndef wavreadH
#define wavreadH

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define WAVE_FORMAT_PCM         0x1
#define WAVE_FORMAT_EXTENSIBLE  0xfffe
#define is_format_chunk(x) !strncmp(x, "fmt ", 4)
#define is_data_chunk(x) !strncmp(x, "data", 4)

#define AUDIO_WINDOW_MS         25




typedef struct {
    char ckID [4];
    uint32_t ckSize;
    char formType [4];
} RiffChunkHeader;

typedef struct {
    char ckID [4];
    uint32_t ckSize;
} ChunkHeader;

typedef struct {
    uint16_t FormatTag, NumChannels;
    uint32_t SampleRate, BytesPerSecond;
    uint16_t BlockAlign, BitsPerSample;
    uint16_t cbSize;
    union {
        uint16_t ValidBitsPerSample;
        uint16_t SamplesPerBlock;
        uint16_t Reserved;
    } Samples;
    int32_t ChannelMask;
    uint16_t SubFormat;
    char GUID [14];
} WaveHeader;




/**
 * Structure to hold information about a wave file.
 * 
 * I don't like the idea of duplicating structures, but I'm doing it here.
 * Just for the sake of convenience and readability.
 */
typedef struct {
    double      duration;
    uint16_t    bitsPerSample;
    uint16_t    numChannels;
    uint16_t    blockAlign;
    uint32_t    numSamples;
    uint32_t    sampleRate;
    uint32_t    dataChunkSize;
}  WaveInfo;


/**
 * Complete WAV file
 */
typedef struct {
    WaveInfo     info;
    void        *data;    
} WaveFile;




int write_pcm_wav_header (
    FILE *outfile,
    uint32_t num_samples,
    int num_channels,
    int bytes_per_sample,
    uint32_t sample_rate
    );

int read_pcm_wav_info(FILE *providedFile, char *infilename, WaveInfo *);


int wav_read_from_file(WaveFile *wf, const char *infilename);

int wav_write_to_file(WaveFile *wf, const char *outfilename);


void wav_destroy(WaveFile *wf);

#endif
