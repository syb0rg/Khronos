#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif

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
	/*
    // Creates temporary file safely
    char flacFile[FILENAME_MAX] = "";
    snprintf(flacFile, FILENAME_MAX, "%sXXXXXX.wav", fileRoot);
    
    // the 5 is for the length of the suffix ".wav"
    return mkstemps(flacFile, 4);
	*/
	FileInfo file = 
	{
		.fd = mkstemp(fileRoot),
		.filename = fileRoot
	};
}
