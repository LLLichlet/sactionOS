typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef uint pde_t;
/* Standard-like types:
   Native host tools get these from the system libc. */
#if !defined(__STDC_HOSTED__) || __STDC_HOSTED__ == 0
typedef unsigned short wchar_t;
typedef unsigned int size_t;
typedef unsigned int uintptr_t;
#define NULL ((void*) 0)
#endif
