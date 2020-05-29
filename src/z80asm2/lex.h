//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#pragma once

// global variables
extern const char* yyfilename;
extern int yylineno;
extern int num_errors;

// lex.l interface
void push_input_file(const char* filename);
void pop_input_file(void);

int yylex(void);

void yyerror(const char* fmt, ...);
void yyerrorat(const char* filename, int lineno, const char* fmt, ...);
