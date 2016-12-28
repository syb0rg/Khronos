#ifndef UTIL_H
#define UTIL_H

typedef struct
{
	const int fd;
	const char* filename;
} FileInfo;

/**
 * @return the temporary file directory for your system
 */
const char* getTmpDir(void);

/**
 * Generates a unique temporary filename given the fileroot, creates and opens the file
 * @return FileInfo of the open file
 */
FileInfo createFileInfo(const char* fileRoot);

#endif // UTIL_H
