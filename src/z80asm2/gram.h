/* A Bison parser, made by GNU Bison 3.6.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENT = 258,                   /* IDENT  */
    INTEGER = 259,                 /* INTEGER  */
    LABEL = 260,                   /* LABEL  */
    STRING = 261,                  /* STRING  */
    A = 262,                       /* A  */
    A_ = 263,                      /* A_  */
    F = 264,                       /* F  */
    F_ = 265,                      /* F_  */
    AF = 266,                      /* AF  */
    AF_ = 267,                     /* AF_  */
    B = 268,                       /* B  */
    B_ = 269,                      /* B_  */
    C = 270,                       /* C  */
    C_ = 271,                      /* C_  */
    BC = 272,                      /* BC  */
    BC_ = 273,                     /* BC_  */
    D = 274,                       /* D  */
    D_ = 275,                      /* D_  */
    E = 276,                       /* E  */
    E_ = 277,                      /* E_  */
    DE = 278,                      /* DE  */
    DE_ = 279,                     /* DE_  */
    H = 280,                       /* H  */
    H_ = 281,                      /* H_  */
    L = 282,                       /* L  */
    L_ = 283,                      /* L_  */
    HL = 284,                      /* HL  */
    HL_ = 285,                     /* HL_  */
    IX = 286,                      /* IX  */
    IXH = 287,                     /* IXH  */
    IXL = 288,                     /* IXL  */
    IY = 289,                      /* IY  */
    IYH = 290,                     /* IYH  */
    IYL = 291,                     /* IYL  */
    SP = 292,                      /* SP  */
    I = 293,                       /* I  */
    R = 294,                       /* R  */
    NC = 295,                      /* NC  */
    NZ = 296,                      /* NZ  */
    Z = 297,                       /* Z  */
    PO = 298,                      /* PO  */
    PE = 299,                      /* PE  */
    P = 300,                       /* P  */
    M = 301,                       /* M  */
    DJNZ = 302,                    /* DJNZ  */
    EX = 303,                      /* EX  */
    HALT = 304,                    /* HALT  */
    LD = 305,                      /* LD  */
    NOP = 306,                     /* NOP  */
    RET = 307,                     /* RET  */
    BYTE = 308,                    /* BYTE  */
    WORD = 309,                    /* WORD  */
    LOR = 310,                     /* LOR  */
    LXOR = 312,                    /* LXOR  */
    LAND = 314,                    /* LAND  */
    NE = 316,                      /* NE  */
    LE = 318,                      /* LE  */
    GE = 320,                      /* GE  */
    LS = 322,                      /* LS  */
    RS = 324,                      /* RS  */
    UPLUS = 326,                   /* UPLUS  */
    UMINUS = 327,                  /* UMINUS  */
    POW = 328                      /* POW  */
};
typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE {
    const char* LABEL;                       /* LABEL  */
    const char* STRING;                      /* STRING  */
    ident_t IDENT;                           /* IDENT  */
    int INTEGER;                             /* INTEGER  */


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAM_H_INCLUDED  */
