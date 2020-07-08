//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "CmdArgs.h"

CmdArgs theCmdArgs;		// singleton

CmdArgs::CmdArgs()
{
}

CmdArgs::~CmdArgs()
{
}

// Parse options and files from argv
bool CmdArgs::ParseArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i) {
		in(argv[i]);
		if (lex())
			argv[i][0] = '\0';		// cancel this argument for next pass
		else {						// TODO: error handling
		}
	}
	return true;					
}
