//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#pragma once

#include "utstring.h"
#include <stdbool.h>
#include <stdlib.h>

char* testlib_norm_eol(char* text);
void testlib_diag(const char* fmt, ...);
void testlib_ok(bool value, const char* filename, int lineno);
void testlib_is(bool straight, int a, int b, const char* filename, int lineno);
void testlib_str_is(bool straight, const char* a, const char* b,
                    const char* filename, int lineno);
bool testlib_spew(const char* filename, const char* buffer, size_t size);
bool testlib_slurp(const char* filename, UT_string* out);
void testlib_exec(const char* test, const char* exp_out, const char* exp_err,
                  const char* filename, int lineno);

#define DIAG(...)		testlib_diag(__VA_ARGS__)
#define OK(x)			testlib_ok((x), __FILE__, __LINE__)
#define NOK(x)			OK(!(x))
#define IS(a, b)		testlib_is(true, (a), (b), __FILE__, __LINE__)
#define ISNT(a, b)		testlib_is(false, (a), (b), __FILE__, __LINE__)
#define PASS()			OK(true)
#define FAIL()			OK(false)
#define ASSERT(x)		do { OK(x); if (!(x)) return; } while (0)
#define STR_IS(a, b)	testlib_str_is(true, (a), (b), __FILE__, __LINE__)
#define STR_ISNT(a, b)	testlib_str_is(false, (a), (b), __FILE__, __LINE__)
#define SPEW(f, b, n)	OK(testlib_spew((f), (b), (n)))
#define SPEW_T(f, b)	OK(testlib_spew((f), (b), strlen(b)))
#define SLURP(f, o)		OK(testlib_slurp((f), (o)))
#define EXEC(t,o,e)		testlib_exec((t), (o), (e), __FILE__, __LINE__)
