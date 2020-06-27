//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#pragma once

#include <stdbool.h>

// supported cpus - in sync with cpu_name[]
typedef enum cpu_e {
    cpuZ80, cpuZ80N, cpuZ180, cpuR2K, cpuR3K,
    cpu8080, cpu8085, cpu8080I, cpu8085I,
    cpuGBZ80,
    MAX_CPU
} cpu_e;

// cpu names - in sync with enum cpu_e
extern const char* cpu_name[];

// parsing of strings
typedef enum string_e {
    strC, strBASIC, strSIMPLE,
} string_e;

// current cpu
extern cpu_e opt_cpu;			// selected CPU
extern string_e opt_string;		// parsing of strings

// command line options
extern bool opt_atoctal;		// '@' is an octal prefix instead of binary
extern bool opt_dotdirective;	// '.' starts directives instead of label names
extern bool opt_hashhex;		// '#' is a hex prefix instead of immediate indicator
extern bool opt_noprec;			// no precedence in expressions TODO??
extern bool opt_labelcol1;		// labels start at col1 instead of '.' prefix or ':' suffix
extern bool opt_ucase;			// all symbols converted to upper case
extern bool opt_verbose;		// be verbose
extern bool opt_swap_ix_iy;		// swap IX and IY
