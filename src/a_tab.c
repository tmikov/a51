
/*  A Bison parser, made from a.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	LEX_IF	258
#define	LEX_ELSE	259
#define	LEX_ENDIF	260
#define	LEX_IFDEF	261
#define	LEX_IFNDEF	262
#define	LEX_A	263
#define	LEX_C	264
#define	LEX_DPTR	265
#define	LEX_PC	266
#define	LEX_AB	267
#define	LEX_DOLLAR	268
#define	LEX_ORG	269
#define	LEX_USING	270
#define	LEX_END	271
#define	LEX_AT	272
#define	LEX_CSEG	273
#define	LEX_DSEG	274
#define	LEX_ISEG	275
#define	LEX_BSEG	276
#define	LEX_XSEG	277
#define	LEX_EQU	278
#define	LEX_SET	279
#define	LEX_CODE	280
#define	LEX_DATA	281
#define	LEX_IDATA	282
#define	LEX_BIT	283
#define	LEX_XDATA	284
#define	LEX_DB	285
#define	LEX_DW	286
#define	LEX_DS	287
#define	LEX_DBIT	288
#define	LEX_NOMOD51	289
#define	LEX_NOPAGING	290
#define	LEX_PAGELENGTH	291
#define	LEX_NOSYMBOLS	292
#define	LEX_DEBUG	293
#define	LEX_PAGEWIDTH	294
#define	LEX_NOTABS	295
#define	LEX_XREF	296
#define	LEX_TITLE	297
#define	LEX_NAME	298
#define	LEX_DATE	299
#define	LEX_INCLUDE	300
#define	LEX_EJECT	301
#define	LEX_NOLIST	302
#define	LEX_LIST	303
#define	LEX_EXTRN	304
#define	LEX_PUBLIC	305
#define	LEX_OR	306
#define	LEX_XOR	307
#define	LEX_AND	308
#define	LEX_NE	309
#define	LEX_LE	310
#define	LEX_GE	311
#define	LEX_SHL	312
#define	LEX_SHR	313
#define	LEX_MOD	314
#define	UPLUS	315
#define	UMINUS	316
#define	LEX_NOT	317
#define	LEX_HIGH	318
#define	LEX_LOW	319
#define	LEX_NUMBER	320
#define	LEX_IDENT	321
#define	LEX_INSTR	322
#define	LEX_RN	323
#define	LEX_ARN	324
#define	LEX_STRING	325

#line 3 "a.y"

#include "global.h"
#include "a51.h"

#if YYDEBUG
#  define YYERROR_VERBOSE         1
#endif
#define YYSTDERR                  stdout

static void CheckCodeSeg ( void );
static UINT StringToWord ( void );

int yylex ( void );


#line 19 "a.y"
typedef union
{
  UINT     uValue;
  TIdent * pIdent;
  TExpr    expr;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		221
#define	YYFLAG		-32768
#define	YYNTBASE	86

#define YYTRANSLATE(x) ((unsigned)(x) <= 325 ? yytranslate[x] : 129)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    79,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,    85,     2,     2,     2,     2,    80,
    81,    64,    60,    83,    61,    67,    65,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    82,     2,    58,
    59,    57,     2,    84,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    62,    63,    66,    68,    69,    70,    71,    72,    73,
    74,    75,    76,    77,    78
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     5,     8,     9,    12,    14,    17,    18,    20,
    23,    26,    28,    31,    34,    35,    41,    45,    49,    53,
    56,    59,    62,    65,    67,    69,    71,    76,    81,    83,
    85,    87,    89,    91,    93,    95,    96,   101,   102,   107,
   110,   113,   115,   119,   121,   124,   127,   130,   132,   134,
   136,   140,   142,   144,   148,   153,   155,   157,   159,   161,
   163,   165,   169,   170,   174,   176,   177,   181,   182,   186,
   189,   192,   194,   198,   200,   202,   204,   208,   210,   212,
   214,   216,   218,   220,   222,   224,   226,   230,   234,   235,
   240,   242,   243,   245,   247,   251,   253,   254,   257,   259,
   262,   267,   272,   275,   277,   279,   281,   283,   286,   289,
   291,   292,   295,   299,   303,   307,   311,   315,   319,   323,
   327,   331,   335,   339,   343,   347,   351,   355,   359,   363,
   366,   369,   372,   375,   378,   382,   384,   386,   388,   390
};

static const short yyrhs[] = {    -1,
    88,    87,    90,     0,    88,    89,     0,     0,    94,    79,
     0,    79,     0,    90,    91,     0,     0,    79,     0,    93,
    91,     0,   106,    79,     0,   117,     0,    98,    79,     0,
    94,    79,     0,     0,    45,    80,    92,    81,    79,     0,
     3,   123,    79,     0,     6,    74,    79,     0,     7,    74,
    79,     0,     4,    79,     0,     5,    79,     0,     1,    79,
     0,    74,    82,     0,    34,     0,    95,     0,    35,     0,
    36,    80,   123,    81,     0,    39,    80,   123,    81,     0,
    37,     0,    46,     0,    47,     0,    48,     0,    38,     0,
    40,     0,    41,     0,     0,    42,    80,    96,    81,     0,
     0,    44,    80,    97,    81,     0,    14,   126,     0,    15,
   123,     0,    16,     0,   105,    17,   126,     0,   105,     0,
    49,   103,     0,    50,   101,     0,    43,    74,     0,   123,
     0,   123,     0,   102,     0,   101,    83,   102,     0,    74,
     0,   104,     0,   103,    83,   104,     0,   115,    80,    74,
    81,     0,    74,     0,    18,     0,    19,     0,    20,     0,
    21,     0,    22,     0,    74,   116,   126,     0,     0,    74,
   107,   108,     0,   108,     0,     0,    30,   109,   111,     0,
     0,    31,   110,   113,     0,    32,    99,     0,    33,   100,
     0,   112,     0,   111,    83,   112,     0,   126,     0,    78,
     0,   114,     0,   113,    83,   114,     0,   126,     0,    25,
     0,    26,     0,    27,     0,    28,     0,    29,     0,    23,
     0,    24,     0,   115,     0,   119,   120,    79,     0,   119,
     1,    79,     0,     0,    74,   118,   120,    79,     0,    75,
     0,     0,   121,     0,   122,     0,   122,    83,   121,     0,
   125,     0,     0,   124,   128,     0,    76,     0,    84,    76,
     0,    84,     8,    60,    10,     0,    84,     8,    60,    11,
     0,    84,    10,     0,    10,     0,     8,     0,     9,     0,
    12,     0,    85,   126,     0,    65,   126,     0,   126,     0,
     0,   127,   128,     0,   128,    51,   128,     0,   128,    52,
   128,     0,   128,    53,   128,     0,   128,    59,   128,     0,
   128,    54,   128,     0,   128,    58,   128,     0,   128,    55,
   128,     0,   128,    57,   128,     0,   128,    56,   128,     0,
   128,    60,   128,     0,   128,    61,   128,     0,   128,    62,
   128,     0,   128,    63,   128,     0,   128,    64,   128,     0,
   128,    65,   128,     0,   128,    66,   128,     0,   128,    67,
   128,     0,    60,   128,     0,    61,   128,     0,    70,   128,
     0,    71,   128,     0,    72,   128,     0,    80,   128,    81,
     0,    73,     0,    78,     0,    74,     0,    77,     0,    13,
     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    63,    64,    67,    68,    72,    73,    77,    78,    82,    83,
    84,    85,    86,    87,    88,    88,    90,    91,    92,    93,
    94,    97,   101,   106,   107,   112,   113,   114,   115,   116,
   117,   118,   119,   120,   121,   122,   123,   123,   124,   128,
   129,   136,   137,   138,   140,   141,   142,   146,   150,   162,
   163,   167,   171,   172,   176,   177,   181,   182,   183,   184,
   185,   190,   197,   198,   198,   202,   203,   203,   204,   204,
   205,   209,   210,   214,   215,   219,   220,   224,   228,   229,
   230,   231,   232,   237,   238,   239,   243,   246,   248,   253,
   257,   261,   262,   266,   267,   271,   276,   277,   285,   287,
   295,   297,   299,   301,   303,   305,   307,   309,   310,   311,
   315,   315,   321,   322,   323,   325,   326,   327,   328,   329,
   330,   332,   333,   335,   336,   338,   339,   340,   342,   344,
   345,   346,   347,   348,   350,   352,   353,   354,   355,   357
};

static const char * const yytname[] = {   "$","error","$undefined.","LEX_IF",
"LEX_ELSE","LEX_ENDIF","LEX_IFDEF","LEX_IFNDEF","LEX_A","LEX_C","LEX_DPTR","LEX_PC",
"LEX_AB","LEX_DOLLAR","LEX_ORG","LEX_USING","LEX_END","LEX_AT","LEX_CSEG","LEX_DSEG",
"LEX_ISEG","LEX_BSEG","LEX_XSEG","LEX_EQU","LEX_SET","LEX_CODE","LEX_DATA","LEX_IDATA",
"LEX_BIT","LEX_XDATA","LEX_DB","LEX_DW","LEX_DS","LEX_DBIT","LEX_NOMOD51","LEX_NOPAGING",
"LEX_PAGELENGTH","LEX_NOSYMBOLS","LEX_DEBUG","LEX_PAGEWIDTH","LEX_NOTABS","LEX_XREF",
"LEX_TITLE","LEX_NAME","LEX_DATE","LEX_INCLUDE","LEX_EJECT","LEX_NOLIST","LEX_LIST",
"LEX_EXTRN","LEX_PUBLIC","LEX_OR","LEX_XOR","LEX_AND","LEX_NE","LEX_LE","LEX_GE",
"'>'","'<'","'='","'+'","'-'","LEX_SHL","LEX_SHR","'*'","'/'","LEX_MOD","'.'",
"UPLUS","UMINUS","LEX_NOT","LEX_HIGH","LEX_LOW","LEX_NUMBER","LEX_IDENT","LEX_INSTR",
"LEX_RN","LEX_ARN","LEX_STRING","'\\n'","'('","')'","':'","','","'@'","'#'",
"file","@1","prim_ctrls","prim_ctrl","program","line","@2","label","directive",
"unsup_dir","@3","@4","pseudo_instr","gen_ds","gen_dbit","public_list","public_def",
"extrn_list","extrn_def","seg_name","definition","@5","def_data","@6","@7","db_list",
"db_val","dw_list","dw_val","seg_ident","def_qual","instruction","@8","instr",
"ops","ops_tail","op","const_expr","@9","op_expr","expr","@10","e1",""
};
#endif

static const short yyr1[] = {     0,
    87,    86,    88,    88,    89,    89,    90,    90,    91,    91,
    91,    91,    91,    91,    92,    91,    91,    91,    91,    91,
    91,    91,    93,    94,    94,    95,    95,    95,    95,    95,
    95,    95,    95,    95,    95,    96,    95,    97,    95,    98,
    98,    98,    98,    98,    98,    98,    98,    99,   100,   101,
   101,   102,   103,   103,   104,   104,   105,   105,   105,   105,
   105,   106,   107,   106,   106,   109,   108,   110,   108,   108,
   108,   111,   111,   112,   112,   113,   113,   114,   115,   115,
   115,   115,   115,   116,   116,   116,   117,   117,   118,   117,
   119,   120,   120,   121,   121,   122,   124,   123,   125,   125,
   125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
   127,   126,   128,   128,   128,   128,   128,   128,   128,   128,
   128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
   128,   128,   128,   128,   128,   128,   128,   128,   128,   128
};

static const short yyr2[] = {     0,
     0,     3,     2,     0,     2,     1,     2,     0,     1,     2,
     2,     1,     2,     2,     0,     5,     3,     3,     3,     2,
     2,     2,     2,     1,     1,     1,     4,     4,     1,     1,
     1,     1,     1,     1,     1,     0,     4,     0,     4,     2,
     2,     1,     3,     1,     2,     2,     2,     1,     1,     1,
     3,     1,     1,     3,     4,     1,     1,     1,     1,     1,
     1,     3,     0,     3,     1,     0,     3,     0,     3,     2,
     2,     1,     3,     1,     1,     1,     3,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     3,     3,     0,     4,
     1,     0,     1,     1,     3,     1,     0,     2,     1,     2,
     4,     4,     2,     1,     1,     1,     1,     2,     2,     1,
     0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
     2,     2,     2,     2,     3,     1,     1,     1,     1,     1
};

static const short yydefact[] = {     4,
     1,    24,    26,     0,    29,    33,     0,    34,    35,     0,
     0,    30,    31,    32,     6,     8,     3,     0,    25,    97,
    97,    36,    38,     0,     5,     0,     0,     0,     0,     0,
     0,    97,     0,     0,     0,     0,   111,    97,    42,    57,
    58,    59,    60,    61,    66,    68,    97,    97,     0,     0,
     0,     0,    89,    91,     9,     7,     0,     0,     0,    44,
     0,    65,    12,     0,    27,   140,     0,     0,     0,     0,
     0,   136,   138,   139,   137,     0,    98,    28,    37,    39,
    22,     0,    20,    21,     0,     0,    40,     0,    41,   111,
   111,    70,    48,    71,    49,    47,    15,    79,    80,    81,
    82,    83,    56,    45,    53,     0,    52,    46,    50,    84,
    85,    23,     0,    86,   111,   111,    10,    14,    13,   111,
    11,     0,   105,   106,   104,   107,   111,    99,     0,   111,
     0,    93,    94,    96,   110,   130,   131,   132,   133,   134,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    17,    18,
    19,   112,    75,    67,    72,    74,    69,    76,    78,     0,
     0,     0,     0,    64,    62,     0,    43,    88,   109,     0,
   103,   100,   108,    87,   111,   135,   113,   114,   115,   117,
   119,   121,   120,   118,   116,   122,   123,   124,   125,   126,
   127,   128,   129,   111,   111,     0,    54,     0,    51,    90,
     0,    95,    73,    77,    16,    55,   101,   102,     0,     0,
     0
};

static const short yydefgoto[] = {   219,
    16,     1,    17,    24,    56,   170,    57,    58,    19,    29,
    30,    59,    92,    94,   108,   109,   104,   105,    60,    61,
   113,    62,    90,    91,   164,   165,   167,   168,   106,   115,
    63,   116,    64,   131,   132,   133,    26,    27,   134,   135,
    88,    77
};

static const short yypact[] = {-32768,
   246,-32768,-32768,   -78,-32768,-32768,   -76,-32768,-32768,   -72,
   -70,-32768,-32768,-32768,-32768,-32768,-32768,   -65,-32768,-32768,
-32768,-32768,-32768,   139,-32768,   -52,   199,   -39,   -38,   -36,
   -63,-32768,   -35,   -33,   -26,   -25,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -24,   -29,
     6,   -19,   217,-32768,-32768,-32768,   189,   -21,   -14,    49,
   -12,-32768,-32768,    51,-32768,-32768,   199,   199,   199,   199,
   199,-32768,-32768,-32768,-32768,   199,   280,-32768,-32768,-32768,
-32768,    -9,-32768,-32768,    -8,    -5,-32768,   199,-32768,    -1,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -11,-32768,     4,-32768,     2,-32768,-32768,
-32768,-32768,    -6,-32768,-32768,    -3,-32768,-32768,-32768,-32768,
-32768,    -4,-32768,-32768,-32768,-32768,-32768,-32768,    -7,-32768,
    10,-32768,     8,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   249,   199,   199,   199,   199,   199,   199,   199,   199,   199,
   199,   199,   199,   199,   199,   199,   199,   199,-32768,-32768,
-32768,   280,-32768,    30,-32768,-32768,    31,-32768,-32768,     5,
     6,    18,   -19,-32768,-32768,    36,-32768,-32768,-32768,    57,
-32768,-32768,-32768,-32768,     3,-32768,   294,   294,   294,   191,
   191,   191,   191,   191,   191,    85,    85,   -28,   -28,    52,
    52,    52,    53,    -1,-32768,    39,-32768,    45,-32768,-32768,
     7,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   132,   133,
-32768
};

static const short yypgoto[] = {-32768,
-32768,-32768,-32768,-32768,    77,-32768,-32768,   136,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   -32,-32768,   -15,-32768,-32768,
-32768,    25,-32768,-32768,-32768,   -42,-32768,   -41,   110,-32768,
-32768,-32768,-32768,    50,   -20,-32768,     9,-32768,-32768,   -37,
-32768,   -48
};


#define	YYLAST		361


static const short yytable[] = {    87,
   180,    20,   181,    21,   123,   124,   125,    22,   126,    23,
   123,   124,   125,    25,   126,    81,   217,   218,   136,   137,
   138,   139,   140,    45,    46,    47,    48,   141,    65,    28,
    98,    99,   100,   101,   102,   155,   156,   157,   158,   162,
    82,    78,    79,    83,    80,    84,    89,    85,    86,    96,
    97,   122,   166,   169,   107,    93,    95,   118,   123,   124,
   125,   127,   126,  -111,   119,   120,   121,   127,   182,   159,
   160,   171,   128,   161,   178,   -92,   163,   175,   128,   103,
   129,   130,   177,   172,   173,   206,   129,   130,   184,   179,
   185,   208,   183,   187,   188,   189,   190,   191,   192,   193,
   194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
  -111,  -111,   204,   205,   210,   127,   211,   215,   158,-32768,
  -111,  -111,  -111,  -111,  -111,   216,   128,  -111,  -111,   -92,
  -111,   220,   221,   117,   129,   130,    18,   174,    -2,    31,
   209,    32,    33,    34,    35,    36,   153,   154,   155,   156,
   157,   158,    37,    38,    39,   207,    40,    41,    42,    43,
    44,   213,   114,   214,   212,   176,   166,   169,    45,    46,
    47,    48,     2,     3,     4,     5,     6,     7,     8,     9,
    10,    49,    11,    50,    12,    13,    14,    51,    52,    31,
     0,    32,    33,    34,    35,    36,     0,     0,     0,     0,
     0,     0,    37,    38,    39,     0,    40,    41,    42,    43,
    44,    66,    53,    54,     0,     0,     0,    55,    45,    46,
    47,    48,     2,     3,     4,     5,     6,     7,     8,     9,
    10,    49,    11,    50,    12,    13,    14,    51,    52,   110,
   111,    98,    99,   100,   101,   102,   -63,   -63,   -63,   -63,
   151,   152,   153,   154,   155,   156,   157,   158,    67,    68,
     0,     0,    53,    54,     0,     0,     0,    55,    69,    70,
    71,    72,    73,     0,     0,    74,    75,     0,    76,     2,
     3,     4,     5,     6,     7,     8,     9,    10,     0,    11,
     0,    12,    13,    14,     0,     0,     0,     0,   112,   142,
   143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
   153,   154,   155,   156,   157,   158,     0,     0,     0,     0,
     0,     0,     0,     0,    15,     0,     0,     0,     0,   186,
   142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
   152,   153,   154,   155,   156,   157,   158,   145,   146,   147,
   148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
   158
};

static const short yycheck[] = {    37,
     8,    80,    10,    80,     8,     9,    10,    80,    12,    80,
     8,     9,    10,    79,    12,    79,    10,    11,    67,    68,
    69,    70,    71,    30,    31,    32,    33,    76,    81,    21,
    25,    26,    27,    28,    29,    64,    65,    66,    67,    88,
    32,    81,    81,    79,    81,    79,    38,    74,    74,    74,
    80,     1,    90,    91,    74,    47,    48,    79,     8,     9,
    10,    65,    12,    13,    79,    17,    79,    65,    76,    79,
    79,    83,    76,    79,    79,    79,    78,   115,    76,    74,
    84,    85,   120,    80,    83,    81,    84,    85,    79,   127,
    83,    74,   130,   142,   143,   144,   145,   146,   147,   148,
   149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
    60,    61,    83,    83,    79,    65,    60,    79,    67,    67,
    70,    71,    72,    73,    74,    81,    76,    77,    78,    79,
    80,     0,     0,    57,    84,    85,     1,   113,     0,     1,
   173,     3,     4,     5,     6,     7,    62,    63,    64,    65,
    66,    67,    14,    15,    16,   171,    18,    19,    20,    21,
    22,   204,    53,   205,   185,   116,   204,   205,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    50,     1,
    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
    -1,    -1,    14,    15,    16,    -1,    18,    19,    20,    21,
    22,    13,    74,    75,    -1,    -1,    -1,    79,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    50,    23,
    24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
    60,    61,    62,    63,    64,    65,    66,    67,    60,    61,
    -1,    -1,    74,    75,    -1,    -1,    -1,    79,    70,    71,
    72,    73,    74,    -1,    -1,    77,    78,    -1,    80,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    -1,    44,
    -1,    46,    47,    48,    -1,    -1,    -1,    -1,    82,    51,
    52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
    62,    63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    81,
    51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    61,    62,    63,    64,    65,    66,    67,    54,    55,    56,
    57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
    67
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Ceco:
   - modified to print debug output on YYSTDERR instead of stderr so it can be redirected
   - added check if alloca() returns NULL when resizing the stack
   - mapped __yy_memcpy() to memcpy()
   - changed the definitions to ANSI style
   - Fixed the alloca() includes to support BC and VisualC
*/
#include <string.h>  // for memcpy

