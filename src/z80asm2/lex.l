/******************************************************************************
* z80asm restart
* Copyright (C) Paulo Custodio, 2011-2020
* License: http://www.perlfoundation.org/artistic_license_2_0
* Repository: https://github.com/z88dk/z88dk
******************************************************************************/

%top{
	#include "lex.h"
	#include "asm.h"
	#include "gram.h"
	#include "parse.h"
	#include "utils.h"
	#include "utstring.h"
	#include <assert.h>
	#include <ctype.h>
	#include <stdbool.h>
	#include <stdlib.h>
	#include <string.h>
	#ifdef _WIN32
	#include <io.h>
	#endif

	#define YY_NO_UNISTD_H
	#define YY_DECL static int yylex_1(void)
}

%option outfile = "lex.c"
%pointer

%{
	/* outer scanner state */
	typedef enum state_e { AT_LABEL, AT_OPCODE, AT_OPERAND } state_e;

	/* structure for recursive includes */
	typedef struct input_t {
		const char*	filename;			/* previous input file name */
		int			lineno;				/* previous input line */
		state_e		state;				/* state of yylex */
		UT_string*	toscan;				/* text to be scanned */
		int			nexttok;			/* next token if peeked, -1 if not */
		YYSTYPE		nextlval;			/* value of next token, if any */
		YY_BUFFER_STATE	buffer;			/* previous input buffer */
	} input_t;

	/* global state of lex */
	const char* yyfilename;
	/* int yylineno; // is defined by flex */
	static state_e yystate;
	static UT_string* yytoscan;
	static int yynexttok = -1;
	static YYSTYPE yynextlval;
	int num_errors;

	/* previous buffers */
	#define INCLUDE_DEPTH 10
	static input_t* input_stack;
	static int input_stack_size;
	static int input_stack_sp;

	/* internal functions */
	static void init(void);
	static int  return_ident(bool atbol);
	static bool parse_integer(const char* text, int base);
	static bool parse_bitmap(const char* text);
	static void lexerror(const char* fmt, ...);
	
	/* global data */
	static UT_string* yypad;
%}

