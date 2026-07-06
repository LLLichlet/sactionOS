/* Compatibility shims for BSD functions not available on MinGW. */
#ifndef MKFS_COMPAT_H
#define MKFS_COMPAT_H

#include <string.h>

#define index(s, c)  strchr((s), (c))
#define bzero(p, n)  memset((p), 0, (n))

static inline void bcopy(const void *src, void *dst, unsigned int n) {
    memmove(dst, src, n);
}

#endif
