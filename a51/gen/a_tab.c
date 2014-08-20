/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 3 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:339  */

#include "common/global.h"
#include "a51.h"

#if YYDEBUG
#  define YYERROR_VERBOSE         1
#endif
#define YYSTDERR                  stdout

static void CheckCodeSeg ( void );
static UINT StringToWord ( void );

int yylex ( void );


#line 82 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "a_tab.h".  */
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
#line 20 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:355  */

  UINT     uValue;
  TIdent * pIdent;
  TExpr    expr;

#line 199 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_TMIKOV_PROG_A51_BUILD_SRC_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 214 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   361

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  141
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  221

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   325

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      79,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    85,     2,     2,     2,     2,
      80,    81,    64,    60,    83,    61,    67,    65,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    82,     2,
      58,    59,    57,     2,    84,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    62,    63,    66,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    63,    63,    63,    67,    68,    72,    73,    77,    78,
      82,    83,    84,    85,    86,    87,    88,    88,    90,    91,
      92,    93,    94,    97,   101,   106,   107,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   122,   123,
     123,   128,   129,   136,   137,   138,   140,   141,   142,   146,
     150,   162,   163,   167,   171,   172,   176,   177,   181,   182,
     183,   184,   185,   190,   197,   197,   198,   202,   202,   203,
     203,   204,   205,   209,   210,   214,   215,   219,   220,   224,
     228,   229,   230,   231,   232,   237,   238,   239,   243,   246,
     249,   248,   257,   260,   262,   266,   267,   271,   276,   276,
     285,   287,   295,   297,   299,   301,   303,   305,   307,   309,
     310,   311,   315,   315,   321,   322,   323,   325,   326,   327,
     328,   329,   330,   332,   333,   335,   336,   338,   339,   340,
     342,   344,   345,   346,   347,   348,   350,   352,   353,   354,
     355,   357
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEX_IF", "LEX_ELSE", "LEX_ENDIF",
  "LEX_IFDEF", "LEX_IFNDEF", "LEX_A", "LEX_C", "LEX_DPTR", "LEX_PC",
  "LEX_AB", "LEX_DOLLAR", "LEX_ORG", "LEX_USING", "LEX_END", "LEX_AT",
  "LEX_CSEG", "LEX_DSEG", "LEX_ISEG", "LEX_BSEG", "LEX_XSEG", "LEX_EQU",
  "LEX_SET", "LEX_CODE", "LEX_DATA", "LEX_IDATA", "LEX_BIT", "LEX_XDATA",
  "LEX_DB", "LEX_DW", "LEX_DS", "LEX_DBIT", "LEX_NOMOD51", "LEX_NOPAGING",
  "LEX_PAGELENGTH", "LEX_NOSYMBOLS", "LEX_DEBUG", "LEX_PAGEWIDTH",
  "LEX_NOTABS", "LEX_XREF", "LEX_TITLE", "LEX_NAME", "LEX_DATE",
  "LEX_INCLUDE", "LEX_EJECT", "LEX_NOLIST", "LEX_LIST", "LEX_EXTRN",
  "LEX_PUBLIC", "LEX_OR", "LEX_XOR", "LEX_AND", "LEX_NE", "LEX_LE",
  "LEX_GE", "'>'", "'<'", "'='", "'+'", "'-'", "LEX_SHL", "LEX_SHR", "'*'",
  "'/'", "LEX_MOD", "'.'", "UPLUS", "UMINUS", "LEX_NOT", "LEX_HIGH",
  "LEX_LOW", "LEX_NUMBER", "LEX_IDENT", "LEX_INSTR", "LEX_RN", "LEX_ARN",
  "LEX_STRING", "'\\n'", "'('", "')'", "':'", "','", "'@'", "'#'",
  "$accept", "file", "$@1", "prim_ctrls", "prim_ctrl", "program", "line",
  "$@2", "label", "directive", "unsup_dir", "$@3", "$@4", "pseudo_instr",
  "gen_ds", "gen_dbit", "public_list", "public_def", "extrn_list",
  "extrn_def", "seg_name", "definition", "$@5", "def_data", "$@6", "$@7",
  "db_list", "db_val", "dw_list", "dw_val", "seg_ident", "def_qual",
  "instruction", "$@8", "instr", "ops", "ops_tail", "op", "const_expr",
  "$@9", "op_expr", "expr", "$@10", "e1", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,    62,    60,    61,
      43,    45,   312,   313,    42,    47,   314,    46,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,    10,
      40,    41,    58,    44,    64,    35
};
# endif

#define YYPACT_NINF -77

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-77)))

