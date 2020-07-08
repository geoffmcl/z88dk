//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include <CmdArgs.h>

#include "legacy.h"

#include <iostream>

int main(int argc, char* argv[]) {
	if (!theCmdArgs.ParseArgs(argc, argv))
		return EXIT_FAILURE;
    return z80asm_main(argc, argv);
}
