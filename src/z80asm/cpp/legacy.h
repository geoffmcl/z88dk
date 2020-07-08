//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include <stdbool.h>

// interface between legacy C code and new C++ code
#ifdef __cplusplus
extern "C" {
#endif

// call legacy
extern char copyrightmsg[];
int z80asm_main(int argc, char* argv[]);

// called by legacy
bool OptionVerbose();

#ifdef __cplusplus
};
#endif
