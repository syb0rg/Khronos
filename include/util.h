#ifndef UTIL_H
#define UTIL_H

/**
 * @return the temporary file directory for your system
 */
const char* getTmpDir(void);

/**
 * Generates a unique temporary filename given the fileroot, creates and opens the file
 * @return file descriptor to the open file
 */
int createSafeFileDescriptor(const char* fileRoot);

/**
 * Fetches the file path given the file descriptor
 * @return file path
 */
const char* getPathFromDescriptor(int fd);

#endif // UTIL_H