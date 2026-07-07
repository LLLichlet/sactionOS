/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * object -- Kernel object system: base header, handle table, and type enums.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#ifndef SACTION_OBJECT_H
#define SACTION_OBJECT_H

#include "types.h"

enum objtype {
    OBJ_NONE = 0,
    OBJ_PROCESS,
    OBJ_THREAD,
    OBJ_FILE,
    OBJ_EVENT,
    OBJ_MUTEX,
    OBJ_SEMAPHORE,
    OBJ_CHANNEL,
    OBJ_SECTION,
    OBJ_TIMER,
    OBJ_IO_COMPLETION,
};

struct object_header {
    struct object_ops* ops;
    enum objtype type;
    uint refcnt;
    uint flags;
    wchar_t* name;
};

#define OBJ_FLAG_NAMED 0x0001

struct object_ops {
    void (*close)(struct object_header* obj);
    int (*read)(struct object_header* obj, void* buf, uint size, uint* bytes_read);
    int (*write)(struct object_header* obj, const void* buf, uint size, uint* bytes_written);
    int (*signal)(struct object_header* obj, uint arg);
    int (*query)(struct object_header* obj, uint info_class, void* buf, uint size);
    int (*set)(struct object_header* obj, uint info_class, const void* buf, uint size);
};

struct handle_entry {
    struct object_header* obj;
    uint access;
    uint flags;
};

#define HANDLE_FLAG_INHERIT 0x0001
#define HANDLE_FLAG_PROTECT 0x0002

#define MAX_HANDLES 64

void obj_acquire(struct object_header* obj);
void obj_release(struct object_header* obj);
void* handle_alloc(struct handle_entry* table, struct object_header* obj, uint access, uint flags);
void handle_free(struct handle_entry* table, void* h);
struct object_header* handle_lookup(struct handle_entry* table, void* h);

#endif
