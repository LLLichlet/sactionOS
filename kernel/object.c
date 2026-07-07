/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * object -- Kernel object lifecycle and handle table operations.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "types.h"
#include "defs.h"
#include "object.h"

void obj_acquire(struct object_header* obj)
{
    obj->refcnt++;
}

void obj_release(struct object_header* obj)
{
    if (--obj->refcnt == 0) {
        if (obj->ops && obj->ops->close)
            obj->ops->close(obj);
        kfree((char*) obj);
    }
}

void* handle_alloc(struct handle_entry* table, struct object_header* obj, uint access, uint flags)
{
    int i;

    for (i = 0; i < MAX_HANDLES; i++) {
        if (table[i].obj == NULL) {
            table[i].obj = obj;
            table[i].access = access;
            table[i].flags = flags;
            obj_acquire(obj);
            return (void*) (uintptr_t) (i + 1);
        }
    }
    return NULL;
}

void handle_free(struct handle_entry* table, void* h)
{
    uint idx;

    if (h == NULL)
        return;
    idx = (uint) (uintptr_t) h - 1;
    if (idx >= MAX_HANDLES || table[idx].obj == NULL)
        return;
    obj_release(table[idx].obj);
    table[idx].obj = NULL;
    table[idx].access = 0;
    table[idx].flags = 0;
}

struct object_header* handle_lookup(struct handle_entry* table, void* h)
{
    uint idx;

    if (h == NULL)
        return NULL;
    idx = (uint) (uintptr_t) h - 1;
    if (idx >= MAX_HANDLES)
        return NULL;
    return table[idx].obj;
}
