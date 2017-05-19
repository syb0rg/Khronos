#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"

int test_getTmpDir(void)
{
    const char* tmp_dir = getTmpDir();
    assert(tmp_dir);
    return 0;
}

int test_createFileInfo(void)
{
    FileInfo info = createFileInfo("");
    assert(info.fd >= 0);
    assert(info.filename);
    assert(unlink(info.filename) == 0);

    info = createFileInfo(NULL);
    assert(info.fd >= 0);
    assert(info.filename);
    assert(unlink(info.filename) == 0);

    char *str = malloc(FILENAME_MAX + 1);
    memset(str, 'a', FILENAME_MAX);
    str[FILENAME_MAX] = '\0';

    info = createFileInfo(str);
    assert(info.fd < 0);
    assert(!info.filename);
    free(str);

    return 0;
}

int main(void)
{
    int err = 0;

    if ((err = test_getTmpDir()))
        return err;
    if ((err = test_createFileInfo()))
        return err;

}