#define YYTABLE_NINF -113

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-113)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -77,     2,   246,   -77,   -77,   -77,   -76,   -77,   -77,   -72,
     -77,   -77,   -70,   -66,   -77,   -77,   -77,   -77,   -77,   -77,
     -63,   -77,   -77,   -77,   -77,   -77,   139,   -77,   -64,   199,
     -52,   -37,   -36,   -61,   -77,   -33,   -31,   -25,   -24,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,   -23,   -22,     6,   -19,   217,   -77,   -77,   -77,   189,
     -14,   -13,    50,    -9,   -77,   -77,    51,   -77,   -77,   199,
     199,   199,   199,   199,   -77,   -77,   -77,   -77,   199,   280,
     -77,   -77,   -77,   -77,    -8,   -77,   -77,    -5,    -4,   -77,
     199,   -77,    -1,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -11,   -77,     4,   -77,
       8,   -77,   -77,   -77,   -77,    -6,   -77,   -77,    -3,   -77,
     -77,   -77,   -77,   -77,    10,   -77,   -77,   -77,   -77,   -77,
     -77,    -7,   -77,    13,   -77,    30,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   249,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   -77,   -77,   -77,   280,   -77,    31,   -77,   -77,    34,
     -77,   -77,     5,     6,    11,   -19,   -77,   -77,    36,   -77,
     -77,   -77,    58,   -77,   -77,   -77,   -77,     3,   -77,   294,
     294,   294,   191,   191,   191,   191,   191,   191,    85,    85,
     -28,   -28,    52,    52,    52,    53,    -1,   -77,    47,   -77,
      56,   -77,   -77,    32,   -77,   -77,   -77,   -77,   -77,   -77,
     -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,    25,    27,     0,    30,    34,     0,
      35,    36,     0,     0,    31,    32,    33,     7,     9,     4,
       0,    26,    98,    98,    37,    39,     0,     6,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,   112,
      98,    43,    58,    59,    60,    61,    62,    67,    69,    98,
      98,     0,     0,     0,     0,    90,    92,    10,     8,     0,
       0,     0,    45,     0,    66,    13,     0,    28,   141,     0,
       0,     0,     0,     0,   137,   139,   140,   138,     0,    99,
      29,    38,    40,    23,     0,    21,    22,     0,     0,    41,
       0,    42,   112,   112,    71,    49,    72,    50,    48,    16,
      80,    81,    82,    83,    84,    57,    46,    54,     0,    53,
      47,    51,    85,    86,    24,     0,    87,   112,   112,    11,
      15,    14,   112,    12,     0,   106,   107,   105,   108,   112,
     100,     0,   112,     0,    94,    95,    97,   111,   131,   132,
     133,   134,   135,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,    20,   113,    76,    68,    73,    75,    70,
      77,    79,     0,     0,     0,     0,    65,    63,     0,    44,
      89,   110,     0,   104,   101,   109,    88,   112,   136,   114,
     115,   116,   118,   120,   122,   121,   119,   117,   123,   124,
     125,   126,   127,   128,   129,   130,   112,   112,     0,    55,
       0,    52,    91,     0,    96,    74,    78,    17,    56,   102,
     103
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -77,   -77,   -77,   -77,   -77,   -77,    73,   -77,   -77,   131,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -41,   -77,   -35,
     -77,   -77,   -77,    26,   -77,   -77,   -77,   -44,   -77,   -51,
     108,   -77,   -77,   -77,   -77,    46,   -21,   -77,     7,   -77,
     -77,   -39,   -77,   -50
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,     2,    19,    26,    58,   172,    59,    60,
      21,    31,    32,    61,    94,    96,   110,   111,   106,   107,
      62,    63,   115,    64,    92,    93,   166,   167,   169,   170,
     108,   117,    65,   118,    66,   133,   134,   135,    28,    29,
     136,   137,    90,    79
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,   182,     3,   183,    22,   125,   126,   127,    23,   128,
      24,   125,   126,   127,    25,   128,    27,    67,    83,   138,
     139,   140,   141,   142,    47,    48,    49,    50,   143,    80,
      30,   100,   101,   102,   103,   104,   157,   158,   159,   160,
     164,    84,   219,   220,    81,    82,    85,    91,    86,    87,
      88,    98,   124,   168,   171,   109,    95,    97,    99,   125,
     126,   127,   129,   128,  -112,   120,   121,   122,   129,   184,
     123,   161,   173,   130,   162,   163,   -93,   165,   177,   130,
     105,   131,   132,   179,   174,   210,   208,   131,   132,   180,
     181,   175,   186,   185,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,  -112,  -112,   187,   206,   212,   129,   207,   213,   160,
    -113,  -112,  -112,  -112,  -112,  -112,   217,   130,  -112,  -112,
     -93,  -112,   119,    20,   211,   131,   132,   218,   209,    -3,
      33,   176,    34,    35,    36,    37,    38,   155,   156,   157,
     158,   159,   160,    39,    40,    41,   216,    42,    43,    44,
      45,    46,   215,   116,   178,     0,   214,   168,   171,    47,
      48,    49,    50,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    51,    13,    52,    14,    15,    16,    53,    54,
      33,     0,    34,    35,    36,    37,    38,     0,     0,     0,
       0,     0,     0,    39,    40,    41,     0,    42,    43,    44,
      45,    46,    68,    55,    56,     0,     0,     0,    57,    47,
      48,    49,    50,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    51,    13,    52,    14,    15,    16,    53,    54,
     112,   113,   100,   101,   102,   103,   104,   -64,   -64,   -64,
     -64,   153,   154,   155,   156,   157,   158,   159,   160,    69,
      70,     0,     0,    55,    56,     0,     0,     0,    57,    71,
      72,    73,    74,    75,     0,     0,    76,    77,     0,    78,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,     0,    14,    15,    16,     0,     0,     0,     0,   114,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     0,
     188,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160
};