#ifndef alloca
  #ifdef __GNUC__
    #define alloca __builtin_alloca
  #else /* not GNU C.  */
    #if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
      #include <alloca.h>
    #else /* not sparc */
      #if defined (MSDOS) || defined (__TURBOC__) || defined(_MSC_VER)
        #include <malloc.h>
        #if defined(_MSC_VER) && !defined(alloca)
          #define alloca _alloca
        #endif   /* Visual C */
      #else /* not MSDOS, not Visual C */
        #if defined(_AIX)
          #include <malloc.h>
          #pragma alloca
        #else /* not MSDOS, or _AIX */
          #ifdef __hpux
            #ifdef __cplusplus
              extern "C" {
                void *alloca (unsigned int);
              };
            #else /* not __cplusplus */
              void *alloca ();
            #endif /* not __cplusplus */
          #endif /* __hpux */
        #endif /* not _AIX */
      #endif /* not MSDOS, not Visual C */
    #endif /* not sparc.  */
  #endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#ifndef YYSTDERR
#  define YYSTDERR stderr
#endif
#define YYNOMORESTACK   do { yyerror( "parser error: stack overflow" ); return 2; } while (0)

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#define __yy_memcpy(FROM,TO,COUNT)	memcpy(TO,FROM,COUNT)

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void
#endif

