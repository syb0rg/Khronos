/**
 * @file main.c
 * @brief Khronos client
 * @version 0.2
 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
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
    snprintf(flacFile, FILENAME_MAX, "%sXXXXXX.flac", fileRoot);
    
    // the 5 is for the length of the suffix ".flac"
    return mkstemps(flacFile, 5);
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

int runKhronos(PaStream *stream, AudioData *data, AudioSnippet *sampleBlock)
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
        double confidence = 0.0;

        if ((err = getFileContents(fileName, (void**)&flacFileBuf, &flacFileLen)))
        {
            fprintf(stdout, "Error reading FLAC file: %s\n", strerror(errno));
            return err;
        }
        
        ServerResponse *resp = sendAudioData(flacFileBuf, (int)flacFileLen, "en-US", data->sampleRate);
        if (!resp)
        {
            fprintf(stderr, "Error sending audio.");
            // TODO specific warning number
            return -1;
        }
        puts(resp->data);
        const char *text = parcel_getItemFromJSON(resp->data, "transcript");
        if (text)
        {
            const char* temp = parcel_getItemFromJSON(resp->data, "confidence");
            if (temp) confidence = strtod(temp, NULL) * 100;
            else confidence = 100;
        }
        freeResponse(resp);
        
        fprintf(stdout, "Recognized text: %s\n", text ?: RED_TEXT("No text recognized."));
        fprintf(stdout, "Confidence: %g%%\n", confidence);
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
            if (!said) say("I could not understand what you said."); // there was some text, but a response was unknown
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
    
    // this is a one time test, the software assumes that once it has a steady connection it is
    // reliable for an indefinite amount of time
    if (testConnection())
    {
        fprintf(stderr, "Speech recognition offline.\n");
        say("Speech recognition offline.");
        return -1;
    }
    
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
        err = runKhronos(stream, data, sampleBlock);
    }
    
    freeAudioData(&data);
    free(sampleBlock->snippet);
    free(stream);
    Pa_Terminate();
    puts(Pa_GetErrorText(err));
    return err;
}