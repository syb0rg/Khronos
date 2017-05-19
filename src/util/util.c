#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#endif //_WIN32

#include "util.h"

const char* getTmpDir(void)
{
    char *tmpdir = NULL;
    if ((tmpdir = getenv("TEMP")))
        return tmpdir;
    else if ((tmpdir = getenv("TMP")))
        return tmpdir;
    else if ((tmpdir = getenv("TMPDIR")))
        return tmpdir;
    else
        return "/tmp/";
}

FileInfo createFileInfo(const char* fileRoot)
{
    if (!fileRoot)
        fileRoot = "";
    // add 8 for random letters at end for mkstemp suffix and null char
    if (strlen(fileRoot) + 8 > FILENAME_MAX)
        return (FileInfo) {-1, NULL};

    char tempFile[FILENAME_MAX] = "";
    snprintf(tempFile, FILENAME_MAX, "%sXXXXXX", fileRoot);

	FileInfo file =
	{
		.fd = mkstemp(tempFile),
		.filename = strdup(tempFile)
	};

    return file;
}
