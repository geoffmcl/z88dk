//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// memory allocation
void* xmalloc(size_t size);
void* xcalloc(size_t count, size_t size);
void* xrealloc(void* ptr, size_t size);
char* xstrdup(const char* src);
char* xstrndup(const char* src, size_t len);

// string pool
const char* spool_add(const char* str);
const char* spool_addn(const char* str, size_t len);

// strings
#define STREMPTY(s)		((s) == NULL || *(s) == '\0')

char* strtoupper(char* str);