static const yytype_int16 yycheck[] =
{
      39,     8,     0,    10,    80,     8,     9,    10,    80,    12,
      80,     8,     9,    10,    80,    12,    79,    81,    79,    69,
      70,    71,    72,    73,    30,    31,    32,    33,    78,    81,
      23,    25,    26,    27,    28,    29,    64,    65,    66,    67,
      90,    34,    10,    11,    81,    81,    79,    40,    79,    74,
      74,    74,     1,    92,    93,    74,    49,    50,    80,     8,
       9,    10,    65,    12,    13,    79,    79,    17,    65,    76,
      79,    79,    83,    76,    79,    79,    79,    78,   117,    76,
      74,    84,    85,   122,    80,    74,    81,    84,    85,    79,
     129,    83,    79,   132,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    60,    61,    83,    83,    79,    65,    83,    60,    67,
      67,    70,    71,    72,    73,    74,    79,    76,    77,    78,
      79,    80,    59,     2,   175,    84,    85,    81,   173,     0,
       1,   115,     3,     4,     5,     6,     7,    62,    63,    64,
      65,    66,    67,    14,    15,    16,   207,    18,    19,    20,
      21,    22,   206,    55,   118,    -1,   187,   206,   207,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    13,    74,    75,    -1,    -1,    -1,    79,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    60,    61,    62,    63,    64,    65,    66,    67,    60,
      61,    -1,    -1,    74,    75,    -1,    -1,    -1,    79,    70,
      71,    72,    73,    74,    -1,    -1,    77,    78,    -1,    80,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      44,    -1,    46,    47,    48,    -1,    -1,    -1,    -1,    82,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      81,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    87,    89,     0,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    44,    46,    47,    48,    79,    88,    90,
      95,    96,    80,    80,    80,    80,    91,    79,   124,   125,
     124,    97,    98,     1,     3,     4,     5,     6,     7,    14,
      15,    16,    18,    19,    20,    21,    22,    30,    31,    32,
      33,    43,    45,    49,    50,    74,    75,    79,    92,    94,
      95,    99,   106,   107,   109,   118,   120,    81,    13,    60,
      61,    70,    71,    72,    73,    74,    77,    78,    80,   129,
      81,    81,    81,    79,   124,    79,    79,    74,    74,   127,
     128,   124,   110,   111,   100,   124,   101,   124,    74,    80,
      25,    26,    27,    28,    29,    74,   104,   105,   116,    74,
     102,   103,    23,    24,    82,   108,   116,   117,   119,    92,
      79,    79,    17,    79,     1,     8,     9,    10,    12,    65,
      76,    84,    85,   121,   122,   123,   126,   127,   129,   129,
     129,   129,   129,   129,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    79,    79,    79,   129,    78,   112,   113,   127,   114,
     115,   127,    93,    83,    80,    83,   109,   127,   121,   127,
      79,   127,     8,    10,    76,   127,    79,    83,    81,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,    83,    83,    81,   105,
      74,   103,    79,    60,   122,   113,   115,    79,    81,    10,
      11
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    88,    87,    89,    89,    90,    90,    91,    91,
      92,    92,    92,    92,    92,    92,    93,    92,    92,    92,
      92,    92,    92,    92,    94,    95,    95,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    97,    96,    98,
      96,    99,    99,    99,    99,    99,    99,    99,    99,   100,
     101,   102,   102,   103,   104,   104,   105,   105,   106,   106,
     106,   106,   106,   107,   108,   107,   107,   110,   109,   111,
     109,   109,   109,   112,   112,   113,   113,   114,   114,   115,
     116,   116,   116,   116,   116,   117,   117,   117,   118,   118,
     119,   118,   120,   121,   121,   122,   122,   123,   125,   124,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   128,   127,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     2,     0,     2,     1,     2,     0,
       1,     2,     2,     1,     2,     2,     0,     5,     3,     3,
       3,     2,     2,     2,     2,     1,     1,     1,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     0,
       4,     2,     2,     1,     3,     1,     2,     2,     2,     1,
       1,     1,     3,     1,     1,     3,     4,     1,     1,     1,
       1,     1,     1,     3,     0,     3,     1,     0,     3,     0,
       3,     2,     2,     1,     3,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       0,     4,     1,     0,     1,     1,     3,     1,     0,     2,
       1,     2,     4,     4,     2,     1,     1,     1,     1,     2,
       2,     1,     0,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     3,     1,     1,     1,
       1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 63 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { AfterPrimaryControls(); }
#line 1501 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 88 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GetIncludeName(); yyclearin; }
#line 1507 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 89 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GenListingLine(); Include(); }
#line 1513 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 90 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { DoIf( (yyvsp[-1].expr).uValue != 0 ); }
#line 1519 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 91 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { DoIf( (yyvsp[-1].pIdent)->e.rType != RELOC_UNDEF ); }
#line 1525 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 92 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { DoIf( (yyvsp[-1].pIdent)->e.rType == RELOC_UNDEF ); }
#line 1531 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 93 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { DoElse(); }
#line 1537 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 94 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { DoEndif(); }
#line 1543 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 97 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { yyerrok; PrintError( ERR_SYNTAX ); }
#line 1549 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 101 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PutLabel( (yyvsp[-1].pIdent) ); }
#line 1555 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 106 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { bNoMod51 = TRUE; }
#line 1561 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 107 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PrintWarning( WARN_UNSUP_DIRECTIVE ); }
#line 1567 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 122 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GetToClosingBrace(); yyclearin; }
#line 1573 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 123 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GetToClosingBrace(); yyclearin; }
#line 1579 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 128 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Org( &(yyvsp[0].expr) ); }
#line 1585 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 130 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      if ((yyvsp[0].expr).uValue > 3)
        PrintError( ERR_INV_USING );
      else
        Using = (yyvsp[0].expr).uValue;
    }
