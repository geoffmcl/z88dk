//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------

%top{
	#include "z80asm_manual.h"
	#include "legacy.h"
	#include <iostream>
}

%option freespace dotall

%class{
	protected:
		bool verbose;
}

%%

-v \z | \
--verbose \z	{ verbose = true; return true; }

-h \z | \
--help \z		{ using namespace std;
				  cout << z80asm_manual;
				  exit(EXIT_SUCCESS);
				  return true; 	// not reached
				}

.				{ return false; }

%%
