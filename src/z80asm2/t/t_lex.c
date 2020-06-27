//-----------------------------------------------------------------------------
// z80asm restart
// Copyright (C) Paulo Custodio, 2011-2020
// License: http://www.perlfoundation.org/artistic_license_2_0
// Repository: https://github.com/z88dk/z88dk
//-----------------------------------------------------------------------------

#include "test.h"
#include "../lex.h"
#include "../gram.h"

void test_continuation_line(void) {
    SPEW_T("test.asm", "lbl1: ld b,23\\ djnz lbl1\\lbl2 ret");
    push_input_file("test.asm");

    STR_IS(yyfilename, "test.asm");
    IS(yylineno, 1);

    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "lbl1");
    IS(yylex_2(), LD);
    IS(yylex_2(), B);
    IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 23);
    IS(yylval.IDENT.atbol, false);

    IS(yylex_2(), '\n');

    STR_IS(yyfilename, "test.asm");
    IS(yylineno, 1);

    IS(yylex_2(), DJNZ);

    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "lbl1");
    IS(yylval.IDENT.id, 0);
    IS(yylval.IDENT.atbol, false);

    IS(yylex_2(), '\n');

    STR_IS(yyfilename, "test.asm");
    IS(yylineno, 1);

    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "lbl2");

    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);

    pop_input_file();
    OK(0 == remove("test.asm"));
}

void test_empty(void) {
    SPEW_T("test.asm", "");
    push_input_file("test.asm");
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
}

void test_end_newline(void) {
    SPEW_T("test.asm", ".");
    push_input_file("test.asm");
    IS(yylex_2(), '.');
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", ".\n");
    push_input_file("test.asm");
    IS(yylex_2(), '.');
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", ".\r");
    push_input_file("test.asm");
    IS(yylex_2(), '.');
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", ".\r\n");
    push_input_file("test.asm");
    IS(yylex_2(), '.');
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
}

void test_comments(void) {
    SPEW_T("test.asm",
           "!;1 2 3 4\n"		// line 1
           ";1 2 3 4\n"		// line 2
           "@//1 2 3 4\n"		// line 3
           "$/* cmt 1\n"		// line 4
           "cmt 2\n"			// line 5
           "cmt 3*/%"			// line 6
          );
    push_input_file("test.asm");
    IS(yylex_2(), '!'); IS(yylineno, 1);
    IS(yylex_2(), '\n'); IS(yylineno, 2);
    IS(yylex_2(), '\n'); IS(yylineno, 3);
    IS(yylex_2(), '@'); IS(yylineno, 3);
    IS(yylex_2(), '\n'); IS(yylineno, 4);
    IS(yylex_2(), '$'); IS(yylineno, 4);
    IS(yylex_2(), '%'); IS(yylineno, 6);
    IS(yylex_2(), '\n'); IS(yylineno, 7);
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm",
           "/*\n"		// line 1
           "\n"		// line 2
           "\n"		// line 3
           "\n"		// line 4
           "\n"		// line 5
           "\n"		// line 6
          );
    EXEC("exec_scan_error",
         "", "test.asm:1: error: unterminated comment\n");
}

void test_label(void) {
    SPEW_T("test.asm", " . defb 3 \n . label ");

    opt_dotdirective = false;
    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "defb");
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_dotdirective = true;
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_dotdirective = false;
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", "defb 3\n defb 3");

    opt_labelcol1 = false;
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_labelcol1 = true;
    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "defb");
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 3);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_labelcol1 = false;
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", " defb : defb\\ . defb defb \\ . defb : defb ");

    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "defb");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "defb");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "defb");
    IS(yylex_2(), ':');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", " . 123 defb ");
    push_input_file("test.asm");
    IS(yylex_2(), '.');
    IS(yylex_2(), INTEGER);
    IS(yylval.INTEGER, 123);
    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "defb");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));
}

void test_ucase(void) {
    SPEW_T("test.asm", "Lbl1: Defb Abc");

    opt_ucase = false;
    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "Lbl1");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "Abc");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_ucase = true;
    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "LBL1");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "ABC");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    opt_ucase = false;
    OK(0 == remove("test.asm"));
}

