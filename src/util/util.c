#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

const char* getTmpDir(void)
{
    char *tmpdir = NULL;
    if ((tmpdir = getenv("TEMP"))) return tmpdir;
    else if ((tmpdir = getenv("TMP"))) return tmpdir;
    else if ((tmpdir = getenv("TMPDIR"))) return tmpdir;
    else return "/tmp/";
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