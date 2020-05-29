//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#include "test.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int count_run;
static int count_failed;
static const char* prog_name;

char* testlib_norm_eol(char* text) {
    char* i, *o;
    for (i = o = text; *i; i++, o++) {
        if (*i == '\r') {
            *o = '\n';
            if (i[1] == '\n')
                i++;
        }
        else
            *o = *i;
    }
    *o = '\0';
    return text;
}

void testlib_diag(const char* fmt, ...) {
    UT_string* buffer;

    utstring_new(buffer);
    va_list ap;
    va_start(ap, fmt);
    utstring_printf_va(buffer, fmt, ap);
    va_end(ap);

    // remove blanks from end of buffer
    char* p = utstring_body(buffer) + utstring_len(buffer);
    while (p > utstring_body(buffer) && isspace(p[-1]))
        p--;
    *p = '\0';

    // print # before each line
    printf("# ");
    for (p = utstring_body(buffer); *p; p++) {
        if (*p == '\n')
            printf("\n# ");
        else
            putchar(*p);
    }
    putchar('\n');

    utstring_free(buffer);
}

void testlib_ok(bool value, const char* filename, int lineno) {
    count_run++;

    if (!value) {
        printf("not ok %d\n", count_run);
        count_failed++;
        DIAG("Failed test at %s line %d\n", filename, lineno);
    }
}

void testlib_is(bool straight, int a, int b, const char* filename, int lineno) {
    bool ok = (a == b);
    if (!straight) ok = !ok;
    testlib_ok(ok, filename, lineno);
    if (!ok) {
        DIAG("a=%d", (a));
        DIAG("b=%d", (b));
    }
}

void testlib_str_is(bool straight, const char* a_, const char* b_,
                    const char* filename, int lineno) {
    UT_string* a; utstring_new(a); utstring_printf(a, "%s", a_);
    testlib_norm_eol(utstring_body(a));
    UT_string* b; utstring_new(b); utstring_printf(b, "%s", b_);
    testlib_norm_eol(utstring_body(b));

    bool ok = (strcmp(utstring_body(a), utstring_body(b)) == 0);
    if (!straight) ok = !ok;
    testlib_ok(ok, filename, lineno);
    if (!ok) {
        DIAG("a=\"%s\"", utstring_body(a));
        DIAG("b=\"%s\"", utstring_body(b));
    }

    utstring_free(a);
    utstring_free(b);
}

bool testlib_spew(const char* filename, const char* buffer, size_t size) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror(filename);
        return false;
    }
    size_t written = fwrite(buffer, 1, size, fp);
    fclose(fp);
    return (written == size);
}

bool testlib_slurp(const char* filename, UT_string* out) {
    char buffer[1024];
    utstring_clear(out);

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror(filename);
        return false;
    }

    size_t nread;
    while ((nread = fread(buffer, 1, sizeof(buffer), fp)) > 0)
        utstring_bincpy(out, buffer, nread);
    fclose(fp);

    return true;
}

void testlib_exec(const char* test, const char* exp_out, const char* exp_err,
                  const char* filename, int lineno) {
    UT_string* cmd; utstring_new(cmd);
    UT_string* outfile; utstring_new(outfile);
    UT_string* errfile; utstring_new(errfile);
    UT_string* out; utstring_new(out);

    int initial_failed = count_failed;

    utstring_printf(outfile, "test%d.out", count_run);
    utstring_printf(errfile, "test%d.err", count_run);
    utstring_printf(cmd, "%s %s > %s 2> %s",
                    prog_name, test, utstring_body(outfile), utstring_body(errfile));
    int result = system(utstring_body(cmd));
    DIAG("%s --> %d", utstring_body(cmd), result);
    testlib_ok(result == 0, filename, lineno);

    if (result == 0) {
        bool ok = testlib_slurp(utstring_body(outfile), out);
        testlib_ok(ok, filename, lineno);
        if (ok)
            testlib_str_is(true, exp_out, utstring_body(out), filename, lineno);

        ok = testlib_slurp(utstring_body(errfile), out);
        testlib_ok(ok, filename, lineno);
        if (ok)
            testlib_str_is(true, exp_err, utstring_body(out), filename, lineno);
    }

    if (initial_failed == count_failed) {
        remove(utstring_body(outfile));
        remove(utstring_body(errfile));
    }

    utstring_free(cmd); utstring_free(outfile); utstring_free(errfile);
    utstring_free(out);
}

int main(int argc, char* argv[]) {
    prog_name = argv[0];

    if (argc == 2) {                // sub-test
#define TEST_CALL(x)
#define TEST_EXEC(x)	extern int  x(void); if (strcmp(argv[1], #x) == 0) { exit(x()); }
#include "test.def"
        return EXIT_FAILURE;        // should not be reached
    }
    else if (argc == 1) {           // main test
#define TEST_CALL(x)	extern void x(void); x();
#define TEST_EXEC(x)
#include "test.def"
        PASS();
    }
    else {
        DIAG("too many arguments");
        FAIL();
    }

    printf("1..%d\n", count_run);
    if (count_failed)
        printf("Failed %d tests of %d\n", count_failed, count_run);
    else
        printf("All tests successful.\n");

    if (!count_failed)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}
