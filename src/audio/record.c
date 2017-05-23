/**
 * @file record.c
 * @brief Continuously records FLAC audio files
 */

#include <math.h>
#include <portaudio.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "audio.h"
#include "debug.h"

const static int FRAMES_PER_BUFFER = 1024;
const static float MIN_TALKING_BUFFERS = 8;
const static float TALKING_THRESHOLD = 0.000750;

AudioData* allocAudioData()
{
    AudioData *data = malloc(sizeof(AudioData));
    data->formatType = paFloat32;
    data->numChannels = 1; // must be mono for sphinx
    data->sampleRate = 16000;  // must be 16kHz for sphinx
    data->size = 0;
    data->recordedSamples = NULL;
    return data;
}

void freeAudioData(AudioData **data)
{
    free((*data)->recordedSamples);
    (*data)->recordedSamples = NULL;
    free(*data);
    *data = NULL;
}

float rms(float *data, size_t len)
{
    double sum = 0.0;
    for(size_t i = 0; i < len; ++i)
    {
        sum += pow(data[i], 2);
    }
    return sqrt(sum / len);
}

long storeWAV(AudioData *data, int fd)
{
    if (!data || fd < 0)
        return -1;

    uint8_t err = SF_ERR_NO_ERROR;
    SF_INFO sfinfo =
    {
        .channels = data->numChannels,
        .samplerate = data->sampleRate,
        .format = SF_FORMAT_WAV | SF_FORMAT_PCM_16
    };

    SNDFILE *outfile = sf_open_fd(fd, SFM_WRITE, &sfinfo, true);
    if (!outfile)
        return -1;

    // Write the entire buffer to the file
    size_t numFrames = data->size / data->numChannels / sizeof(float);
    long written = sf_writef_float(outfile, data->recordedSamples, numFrames);
    err = numFrames - written;

    // Force write to disk and close file
    sf_write_sync(outfile);
    sf_close(outfile);
    puts("Wrote to file!!!!");
    return err;
}

int init(PaStream **stream, AudioData *data, AudioSnippet *sampleBlock)
{
    if (!stream || !data || !sampleBlock)
        return -1;

    PaError err = Pa_Initialize();
    if (err)
        return err;

    err = Pa_GetDeviceCount();
    if (err < 0) // PaErrorCode thrown
    {
        debug_print("%s\n", "Error retrieving device count");
        fprintf(stdout, "%s\n", "Error retrieving device count");
        return err;
    }
    else if (err == 0)
    {
        debug_print("%s\n", "No audio devices available");
        fprintf(stdout, "%s\n", "No audio devices available");
        return -1;
    }

    const PaDeviceInfo *info = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    if (!info)
    {
        debug_print("%s\n", "Unable to find info on default input device.");
        fprintf(stdout, "%s\n", "Unable to find info on default input device.");
        return -1;
    }

    PaStreamParameters inputParameters =
    {
        .device = Pa_GetDefaultInputDevice(),
        .channelCount = data->numChannels,
        .sampleFormat = data->formatType,
        .suggestedLatency = info->defaultHighInputLatency,
        .hostApiSpecificStreamInfo = NULL
    };
    err = Pa_OpenStream(stream, &inputParameters, NULL, data->sampleRate, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err)
        return err;

    sampleBlock->size = FRAMES_PER_BUFFER * sizeof(float) * data->numChannels;
    sampleBlock->snippet = malloc(sampleBlock->size);

    return Pa_StartStream(*stream);
}

int processStream(PaStream *stream, AudioData *data, AudioSnippet *sampleBlock, int fd, bool *sampleComplete)
{
    if (!stream || !data || !sampleBlock || fd < 0 || !sampleComplete)
        return -1;
    static int i = 0;
    static time_t talking = 0;
    static time_t silence = 0;
    PaError err = 0;
    // error value needs to be stored, but PaInputOverflow avoided
    Pa_ReadStream(stream, sampleBlock->snippet, FRAMES_PER_BUFFER);
    if (err)
        return err;
    else if(rms(sampleBlock->snippet, FRAMES_PER_BUFFER) > TALKING_THRESHOLD) // talking
    {
        printf("Listening: %d\n", i);
        i++;
        time(&talking);
        data->recordedSamples = realloc(data->recordedSamples, sampleBlock->size * i);
        data->size = sampleBlock->size * i;
        if (data->recordedSamples)
        {
            size_t skipIndex = (i - 1) * sampleBlock->size;
            char *destination = (char*)data->recordedSamples + skipIndex;
            memcpy(destination, sampleBlock->snippet, sampleBlock->size);
        }
        else
        {
            free(data->recordedSamples);
            data->recordedSamples = NULL;
            data->size = 0;
        }
    }
    else //silence
    {
        double test = difftime(time(&silence), talking);
        if (test >= 1.5 && test <= 10 && data->recordedSamples && i >= MIN_TALKING_BUFFERS)
        {
            if (sampleComplete)
                *sampleComplete = true;
            storeWAV(data, fd);
            talking = 0;
            free(data->recordedSamples);
            data->recordedSamples = NULL;
            data->size = 0;
            i = 0;
        }
    }
    return err;
}