#line 1596 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 136 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { YYACCEPT; }
#line 1602 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 137 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { StartSeg( (yyvsp[-2].uValue) ); Org( &(yyvsp[0].expr) ); }
#line 1608 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 138 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { StartSeg( (yyvsp[0].uValue) ); }
#line 1614 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 142 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PrintWarning( WARN_UNSUP_DIRECTIVE ); }
#line 1620 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 146 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { UseSpace( (yyvsp[0].expr).uValue ); }
#line 1626 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 151 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      if (CurSegment != O51_BSEG)
        PrintError( ERR_NOT_IN_BSEG );
      else
      {
        UseSpace( (yyvsp[0].expr).uValue );
      }
    }
#line 1639 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 167 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Public( (yyvsp[0].pIdent) ); }
#line 1645 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 176 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Extern( (yyvsp[-1].pIdent), (yyvsp[-3].uValue) ); }
#line 1651 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 177 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Extern( (yyvsp[0].pIdent), O51_NOSEG ); }
#line 1657 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 181 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_CSEG; }
#line 1663 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 182 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_DSEG; }
#line 1669 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 183 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_ISEG; }
#line 1675 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 184 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_BSEG; }
#line 1681 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 185 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_XSEG; }
#line 1687 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 191 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      // Warning on common error: "extern data(x)"
      if (_stricmp( (yyvsp[-2].pIdent)->szName, "extern" ) == 0)
        PrintWarning( WARN_EXTERN_IDENT );
      Define( (yyvsp[-2].pIdent), (yyvsp[-1].uValue), &(yyvsp[0].expr) );
    }
