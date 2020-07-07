/*
Z88DK Z80 Macro Assembler

Copyright (C) Gunther Strube, InterLogic 1993-99
Copyright (C) Paulo Custodio, 2011-2020
License: The Artistic License 2.0, http://www.perlfoundation.org/artistic_license_2_0
*/

#pragma once

#include "config.h"

#ifndef PREFIX
#define PREFIX "/usr/local/share/z88dk"
#endif

#ifndef Z88DK_VERSION
#define Z88DK_VERSION "build " __DATE__
#endif

#ifndef EXEC_PREFIX
#define EXEC_PREFIX ""
#endif

extern char copyrightmsg[];
