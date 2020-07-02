//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

// interface between legacy C code and new C++ code
#ifdef __cplusplus
extern "C" {
#endif

int z80asm_main(int argc, char* argv[]);

#ifdef __cplusplus
};
#endif
