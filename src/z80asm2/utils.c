//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#include "utils.h"
#include "uthash.h"
#include <ctype.h>
#include <string.h>

// string pool
typedef struct spool_t {
    char* str;
    UT_hash_handle hh;
} spool_t;

static spool_t* g_spool = NULL;


// memory allocation
static void* check_mem(void* p) {
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void* xmalloc(size_t size) {
    return check_mem(malloc(size));
}

void* xcalloc(size_t count, size_t size) {
    return check_mem(calloc(count, size));
}

void* xrealloc(void* ptr, size_t size) {
    return check_mem(realloc(ptr, size));
}

char* xstrdup(const char* src) {
    if (src)
        return check_mem(strdup(src));
    else
        return NULL;
}

char* xstrndup(const char* src, size_t len) {
    if (src) {
        char* dst = xmalloc(len + 1);
        strncpy(dst, src, len);
        dst[len] = '\0';
        return dst;
    }
    else
        return NULL;
}


// string pool
static void spool_dtor(void) {
    spool_t* elem, *tmp;
    HASH_ITER(hh, g_spool, elem, tmp) {
        HASH_DEL(g_spool, elem);
        free(elem->str);
        free(elem);
    }
}

const char* spool_add(const char* str) {
    if (str == NULL) return NULL;		// special case

    spool_t* found;
    HASH_FIND_STR(g_spool, str, found);
    if (found) return found->str;		// found

    if (g_spool == NULL)				// free memory on exit
        atexit(spool_dtor);

    found = xcalloc(1, sizeof(spool_t));
    found->str = xstrdup(str);
    HASH_ADD_STR(g_spool, str, found);
    return found->str;
}

const char* spool_addn(const char* str, size_t len) {
    char* copy = xstrndup(str, len);
    const char* ret = spool_add(copy);
    free(copy);
    return ret;
}

// strings
char* strtoupper(char* str) {
    for (char* p = str; *p; p++)
        *p = toupper(*p);
    return str;
}
