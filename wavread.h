//---------------------------------------------------------------------------

#ifndef wavreadH
#define wavreadH

#include <stdint.h>
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
    double      Duration;
    uint16_t    BitsPerSample;
    uint16_t    NumChannels;
    uint16_t    BlockAlign;
    uint32_t    NumSamples;
    uint32_t    SampleRate;
    uint32_t    DataChunkSize;
}  WaveInfo;




int write_pcm_wav_header (
    FILE *outfile,
    uint32_t num_samples,
    int num_channels,
    int bytes_per_sample,
    uint32_t sample_rate
    );

int read_pcm_wav_info(FILE *providedFile, char *infilename, WaveInfo *);





#endif