int
yyparse(YYPARSE_PARAM_DECL)
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(YYSTDERR, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      if (!yyss)
        YYNOMORESTACK;
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      if (!yyvs)
        YYNOMORESTACK;
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      if (!yyls)
        YYNOMORESTACK;
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(YYSTDERR, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(YYSTDERR, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(YYSTDERR, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(YYSTDERR, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (YYSTDERR, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (YYSTDERR, yychar, yylval);
#endif
	  fprintf (YYSTDERR, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(YYSTDERR, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (YYSTDERR, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (YYSTDERR, "%s ", yytname[yyrhs[i]]);
      fprintf (YYSTDERR, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 63 "a.y"
{ AfterPrimaryControls() ;
    break;}
case 15:
#line 88 "a.y"
{ GetIncludeName(); yyclearin; ;
    break;}
case 16:
#line 89 "a.y"
{ GenListingLine(); Include(); ;
    break;}
case 17:
#line 90 "a.y"
{ DoIf( yyvsp[-1].expr.uValue != 0 ) ;
    break;}
case 18:
#line 91 "a.y"
{ DoIf( yyvsp[-1].pIdent->e.rType != RELOC_UNDEF ) ;
    break;}
case 19:
#line 92 "a.y"
{ DoIf( yyvsp[-1].pIdent->e.rType == RELOC_UNDEF ) ;
    break;}
case 20:
#line 93 "a.y"
{ DoElse() ;
    break;}
case 21:
#line 94 "a.y"
{ DoEndif() ;
    break;}
case 22:
#line 97 "a.y"
{ yyerrok; PrintError( ERR_SYNTAX ); ;
    break;}
case 23:
#line 101 "a.y"
{ PutLabel( yyvsp[-1].pIdent ) ;
    break;}
case 24:
#line 106 "a.y"
{ bNoMod51 = TRUE ;
    break;}
case 25:
#line 107 "a.y"
{ PrintWarning( WARN_UNSUP_DIRECTIVE ) ;
    break;}
case 36:
#line 122 "a.y"
{ GetToClosingBrace(); yyclearin; ;
    break;}
case 38:
#line 123 "a.y"
{ GetToClosingBrace(); yyclearin; ;
    break;}
case 40:
#line 128 "a.y"
{ Org( &yyvsp[0].expr ) ;
    break;}
case 41:
#line 130 "a.y"
{
      if (yyvsp[0].expr.uValue > 3)
        PrintError( ERR_INV_USING );
      else
        Using = yyvsp[0].expr.uValue;
    ;
    break;}
case 42:
#line 136 "a.y"
{ YYACCEPT ;
    break;}
case 43:
#line 137 "a.y"
{ StartSeg( yyvsp[-2].uValue ); Org( &yyvsp[0].expr ) ;
    break;}
case 44:
#line 138 "a.y"
{ StartSeg( yyvsp[0].uValue ) ;
    break;}
case 47:
#line 142 "a.y"
{ PrintWarning( WARN_UNSUP_DIRECTIVE ) ;
    break;}
case 48:
#line 146 "a.y"
{ UseSpace( yyvsp[0].expr.uValue ) ;
    break;}
case 49:
#line 151 "a.y"
{
      if (CurSegment != O51_BSEG)
        PrintError( ERR_NOT_IN_BSEG );
      else
      {
        UseSpace( yyvsp[0].expr.uValue );
      }
    ;
    break;}
case 52:
#line 167 "a.y"
{ Public( yyvsp[0].pIdent ) ;
    break;}
case 55:
#line 176 "a.y"
{ Extern( yyvsp[-1].pIdent, yyvsp[-3].uValue ) ;
    break;}
case 56:
#line 177 "a.y"
{ Extern( yyvsp[0].pIdent, O51_NOSEG ) ;
    break;}
case 57:
#line 181 "a.y"
{ yyval.uValue = O51_CSEG ;
    break;}
case 58:
#line 182 "a.y"
{ yyval.uValue = O51_DSEG ;
    break;}
case 59:
#line 183 "a.y"
{ yyval.uValue = O51_ISEG ;
    break;}
case 60:
#line 184 "a.y"
{ yyval.uValue = O51_BSEG ;
    break;}
case 61:
#line 185 "a.y"
{ yyval.uValue = O51_XSEG ;
    break;}
case 62:
#line 191 "a.y"
{
      // Warning on common error: "extern data(x)"
      if (_stricmp( yyvsp[-2].pIdent->szName, "extern" ) == 0)
        PrintWarning( WARN_EXTERN_IDENT );
      Define( yyvsp[-2].pIdent, yyvsp[-1].uValue, &yyvsp[0].expr )
    ;
    break;}
case 63:
#line 197 "a.y"
{ PutLabel( yyvsp[0].pIdent ) ;
    break;}
case 66:
#line 202 "a.y"
{ CheckCodeSeg() ;
    break;}
case 68:
#line 203 "a.y"
{ CheckCodeSeg() ;
    break;}
case 74:
#line 214 "a.y"
{ GenDB( &yyvsp[0].expr );   ;
    break;}
case 75:
#line 215 "a.y"
{ GenDBString();  ;
    break;}
case 78:
#line 224 "a.y"
{ GenDW( &yyvsp[0].expr );   ;
    break;}
case 79:
#line 228 "a.y"
{ yyval.uValue = O51_CSEG ;
    break;}
case 80:
#line 229 "a.y"
{ yyval.uValue = O51_DSEG ;
    break;}
case 81:
#line 230 "a.y"
{ yyval.uValue = O51_ISEG ;
    break;}
case 82:
#line 231 "a.y"
{ yyval.uValue = O51_BSEG ;
    break;}
case 83:
#line 232 "a.y"
{ yyval.uValue = O51_XSEG ;
    break;}
case 84:
#line 237 "a.y"
{ yyval.uValue = O51_NOSEG ;
    break;}
case 85:
#line 238 "a.y"
{ yyval.uValue = O51_NOSEG | 0x8000 ;
    break;}
case 87:
#line 243 "a.y"
{ Compile( yyvsp[-2].uValue ) ;
    break;}
case 88:
#line 247 "a.y"
{ PrintError( ERR_INV_OP ) ;
    break;}
case 89:
#line 249 "a.y"
{
      PrintError( ERR_INV_INSTR, yyvsp[0].pIdent->szName );
      OpCount = 0
    ;
    break;}
case 91:
#line 257 "a.y"
{ OpCount = 0 ;
    break;}
case 96:
#line 271 "a.y"
{ PutOp( &yyvsp[0].expr ) ;
    break;}
case 97:
#line 276 "a.y"
{ bNeedConst = TRUE ;
    break;}
case 98:
#line 277 "a.y"
{
      UINT dummy;
      yyval.expr = GetConstValue( &yyvsp[0].expr, &dummy ) ? yyvsp[0].expr : EmptyExpr;
    ;
    break;}
case 99:
#line 286 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_RN; yyval.expr.uValue = yyvsp[0].uValue ;
    break;}
case 100:
#line 288 "a.y"
{
      yyval.expr = EmptyExpr;
      yyval.expr.addr = OP_AT_R0;
      yyval.expr.uValue = yyvsp[0].uValue & 1;
      if (yyvsp[0].uValue > 1)
        PrintError( ERR_INV_INDEX_REG );
    ;
    break;}
case 101:
#line 296 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_AT_A_DPTR ;
    break;}
case 102:
#line 298 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_AT_A_PC ;
    break;}
case 103:
#line 300 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_AT_DPTR ;
    break;}
case 104:
#line 302 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_DPTR ;
    break;}
case 105:
#line 304 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_A ;
    break;}
case 106:
#line 306 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_C ;
    break;}
case 107:
#line 308 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.addr = OP_AB ;
    break;}
case 108:
#line 309 "a.y"
{ yyval.expr = yyvsp[0].expr; yyval.expr.addr = OP_IMM; ;
    break;}
case 109:
#line 310 "a.y"
{ yyval.expr = yyvsp[0].expr; yyval.expr.addr = OP_NOT_BIT; yyval.expr.seg = O51_BSEG; ;
    break;}
case 110:
#line 311 "a.y"
{ yyval.expr = yyvsp[0].expr; yyval.expr.addr = OP_DIR; ;
    break;}
case 111:
#line 315 "a.y"
{ bNeedConst = FALSE; ;
    break;}
case 112:
#line 315 "a.y"
{ yyval.expr = yyvsp[0].expr ;
    break;}
case 113:
#line 321 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_OR ); ;
    break;}
case 114:
#line 322 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_XOR ); ;
    break;}
