/**
 * @file audio.h
 * @brief Contains AudioData structure, and relevent audio functions.
 */
#ifndef AUDIO_H
#define AUDIO_H

#include <portaudio.h>
#include <stdbool.h>

/**
 * Holds all of the necessary information for building an
 * audio file.
 * @var duration Contains how long the audio file will be in seconds.
 * @var formatType Contains the audio formatting type.
 * @var numberOfChannels Contains the number of audio channels.
 * @var sampleRate Contains the sample rate in Hertz.
 * @var frameIndex Contains the current frame to be processed.
 * @var maxFrameIndex Contains the number of frames that the audio file will store.
 * @var recordedSamples Contains the raw PCM audio data.
 */
typedef struct
{
    uint16_t formatType;
    uint8_t numChannels;
    uint32_t sampleRate;
    size_t size;
    float *recordedSamples;
} __attribute__((__packed__)) AudioData;

typedef struct
{
    size_t size;
    float *snippet;
} __attribute__((__packed__)) AudioSnippet;

long storeFLAC(AudioData *data, int fd);
AudioData* allocAudioData();
void freeAudioData(AudioData **data);
int init(PaStream **stream, AudioData *data, AudioSnippet *sampleBlock);
int processStream(PaStream *stream, AudioData *data, AudioSnippet *sampleBlock, int fd, bool *flag);

#endif // AUDIO_H