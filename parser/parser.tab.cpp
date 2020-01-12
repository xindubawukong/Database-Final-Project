/* A Bison parser, made by GNU Bison 3.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"

#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#include <cstdio>
#define __STRICT_ANSI__
#else
#include <cstdio>
#endif

#include <stdlib.h>
#include "../global.h"
#include "ast.hh"
#include <cstring>
#include "parser.yy.cpp"

int yylex();
void yyerror(const char *);

// class Tree;

#line 91 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_USERS_XDBWK_DESKTOP_THU41_DATABASE_DATABASE_FINAL_PROJECT_PARSER_PARSER_TAB_HPP_INCLUDED
# define YY_YY_USERS_XDBWK_DESKTOP_THU41_DATABASE_DATABASE_FINAL_PROJECT_PARSER_PARSER_TAB_HPP_INCLUDED
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
    QUIT = 258,
    CREATE = 259,
    DROP = 260,
    USE = 261,
    SHOW = 262,
    TABLES = 263,
    DESC = 264,
    ADD = 265,
    CHANGE = 266,
    ALTER = 267,
    CLOSE = 268,
    DATABASES = 269,
    DATABASE = 270,
    TABLE = 271,
    INDEX = 272,
    ON = 273,
    CONSTRAINT = 274,
    SELECT = 275,
    FROM = 276,
    WHERE = 277,
    IS = 278,
    NOT = 279,
    AND = 280,
    LEX_GE = 281,
    LEX_LE = 282,
    LEX_NE = 283,
    INSERT = 284,
    INTO = 285,
    VALUES = 286,
    DELETE = 287,
    SET = 288,
    UPDATE = 289,
    T_INT = 290,
    T_FLOAT = 291,
    VARCHAR = 292,
    PRIMARY = 293,
    FOREIGN = 294,
    REFERENCES = 295,
    NOTNULL = 296,
    DEFAULT = 297,
    VALUE_INT = 298,
    VALUE_FLOAT = 299,
    VALUE_STRING = 300,
    IDENTIFIER = 301,
    VALUE_NULL = 302
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 22 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"

    int ivalue;
    AttrType attrType;
    float fvalue;
    char *string;
    parser::Tree* tree;
    parser::ValueLists* valueliststree;
    parser::ValueList* valuelisttree;
    querylanguage::Value* my_value;
    CompOp compop;
    parser::Col* my_col;
    parser::Expr* my_expr;
    parser::WhereClause* whereclause;
    parser::ColumnList* columnList;
    parser::Type* type;
    parser::Field* field;
    parser::FieldList* fieldList;
    parser::Constraint* cons;
    parser::ConstraintList* consList;

#line 212 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_USERS_XDBWK_DESKTOP_THU41_DATABASE_DATABASE_FINAL_PROJECT_PARSER_PARSER_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   187

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  191

#define YYUNDEFTOK  2
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      49,    51,    53,     2,    50,     2,    54,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    48,
      56,    52,    55,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    77,    77,    81,    87,    91,    95,    99,   103,   107,
     113,   119,   127,   134,   141,   148,   154,   160,   168,   176,
     183,   190,   194,   201,   208,   214,   218,   224,   228,   235,
     239,   246,   247,   253,   257,   263,   267,   273,   277,   283,
     287,   291,   295,   301,   305,   309,   313,   317,   321,   327,
     333,   341,   346,   352,   356,   362,   366,   372,   376,   380,
     384,   388,   392,   396,   402,   407,   413,   419,   425,   431,
     439,   445,   451,   459,   465,   471,   477,   484,   489,   495,
     500,   505,   511,   517
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "QUIT", "CREATE", "DROP", "USE", "SHOW",
  "TABLES", "DESC", "ADD", "CHANGE", "ALTER", "CLOSE", "DATABASES",
  "DATABASE", "TABLE", "INDEX", "ON", "CONSTRAINT", "SELECT", "FROM",
  "WHERE", "IS", "NOT", "AND", "LEX_GE", "LEX_LE", "LEX_NE", "INSERT",
  "INTO", "VALUES", "DELETE", "SET", "UPDATE", "T_INT", "T_FLOAT",
  "VARCHAR", "PRIMARY", "FOREIGN", "REFERENCES", "NOTNULL", "DEFAULT",
  "VALUE_INT", "VALUE_FLOAT", "VALUE_STRING", "IDENTIFIER", "VALUE_NULL",
  "';'", "'('", "','", "')'", "'='", "'*'", "'.'", "'>'", "'<'", "$accept",
  "program", "stmt", "sysStmt", "dbStmt", "tbStmt", "setClause",
  "valueLists", "valueList", "selector", "cols", "col", "tableList",
  "whereClause", "op", "expr", "keyConstraints", "keyConstrint", "idxStmt",
  "alterStmt", "fieldList", "field", "type", "value", "columnList",
  "dbName", "tbName", "columnName", "fkName", "idxName", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,    59,    40,
      44,    41,    61,    42,    46,    62,    60
};
# endif

#define YYPACT_NINF (-94)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-81)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      91,   -94,   102,   111,   -38,    57,     5,   -94,    -8,    -5,
      12,   -10,    44,    91,     2,    16,    26,    43,    45,   -38,
     -10,     7,   -38,   -10,     7,   -94,   -94,   -94,   -94,   -38,
     -10,   -10,    32,   -94,    78,    52,   -94,    56,   -94,   -10,
     -10,   -94,   120,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,    66,   -94,   126,   -94,   -94,   -94,   -94,   -94,     6,
     -10,   109,   110,   127,   135,   110,   110,   -10,    29,   -12,
     110,   -19,   -94,   -94,   -94,   -94,   112,   109,   113,   107,
      -3,   -94,    94,   115,   -94,   114,   -94,   114,   116,   110,
     110,   109,   -10,    69,   117,   -13,   137,   110,    69,    41,
     -94,   -94,   -94,   119,    35,   110,   -94,   -94,   130,   110,
     -94,   -94,   137,   -94,   -94,   -94,   -94,   -94,    38,   -94,
     123,    -6,   -94,   -94,   -94,   -94,   -94,   -94,    62,   109,
     121,   -94,   125,   114,    82,   -94,   -94,   132,   124,    69,
      84,   -94,   128,    86,    69,   -94,    69,   129,   -94,   -94,
     -94,   -94,   137,    69,   110,   131,   100,   -94,   133,    69,
     -94,   110,   -94,   110,   -94,   -94,    90,   -94,   -94,    92,
     110,   -94,   -94,   -94,   -94,    95,   -94,   -94,    97,   138,
     139,   -10,   -10,   134,   136,   110,   110,    99,   101,   -94,
     -94
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     9,     0,     0,     0,     0,     0,    15,     0,     0,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,    14,    17,    10,     0,
       0,     0,    81,    31,     0,    32,    33,     0,    35,     0,
       0,    80,     0,     1,     3,     4,     5,     6,     7,     8,
      12,     0,    83,     0,    13,    20,    56,    11,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    34,    81,    36,     0,     0,    24,     0,
       0,    64,     0,     0,    58,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    22,     0,    23,     0,     0,     0,
      19,    70,    72,     0,    66,     0,    82,    60,     0,     0,
      61,    63,    21,    38,    73,    75,    74,    76,     0,    29,
       0,     0,    44,    45,    46,    43,    47,    48,     0,     0,
       0,    25,     0,     0,     0,    51,    65,     0,    67,     0,
       0,    77,     0,     0,     0,    27,     0,     0,    40,    49,
      39,    50,    42,     0,     0,     0,     0,    18,     0,     0,
      68,     0,    55,     0,    57,    30,     0,    41,    26,     0,
       0,    52,    71,    69,    78,     0,    28,    53,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -94,   168,   -94,   -94,   -94,   -94,   -94,   -94,    36,   -94,
     -94,    -4,   -94,   -89,   -94,   -94,   -94,    30,   -94,   -94,
     -94,   -67,   -94,   -93,   -85,     8,   -11,    -7,   -81,   163
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    13,    14,    15,    16,    78,    94,   118,    34,
      35,    95,    71,    96,   128,   150,   134,   135,    17,    18,
      80,    81,   104,   119,   140,    26,    37,   141,   107,    53
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,    38,   112,    91,    36,   131,   108,    87,    25,    51,
     121,    68,    55,   122,   123,   124,    69,    70,   147,    58,
      59,    31,   110,   111,   143,    39,    88,    50,    63,    64,
      54,    92,   136,    40,    74,   151,    41,    57,    32,   125,
     152,   148,   126,   127,    43,    33,   160,    99,   100,    72,
      45,   165,   155,    52,    38,    75,    83,    73,    79,    82,
     168,    86,    89,    90,    46,    27,   173,    84,    85,   169,
      38,    28,    29,    30,    47,    74,   138,   139,   175,   132,
     133,   113,    82,    82,    38,   178,   -80,    74,   144,   145,
     130,    48,    82,    49,     1,     2,     3,     4,     5,    60,
     187,   188,    61,     6,     7,   114,   115,   116,    32,   117,
      62,     8,   114,   115,   116,    66,   117,    19,    20,    21,
       9,    38,    38,    10,   149,    11,    22,    23,    24,   101,
     102,   103,   156,   157,   161,   162,   161,   164,   132,   133,
     144,   176,   161,   177,    67,   161,   179,   161,   180,   161,
     189,   161,   190,    65,   174,    32,    74,    77,    76,    98,
     106,    93,   129,    97,   105,   109,   159,   120,   137,   142,
     183,   184,   146,   153,   154,   158,   167,   163,   181,   182,
     170,    44,   166,   185,   172,   186,   171,    56
};

static const yytype_uint8 yycheck[] =
{
      11,     8,    91,    22,     8,    98,    87,    19,    46,    20,
      23,     5,    23,    26,    27,    28,    10,    11,    24,    30,
      31,    16,    89,    90,   109,    30,    38,    19,    39,    40,
      22,    50,    99,    21,    46,   128,    46,    29,    46,    52,
     129,    47,    55,    56,     0,    53,   139,    50,    51,    60,
      48,   144,   133,    46,    61,    62,    67,    61,    65,    66,
     153,    68,    69,    70,    48,     8,   159,    38,    39,   154,
      77,    14,    15,    16,    48,    46,    41,    42,   163,    38,
      39,    92,    89,    90,    91,   170,    54,    46,    50,    51,
      97,    48,    99,    48,     3,     4,     5,     6,     7,    21,
     185,   186,    50,    12,    13,    43,    44,    45,    46,    47,
      54,    20,    43,    44,    45,    49,    47,    15,    16,    17,
      29,   128,   129,    32,   128,    34,    15,    16,    17,    35,
      36,    37,    50,    51,    50,    51,    50,    51,    38,    39,
      50,    51,    50,    51,    18,    50,    51,    50,    51,    50,
      51,    50,    51,    33,   161,    46,    46,    22,    31,    52,
      46,    49,    25,    50,    49,    49,    42,    50,    49,    39,
     181,   182,    49,    52,    49,    43,    47,    49,    40,    40,
      49,    13,   146,    49,    51,    49,   156,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    12,    13,    20,    29,
      32,    34,    58,    59,    60,    61,    62,    75,    76,    15,
      16,    17,    15,    16,    17,    46,    82,     8,    14,    15,
      16,    16,    46,    53,    66,    67,    68,    83,    84,    30,
      21,    46,    83,     0,    58,    48,    48,    48,    48,    48,
      82,    83,    46,    86,    82,    83,    86,    82,    83,    83,
      21,    50,    54,    83,    83,    33,    49,    18,     5,    10,
      11,    69,    83,    68,    46,    84,    31,    22,    63,    84,
      77,    78,    84,    83,    38,    39,    84,    19,    38,    84,
      84,    22,    50,    49,    64,    68,    70,    50,    52,    50,
      51,    35,    36,    37,    79,    49,    46,    85,    85,    49,
      78,    78,    70,    83,    43,    44,    45,    47,    65,    80,
      50,    23,    26,    27,    28,    52,    55,    56,    71,    25,
      84,    80,    38,    39,    73,    74,    78,    49,    41,    42,
      81,    84,    39,    81,    50,    51,    49,    24,    47,    68,
      72,    80,    70,    52,    49,    85,    50,    51,    43,    42,
      80,    50,    51,    49,    51,    80,    65,    47,    80,    81,
      49,    74,    51,    80,    84,    81,    51,    51,    81,    51,
      51,    40,    40,    83,    83,    49,    49,    81,    81,    51,
      51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    59,    59,    59,    59,
      60,    60,    61,    61,    61,    61,    61,    61,    62,    62,
      62,    62,    62,    62,    62,    63,    63,    64,    64,    65,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      70,    70,    70,    71,    71,    71,    71,    71,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    76,    76,    76,
      76,    76,    76,    76,    77,    77,    78,    78,    78,    78,
      79,    79,    79,    80,    80,    80,    80,    81,    81,    82,
      83,    84,    85,    86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     1,
       2,     3,     3,     3,     2,     1,     3,     2,     8,     6,
       3,     6,     5,     5,     4,     3,     5,     3,     5,     1,
       3,     1,     1,     1,     3,     1,     3,     1,     3,     3,
       3,     4,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,    10,     8,     3,     8,     5,    15,
       6,     6,     5,     6,     1,     3,     2,     3,     4,     5,
       1,     4,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
          ++yyp;
          ++yyformat;
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

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
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
#line 78 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1518 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 3:
#line 82 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1526 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 4:
#line 88 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {

      }
#line 1534 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 5:
#line 92 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {

      }
#line 1542 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 6:
#line 96 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {

      }
#line 1550 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 7:
#line 100 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {

      }
#line 1558 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 8:
#line 104 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {

      }
#line 1566 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 9:
#line 108 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          YYACCEPT;
      }
#line 1574 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 10:
#line 114 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::ShowDatabase();
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
        }
#line 1584 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 11:
#line 120 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::ShowDatabase((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
        }
#line 1594 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 12:
#line 128 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::CreateDatabase((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            delete (yyvsp[0].string);
            parser::Tree::run();
        }
#line 1605 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 13:
#line 135 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::DropDatabase((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            delete (yyvsp[0].string);
            parser::Tree::run();
        }
#line 1616 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 14:
#line 142 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::UseDatabase((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            delete (yyvsp[0].string);
            parser::Tree::run();
        }
#line 1627 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 15:
#line 149 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::CloseDatabase();
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
        }
#line 1637 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 16:
#line 155 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::ShowTable((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
        }
#line 1647 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 17:
#line 161 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::ShowTable();
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
        }
#line 1657 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 18:
#line 169 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            std::cout << (yyvsp[-5].string) << std::endl;
            (yyval.tree) = new parser::CreateTable((yyvsp[-5].string), (yyvsp[-3].fieldList), (yyvsp[-1].consList));
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
            delete (yyvsp[-5].string);
        }
#line 1669 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 19:
#line 177 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::CreateTable((yyvsp[-3].string), (yyvsp[-1].fieldList));
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
            delete (yyvsp[-3].string);
        }
#line 1680 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 20:
#line 184 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.tree) = new parser::DropTable((yyvsp[0].string));
            parser::Tree::setInstance((yyval.tree));
            parser::Tree::run();
            delete (yyvsp[0].string);
        }
#line 1691 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 21:
#line 191 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1699 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 22:
#line 195 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          std::cout << "insert" << std::endl;
          (yyval.tree) = new parser::Insert((yyvsp[-2].string), (yyvsp[0].valueliststree));
          parser::Tree::setInstance((yyval.tree));
          parser::Tree::run();
        }
#line 1710 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 23:
#line 202 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          std::cout << "yacc delete" << std::endl;
          (yyval.tree) = new parser::Delete((yyvsp[-2].string), (yyvsp[0].whereclause));
          parser::Tree::setInstance((yyval.tree));
          parser::Tree::run();
        }
#line 1721 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 24:
#line 209 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1729 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 25:
#line 215 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {

                }
#line 1737 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 26:
#line 219 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {

                }
#line 1745 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 27:
#line 225 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.valueliststree) = new parser::ValueLists((yyvsp[-1].valuelisttree));
                }
#line 1753 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 28:
#line 229 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.valueliststree) = (yyvsp[-4].valueliststree);
                  (yyval.valueliststree)->AddValueList((yyvsp[-1].valuelisttree));
                }
#line 1762 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 29:
#line 236 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.valuelisttree) = new parser::ValueList((yyvsp[0].my_value));
                }
#line 1770 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 30:
#line 240 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.valuelisttree) = (yyvsp[-2].valuelisttree);
                  (yyval.valuelisttree)->AddValue((yyvsp[0].my_value));
                }
#line 1779 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 32:
#line 248 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {

                }
#line 1787 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 33:
#line 254 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1795 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 34:
#line 258 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1803 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 35:
#line 264 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.my_col) = new parser::Col(NULL, (yyvsp[0].string));
        }
#line 1811 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 36:
#line 268 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.my_col) = new parser::Col((yyvsp[-2].string), (yyvsp[0].string));
        }
#line 1819 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 37:
#line 274 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {

                }
#line 1827 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 38:
#line 278 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {

                }
#line 1835 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 39:
#line 284 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.whereclause) = new parser::WhereClause((yyvsp[-2].my_col), (yyvsp[-1].compop), (yyvsp[0].my_expr));
                }
#line 1843 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 40:
#line 288 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  // TBD
                }
#line 1851 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 41:
#line 292 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  // TBD
                }
#line 1859 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 42:
#line 296 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                  (yyval.whereclause) = new parser::WhereClause((yyvsp[-2].whereclause), (yyvsp[0].whereclause));
                }
#line 1867 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 43:
#line 302 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::EQ_OP;
        }
#line 1875 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 44:
#line 306 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::GE_OP;
        }
#line 1883 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 45:
#line 310 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::LE_OP;
        }
#line 1891 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 46:
#line 314 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::NE_OP;
        }
#line 1899 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 47:
#line 318 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::GT_OP;
        }
#line 1907 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 48:
#line 322 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.compop) = CompOp::LT_OP;
        }
#line 1915 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 49:
#line 328 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.my_expr) = new parser::Expr();
          (yyval.my_expr)->col = (yyvsp[0].my_col);
          (yyval.my_expr)->is_value = false;
        }
#line 1925 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 50:
#line 334 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
          (yyval.my_expr) = new parser::Expr();
          (yyval.my_expr)->value = (yyvsp[0].my_value);
          (yyval.my_expr)->is_value = true;
        }
#line 1935 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 51:
#line 342 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                   (yyval.consList) = new parser::ConstraintList();
                   (yyval.consList)->add((yyvsp[0].cons));
                }
#line 1944 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 52:
#line 347 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
                {
                   (yyval.consList)->add((yyvsp[0].cons));
                }
#line 1952 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 53:
#line 353 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
            {
              (yyval.cons) = new parser::Constraint(true, (yyvsp[-1].columnList), "PrimaryKey", "");
            }
#line 1960 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 54:
#line 357 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
            {
              (yyval.cons) = new parser::Constraint(false, (yyvsp[-6].columnList), (yyvsp[-8].string), (yyvsp[-3].string), (yyvsp[-1].columnList));
            }
#line 1968 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 55:
#line 363 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1976 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 56:
#line 367 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1984 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 57:
#line 373 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 1992 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 58:
#line 377 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2000 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 59:
#line 381 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2008 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 60:
#line 385 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2016 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 61:
#line 389 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2024 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 62:
#line 393 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2032 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 63:
#line 397 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {

        }
#line 2040 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 64:
#line 403 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
          {    
              (yyval.fieldList) = new parser::FieldList();
              (yyval.fieldList)->add((yyvsp[0].field));
          }
#line 2049 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 65:
#line 408 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
          {
              (yyval.fieldList)->add((yyvsp[0].field));
          }
#line 2057 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 66:
#line 414 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.field) = new parser::Field((yyvsp[-1].string), (yyvsp[0].type)->attrType, (yyvsp[0].type)->attrLength);
          delete (yyvsp[-1].string);
          delete (yyvsp[0].type);
      }
#line 2067 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 67:
#line 420 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.field) = new parser::Field((yyvsp[-2].string), (yyvsp[-1].type)->attrType, (yyvsp[-1].type)->attrLength, true);
          delete (yyvsp[-2].string);
          delete (yyvsp[-1].type);
      }
#line 2077 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 68:
#line 426 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.field) = new parser::Field((yyvsp[-3].string), (yyvsp[-2].type)->attrType, (yyvsp[-2].type)->attrLength, false, (yyvsp[0].my_value));
          delete (yyvsp[-3].string);
          delete (yyvsp[-2].type);
      }
#line 2087 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 69:
#line 432 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.field) = new parser::Field((yyvsp[-4].string), (yyvsp[-3].type)->attrType, (yyvsp[-3].type)->attrLength, true, (yyvsp[0].my_value));
          delete (yyvsp[-4].string);
          delete (yyvsp[-3].type);
      }
#line 2097 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 70:
#line 440 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.type) = new parser::Type();
          (yyval.type)->attrType = AttrType::INT;
          (yyval.type)->attrLength = 4;
      }
#line 2107 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 71:
#line 446 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {         
          (yyval.type) = new parser::Type();
          (yyval.type)->attrType = AttrType::STRING;
          (yyval.type)->attrLength = (yyvsp[-1].ivalue);
      }
#line 2117 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 72:
#line 452 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
          (yyval.type) = new parser::Type();
          (yyval.type)->attrType = AttrType::FLOAT;
          (yyval.type)->attrLength = 4;
      }
#line 2127 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 73:
#line 460 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
        (yyval.my_value) = new querylanguage::Value();
        (yyval.my_value)->type = AttrType::INT;
        (yyval.my_value)->data = new int((yyvsp[0].ivalue));
      }
#line 2137 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 74:
#line 466 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
        (yyval.my_value) = new querylanguage::Value();
        (yyval.my_value)->type = AttrType::STRING;
        (yyval.my_value)->data = (void*)(yyvsp[0].string);
      }
#line 2147 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 75:
#line 472 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
        (yyval.my_value) = new querylanguage::Value();
        (yyval.my_value)->type = AttrType::FLOAT;
        (yyval.my_value)->data = new float((yyvsp[0].fvalue));
      }
#line 2157 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 76:
#line 478 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
      {
        (yyval.my_value) = new querylanguage::Value();
        (yyval.my_value)->data = NULL;
      }
#line 2166 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 77:
#line 485 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
            {
                (yyval.columnList) = new parser::ColumnList();
                (yyval.columnList)->add((yyvsp[0].string));
            }
#line 2175 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 78:
#line 490 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
            {
                (yyval.columnList)->add((yyvsp[0].string));
            }
#line 2183 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 79:
#line 496 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.string) = (yyvsp[0].string);
        }
#line 2191 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 80:
#line 501 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.string) = (yyvsp[0].string);
        }
#line 2199 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 81:
#line 506 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
            {
                (yyval.string) = (yyvsp[0].string);
            }
#line 2207 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 82:
#line 512 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.string) = (yyvsp[0].string);
        }
#line 2215 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;

  case 83:
#line 518 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"
        {
            (yyval.string) = (yyvsp[0].string);
        }
#line 2223 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"
    break;


#line 2227 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.cpp"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 523 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.y"

void yyerror(const char *msg) {
    printf("YACC error: %s\n", msg);
}

char start_parse(const char *expr_input)
{
    char ret;
    if(expr_input){
        YY_BUFFER_STATE my_string_buffer = yy_scan_string(expr_input);
        yy_switch_to_buffer( my_string_buffer ); // switch flex to the buffer we just created
        ret = yyparse();
        yy_delete_buffer(my_string_buffer );
    }else{
        ret = yyparse();
    }
    return ret;
}
