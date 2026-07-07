/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * obj_console -- Console device object (device name "CON:").
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "types.h"
#include "defs.h"
#include "object.h"

struct obj_console {
    struct object_header hdr;
};

static void console_close(struct object_header* obj)
{
    kfree((char*) obj);
}

static int console_write(struct object_header* obj, const void* buf, uint size, uint* written)
{
    const char* p = (const char*) buf;
    uint i;

    for (i = 0; i < size; i++)
        uartputc(p[i]);
    if (written)
        *written = size;
    return 0;
}

static int console_read(struct object_header* obj, void* buf, uint size, uint* bytes_read)
{
    return -1; /* not implemented */
}

static struct object_ops console_ops = {
    .close = console_close,
    .read = console_read,
    .write = console_write,
};

struct object_header* obj_console_create(void)
{
    struct obj_console* con;

    con = (struct obj_console*) kalloc();
    if (!con)
        return NULL;
    memset(con, 0, sizeof(*con));
    con->hdr.ops = &console_ops;
    con->hdr.type = OBJ_FILE;
    con->hdr.refcnt = 1;
    return &con->hdr;
}
