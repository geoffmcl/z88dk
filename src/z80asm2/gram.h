/* A Bison parser, made by GNU Bison 3.5.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_GRAM_H_INCLUDED
# define YY_YY_GRAM_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

#include "asm.h"
#include "lex.h"
#include "parse.h"


/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype {
    IDENT = 258,
    INTEGER = 259,
    LABEL = 260,
    STRING = 261,
    A = 262,
    A_ = 263,
    F = 264,
    F_ = 265,
    AF = 266,
    AF_ = 267,
    B = 268,
    B_ = 269,
    C = 270,
    C_ = 271,
    BC = 272,
    BC_ = 273,
    D = 274,
    D_ = 275,
    E = 276,
    E_ = 277,
    DE = 278,
    DE_ = 279,
    H = 280,
    H_ = 281,
    L = 282,
    L_ = 283,
    HL = 284,
    HL_ = 285,
    IX = 286,
    IXH = 287,
    IXL = 288,
    IY = 289,
    IYH = 290,
    IYL = 291,
    SP = 292,
    I = 293,
    R = 294,
    NC = 295,
    NZ = 296,
    Z = 297,
    PO = 298,
    PE = 299,
    P = 300,
    M = 301,
    DJNZ = 302,
    EX = 303,
    HALT = 304,
    LD = 305,
    NOP = 306,
    RET = 307,
    BYTE = 308,
    WORD = 309,
    LOR = 310,
    LXOR = 312,
    LAND = 314,
    NE = 316,
    LE = 318,
    GE = 320,
    LS = 322,
    RS = 324,
    UPLUS = 326,
    UMINUS = 327,
    POW = 328
};
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE {

    /* LABEL  */
    const char* LABEL;
    /* STRING  */
    const char* STRING;
    /* IDENT  */
    ident_t IDENT;
    /* INTEGER  */
    int INTEGER;

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAM_H_INCLUDED  */