EOL			\r\n|\r|\n
NEOL		[^\r\n]
WS			[ \t\v\f]
ID0			[_a-zA-Z]
ID1			[_a-zA-Z0-9.]
ID			{ID0}{ID1}*
NID			[^_a-zA-Z.]
BIN			[01]
OCT			[0-7]
DEC			[0-9]
HEX			[0-9a-fA-F]
BITS		[-#]

/* C comment - / * .. * / */
%x comment

/* C-strings, BASIC-strings, SIMPLE-strings */
%x str_c
%x str_basic
%x str_simple

/* charcater constant, same as double-quoted strings but single quoted and maximum of 1 character */
%x cc_c
%x cc_basic
%x cc_simple

%s atoctal
%s atbinary
%s hashhex

%%
					/* setup */
					int yylineno1 = yylineno;
					if (opt_atoctal) BEGIN(atoctal); else BEGIN(atbinary);
					if (opt_hashhex) BEGIN(hashhex);

					/* white space */
{EOL}				{ yylineno++; return '\n'; }
{WS}+				{ }					/* skip white space */
\\					{ unput('\n'); yylineno--; }	/* logical line separator */

					/* line comment */
(;|"//"){NEOL}*		{ }

					/* C-comment */
"/*"				{ BEGIN(comment); yylineno1 = yylineno; }
<comment>{
	{EOL}			{ yylineno++; }
	[^*\n\r]+		{ }
	"*"+[^*/\n\r]* 	{ }
	"*"+"/"			{ BEGIN(INITIAL); }
	<<EOF>>			{ BEGIN(INITIAL); yyerrorat(yyfilename, yylineno1, "unterminated comment"); }
}
					/* numbers */
<hashhex>"#"{HEX}+/{NID} { 
					  yylval.INTEGER = (int)strtol(yytext+1, NULL, 16); return INTEGER;
					}
"$"{HEX}+/{NID}		{ yylval.INTEGER = (int)strtol(yytext+1, NULL, 16); return INTEGER; }
"%"{BIN}+/{NID}		{ yylval.INTEGER = (int)strtol(yytext+1, NULL, 2); return INTEGER; }

<atbinary>"@"{BIN}+/{NID} { 
					  yylval.INTEGER = (int)strtol(yytext+1, NULL, 2); return INTEGER; 
					}
<atoctal>"@"{OCT}+/{NID} {
					  yylval.INTEGER = (int)strtol(yytext+1, NULL, 8); return INTEGER;
					}
"%\""{BITS}+"\""	{ if (parse_bitmap(yytext+2)) return INTEGER; else lexerror("invalid number"); }
<atbinary>"@\""{BITS}+"\"" {
					  if (parse_bitmap(yytext+2)) return INTEGER; else lexerror("invalid number"); }

0[bB]{BIN}+			{ if (parse_integer(yytext+2,  2)) return INTEGER; else lexerror("invalid number"); }
0[oOqQ]{OCT}+		{ if (parse_integer(yytext+2,  8)) return INTEGER; else lexerror("invalid number"); }
0[x]{HEX}+			{ if (parse_integer(yytext+2, 16)) return INTEGER; else lexerror("invalid number"); }
{BIN}+[bB]			{ if (parse_integer(yytext,    2)) return INTEGER; else lexerror("invalid number"); }
{OCT}+[oOqQ]		{ if (parse_integer(yytext,    8)) return INTEGER; else lexerror("invalid number"); }
{DEC}+[dD]			{ if (parse_integer(yytext,   10)) return INTEGER; else lexerror("invalid number"); }
{DEC}{HEX}*[hH]		{ if (parse_integer(yytext,   16)) return INTEGER; else lexerror("invalid number"); }
{DEC}+				{ if (parse_integer(yytext,   10)) return INTEGER; else lexerror("invalid number"); }

					/* alternate registers or identifier */
^{ID}				{ return return_ident(true); }

"A'"				|
"B'"				|
"C'"				|
"D'"				|
"E'"				|
"H'"				|
"L'"				|
"AF'"				|
"BC'"				|
"DE'"				|
"HL'"				|
{ID}				{ return return_ident(false); }

					/* operators */
[!&|^*=<>()\[\]{}+,\-/:?~#$%@.]	return *yytext;
"!=" 				return NE;
"<>" 				return NE;
"&&"				return LAND;
"||"				return LOR;
"^^"				return LXOR;
"**"				return POW;
"=="				return '=';
">=" 				return GE;
"<=" 				return LE;
"<<" 				return LS;
">>" 				return RS;

					/* strings */
\"					{ 
					  init(); utstring_clear(yypad);
					  switch (opt_string) { 
					  case strC: BEGIN(str_c); break;
					  case strBASIC: BEGIN(str_basic); break;
					  case strSIMPLE: BEGIN(str_simple); break;
					  default: assert(0);
					  }
					}
<str_c,str_basic,str_simple>{
	\"				{ BEGIN(INITIAL); yylval.STRING = spool_add(utstring_body(yypad)); return STRING; }
	{EOL}			|
	<<EOF>>			{ BEGIN(INITIAL); unput('\n'); lexerror("unterminated string"); }
}
<str_c,cc_c>{
	\\{OCT}{1,3} 	{ int c = (int)strtol(yytext+1, NULL, 8);
					  if (c > 0xff) { 
						  BEGIN(INITIAL);
						  lexerror("octal character constant out of bounds");
					  } else 
						  utstring_printf(yypad, "%c", c & 0xff);
					}
	\\[xX]{HEX}+ 	{ int c = (int)strtol(yytext+2, NULL, 16);
					  if (c > 0xff) {
						  BEGIN(INITIAL);
						  lexerror("hex character constant out of bounds");
					  } else
						  utstring_printf(yypad, "%c", c & 0xff);
					}
	\\a 			{ utstring_printf(yypad, "\a"); }
	\\b  			{ utstring_printf(yypad, "\b"); }
	\\e  			{ utstring_printf(yypad, "\x1b"); }
	\\f  			{ utstring_printf(yypad, "\f"); }
	\\n  			{ utstring_printf(yypad, "\n"); }
	\\r  			{ utstring_printf(yypad, "\r"); }
	\\t  			{ utstring_printf(yypad, "\t"); }
	\\v  			{ utstring_printf(yypad, "\v"); }
	\\\\  			{ utstring_printf(yypad, "\\"); }
	\\\'  			{ utstring_printf(yypad, "'"); }
	\\\"  			{ utstring_printf(yypad, "\""); }
	\\\?  			{ utstring_printf(yypad, "?"); }
	\\				{ BEGIN(INITIAL);
					  lexerror("bad escape sequence"); }
}
<str_c>{
	[^\\\n\"\r]+	{ utstring_printf(yypad, "%s", yytext); }
}
<str_basic>{
	\"\"			{ utstring_printf(yypad, "\""); }
	[^\"\n\r]+		{ utstring_printf(yypad, "%s", yytext); }
}
<str_simple>{
	[^\"\n\r]+		{ utstring_printf(yypad, "%s", yytext); }
}

					/* character constants */
\'					{ 
					  init(); utstring_clear(yypad);
					  switch (opt_string) { 
					  case strC: BEGIN(cc_c); break;
					  case strBASIC: BEGIN(cc_basic); break;
					  case strSIMPLE: BEGIN(cc_simple); break;
					  default: assert(0);
					  }
					}
