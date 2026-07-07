/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * init -- First user-space process: opens CON:, writes a banner, then idles.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "saction_api.h"

void main(void)
{
    HANDLE stdout;
    const char* msg = "Hello from LibSaction!\n";
    uint written;

    static const wchar_t con_path[] = {'C', 'O', 'N', ':', 0};

    stdout = CreateFile(con_path, GENERIC_WRITE, 0, OPEN_EXISTING, 0);
    if (stdout != INVALID_HANDLE_VALUE) {
        WriteFile(stdout, msg, 24, &written);
        CloseHandle(stdout);
    }

    for (;;)
        ;
}
