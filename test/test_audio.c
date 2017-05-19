#include <assert.h>
#include <stdio.h>
#include "audio.h"

int test_storeWAV(void)
{
    long val = storeWAV(NULL, -1);
    assert(val == -1);
    val = storeWAV(NULL, 0);
    assert(val == -1);
    val = storeWAV(&(AudioData){0}, -1);
    assert(val == -1);
    val = storeWAV(&(AudioData){0}, 0);
    assert(val == -1);

    return 0;
}

int test_allocAudioData(void)
{
    AudioData* data = allocAudioData();
    assert(data->formatType == paFloat32);
    assert(data->numChannels == 1);
    assert(data->sampleRate == 16000);
    assert(data->size == 0);
    assert(!data->recordedSamples);

    return 0;
}

int test_freeAudioData(void)
{
    AudioData* data = allocAudioData();
    freeAudioData(&data);
    assert(!data);

    return 0;
}

int test_init(void)
{
    int err = init(NULL, NULL, NULL);
    assert(err == -1);
    err = init(&(PaStream*){0}, &(AudioData){0}, &(AudioSnippet){0});
    assert(err == paInvalidChannelCount);

    return 0;
}

int test_processStream(void)
{
    int err = processStream(NULL, NULL, NULL, -1, NULL);
    assert(err == -1);

    return 0;
}

int main(void)
{
    int err = 0;
    if ((err = test_storeWAV()))
        return err;
    if ((err = test_allocAudioData()))
        return err;
    if ((err = test_freeAudioData()))
        return err;
    if ((err = test_init()))
        return err;
    if ((err = test_processStream()))
        return err;
}
