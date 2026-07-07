/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * file -- File I/O API: CreateFile, ReadFile, WriteFile, CloseHandle.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "saction_api.h"
#include "object.h"

extern void* ScCreateObject(uint type, const char* name, uint access);
extern int ScWrite(void* h, const void* buf, uint size);
extern int ScClose(void* h);

HANDLE CreateFile(const wchar_t* path, uint access, uint share, uint create_disp, uint flags)
{
    char npath[128];
    int i;

    (void) share;
    (void) create_disp;
    (void) flags;

    for (i = 0; path[i] && i < 127; i++)
        npath[i] = (char) path[i];
    npath[i] = 0;

    return (HANDLE) ScCreateObject(OBJ_FILE, npath, access);
}

int WriteFile(HANDLE h, const void* buf, uint size, uint* bytes_written)
{
    int ret;

    ret = ScWrite((void*) h, buf, size);
    if (bytes_written)
        *bytes_written = (ret == 0) ? size : 0;
    return ret;
}

int CloseHandle(HANDLE h)
{
    return ScClose((void*) h);
}
