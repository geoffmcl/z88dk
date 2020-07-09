//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#include "CmdArgs.h"
#include "z80asm_usage.h"
#include "config.h"

#include <iostream>

#ifndef Z88DK_VERSION
#define Z88DK_VERSION "build " __DATE__
#endif

static const char copyrightmsg[] {
	"Z80 Module Assembler " Z88DK_VERSION "\n"
	"(c) InterLogic 1993-2009, Paulo Custodio 2011-2020" };

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
	using namespace std;

	// if no arguments, just show usage and exit
	if (argc == 1) {
		cout << copyrightmsg << endl << endl
			<< z80asm_usage;
		exit(EXIT_SUCCESS);
	}

	for (int i = 1; i < argc; ++i) {
		in(argv[i]);
		if (lex())
			argv[i][0] = '\0';		// cancel this argument for next pass
		else {						// TODO: error handling
		}
	}
	return true;					
}
