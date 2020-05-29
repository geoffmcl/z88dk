//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#pragma once

#include <stdbool.h>

// type for identifiers
typedef struct ident_t {
    const char* name;			// text, points to string pool
    int			id;				// keyword id, 0 if none
    bool		atbol;			// true if at begin of line
} ident_t;

// NOTE: this function requires the input in upper case
int lookup_keyword(char* name);

// check id
bool is_opcode(int id);
bool is_directive(int id);
bool is_opcode_or_directive(int id);
bool is_flag(int id);
bool is_register(int id);
bool is_flag_or_register(int id);
