/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_HOME_TMIKOV_PROG_A51_BUILD_SRC_A_TAB_H_INCLUDED
# define YY_YY_HOME_TMIKOV_PROG_A51_BUILD_SRC_A_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LEX_IF = 258,
    LEX_ELSE = 259,
    LEX_ENDIF = 260,
    LEX_IFDEF = 261,
    LEX_IFNDEF = 262,
    LEX_A = 263,
    LEX_C = 264,
    LEX_DPTR = 265,
    LEX_PC = 266,
    LEX_AB = 267,
    LEX_DOLLAR = 268,
    LEX_ORG = 269,
    LEX_USING = 270,
    LEX_END = 271,
    LEX_AT = 272,
    LEX_CSEG = 273,
    LEX_DSEG = 274,
    LEX_ISEG = 275,
    LEX_BSEG = 276,
    LEX_XSEG = 277,
    LEX_EQU = 278,
    LEX_SET = 279,
    LEX_CODE = 280,
    LEX_DATA = 281,
    LEX_IDATA = 282,
    LEX_BIT = 283,
    LEX_XDATA = 284,
    LEX_DB = 285,
    LEX_DW = 286,
    LEX_DS = 287,
    LEX_DBIT = 288,
    LEX_NOMOD51 = 289,
    LEX_NOPAGING = 290,
    LEX_PAGELENGTH = 291,
    LEX_NOSYMBOLS = 292,
    LEX_DEBUG = 293,
    LEX_PAGEWIDTH = 294,
    LEX_NOTABS = 295,
    LEX_XREF = 296,
    LEX_TITLE = 297,
    LEX_NAME = 298,
    LEX_DATE = 299,
    LEX_INCLUDE = 300,
    LEX_EJECT = 301,
    LEX_NOLIST = 302,
    LEX_LIST = 303,
    LEX_EXTRN = 304,
    LEX_PUBLIC = 305,
    LEX_OR = 306,
    LEX_XOR = 307,
    LEX_AND = 308,
    LEX_NE = 309,
    LEX_LE = 310,
    LEX_GE = 311,
    LEX_SHL = 312,
    LEX_SHR = 313,
    LEX_MOD = 314,
    UPLUS = 315,
    UMINUS = 316,
    LEX_NOT = 317,
    LEX_HIGH = 318,
    LEX_LOW = 319,
    LEX_NUMBER = 320,
    LEX_IDENT = 321,
    LEX_INSTR = 322,
    LEX_RN = 323,
    LEX_ARN = 324,
    LEX_STRING = 325
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 20 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1909  */

  UINT     uValue;
  TIdent * pIdent;
  TExpr    expr;

#line 131 "/home/tmikov/prog/a51/build/src/a_tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_TMIKOV_PROG_A51_BUILD_SRC_A_TAB_H_INCLUDED  */
