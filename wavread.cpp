//---------------------------------------------------------------------------


#pragma hdrstop

#include "wavread.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

/**
 * write_pcm_wav_header
 * 
 * Writes a PCM WAV header to outfile
 * 
 * @param outfile 
 * @param num_samples 
 * @param num_channels 
 * @param bytes_per_sample 
 * @param sample_rate 
 * @return int 
 */
int write_pcm_wav_header (FILE *outfile, uint32_t num_samples, int num_channels, int bytes_per_sample, uint32_t sample_rate)
{
    RiffChunkHeader riffhdr;
    ChunkHeader datahdr, fmthdr;
    WaveHeader wavhdr;

    int wavhdrsize = 16;
    uint32_t total_data_bytes = num_samples * bytes_per_sample * num_channels;

    memset (&wavhdr, 0, sizeof (wavhdr));

    wavhdr.FormatTag = WAVE_FORMAT_PCM;
    wavhdr.NumChannels = num_channels;
    wavhdr.SampleRate = sample_rate;
    wavhdr.BytesPerSecond = sample_rate * num_channels * bytes_per_sample;
    wavhdr.BlockAlign = bytes_per_sample * num_channels;
    wavhdr.BitsPerSample = bytes_per_sample * 8;

    memcpy (riffhdr.ckID, "RIFF", sizeof (riffhdr.ckID));
    memcpy (riffhdr.formType, "WAVE", sizeof (riffhdr.formType));
    riffhdr.ckSize = sizeof (riffhdr) + wavhdrsize + sizeof (datahdr) + total_data_bytes;
    memcpy (fmthdr.ckID, "fmt ", sizeof (fmthdr.ckID));
    fmthdr.ckSize = wavhdrsize;

    memcpy (datahdr.ckID, "data", sizeof (datahdr.ckID));
    datahdr.ckSize = total_data_bytes;

    return fwrite (&riffhdr, sizeof (riffhdr), 1, outfile) &&
        fwrite (&fmthdr, sizeof (fmthdr), 1, outfile) &&
        fwrite (&wavhdr, wavhdrsize, 1, outfile) &&
        fwrite (&datahdr, sizeof (datahdr), 1, outfile);
}


/**
 * read_pcm_wav_info
 * 
 * Accepts a file name or or a file pointer and returns information about
 * the file in the WaveInfo structure.
 * 
 * - If a FILE* is provided, the file is not closed when the function returns.
 * 
 * - If a file name is provided, the file is opened and closed when the function
 *   returns.
 * 
 * Return value is 1 if the file is a valid PCM WAV file, 0 otherwise.
 * 
 * @param providedFile 
 * @param infilename 
 * @param wi 
 * @return int 
 */
int read_pcm_wav_info(FILE *providedFile, char *infilename, WaveInfo *wi)
{
    uint32_t numSamples = 0;
    RiffChunkHeader riffChunkHeader;
    WaveHeader  waveHeader = { 0 };
    ChunkHeader chunkHeader;
    FILE *infile;


    /* Did they give us a file or do we need to open it ourselves?
     */
    if (providedFile != NULL) {
        infile = providedFile;
    }
    else if (infilename != NULL) {
        infile = fopen(infilename, "rb");
    }
    else {
        return 0;
    }

    if (infile == NULL) {
        fprintf(stderr, "Error: Cannot open input file %s\n", infilename);
        return 0;
    }

    /* Check if it is a valid WAV file
     */
    if (!fread(&riffChunkHeader, sizeof(RiffChunkHeader), 1, infile) ||
        strncmp(riffChunkHeader.ckID, "RIFF", 4) ||
        strncmp(riffChunkHeader.formType, "WAVE", 4)) {
            fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
            return 0;
    }
    /*
     * loop through all elements of the RIFF wav header (until the data chuck)
     */
    int data_chunk_found = 0;
    while(!data_chunk_found) {
        if (!fread(&chunkHeader, sizeof(ChunkHeader), 1, infile)) {
            fprintf(stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
            return 0;
        }

        /* Format Chunk
         *
         * get some info out of it and make sure its a .wav file we can handle
         */
        if (is_format_chunk(chunkHeader.ckID)) {
                        int format, bits_per_sample;

            if (chunkHeader.ckSize < 16 || chunkHeader.ckSize > sizeof (WaveHeader) ||
                !fread (&waveHeader, chunkHeader.ckSize, 1, infile)) {
                    fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
                    return 0;
            }

            format = (waveHeader.FormatTag == WAVE_FORMAT_EXTENSIBLE && chunkHeader.ckSize == 40) ?
                waveHeader.SubFormat : waveHeader.FormatTag;

            bits_per_sample = (chunkHeader.ckSize == 40 && waveHeader.Samples.ValidBitsPerSample) ?
                waveHeader.Samples.ValidBitsPerSample : waveHeader.BitsPerSample;

            if (bits_per_sample != 16) {
                fprintf (stderr, "\"%s\" is not a 16-bit .WAV file!\n", infilename);
                return 0;
            }

            if (waveHeader.NumChannels < 1 || waveHeader.NumChannels > 2) {
                fprintf (stderr, "\"%s\" is not a mono or stereo .WAV file!\n", infilename);
                return 0;
            }

            if (waveHeader.BlockAlign != waveHeader.NumChannels * 2) {
                fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
                return 0;
            }

            if (format == WAVE_FORMAT_PCM) {
                if (waveHeader.SampleRate < 8000 || waveHeader.SampleRate > 48000) {
                    fprintf (stderr, "\"%s\" sample rate is %lu, must be 8000 to 48000!\n", infilename, (unsigned long) waveHeader.SampleRate);
                    return 0;
                }
            }
            else {
                fprintf (stderr, "\"%s\" is not a PCM .WAV file!\n", infilename);
                return 0;
            }
        } 
        else if ( is_data_chunk(chunkHeader.ckID) ) {
        
            /* Data Chunk
             *
             * get the size and exit the parsing loop
             */ 
            if (!waveHeader.SampleRate) {
                fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
                return 0;
            }

            if (!chunkHeader.ckSize) {
                fprintf (stderr, "this .WAV file has no audio samples, probably is corrupt!\n");
                return 0;
            }

            if (chunkHeader.ckSize % waveHeader.BlockAlign) {
                fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
                return 0;
            }

            numSamples = chunkHeader.ckSize / waveHeader.BlockAlign;

            if (!numSamples) {
                fprintf (stderr, "this .WAV file has no audio samples, probably is corrupt!\n");
                return 0;
            }

            data_chunk_found = 1;
        }
        else {
            /* Unknown chunk
             *
             * ignore everything in here
             */
            uint32_t bytes_to_eat = (chunkHeader.ckSize + 1) & ~1L;
            char dummy;

            while (bytes_to_eat--)
                if (!fread (&dummy, 1, 1, infile)) {
                    fprintf (stderr, "\"%s\" is not a valid .WAV file!\n", infilename);
                    return 0;
                }
        }
    }


    if (providedFile == NULL) {
        fclose(infile);
    }
    else {
        // We used the provided file, so leave the pointer alone
        // Caller closes the file!
    }


	if (wi != NULL) {
        wi->Duration = ((double)numSamples / waveHeader.SampleRate);
        wi->BitsPerSample = waveHeader.BitsPerSample;
        wi->NumChannels = waveHeader.NumChannels;
        wi->SampleRate = waveHeader.SampleRate;
        wi->NumSamples =  numSamples;
        wi->BlockAlign = waveHeader.BlockAlign;
    }

    return 1;
}
