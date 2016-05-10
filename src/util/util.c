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
    char *filename = malloc(PATH_MAX);
#ifdef __APPLE__
    if (fcntl(fd, F_GETPATH, filename) != -1) return filename;
    return NULL;
#endif
#ifdef __unix__
    char proclnk[PATH_MAX] = {0};
    ssize_t r = -1;
    snprintf(proclnk, sizeof(proclnk), "/proc/self/fd/%d", fd);  // make more portable
    if ((r = readlink(proclnk, filename, PATH_MAX)) < 0) return NULL;
    filename[r] = '\0';
    return filename;
#endif
}
