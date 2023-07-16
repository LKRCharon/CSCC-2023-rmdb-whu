/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:339  */

#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;

#line 81 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:339  */

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
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yacc.tab.h".  */
#ifndef YY_YY_HOME_CHARON_RMDB_SRC_PARSER_YACC_TAB_H_INCLUDED
# define YY_YY_HOME_CHARON_RMDB_SRC_PARSER_YACC_TAB_H_INCLUDED
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
    SHOW = 258,
    TABLES = 259,
    CREATE = 260,
    TABLE = 261,
    DROP = 262,
    DESC = 263,
    INSERT = 264,
    INTO = 265,
    VALUES = 266,
    DELETE = 267,
    FROM = 268,
    ASC = 269,
    ORDER = 270,
    BY = 271,
    LIMIT = 272,
    WHERE = 273,
    UPDATE = 274,
    SET = 275,
    SELECT = 276,
    INT = 277,
    BIGINT = 278,
    CHAR = 279,
    FLOAT = 280,
    DATETIME = 281,
    INDEX = 282,
    AND = 283,
    JOIN = 284,
    EXIT = 285,
    HELP = 286,
    TXN_BEGIN = 287,
    TXN_COMMIT = 288,
    TXN_ABORT = 289,
    TXN_ROLLBACK = 290,
    ORDER_BY = 291,
    LEQ = 292,
    NEQ = 293,
    GEQ = 294,
    T_EOF = 295,
    IDENTIFIER = 296,
    VALUE_STRING = 297,
    VALUE_INT = 298,
    VALUE_FLOAT = 299
  };
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (void);

#endif /* !YY_YY_HOME_CHARON_RMDB_SRC_PARSER_YACC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 185 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:358  */

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
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  40
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   127

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,    53,     2,    48,     2,    49,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    45,
      51,    50,    52,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    57,    57,    62,    67,    72,    80,    81,    82,    83,
      87,    91,    95,    99,   106,   110,   117,   121,   125,   129,
     133,   140,   144,   148,   152,   159,   163,   170,   174,   181,
     188,   192,   196,   200,   204,   212,   216,   223,   227,   231,
     238,   245,   246,   253,   257,   264,   268,   275,   279,   286,
     290,   294,   298,   302,   306,   313,   317,   324,   328,   335,
     342,   346,   350,   354,   358,   365,   369,   373,   378,   385,
     386,   387,   390,   395,   399,   401
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "SHOW", "TABLES", "CREATE", "TABLE",
  "DROP", "DESC", "INSERT", "INTO", "VALUES", "DELETE", "FROM", "ASC",
  "ORDER", "BY", "LIMIT", "WHERE", "UPDATE", "SET", "SELECT", "INT",
  "BIGINT", "CHAR", "FLOAT", "DATETIME", "INDEX", "AND", "JOIN", "EXIT",
  "HELP", "TXN_BEGIN", "TXN_COMMIT", "TXN_ABORT", "TXN_ROLLBACK",
  "ORDER_BY", "LEQ", "NEQ", "GEQ", "T_EOF", "IDENTIFIER", "VALUE_STRING",
  "VALUE_INT", "VALUE_FLOAT", "';'", "'('", "')'", "','", "'.'", "'='",
  "'<'", "'>'", "'*'", "$accept", "start", "stmt", "txnStmt", "dbStmt",
  "ddl", "dml", "fieldList", "colNameList", "field", "type", "valueList",
  "value", "condition", "optWhereClause", "whereClause", "col", "colList",
  "op", "expr", "setClauses", "setClause", "selector", "tableList",
  "opt_order_clause", "order_clause", "opt_asc_desc", "optLimitClause",
  "tbName", "colName", YY_NULLPTR
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
     295,   296,   297,   298,   299,    59,    40,    41,    44,    46,
      61,    60,    62,    42
};
# endif

#define YYPACT_NINF -78

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-78)))

