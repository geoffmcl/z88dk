//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#include "asm.h"
#include "uthash.h"
#include <stdbool.h>
#include <stdlib.h>

// cpu names - in sync with enum cpu_e
const char* cpu_name[MAX_CPU] = {
    "z80", "z80n", "z180", "r2k", "r3k",
    "8080", "8085", "8080I", "8085I",
    "gbz80",
};

// command line options
cpu_e opt_cpu = cpuZ80;
string_e opt_string = strC;

bool opt_atoctal;
bool opt_dotdirective;
bool opt_hashhex;
bool opt_noprec;
bool opt_labelcol1;
bool opt_ucase;
bool opt_verbose;
bool opt_swap_ix_iy;