void test_just_opcode(void) {
    SPEW_T("test.asm", "ret\n ret\\ret\\ ret");

    push_input_file("test.asm");
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));
}

void test_label_and_opcode(void) {
    SPEW_T("test.asm", "ret ret\\ret ret");

    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "ret");
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "ret");
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));
}

void test_opcode_and_operand(void) {
    SPEW_T("test.asm", "ret rx\\ret ry");

    push_input_file("test.asm");
    IS(yylex_2(), RET);
    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "rx");
    IS(yylex_2(), '\n');
    IS(yylex_2(), RET);
    IS(yylex_2(), IDENT);
    STR_IS(yylval.IDENT.name, "ry");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));
}

void test_all_fields(void) {
    SPEW_T("test.asm",
           ".label\n"
           "label:\n"
           ".label:\n"
           "label:ret z;comment\n"
           ".label ret z; comment\n"
           ".label		ret z; comment\n"
           "label ret z;comment\n"
           "label /*\n\n*/ ret /*\n\n*/z");

    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), ':');
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), RET);
    IS(yylex_2(), Z);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), RET);
    IS(yylex_2(), Z);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), RET);
    IS(yylex_2(), Z);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), RET);
    IS(yylex_2(), Z);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "label");
    IS(yylex_2(), RET);
    IS(yylex_2(), Z);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();

    OK(0 == remove("test.asm"));
}

void test_ident(void) {
    SPEW_T("test.asm", "ex af,af'");
    push_input_file("test.asm");
    IS(yylex_2(), EX);
    IS(yylex_2(), AF);
    IS(yylex_2(), ',');
    IS(yylex_2(), AF_);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm", "Abc123.zxy_pt ret\n_Abc123.zxy_pt ret");
    push_input_file("test.asm");
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "Abc123.zxy_pt");
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), LABEL);
    STR_IS(yylval.LABEL, "_Abc123.zxy_pt");
    IS(yylex_2(), RET);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
}

