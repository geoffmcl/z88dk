//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#include "parse.h"
#include "gram.h"
#include "lex.h"
#include "utils.h"
#include "uthash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_TOKEN 512		// fix this if gram.y defines tokens > MAX_TOKEN

// map text to ids
typedef enum kw_flag_e {
    kwOPCODE = 1,
    kwDIRECTIVE = 2,
    kwFLAG = 4,
    kwREGISTER = 8,
    kwALL = -1,
} kw_flag_e;

typedef struct keyword_t {
    const char*	name;
    int			id;
    kw_flag_e 	flags;
    UT_hash_handle hh;
} keyword_t;

static keyword_t g_keywords_table[] = {
    //name		id		flags
    // opcodes
    {"DJNZ",	DJNZ,	kwOPCODE,			},
    {"EX",		EX,		kwOPCODE,			},
    {"HALT",	HALT,	kwOPCODE,			},
    {"LD",		LD,		kwOPCODE,			},
    {"NOP",		NOP,	kwOPCODE,			},
    {"RET",		RET,	kwOPCODE,			},

    // directives
    {"DEFB",	BYTE,	kwDIRECTIVE,		},

    // flags
    {"NC",		NC,		kwFLAG,				},
    {"Z",		Z,		kwFLAG,				},
    {"NZ",		NZ,		kwFLAG,				},
    {"PO",		PO,		kwFLAG,				},
    {"PE",		PE,		kwFLAG,				},
    {"P",		P,		kwFLAG,				},
    {"M",		M,		kwFLAG,				},

    // registers
    {"A",		A,		kwREGISTER,			},
    {"B",		B,     	kwREGISTER,			},
    {"C",		C,     	kwREGISTER | kwFLAG,	},
    {"D",		D,     	kwREGISTER,			},
    {"E",		E,     	kwREGISTER,			},
    {"H",		H,     	kwREGISTER,			},
    {"L",		L,     	kwREGISTER,			},
    {"I",		I,		kwREGISTER,			},
    {"R",		R,     	kwREGISTER,			},

    {"AF",		AF,    	kwREGISTER,			},
    {"BC",		BC,    	kwREGISTER,			},
    {"DE",		DE,    	kwREGISTER,			},
    {"HL",		HL,    	kwREGISTER,			},
    {"SP",		SP,    	kwREGISTER,			},
    {"IX",		IX,    	kwREGISTER,			},
    {"IY",		IY,    	kwREGISTER,			},

    {"IXH",		IXH,   	kwREGISTER,			},
    {"IXL",		IXL,   	kwREGISTER,			},
    {"IYH",		IYH,   	kwREGISTER,			},
    {"IYL",		IYL,   	kwREGISTER,			},

    {"A'",		A_,		kwREGISTER,			},
    {"B'",		B_,     kwREGISTER,			},
    {"C'",		C_,     kwREGISTER,			},
    {"D'",		D_,     kwREGISTER,			},
    {"E'",		E_,     kwREGISTER,			},
    {"H'",		H_,     kwREGISTER,			},
    {"L'",		L_,     kwREGISTER,			},
    {"AF'",		AF_,    kwREGISTER,			},
    {"BC'",		BC_,    kwREGISTER,			},
    {"DE'",		DE_,    kwREGISTER,			},
    {"HL'",		HL_,    kwREGISTER,			},

    {NULL}
};

static keyword_t* g_keywords_hash;
static int lu_flags[MAX_TOKEN];

// keyword lookup
static void dtor(void) {
    keyword_t* elem, *tmp;
    HASH_ITER(hh, g_keywords_hash, elem, tmp) {
        HASH_DEL(g_keywords_hash, elem);
    }
}

static void init(void) {
    static bool inited = false;
    if (!inited) {
        for (keyword_t* elem = g_keywords_table; elem->name; elem++) {
            HASH_ADD_STR(g_keywords_hash, name, elem);

            assert(elem->id < MAX_TOKEN);		// fix MAX_TOKEN if grammar defines more tokens
            lu_flags[elem->id] = elem->flags;
        }
        atexit(dtor);
        inited = true;
    }
}

int lookup_keyword(char* name) {
    init();
    keyword_t* found;
    HASH_FIND_STR(g_keywords_hash, name, found);
    if (found)
        return found->id;
    else
        return 0;
}

static int kw_flag(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return lu_flags[id];
}

bool is_directive(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & kwDIRECTIVE) != 0;
}

bool is_opcode(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & kwOPCODE) != 0;
}

bool is_opcode_or_directive(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & (kwDIRECTIVE | kwOPCODE)) != 0;
}

bool is_flag(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & kwFLAG) != 0;
}

bool is_register(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & kwREGISTER) != 0;
}

bool is_flag_or_register(int id) {
    init();
    assert(id >= 0 && id < MAX_TOKEN);
    return(lu_flags[id] & (kwFLAG | kwREGISTER)) != 0;
}