case 115:
#line 323 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_AND ); ;
    break;}
case 116:
#line 325 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '=' ); ;
    break;}
case 117:
#line 326 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_NE ); ;
    break;}
case 118:
#line 327 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '<' ); ;
    break;}
case 119:
#line 328 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_LE ); ;
    break;}
case 120:
#line 329 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '>' ); ;
    break;}
case 121:
#line 330 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_GE ); ;
    break;}
case 122:
#line 332 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '+' ); ;
    break;}
case 123:
#line 333 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '-' ); ;
    break;}
case 124:
#line 335 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_SHL ); ;
    break;}
case 125:
#line 336 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_SHR ); ;
    break;}
case 126:
#line 338 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '*' ); ;
    break;}
case 127:
#line 339 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '/' ); ;
    break;}
case 128:
#line 340 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, LEX_MOD ); ;
    break;}
case 129:
#line 342 "a.y"
{ Expr( &yyval.expr, &yyvsp[-2].expr, &yyvsp[0].expr, '.' ); ;
    break;}
case 130:
#line 344 "a.y"
{ yyval.expr = yyvsp[0].expr; ;
    break;}
case 131:
#line 345 "a.y"
{ Expr( &yyval.expr, &yyvsp[0].expr, NULL, UMINUS ); ;
    break;}