void test_numbers(void) {
#define T(x)	IS(yylex_2(), INTEGER); IS(yylval.INTEGER, (x)); IS(yylex_2(), ',');

    SPEW_T("test.asm",
           "defb	0, 1, 2, 3, 9, 10, 20, 30,							; decimal without suffix\n"
           "defb	0d, 1d, 2d, 3d, 9d, 10D, 20D, 30D,					; decimal with suffix\n"

           "defb	  0, 1b, 10b, 11b, 100B, 101B, 110B, 111B,			; binary with suffix\n"
           "defb	0b0, 0b1, 0b10, 0b11, 0B100, 0B101, 0B110, 0B111,	; binary with prefix\n"
           "defb    %0, %1, %10, %11, %100, %101, %110, %111,			; binary with prefix\n"
           "defb	 @0, @1, @10, @11, @100, @101, @110, @111,			; binary with prefix\n"

           "defb	%\"-\", %\"#\", %\"#-\", %\"##\", %\"#--\", %\"#-#\", %\"##-\", %\"###\",	; bitmap\n"
           "defb	@\"-\", @\"#\", @\"#-\", @\"##\", @\"#--\", @\"#-#\", @\"##-\", @\"###\",	; bitmap\n"

           "defb	0, 1q, 2q, 3q, 7Q, 10Q, 20Q, 30Q,					; octal with suffix\n"
           "defb	0, 1o, 2o, 3o, 7O, 10O, 20O, 30O,					; octal with suffix\n"
           "defb	0, 0q1, 0q2, 0q3, 0Q7, 0Q10, 0Q20, 0Q30,			; octal with prefix\n"
           "defb	0, 0o1, 0o2, 0o3, 0O7, 0O10, 0O20, 0O30,			; octal with prefix\n"

           "defb	  0, 1, 2, 3, 9, 0ah, 0bh, 0ch, 0dh, 0EH, 0FH, 10H, 20H, 30H,	; hex with suffix\n"
           "defb	 $0, $1, $2, $3, $9, $a, $b, $c, $d, $E, $F, $10, $20, $30,		; hex with prefix\n"
           "defb	0x0, 0x1, 0x2, 0x3, 0x9, 0xa, 0xb, 0xc, 0xd, 0xE, 0xF, 0x10, 0x20, 0x30,	; hex with prefix\n"

           "defb	0b111, 0b111h,	; 7, 45329 \n"
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(20); T(30);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(20); T(30);
    IS(yylex_2(), '\n');

    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');

    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(4); T(5); T(6); T(7);
    IS(yylex_2(), '\n');

    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');

    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(11); T(12);
    T(13); T(14); T(15); T(16); T(32); T(48); IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(11); T(12);
    T(13); T(14); T(15); T(16); T(32); T(48); IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(11); T(12);
    T(13); T(14); T(15); T(16); T(32); T(48); IS(yylex_2(), '\n');

    IS(yylex_2(), BYTE); T(7); T(45329); IS(yylex_2(), '\n');

    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    opt_atoctal = true;
    SPEW_T("test.asm",
           "defb	 0, 0q1, 0q2, 0q3, 0Q7, 0Q10, 0Q20, 0Q30,	; octal with prefix\n"
           "defb	@0,  @1,  @2,  @3,  @7,  @10,  @20,  @30,	; octal with prefix\n"
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(7); T(8); T(16); T(24);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
    opt_atoctal = false;

    opt_hashhex = true;
    SPEW_T("test.asm",
           "defb	$0, $1, $2, $3, $9, $a, $b, $c, $d, $E, $F, $10, $20, $30,		; hex with prefix\n"
           "defb	#0, #1, #2, #3, #9, #a, #b, #c, #d, #E, #F, #10, #20, #30,		; hex with prefix\n"
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(11); T(12);
    T(13); T(14); T(15); T(16); T(32); T(48); IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE); T(0); T(1); T(2); T(3); T(9); T(10); T(11); T(12);
    T(13); T(14); T(15); T(16); T(32); T(48); IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
    opt_hashhex = false;

    opt_hashhex = true;
    SPEW_T("test.asm", "defb	%2, @2, #define, $define, #def, $def");
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), '%');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 2);
    IS(yylex_2(), ',');
    IS(yylex_2(), '@');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 2);
    IS(yylex_2(), ',');
    IS(yylex_2(), '#');
    IS(yylex_2(), IDENT); STR_IS(yylval.IDENT.name, "define");
    IS(yylex_2(), ',');
    IS(yylex_2(), '$');
    IS(yylex_2(), IDENT); STR_IS(yylval.IDENT.name, "define");
    IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0xdef);
    IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0xdef);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));
    opt_hashhex = false;

#undef T
}

void test_number_errors(void) {
    SPEW_T("test.asm",
           "defb	0a     \n"		// line 1
           "defb	0_     \n"		// line 2
           "defb	0.     \n"		// line 3
           "defb	0d0    \n"		// line 4
           "defb	0dx    \n"		// line 5
           "defb	0d_    \n"		// line 6
           "defb	0d.    \n"		// line 7
           "defb	 102b  \n"		// line 8
           "defb	 102b_ \n"		// line 9
           "defb	 102b. \n"		// line 10
           "defb	0b102  \n"		// line 11
           "defb 	10q_   \n"		// line 12
           "defb 	10q.   \n"		// line 13
           "defb 	108q   \n"		// line 14
           "defb	0q108  \n"		// line 15
           "defb 	10o_   \n"		// line 16
           "defb 	10o.   \n"		// line 17
           "defb 	108o   \n"		// line 18
           "defb	0o108  \n"		// line 19
           "defb	10h_   \n"		// line 20
           "defb	10h.   \n"		// line 21
           "defb	10gh   \n"		// line 22
           "defb	0x10g  \n"		// line 23
          );
    EXEC("exec_scan_error", "",
         "test.asm:1: error: invalid number\n"
         "test.asm:2: error: invalid number\n"
         "test.asm:3: error: invalid number\n"
         "test.asm:4: error: invalid number\n"
         "test.asm:5: error: invalid number\n"
         "test.asm:6: error: invalid number\n"
         "test.asm:7: error: invalid number\n"
         "test.asm:8: error: invalid number\n"
         "test.asm:9: error: invalid number\n"
         "test.asm:10: error: invalid number\n"
         "test.asm:11: error: invalid number\n"
         "test.asm:12: error: invalid number\n"
         "test.asm:13: error: invalid number\n"
         "test.asm:14: error: invalid number\n"
         "test.asm:15: error: invalid number\n"
         "test.asm:16: error: invalid number\n"
         "test.asm:17: error: invalid number\n"
         "test.asm:18: error: invalid number\n"
         "test.asm:19: error: invalid number\n"
         "test.asm:20: error: invalid number\n"
         "test.asm:21: error: invalid number\n"
         "test.asm:22: error: invalid number\n"
         "test.asm:23: error: invalid number\n"
        );
}

