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

FileInfo createFileInfo(char* fileRoot)
{
	FileInfo file =
	{
		.fd = mkstemp(fileRoot),
		.filename = fileRoot
	};
    
    return file;
}