#define YYTABLE_NINF -75

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      42,     6,     5,     8,   -25,    10,    13,   -25,   -16,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,    29,   -11,   -78,   -78,
     -78,   -78,   -78,    27,   -25,   -25,   -25,   -25,   -78,   -78,
     -25,   -25,    24,     3,   -78,   -78,    14,    40,    22,   -78,
     -78,   -78,   -25,    48,    49,   -78,    50,    86,    80,    59,
      60,   -25,    59,   -78,    59,    59,    59,    56,    60,   -78,
     -78,    -5,   -78,    53,   -78,   -12,   -78,   -78,    33,   -78,
      61,    41,   -78,    43,    16,   -78,    76,    18,    59,   -78,
      16,   -25,   -25,    90,   -78,    59,   -78,   -78,    62,   -78,
     -78,   -78,   -78,    59,   -78,   -78,   -78,   -78,    45,   -78,
      60,   -78,   -78,   -78,   -78,   -78,   -78,    23,   -78,   -78,
     -78,   -78,    91,    89,   -78,    66,   -78,   -78,    16,   -78,
     -78,   -78,   -78,    60,    67,   -78,    64,   -78,     4,    65,
     -78,   -78,   -78,   -78,   -78,    60,     4,   -78
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       3,    10,    11,    12,    13,     5,     0,     0,     9,     6,
       7,     8,    14,     0,     0,     0,     0,     0,    74,    18,
       0,     0,     0,    75,    60,    47,    61,     0,     0,    46,
       1,     2,     0,     0,     0,    17,     0,     0,    41,     0,
       0,     0,     0,    15,     0,     0,     0,     0,     0,    22,
      75,    41,    57,     0,    48,    41,    62,    45,     0,    25,
       0,     0,    27,     0,     0,    43,    42,     0,     0,    23,
       0,     0,     0,    66,    16,     0,    30,    31,     0,    33,
      34,    29,    19,     0,    20,    39,    37,    38,     0,    35,
       0,    53,    52,    54,    49,    50,    51,     0,    58,    59,
      64,    63,     0,    73,    26,     0,    28,    21,     0,    44,
      55,    56,    40,     0,     0,    24,     0,    36,    71,    65,
      72,    32,    70,    69,    67,     0,    71,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,    58,    31,
     -78,   -78,   -77,    12,   -46,   -78,    -8,   -78,   -78,   -78,
     -78,    39,   -78,   -78,   -78,   -78,   -18,   -78,    -3,   -47
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    19,    20,    21,    68,    71,    69,
      91,    98,    99,    75,    59,    76,    77,    36,   107,   122,
      61,    62,    37,    65,   113,   129,   134,   125,    38,    39
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      35,    29,    63,   109,    32,    67,    58,    70,    72,    72,
      22,    24,   132,    58,    26,    79,    28,    81,   133,    83,
      30,    43,    44,    45,    46,    33,    31,    47,    48,    40,
     120,    63,    25,    23,    41,    27,    82,    34,    70,    53,
      42,   127,    64,    78,    49,     1,   116,     2,    66,     3,
       4,     5,   -74,    51,     6,   101,   102,   103,    95,    96,
      97,     7,    50,     8,    33,    95,    96,    97,   104,   105,
     106,    52,     9,    10,    11,    12,    13,    14,   110,   111,
      84,    85,    15,    86,    87,    88,    89,    90,    92,    93,
      94,    93,   117,   118,    54,    55,    56,    57,    58,   121,
      60,    33,    74,    80,   100,   112,   124,   123,   115,   126,
     130,   131,   119,   135,    73,   128,   114,   108,   137,     0,
       0,     0,     0,     0,     0,     0,     0,   136
};

static const yytype_int16 yycheck[] =
{
       8,     4,    49,    80,     7,    52,    18,    54,    55,    56,
       4,     6,     8,    18,     6,    61,    41,    29,    14,    65,
      10,    24,    25,    26,    27,    41,    13,    30,    31,     0,
     107,    78,    27,    27,    45,    27,    48,    53,    85,    42,
      13,   118,    50,    48,    20,     3,    93,     5,    51,     7,
       8,     9,    49,    13,    12,    37,    38,    39,    42,    43,
      44,    19,    48,    21,    41,    42,    43,    44,    50,    51,
      52,    49,    30,    31,    32,    33,    34,    35,    81,    82,
      47,    48,    40,    22,    23,    24,    25,    26,    47,    48,
      47,    48,    47,    48,    46,    46,    46,    11,    18,   107,
      41,    41,    46,    50,    28,    15,    17,    16,    46,    43,
      43,    47,   100,    48,    56,   123,    85,    78,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     7,     8,     9,    12,    19,    21,    30,
      31,    32,    33,    34,    35,    40,    55,    56,    57,    58,
      59,    60,     4,    27,     6,    27,     6,    27,    41,    82,
      10,    13,    82,    41,    53,    70,    71,    76,    82,    83,
       0,    45,    13,    82,    82,    82,    82,    82,    82,    20,
      48,    13,    49,    82,    46,    46,    46,    11,    18,    68,
      41,    74,    75,    83,    70,    77,    82,    83,    61,    63,
      83,    62,    83,    62,    46,    67,    69,    70,    48,    68,
      50,    29,    48,    68,    47,    48,    22,    23,    24,    25,
      26,    64,    47,    48,    47,    42,    43,    44,    65,    66,
      28,    37,    38,    39,    50,    51,    52,    72,    75,    66,
      82,    82,    15,    78,    63,    46,    83,    47,    48,    67,
      66,    70,    73,    16,    17,    81,    43,    66,    70,    79,
      43,    47,     8,    14,    80,    48,    70,    80
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    54,    55,    55,    55,    55,    56,    56,    56,    56,
      57,    57,    57,    57,    58,    58,    59,    59,    59,    59,
      59,    60,    60,    60,    60,    61,    61,    62,    62,    63,
      64,    64,    64,    64,    64,    65,    65,    66,    66,    66,
      67,    68,    68,    69,    69,    70,    70,    71,    71,    72,
      72,    72,    72,    72,    72,    73,    73,    74,    74,    75,
      76,    76,    77,    77,    77,    78,    78,    79,    79,    80,
      80,    80,    81,    81,    82,    83
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     6,     3,     2,     6,
       6,     7,     4,     5,     7,     1,     3,     1,     3,     2,
       1,     1,     4,     1,     1,     1,     3,     1,     1,     1,
       3,     0,     2,     1,     3,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       1,     1,     1,     3,     3,     3,     0,     2,     4,     1,
       1,     0,     2,     0,     1,     1
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
      yyerror (&yylloc, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc);
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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 58 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        parse_tree = (yyvsp[-1].sv_node);
        YYACCEPT;
    }