void test_scan_error(void) {
    SPEW_T("test.asm", "defb ````");
    EXEC("exec_scan_error", "", "test.asm:1: error: invalid character\n");
}

void test_strings(void) {
#define SQ	"'"
#define DQ	"\""
#define BS	"\\"
#define NL	"\n"
    opt_string = strC;
    SPEW_T("test.asm",
           "defb " DQ "0123" DQ NL
           "defb " DQ BS "a" BS "b" BS "e" BS "f" BS "n" BS "r" BS "t" BS "v" BS BS BS SQ
           BS DQ
           BS "?" DQ NL
           "defb " SQ BS "a" SQ "," SQ BS "b" SQ "," SQ BS "e" SQ "," SQ BS "f" SQ "," SQ
           BS "n" SQ ","
           SQ BS "r" SQ "," SQ BS "t" SQ "," SQ BS "v" SQ "," SQ BS BS SQ "," SQ BS SQ SQ
           ","
           SQ BS DQ SQ "," SQ BS "?" SQ NL
           "defb " DQ BS "1" BS "2" BS "3" BS "7" BS "17" BS "27" BS "37" BS "77" BS "177"
           BS "277"
           BS "377" BS "3771" DQ NL
           "defb " SQ BS "1" SQ "," SQ BS "2" SQ "," SQ BS "3" SQ "," SQ BS "7" SQ "," SQ
           BS "17" SQ ","
           SQ BS "27" SQ "," SQ BS "37" SQ "," SQ BS "77" SQ "," SQ BS "177" SQ ","
           SQ BS "277" SQ "," SQ BS "377" SQ NL
           "defb " DQ BS "x1" BS "X2" BS "x9" BS "XA" BS "xb" BS "XC" BS "xd" BS "XE" BS
           "xf"
           BS "X10" BS "xff" DQ NL
           "defb " SQ BS "x1" SQ "," SQ BS "X2" SQ "," SQ BS "x9" SQ "," SQ BS "XA" SQ ","
           SQ BS "xb" SQ "," SQ BS "XC" SQ "," SQ BS "xd" SQ "," SQ BS "XE" SQ ","
           SQ BS "xf" SQ "," SQ BS "X10" SQ "," SQ BS "xff" SQ NL
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "0123");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "\a\b\x1b\f\n\r\t\v\\'\"?");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 7); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 8); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 27); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 12); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 10); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 13); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 9); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 11); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0x5c); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0x27); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0x22); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 0x3f);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING,
                                  "\1" "\2" "\3" "\7" "\17" "\27" "\37" "\77" "\177" "\277" "\377" "\377" "1");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 1); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 2); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 3); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 7); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 15); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 23); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 31); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 63); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 127); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 191); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 255);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING,
                                  "\x1" "\x2" "\x9" "\xa" "\xb" "\xc" "\xd" "\xe" "\xf" "\x10" "\xff");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 1); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 2); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 9); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 10); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 11); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 12); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 13); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 14); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 15); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 16); IS(yylex_2(), ',');
    IS(yylex_2(), INTEGER); IS(yylval.INTEGER, 255);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm",
           "defb " DQ BS "400" DQ NL	// line 1
           "defb " SQ BS "400" SQ NL	// line 2
           "defb " DQ BS "x100" DQ NL	// line 3
           "defb " SQ BS "x100" SQ NL	// line 4
           "defb " SQ NL				// line 5
           "defb " DQ NL				// line 6
           "defb " SQ SQ NL			// line 7
           "defb " SQ "xx" SQ NL		// line 8
           "defb " DQ BS "_" DQ NL		// line 9
          );
    EXEC("exec_scan_error", "",
         "test.asm:1: error: octal character constant out of bounds\n"
         "test.asm:2: error: octal character constant out of bounds\n"
         "test.asm:3: error: hex character constant out of bounds\n"
         "test.asm:4: error: hex character constant out of bounds\n"
         "test.asm:5: error: unterminated character constant\n"
         "test.asm:6: error: unterminated string\n"
         "test.asm:7: error: character constant must have size 1\n"
         "test.asm:8: error: character constant must have size 1\n"
         "test.asm:9: error: bad escape sequence\n"
        );

    opt_string = strBASIC;
    SPEW_T("test.asm",
           "defb " DQ "0123" DQ NL
           "defb " DQ BS "a" BS "b" BS "e" DQ NL
           "defb " DQ BS "1" BS "2" BS "3" DQ NL
           "defb " DQ BS "x1" BS "X2" DQ NL
           "defb " DQ SQ DQ NL
           "defb " DQ "hello" DQ DQ "world" DQ DQ DQ NL
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "0123");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "a" BS "b" BS "e");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "1" BS "2" BS "3");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "x1" BS "X2");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, SQ);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "hello" DQ "world" DQ);
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm",
           "defb " SQ NL				// line 1
           "defb " DQ NL				// line 2
           "defb " DQ DQ DQ NL			// line 3
           "defb " SQ SQ NL			// line 4
           "defb " SQ "xx" SQ NL		// line 5
          );
    EXEC("exec_scan_error", "",
         "test.asm:1: error: unterminated character constant\n"
         "test.asm:2: error: unterminated string\n"
         "test.asm:3: error: unterminated string\n"
         "test.asm:4: error: character constant must have size 1\n"
         "test.asm:5: error: character constant must have size 1\n"
        );

    opt_string = strSIMPLE;
    SPEW_T("test.asm",
           "defb " DQ "0123" DQ NL
           "defb " DQ BS "a" BS "b" BS "e" DQ NL
           "defb " DQ BS "1" BS "2" BS "3" DQ NL
           "defb " DQ BS "x1" BS "X2" DQ NL
           "defb " DQ SQ DQ NL
           "defb " DQ "hello" DQ DQ "world" DQ DQ DQ NL
          );
    push_input_file("test.asm");
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "0123");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "a" BS "b" BS "e");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "1" BS "2" BS "3");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, BS "x1" BS "X2");
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, SQ);
    IS(yylex_2(), '\n');
    IS(yylex_2(), BYTE);
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "hello");
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "world");
    IS(yylex_2(), STRING); STR_IS(yylval.STRING, "");
    IS(yylex_2(), '\n');
    IS(yylex_2(), 0);
    IS(yylex_2(), 0);
    pop_input_file();
    OK(0 == remove("test.asm"));

    SPEW_T("test.asm",
           "defb " SQ NL				// line 1
           "defb " DQ NL				// line 2
           "defb " SQ SQ NL			// line 3
           "defb " SQ "xx" SQ NL		// line 4
          );
    EXEC("exec_scan_error", "",
         "test.asm:1: error: unterminated character constant\n"
         "test.asm:2: error: unterminated string\n"
         "test.asm:3: error: character constant must have size 1\n"
         "test.asm:4: error: character constant must have size 1\n"
        );

    opt_string = strC;

#undef SQ
#undef DQ
#undef BS
#undef NL
}

int exec_scan_error(void) {
    push_input_file("test.asm");
    while (yylex_2() != 0) {}
    pop_input_file();
    OK(0 == remove("test.asm"));
    return EXIT_SUCCESS;
}