#line 1698 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 197 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PutLabel( (yyvsp[0].pIdent) ); }
#line 1704 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 202 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { CheckCodeSeg(); }
#line 1710 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 203 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { CheckCodeSeg(); }
#line 1716 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 214 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GenDB( &(yyvsp[0].expr) );   }
#line 1722 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 215 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GenDBString();  }
#line 1728 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 224 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { GenDW( &(yyvsp[0].expr) );   }
#line 1734 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 228 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_CSEG; }
#line 1740 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 229 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_DSEG; }
#line 1746 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 230 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_ISEG; }
#line 1752 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 231 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_BSEG; }
#line 1758 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 232 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_XSEG; }
#line 1764 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 237 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_NOSEG; }
#line 1770 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 238 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.uValue) = O51_NOSEG | 0x8000; }
#line 1776 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 243 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Compile( (yyvsp[-2].uValue) ); }
#line 1782 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 247 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PrintError( ERR_INV_OP ); }
#line 1788 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 249 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      PrintError( ERR_INV_INSTR, (yyvsp[0].pIdent)->szName );
      OpCount = 0;
    }
#line 1797 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 257 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { OpCount = 0; }
#line 1803 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 271 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { PutOp( &(yyvsp[0].expr) ); }
#line 1809 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 276 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { bNeedConst = TRUE; }
#line 1815 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 277 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      UINT dummy;
      (yyval.expr) = GetConstValue( &(yyvsp[0].expr), &dummy ) ? (yyvsp[0].expr) : EmptyExpr;
    }
#line 1824 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 286 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_RN; (yyval.expr).uValue = (yyvsp[0].uValue); }
#line 1830 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 288 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      (yyval.expr) = EmptyExpr;
      (yyval.expr).addr = OP_AT_R0;
      (yyval.expr).uValue = (yyvsp[0].uValue) & 1;
      if ((yyvsp[0].uValue) > 1)
        PrintError( ERR_INV_INDEX_REG );
    }
#line 1842 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 296 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_AT_A_DPTR; }
#line 1848 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 298 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_AT_A_PC; }
#line 1854 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 300 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_AT_DPTR; }
#line 1860 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 302 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_DPTR; }
#line 1866 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 304 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_A; }
#line 1872 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 306 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_C; }
#line 1878 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 308 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).addr = OP_AB; }
#line 1884 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 309 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); (yyval.expr).addr = OP_IMM; }
#line 1890 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 310 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); (yyval.expr).addr = OP_NOT_BIT; (yyval.expr).seg = O51_BSEG; }
#line 1896 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 311 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); (yyval.expr).addr = OP_DIR; }
#line 1902 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 315 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { bNeedConst = FALSE; }
#line 1908 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 315 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1914 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 321 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_OR ); }
#line 1920 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 322 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_XOR ); }
#line 1926 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 323 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_AND ); }
#line 1932 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 325 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '=' ); }
#line 1938 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 326 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_NE ); }
#line 1944 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 327 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '<' ); }
#line 1950 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 328 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_LE ); }
#line 1956 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 329 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '>' ); }
#line 1962 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 330 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_GE ); }
#line 1968 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 332 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '+' ); }
#line 1974 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 333 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '-' ); }
#line 1980 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 335 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_SHL ); }
#line 1986 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 336 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_SHR ); }
#line 1992 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 338 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '*' ); }
#line 1998 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 339 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '/' ); }
#line 2004 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 340 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), LEX_MOD ); }
#line 2010 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 342 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[-2].expr), &(yyvsp[0].expr), '.' ); }
#line 2016 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 344 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2022 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 345 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[0].expr), NULL, UMINUS ); }
#line 2028 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 346 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[0].expr), NULL, LEX_NOT ); }
#line 2034 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 347 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[0].expr), NULL, LEX_HIGH ); }
#line 2040 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 348 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { Expr( &(yyval.expr), &(yyvsp[0].expr), NULL, LEX_LOW ); }
#line 2046 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 350 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 2052 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 352 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).uValue = (yyvsp[0].uValue) & 0xFFFF; (yyval.expr).rType = RELOC_CONST; }
#line 2058 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 353 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).uValue = StringToWord(); (yyval.expr).rType = RELOC_CONST; }
#line 2064 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 354 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { MakeIdent( &(yyval.expr), (yyvsp[0].pIdent) ); }
#line 2070 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 356 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    { (yyval.expr) = EmptyExpr; (yyval.expr).uValue = (yyvsp[0].uValue) + (Using << 3); (yyval.expr).seg = O51_DSEG; (yyval.expr).rType = RELOC_CONST; }
#line 2076 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 358 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1646  */
    {
      (yyval.expr) = EmptyExpr;
      (yyval.expr).uValue = GetCurPos();
      (yyval.expr).seg = CurSegment;
      (yyval.expr).rType = RELOC_FRAME;
    }
#line 2087 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
    break;


#line 2091 "/home/tmikov/prog/a51/build/src/a_tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 366 "/home/tmikov/prog/a51/src/a.y" /* yacc.c:1906  */


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


