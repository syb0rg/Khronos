#ifndef UTIL_H
#define UTIL_H

const char* getTmpDir(void);
int createSafeFileDescriptor(const char* fileRoot);
const char* getPathFromDescriptor(int fd);

#endif // UTIL_H