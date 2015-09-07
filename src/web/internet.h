#ifndef INTERNET_H
#define INTERNET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <string.h>

typedef struct
{
	char *data;
	int length;
} __attribute__((__packed__)) ServerResponse;

void freeResponse(ServerResponse *resp);
ServerResponse *sendAudioData(void *data, int length, const char *language, uint32_t sample_rate);
int getFileContents(const char *file, void **buf, size_t *size);
int testConnection(void);
size_t httpCallback(char *ptr, size_t count, size_t blocksize, void *userdata);

#endif // INTERNET_H