case 132:
#line 346 "a.y"
{ Expr( &yyval.expr, &yyvsp[0].expr, NULL, LEX_NOT ); ;
    break;}
case 133:
#line 347 "a.y"
{ Expr( &yyval.expr, &yyvsp[0].expr, NULL, LEX_HIGH ); ;
    break;}
case 134:
#line 348 "a.y"
{ Expr( &yyval.expr, &yyvsp[0].expr, NULL, LEX_LOW ); ;
    break;}
case 135:
#line 350 "a.y"
{ yyval.expr = yyvsp[-1].expr ;
    break;}
case 136:
#line 352 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.uValue = yyvsp[0].uValue & 0xFFFF; yyval.expr.rType = RELOC_CONST ;
    break;}
case 137:
#line 353 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.uValue = StringToWord(); yyval.expr.rType = RELOC_CONST ;
    break;}
case 138:
#line 354 "a.y"
{ MakeIdent( &yyval.expr, yyvsp[0].pIdent ); ;
    break;}
case 139:
#line 356 "a.y"
{ yyval.expr = EmptyExpr; yyval.expr.uValue = yyvsp[0].uValue + (Using << 3); yyval.expr.seg = O51_DSEG; yyval.expr.rType = RELOC_CONST ;
    break;}
case 140:
#line 358 "a.y"
{
      yyval.expr = EmptyExpr;
      yyval.expr.uValue = GetCurPos();
      yyval.expr.seg = CurSegment;
      yyval.expr.rType = RELOC_FRAME;
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 450 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (YYSTDERR, "state stack now");
      while (ssp1 != yyssp)
	fprintf (YYSTDERR, " %d", *++ssp1);
      fprintf (YYSTDERR, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(YYSTDERR, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (YYSTDERR, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (YYSTDERR, " %d", *++ssp1);
      fprintf (YYSTDERR, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(YYSTDERR, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 366 "a.y"


// This function is useless
void yyerror ( char * msg )
{
  PrintErrorStr( ERR_ERROR, msg );
};

int yylex ( void )
{
  int lex = GetLex();

  switch (lex)
  {
    case LEX_NUMBER:
      yylval.uValue = LexNumber;
      break;

    // return the code of the instruction
    case LEX_INSTR:
      ASSERT( pLexSymbol );
      yylval.uValue = pLexSymbol->code;
      break;

    // return the number of the register
    case LEX_ARN:
    case LEX_RN:
      yylval.uValue = pLexSymbol->e.uValue;
      break;

    case LEX_IDENT:
      yylval.pIdent = pLexSymbol;
      if (bGenerate && bMakeXref)
      {
        pLexSymbol->lastXref =
          AddXref( IncludeTop->pName, IncludeTop->CurLine, pLexSymbol->lastXref );
      }
      break;

    default:
      if (pLexSymbol)
        yylval.pIdent = pLexSymbol;
      break;
  }

  return lex;
};

static void CheckCodeSeg ( void )
{
  if (CurSegment != O51_CSEG)
    PrintError( ERR_NOT_IN_CSEG );
};

static UINT StringToWord ( void )
{
  // The string must be the same in the code.
  // The ordering of a word generated: HI LO, so the first byte of
  // the string is high, the second is low

  if (LexStringLen == 2)
    return (LexString[0] << 8) + LexString[1];
  if (LexStringLen == 1)
    return LexString[0];

  PrintError( ERR_INV_NUM_CONST );
  return 0;
};