<cc_c,cc_basic,cc_simple>{
	\'				{ BEGIN(INITIAL); 
					  if (utstring_len(yypad) != 1) 
						  lexerror("character constant must have size 1");
					  else {
						  yylval.INTEGER = (unsigned char)*utstring_body(yypad);
						  return INTEGER;
					  }
					}
	{EOL}			|
	<<EOF>>			{ BEGIN(INITIAL); unput('\n'); lexerror("unterminated character constant"); }
}
<cc_c>{
	[^\\\n\'\r]+	{ utstring_printf(yypad, "%s", yytext); }
}
<cc_basic>{
	\'\'			{ utstring_printf(yypad, "'"); }
	[^\'\n\r]+		{ utstring_printf(yypad, "%s", yytext); }
}

<cc_simple>{
	[^\'\n\r]+		{ utstring_printf(yypad, "%s", yytext); }
}

.					lexerror("invalid character");

%%

/*---------------------------------------------------------------------------*/

static void dtor(void) {
	/* free UT_string yypad */
	utstring_free(yypad);

	/* free input_stack */
	while (input_stack_sp > 0)
		pop_input_file();
	free(input_stack); input_stack = NULL;
	input_stack_size = input_stack_sp = 0;

	/* free toscan buffer */
	utstring_free(yytoscan);
}

static void init(void) {
	static bool inited = false;
	if (!inited) {
		utstring_new(yypad);
		utstring_new(yytoscan);

		atexit(dtor);
		inited = true;
	}
}

/*---------------------------------------------------------------------------*/

int yywrap(void) { 
	return 1; 
}

static int return_ident(bool atbol) {
	init();

	// convert copy of yytext to upper case to lookup
	utstring_clear(yypad);
	utstring_bincpy(yypad, yytext, yyleng);
	strtoupper(utstring_body(yypad));

	if (opt_ucase) 
		yylval.IDENT.name = spool_add(utstring_body(yypad));
	else
		yylval.IDENT.name = spool_add(yytext);

	yylval.IDENT.id = lookup_keyword(utstring_body(yypad));
	yylval.IDENT.atbol = atbol;
	return IDENT;
}

static bool parse_integer(const char* text, int base) { 
	/* convert number */
	yylval.INTEGER = (int)strtol(text, NULL, base);

	/* peek next character */
	char c = input(); if (c != 0) unput(c);

	if (c == '.' || c == '_' || isalnum(c))
		return false;
	else
		return true;
}

static bool parse_bitmap(const char* text) {
	init();

	/* convert number */
	yylval.INTEGER = 0;
	const char* p;
	for (p = text; *p; p++) {
		if (*p == '-') yylval.INTEGER *= 2;
		else if (*p == '#') yylval.INTEGER = yylval.INTEGER * 2 + 1;
		else break;
	}

	if (*p == '"')
		return true;
	else
		return false;
}

/*---------------------------------------------------------------------------*/

void push_input_file(const char* filename) {
	init();

	/* TODO: search include path */

	/* grow stack if needed */
	if (input_stack_sp >= input_stack_size) {
		input_stack_size += INCLUDE_DEPTH;
		input_stack = xrealloc(input_stack, input_stack_size * sizeof(input_t));
	}

	/* open the file, get its size */
	FILE* in = fopen(filename, "rb");
	if (in == NULL) {
		perror(filename);
		yyerror("open failed");
		exit(EXIT_FAILURE);
	}

	fseek(in, 0, SEEK_END);
	int size = ftell(in);
	if (size < 0) {
		perror(filename);
		yyerror("seek failed");
		exit(EXIT_FAILURE);
	}
	fseek(in, 0, SEEK_SET);

	/* save current input */
	input_t* input = &input_stack[input_stack_sp++];
	input->filename = yyfilename;
	input->lineno = yylineno;
	input->state = yystate;
	input->toscan = yytoscan;
	input->nexttok = yynexttok;
	input->nextlval = yynextlval;
	input->buffer = YY_CURRENT_BUFFER;
	
	/* slurp whole file */
	utstring_new(yytoscan);
	utstring_reserve(yytoscan, size + 4);		/* +1 newline +2 NUL end-of-buffer +1 for std NUL */
	if (size != fread(utstring_body(yytoscan), 1, size, in)) {
		perror(filename);
		yyerror("read failed");
		exit(EXIT_FAILURE);
	}
	utstring_len(yytoscan) = size;
	fclose(in);

	/* append newline and 2 NULs required by yylex */
	if (size > 0 && utstring_body(yytoscan)[size - 1] != '\n')
		utstring_bincpy(yytoscan, "\n", 1);
	utstring_bincpy(yytoscan, "\0\0", 2);		/* 2 EOB needed by yylex */

	/* set current buffer */
	yyfilename = spool_add(filename);
	yylineno = 1;
	yystate = AT_LABEL;
	yynexttok = -1;
	YY_BUFFER_STATE buffer = yy_scan_buffer(utstring_body(yytoscan), utstring_len(yytoscan));
	if (buffer == NULL)
		YY_FATAL_ERROR("yy_scan_buffer() failed");
}

void pop_input_file(void) {
	if (input_stack_sp > 0) {
		yy_delete_buffer(YY_CURRENT_BUFFER);

		input_t* input = &input_stack[--input_stack_sp];
		utstring_free(yytoscan);
		yytoscan = input->toscan;
		yyfilename = input->filename;
		yylineno = input->lineno;
		yystate = input->state;
		yynexttok = input->nexttok;
		yynextlval = input->nextlval;
		yy_switch_to_buffer(input->buffer);
	}
}

/*---------------------------------------------------------------------------*/

static void prefix_at(const char* filename, int lineno, const char* prefix) {
	bool have_prefix;
	if (!STREMPTY(filename)) {
		fprintf(stderr, "%s:", filename);
		have_prefix = true;
	}
	if (lineno) {
		fprintf(stderr, "%d:", lineno);
		have_prefix = true;
	}
	if (have_prefix)
		fprintf(stderr, " ");
	fprintf(stderr, "%s: ", prefix);
}

static void error_prefix_at(const char* filename, int lineno) {
	prefix_at(filename, lineno, "error");
}

static void warn_prefix_at(const char* filename, int lineno) {
	prefix_at(filename, lineno, "warning");
}

#define DO_ERROR(file, line) \
	va_list ap; \
	va_start(ap, fmt); \
	error_prefix_at((file), (line)); \
	vfprintf(stderr, fmt, ap); \
	va_end(ap); \
	fprintf(stderr, "\n"); \
	num_errors++

void yyerror(const char* fmt, ...) {
	DO_ERROR(yyfilename, yylineno);
}

void yyerrorat(const char* filename, int lineno, const char* fmt, ...) {
	DO_ERROR(filename, lineno);
}

// yyerror and skip to end of line
static void lexerror(const char* fmt, ...) {
	DO_ERROR(yyfilename, yylineno);
	int c;
	while ((c = input()) != 0 && c != '\n') {
	}
	if (c != 0)
		unput(c);
}

#undef DO_ERROR

/*---------------------------------------------------------------------------*/

static int peek_token(void) {
	YYSTYPE savelval = yylval;
	yynexttok = yylex_1();
	yynextlval = yylval;
	yylval = savelval;
	return yynexttok;
}

static int get_next_token(void) {
	assert(yynexttok >= 0);
	int tok = yynexttok;
	yynexttok = -1;
	yylval = yynextlval;
	return tok;
}

static void clear_queue(void) {
	yynexttok = -1;
}

int yylex(void) {
	/* get next token */
	int tok = (yynexttok >= 0) ? get_next_token() : yylex_1();
	switch (yystate) {
	case AT_LABEL:
		switch (tok) {
		case '.':
			switch (peek_token()) {
			case IDENT:
				if (opt_dotdirective && is_directive(yynextlval.IDENT.id)) {
					yystate = AT_OPERAND;
					clear_queue();
					return yynextlval.IDENT.id;
				}
				else {
					yystate = AT_OPCODE;
					yylval.LABEL = yynextlval.IDENT.name;
					clear_queue();
					return LABEL;
				}
			default:
				yystate = AT_OPERAND;
				return tok;
			}
		case IDENT:
			if (opt_labelcol1 && yylval.IDENT.atbol) {
				yystate = AT_OPCODE;
				yylval.LABEL = yylval.IDENT.name;
				return LABEL;
			}
			else {
				switch (peek_token()) {
				case ':': {
					yystate = AT_OPCODE;
					yylval.LABEL = yylval.IDENT.name;
					clear_queue();
					return LABEL;
				}
				case IDENT: {
					if (is_opcode_or_directive(yynextlval.IDENT.id)) {		/* LABEL OPCODE */
						yystate = AT_OPCODE;
						yylval.LABEL = yylval.IDENT.name;
						return LABEL;
					}
					else {							/* OPCODE IDENT */
						yystate = AT_OPERAND;
						return (is_opcode_or_directive(yylval.IDENT.id)) ?
							yylval.IDENT.id : IDENT;
					}
				}
				default:
					yystate = AT_OPERAND;
					return (is_opcode_or_directive(yylval.IDENT.id)) ?
						yylval.IDENT.id : IDENT;
				}
			}
		case '\n':
			yystate = AT_LABEL;
			return tok;
		default:
			yystate = AT_OPERAND;
			return tok;
		}
	case AT_OPCODE:
		switch (tok) {
		case IDENT:
			yystate = AT_OPERAND;
			return (is_opcode_or_directive(yylval.IDENT.id)) ?
				yylval.IDENT.id : IDENT;
		case '\n':
			yystate = AT_LABEL;
			return tok;
		default:
			return tok;
		}
	case AT_OPERAND:
		switch (tok) {
		case '\n':
			yystate = AT_LABEL;
			return tok;
		case IDENT:
			return (is_flag_or_register(yylval.IDENT.id)) ?
				yylval.IDENT.id : IDENT;
		default:
			return tok;
		}
	default:
		assert(0); return 0;
	}
}
