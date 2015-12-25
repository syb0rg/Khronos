/**
 * @file main.c
 * @brief Khronos client
 * @version 0.2
 */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
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

int main(int argc, char *argv[])
{
    srand ((unsigned) time(NULL));
    AudioData *data = allocAudioData();
    AudioSnippet *sampleBlock = &((AudioSnippet)
                                  {
                                      .size = 0,
                                      .snippet = NULL
                                  });
    ServerResponse *resp = NULL;
    char *flacFileBuf;
    size_t flacFileLen;
    double confidence = 0.0;
    int fd = -1;
    
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
    
    if (testConnection())
    {
        fprintf(stderr, "Speech recognition offline.\n");
        say("Speech recognition offline.");
        return -1;
    }
    
    PaStream *stream = NULL;
    PaError err = init(&stream, data, sampleBlock);
    const char *fileRoot = getTmpDir();
    bool flag = false;
    
    while (!err)
    {
        // Creates temporary file safely
        char flacFile[FILENAME_MAX] = "";
        snprintf(flacFile, FILENAME_MAX, "%sXXXXXX.flac", fileRoot);
        fd = mkstemps(flacFile, 5); // the 5 is for the length of the suffix ".flac"
        
        if ((err = processStream(stream, data, sampleBlock, fd, &flag)))
        {
            fprintf(stderr, "Error recording FLAC file: %d\n", err);
            break;
        }
        else if (flag)
        {
            if ((err = getFileContents(flacFile, (void**)&flacFileBuf, &flacFileLen)))
            {
                fprintf(stdout, "Error reading FLAC file: %s\n", strerror(errno));
                break;
            }
            resp = sendAudioData(flacFileBuf, (int)flacFileLen, "en-US", data->sampleRate);
            if (!resp)
            {
                fprintf(stderr, "Error sending audio.");
                break;
            }

            const char *text = parcel_getItemFromJSON(resp->data, "transcript");
            if (text) confidence = strtod(parcel_getItemFromJSON(resp->data, "confidence") ?: "0", NULL) * 100;
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
            flag = false;
        }
        close(fd);
        unlink(flacFile);
    }
    freeAudioData(&data);
    freeResponse(resp);
    free(sampleBlock->snippet);
    free(stream);
    Pa_Terminate();
    puts(Pa_GetErrorText(err));
    return err;
}