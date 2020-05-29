/******************************************************************************
* z80asm restart
* Copyright (C) Paulo Custodio, 2011-2020
* License: http://www.perlfoundation.org/artistic_license_2_0
* Repository: https://github.com/z88dk/z88dk
******************************************************************************/

%code top {
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
}

%code requires {
	#include "asm.h"
	#include "lex.h"
	#include "parse.h"
}

%output		"gram.c"
%defines 	"gram.h"

%define api.value.type union
%token <ident_t>		IDENT
%token <int>			INTEGER
%token <const char*>	LABEL
%token <const char*>	STRING

/* registers */
%token A A_ F F_ AF AF_ B B_ C C_ BC BC_ D D_ E E_ DE DE_ H H_ L L_ HL HL_ IX IXH IXL IY IYH IYL SP I R   

/* flags */
%token NC NZ Z PO PE P M

/* opcodes */
%token DJNZ EX HALT LD NOP RET

/* directives */
%token BYTE WORD

// %destructor{ free($$); } <const char*>

%right '?' ':'
%left LOR	"||"
%left LXOR	"^^"
%left LAND	"&&"
%left '|'
%left '^'
%left '&'
%left '=' NE "!="
%left '<' '>' LE "<=" GE ">="
%left LS "<<" RS ">>"
%left '+' '-'
%left '*' '/' '%'
%precedence UPLUS UMINUS 
%right '!' '~'
%right POW

%%

input:
  %empty
| input line
;

line:
  label stmt
;

label:
  %empty
| LABEL
;

stmt:
		'\n'
| NOP	'\n'
| HALT	'\n'
| RET	'\n'
;

%%
