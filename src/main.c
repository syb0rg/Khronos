/**
 * @file main.c
 * @brief Khronos client
 * @version 0.4
 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <pocketsphinx/pocketsphinx.h>
#include <sphinxbase/err.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <tritium/tritium.h>
#include <unistd.h>

#include "audio.h"
#include "internet.h"
#include "command.h"
#include "color.h"
#include "parcel.h"

// TODO: make more portable
#define MODELDIR "/usr/local/share/pocketsphinx/model"

cst_voice *register_cmu_us_rms(const char *str);

const char* getTmpDir(void)
{
    char *tmpdir = NULL;
    if ((tmpdir = getenv("TEMP"))) return tmpdir;
    else if ((tmpdir = getenv("TMP"))) return tmpdir;
    else if ((tmpdir = getenv("TMPDIR"))) return tmpdir;
    else return "/tmp/";
}

static void say(const char *str)
{
    cst_voice* v = register_cmu_us_rms(NULL);
    tritium_textToSpeech(str, v, "play");
}

int createSafeFileDescriptor(const char* fileRoot)
{
    // Creates temporary file safely
    char flacFile[FILENAME_MAX] = "";
    snprintf(flacFile, FILENAME_MAX, "%sXXXXXX.wav", fileRoot);
    
    // the 5 is for the length of the suffix ".wav"
    return mkstemps(flacFile, 4);
}

const char* getPathFromDescriptor(int fd)
{
    char* filePath = malloc(PATH_MAX);
    if (fcntl(fd, F_GETPATH, filePath) != -1)
    {
        return filePath;
    }
    return NULL;
}

static const char* recognizeFromFile(ps_decoder_t *ps, const char* fileName)
{
    int16 adbuf[2048];
    FILE *file = NULL;
    const char *hyp = NULL;
    size_t k = 0;
    
    if ((file = fopen(fileName, "rb")) == NULL)
    {
        fprintf(stderr, "Failed to open file '%s' for reading", fileName);
    }
    
    // verify .wav file?  I trust libsndfile to make a valid one
    ps_start_utt(ps);
    bool uttStarted = false;
    
    while ((k = fread(adbuf, sizeof(int16), 2048, file)) > 0)
    {
        ps_process_raw(ps, adbuf, k, false, false);
        bool inSpeech = ps_get_in_speech(ps);
        if (inSpeech && !uttStarted) uttStarted = true;
        if (!inSpeech && uttStarted)
        {
            ps_end_utt(ps);
            hyp = ps_get_hyp(ps, NULL);
            ps_start_utt(ps);
            uttStarted = false;
        }
    }
    ps_end_utt(ps);
    
    if (uttStarted)
    {
        hyp = ps_get_hyp(ps, NULL);
    }
    
    fclose(file);
    return hyp;
}


int runKhronos(PaStream *stream, AudioData *data, AudioSnippet *sampleBlock, ps_decoder_t *ps)
{
    int err = 0;
    bool sampleComplete = false;
    int fd = createSafeFileDescriptor(getTmpDir());
    const char *fileName = getPathFromDescriptor(fd);
    if ((err = processStream(stream, data, sampleBlock, fd, &sampleComplete)))
    {
        fprintf(stderr, "Error recording FLAC file: %d\n", err);
        return err;
    }
    else if (sampleComplete)
    {
        char *flacFileBuf = NULL;
        size_t flacFileLen = 0;

        if ((err = getFileContents(fileName, (void**)&flacFileBuf, &flacFileLen)))
        {
            fprintf(stdout, "Error reading FLAC file: %s\n", strerror(errno));
            return err;
        }
        
        const char *text = recognizeFromFile(ps, fileName);
        
        if (text) fprintf(stdout, "Recognized text: %s\n", text);
        else puts(RED_TEXT("No text recognized."));
        if (text)
        {
            bool said = false;
            for (size_t i = 0; i < ARRAY_SIZE(commands); ++i)
            {
                const Command *p = commands + i;
                if (strstr(text, p->cmd))
                {
                    const char* out = p->fn();
                    fprintf(stdout, "%s\n", out);
                    say(out);
                    said = true;
                }
            }
            // there was some text, but a response was unknown
            if (!said) say("I could not understand what you said.");
        }
        sampleComplete = false;
    }
    close(fd);
    unlink(fileName);
    free((void*) fileName);
    return err;
}

int main(int argc, char *argv[])
{
    srand ((unsigned) time(NULL));
    
    // turn off pocketsphinx output
    err_set_logfp(NULL);
    err_set_debug_level(0);
    
    // handle command line arguments
    while (argc--)
    {
        if (streq("--help", argv[argc]) || streq("-h", argv[argc]) || streq("help", argv[argc]))
        {
            puts(printHelp());
            return 0;
        }
        if (streq("--version", argv[argc]) || streq("-v", argv[argc]) || streq("version", argv[argc]))
        {
            puts(printVersionInfo());
            return 0;
        }
        if (streq("--say", argv[argc]) || streq("-s", argv[argc]) || streq("say", argv[argc]))
        {
            say(argv[argc + 1]);
            return 0;
        }
    }
    
    // initialize pocketsphinx stuff
    cmd_ln_t *config = cmd_ln_init(NULL, ps_args(), TRUE,
                                   "-hmm", MODELDIR "/en-us/en-us",
                                   "-lm", MODELDIR "/en-us/en-us.lm.bin",
                                   "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                                   NULL);
    if (!config)
    {
        fprintf(stderr, "Failed to create config object, see log for details\n");
        return -1;
    }
    ps_decoder_t *ps = ps_init(config);
    if (!ps)
    {
        fprintf(stderr, "Failed to create recognizer, see log for details\n");
        return -1;
    }
    
    // initialize stuff for PortAudio
    AudioData *data = allocAudioData();
    AudioSnippet *sampleBlock = &((AudioSnippet)
                                  {
                                      .size = 0,
                                      .snippet = NULL
                                  });
    
    PaStream *stream = NULL;
    PaError err = init(&stream, data, sampleBlock);
    
    while (!err)
    {
        err = runKhronos(stream, data, sampleBlock, ps);
    }
    
    //cleanup
    freeAudioData(&data);
    free(sampleBlock->snippet);
    free(stream);
    Pa_Terminate();
    puts(Pa_GetErrorText(err));
    return err;
}