#line 1462 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 63 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
#line 1471 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 68 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1480 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 73 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1489 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 88 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<TxnBegin>();
    }
#line 1497 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 92 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<TxnCommit>();
    }
#line 1505 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 96 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<TxnAbort>();
    }
#line 1513 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 100 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<TxnRollback>();
    }
#line 1521 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 107 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<ShowTables>();
    }
#line 1529 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 111 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
    }
#line 1537 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 118 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<CreateTable>((yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
    }
#line 1545 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 122 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
    }
#line 1553 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 126 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
    }
#line 1561 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 130 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<CreateIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1569 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 134 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1577 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 141 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-4].sv_str), (yyvsp[-1].sv_vals));
    }
#line 1585 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 145 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str), (yyvsp[0].sv_conds));
    }
#line 1593 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 149 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<UpdateStmt>((yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses), (yyvsp[0].sv_conds));
    }
#line 1601 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 153 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_node) = std::make_shared<SelectStmt>((yyvsp[-5].sv_cols), (yyvsp[-3].sv_strs), (yyvsp[-2].sv_conds), (yyvsp[-1].sv_orderby),(yyvsp[0].sv_limit));
    }
#line 1609 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 160 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_fields) = std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
    }
#line 1617 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 164 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_fields).push_back((yyvsp[0].sv_field));
    }
#line 1625 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 171 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1633 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 175 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1641 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 182 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str), (yyvsp[0].sv_type_len));
    }
#line 1649 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 189 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
#line 1657 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 193 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_BIGINT, sizeof(long long));
    }
#line 1665 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 197 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1673 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 201 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(double));
    }
#line 1681 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 205 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_DATETIME, 8);
    }
#line 1689 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 213 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_vals) = std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
    }
#line 1697 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 217 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_vals).push_back((yyvsp[0].sv_val));
    }
#line 1705 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 224 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
    }
#line 1713 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 228 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
    }
#line 1721 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 232 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
    }
#line 1729 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 239 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_cond) = std::make_shared<BinaryExpr>((yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
    }
#line 1737 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 245 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { /* ignore*/ }
#line 1743 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 247 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 1751 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 254 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_conds) = std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
    }
#line 1759 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 258 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
    }
#line 1767 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 265 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 1775 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 269 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
    }
#line 1783 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 276 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
    }
#line 1791 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 280 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_cols).push_back((yyvsp[0].sv_col));
    }
#line 1799 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 287 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_EQ;
    }
#line 1807 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 291 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_LT;
    }
#line 1815 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 295 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_GT;
    }
#line 1823 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 299 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_NE;
    }
#line 1831 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 303 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_LE;
    }
#line 1839 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 307 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_comp_op) = SV_OP_GE;
    }
#line 1847 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 314 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
    }
#line 1855 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 318 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
    }
#line 1863 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 325 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{(yyvsp[0].sv_set_clause)};
    }
#line 1871 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 329 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
    }
#line 1879 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 336 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_set_clause) = std::make_shared<SetClause>((yyvsp[-2].sv_str), (yyvsp[0].sv_val));
    }
#line 1887 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 343 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_cols) = {};
    }
#line 1895 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 351 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1903 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 355 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1911 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 359 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1919 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 366 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { 
        (yyval.sv_orderby) = (yyvsp[0].sv_orderby); 
    }
#line 1927 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 369 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { /* ignore*/ }
#line 1933 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 374 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { 
          (yyval.sv_orderby) = std::vector<std::shared_ptr<OrderBy>>();
          (yyval.sv_orderby).push_back(std::make_shared<ast::OrderBy>((yyvsp[-1].sv_col),(yyvsp[0].sv_orderby_dir)));
    }
#line 1942 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 379 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_orderby).push_back(std::make_shared<ast::OrderBy>((yyvsp[-1].sv_col),(yyvsp[0].sv_orderby_dir)));
    }
#line 1950 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 385 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { (yyval.sv_orderby_dir) = OrderBy_ASC;     }
#line 1956 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 386 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { (yyval.sv_orderby_dir) = OrderBy_DESC;    }
#line 1962 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 387 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    { (yyval.sv_orderby_dir) = OrderBy_DEFAULT; }
#line 1968 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 391 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_limit)=(yyvsp[0].sv_int);
    }
#line 1976 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 395 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1646  */
    {
        (yyval.sv_limit) = 0;
    }
#line 1984 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
    break;


#line 1988 "/home/charon/rmdb/src/parser/yacc.tab.cpp" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, YY_("syntax error"));
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
        yyerror (&yylloc, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
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
#line 402 "/home/charon/rmdb/src/parser/yacc.y" /* yacc.c:1906  */

