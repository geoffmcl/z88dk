//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

#pragma once

#include "options.yy.h"

#include <array>
#include <vector>

class CmdArgs : private OptionsLexer
{
public:
	CmdArgs();
	virtual ~CmdArgs();

	// Parse options and files from argv
	bool ParseArgs(int argc, char* argv[]);

	// access options from OptionsLexer
	bool isVerbose() const { return verbose; }
};

extern CmdArgs theCmdArgs;		// singleton
