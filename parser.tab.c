/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <math.h>
    #include "symbolTable.h"
    #include "quadruplet.h"
    #include "pile.h"
    #include  <ctype.h>



    //declarations 

    typedef struct {
    int baseType;      // ENTIER, FLOTTANT, STRING, etc.
    bool isArray;      // Whether it's an array
    bool isConst;      // Whether it's a constant
    int arraySize;     // Size if it's an array
    char* structName;  // Name if it's a struct
} TypeInfo;
    TableSymbole *TS;
    qTable *TQ;
    qPile *P;
    

    qNoeud* quad;
    Symbole* node;
    char* currentFunctionCalled=NULL;
    ParametresList* currentParametresList;
    int sauvLabel;
    char* saveFunctionDec=NULL;
    bool itReturn=false;
    int sauvDebut;
    int sauvFin;

    int qC = 0;
    

    extern int colnum ;
    extern int lignenum ;

    extern FILE *yyin;
    int yylex();
    void yyerror(const char *s);
    int line = 1;        // Numéro de ligne courant
    int linecol = 0;         // Numéro de colonne courant
    char *yyin_filename = NULL;
    int sauv = 0;
    int sauvline = 1;

    #define TYPE_ENTIER "ENTIER"
    #define TYPE_FLOTTANT "FLOTTANT"
    #define TYPE_CHAR "CHAR"
    #define TYPE_STRING "STRING"
    #define TYPE_BOOLEAN "BOOLEAN"

// Modified type checking function to work with your symbol table
bool areTypesCompatible(const char* type1, const char* type2) {
    if (strcmp(type1, type2) == 0) {
        return true;
    }
    
    // Allow implicit conversion from int to float
    if (strcmp(type1, TYPE_FLOTTANT) == 0 && strcmp(type2, TYPE_ENTIER) == 0) {
        return true;
    }
    
    return false;
}

// Modified function to check if an identifier is declared
bool isDeclared(const char* id) {
    Symbole* found;
    return rechercherSymbole(TS, id, &found);
}

// Function to get type of an identifier
char* getIdentifierType(const char* id) {
    Symbole* found;
    if (rechercherSymbole(TS, id, &found)) {
        return found->type;
    }
    return NULL;
}

// Function to determine if a literal is valid for a given type
bool isValidLiteralForType(const char* literal, const char* type) {
    if (strcmp(type, TYPE_ENTIER) == 0) {
        // Check if string contains only digits
        for (int i = 0; literal[i] != '\0'; i++) {
            if (!isdigit(literal[i])) return false;
        }
        return true;
    }
    else if (strcmp(type, TYPE_FLOTTANT) == 0) {
        // Check if string is a valid float
        char* endptr;
        strtof(literal, &endptr);
        return *endptr == '\0';
    }
    else if (strcmp(type, TYPE_BOOLEAN) == 0) {
        // Check if string is "true" or "false"
        return (strcmp(literal, "true") == 0 || strcmp(literal, "false") == 0);
    }
    // Add more type checks as needed
    return false;
}
TypeInfo currentType; 
    char currentFunction[256]; 
    bool inLoop = false; 
    int loopNestingLevel = 0;
void semanticError(const char* message, int line) { 
    fprintf(stderr, "Semantic error at line %d: %s\n", line, message); 
    exit(1);
}
ParametresList* creerParametresList() {
    ParametresList* list = (ParametresList*)malloc(sizeof(ParametresList));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

bool ajouterParametreUnion(ParametresList* list, const char* nom, const char* type) {
    // Check if the parameter already exists
    ParametreNode* current = list->head;
    while (current) {
        if (strcmp(current->parametre.nom, nom) == 0) {
            return false; // Parameter already exists
        }
        current = current->next;
    }

    // Add the new parameter
    ParametreNode* newNode = (ParametreNode*)malloc(sizeof(ParametreNode));
    newNode->parametre.nom = strdup(nom);
    newNode->parametre.type = strdup(type);
    newNode->next = NULL;

    if (list->tail) {
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }
    list->tail = newNode;
    list->count++;
    return true;
}

void libererParametresList(ParametresList* list) {
    ParametreNode* current = list->head;
    while (current) {
        ParametreNode* next = current->next;
        free(current->parametre.nom);
        free(current->parametre.type);
        free(current);
        current = next;
    }
    free(list);
}



#line 237 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DEBUT = 3,                      /* DEBUT  */
  YYSYMBOL_FIN = 4,                        /* FIN  */
  YYSYMBOL_WHILE = 5,                      /* WHILE  */
  YYSYMBOL_FOR = 6,                        /* FOR  */
  YYSYMBOL_PRINT = 7,                      /* PRINT  */
  YYSYMBOL_INPUT = 8,                      /* INPUT  */
  YYSYMBOL_FONCTION = 9,                   /* FONCTION  */
  YYSYMBOL_VIDE = 10,                      /* VIDE  */
  YYSYMBOL_RETURN = 11,                    /* RETURN  */
  YYSYMBOL_COMMENT = 12,                   /* COMMENT  */
  YYSYMBOL_COMMENT_PLUS = 13,              /* COMMENT_PLUS  */
  YYSYMBOL_OR = 14,                        /* OR  */
  YYSYMBOL_AND = 15,                       /* AND  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_ELSE = 18,                      /* ELSE  */
  YYSYMBOL_ELIF = 19,                      /* ELIF  */
  YYSYMBOL_DEB_CORPS = 20,                 /* DEB_CORPS  */
  YYSYMBOL_FIN_CORPS = 21,                 /* FIN_CORPS  */
  YYSYMBOL_TRUE = 22,                      /* TRUE  */
  YYSYMBOL_FALSE = 23,                     /* FALSE  */
  YYSYMBOL_PAR_OUV = 24,                   /* PAR_OUV  */
  YYSYMBOL_PAR_FERM = 25,                  /* PAR_FERM  */
  YYSYMBOL_CARACTERE = 26,                 /* CARACTERE  */
  YYSYMBOL_CHAINE = 27,                    /* CHAINE  */
  YYSYMBOL_INT = 28,                       /* INT  */
  YYSYMBOL_FLOAT = 29,                     /* FLOAT  */
  YYSYMBOL_DEB_TABLEAU = 30,               /* DEB_TABLEAU  */
  YYSYMBOL_FIN_TABLEAU = 31,               /* FIN_TABLEAU  */
  YYSYMBOL_TABLE = 32,                     /* TABLE  */
  YYSYMBOL_ENREGISTREMENT = 33,            /* ENREGISTREMENT  */
  YYSYMBOL_POINTEUR = 34,                  /* POINTEUR  */
  YYSYMBOL_CONST = 35,                     /* CONST  */
  YYSYMBOL_ENTIER = 36,                    /* ENTIER  */
  YYSYMBOL_FLOTTANT = 37,                  /* FLOTTANT  */
  YYSYMBOL_STRING = 38,                    /* STRING  */
  YYSYMBOL_CHAR = 39,                      /* CHAR  */
  YYSYMBOL_BOOLEAN = 40,                   /* BOOLEAN  */
  YYSYMBOL_ID = 41,                        /* ID  */
  YYSYMBOL_INCREM = 42,                    /* INCREM  */
  YYSYMBOL_DECREM = 43,                    /* DECREM  */
  YYSYMBOL_PLUS = 44,                      /* PLUS  */
  YYSYMBOL_DIV = 45,                       /* DIV  */
  YYSYMBOL_MOINS = 46,                     /* MOINS  */
  YYSYMBOL_MULT = 47,                      /* MULT  */
  YYSYMBOL_MOD = 48,                       /* MOD  */
  YYSYMBOL_PUISS = 49,                     /* PUISS  */
  YYSYMBOL_ASSIGN = 50,                    /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 51,                 /* SEMICOLON  */
  YYSYMBOL_INF = 52,                       /* INF  */
  YYSYMBOL_INF_EGAL = 53,                  /* INF_EGAL  */
  YYSYMBOL_SUP = 54,                       /* SUP  */
  YYSYMBOL_SUPP_EGAL = 55,                 /* SUPP_EGAL  */
  YYSYMBOL_EQUAL = 56,                     /* EQUAL  */
  YYSYMBOL_NOT_EQUAL = 57,                 /* NOT_EQUAL  */
  YYSYMBOL_FROM = 58,                      /* FROM  */
  YYSYMBOL_TO = 59,                        /* TO  */
  YYSYMBOL_VIRGULE = 60,                   /* VIRGULE  */
  YYSYMBOL_EGALE = 61,                     /* EGALE  */
  YYSYMBOL_PASEGALE = 62,                  /* PASEGALE  */
  YYSYMBOL_MUL = 63,                       /* MUL  */
  YYSYMBOL_YYACCEPT = 64,                  /* $accept  */
  YYSYMBOL_programme = 65,                 /* programme  */
  YYSYMBOL_66_1 = 66,                      /* $@1  */
  YYSYMBOL_type = 67,                      /* type  */
  YYSYMBOL_tableau = 68,                   /* tableau  */
  YYSYMBOL_type_Struct = 69,               /* type_Struct  */
  YYSYMBOL_valeur = 70,                    /* valeur  */
  YYSYMBOL_variable = 71,                  /* variable  */
  YYSYMBOL_expression = 72,                /* expression  */
  YYSYMBOL_incrementation = 73,            /* incrementation  */
  YYSYMBOL_declarations = 74,              /* declarations  */
  YYSYMBOL_declaration = 75,               /* declaration  */
  YYSYMBOL_76_2 = 76,                      /* $@2  */
  YYSYMBOL_functions = 77,                 /* functions  */
  YYSYMBOL_fonction = 78,                  /* fonction  */
  YYSYMBOL_79_3 = 79,                      /* $@3  */
  YYSYMBOL_declarationfonction = 80,       /* declarationfonction  */
  YYSYMBOL_parametres = 81,                /* parametres  */
  YYSYMBOL_parametre = 82,                 /* parametre  */
  YYSYMBOL_instructions = 83,              /* instructions  */
  YYSYMBOL_instruction = 84,               /* instruction  */
  YYSYMBOL_read = 85,                      /* read  */
  YYSYMBOL_write = 86,                     /* write  */
  YYSYMBOL_retourner = 87,                 /* retourner  */
  YYSYMBOL_assignment = 88,                /* assignment  */
  YYSYMBOL_89_4 = 89,                      /* $@4  */
  YYSYMBOL_90_5 = 90,                      /* $@5  */
  YYSYMBOL_condition = 91,                 /* condition  */
  YYSYMBOL_ifstatement = 92,               /* ifstatement  */
  YYSYMBOL_loop = 93,                      /* loop  */
  YYSYMBOL_while_partie_une = 94,          /* while_partie_une  */
  YYSYMBOL_while_partie_deux = 95,         /* while_partie_deux  */
  YYSYMBOL_while_partie_trois = 96,        /* while_partie_trois  */
  YYSYMBOL_for_partie_une = 97,            /* for_partie_une  */
  YYSYMBOL_for_partie_deux = 98,           /* for_partie_deux  */
  YYSYMBOL_for_partie_trois = 99,          /* for_partie_trois  */
  YYSYMBOL_corps = 100,                    /* corps  */
  YYSYMBOL_elsebloc = 101,                 /* elsebloc  */
  YYSYMBOL_elsestatement = 102,            /* elsestatement  */
  YYSYMBOL_elifstatement = 103,            /* elifstatement  */
  YYSYMBOL_elifkey = 104,                  /* elifkey  */
  YYSYMBOL_call = 105,                     /* call  */
  YYSYMBOL_106_6 = 106,                    /* $@6  */
  YYSYMBOL_parametresCall = 107,           /* parametresCall  */
  YYSYMBOL_parametreCall = 108             /* parametreCall  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   395

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  209

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   318


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   241,   241,   241,   259,   265,   271,   277,   283,   289,
     297,   329,   331,   337,   343,   349,   353,   357,   363,   387,
     412,   456,   457,   458,   508,   558,   608,   667,   712,   758,
     799,   806,   863,   921,   978,  1036,  1099,  1160,  1206,  1255,
    1300,  1347,  1349,  1353,  1353,  1383,  1384,  1385,  1421,  1423,
    1426,  1426,  1461,  1494,  1518,  1521,  1529,  1541,  1556,  1559,
    1561,  1563,  1566,  1567,  1568,  1569,  1570,  1571,  1572,  1573,
    1574,  1575,  1576,  1579,  1583,  1584,  1585,  1589,  1619,  1619,
    1684,  1684,  1742,  1755,  1772,  1773,  1777,  1787,  1802,  1820,
    1830,  1857,  1874,  1875,  1878,  1885,  1893,  1897,  1910,  1924,
    1940,  1939,  2002,  2004,  2018,  2033,  2034
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DEBUT", "FIN",
  "WHILE", "FOR", "PRINT", "INPUT", "FONCTION", "VIDE", "RETURN",
  "COMMENT", "COMMENT_PLUS", "OR", "AND", "NOT", "IF", "ELSE", "ELIF",
  "DEB_CORPS", "FIN_CORPS", "TRUE", "FALSE", "PAR_OUV", "PAR_FERM",
  "CARACTERE", "CHAINE", "INT", "FLOAT", "DEB_TABLEAU", "FIN_TABLEAU",
  "TABLE", "ENREGISTREMENT", "POINTEUR", "CONST", "ENTIER", "FLOTTANT",
  "STRING", "CHAR", "BOOLEAN", "ID", "INCREM", "DECREM", "PLUS", "DIV",
  "MOINS", "MULT", "MOD", "PUISS", "ASSIGN", "SEMICOLON", "INF",
  "INF_EGAL", "SUP", "SUPP_EGAL", "EQUAL", "NOT_EQUAL", "FROM", "TO",
  "VIRGULE", "EGALE", "PASEGALE", "MUL", "$accept", "programme", "$@1",
  "type", "tableau", "type_Struct", "valeur", "variable", "expression",
  "incrementation", "declarations", "declaration", "$@2", "functions",
  "fonction", "$@3", "declarationfonction", "parametres", "parametre",
  "instructions", "instruction", "read", "write", "retourner",
  "assignment", "$@4", "$@5", "condition", "ifstatement", "loop",
  "while_partie_une", "while_partie_deux", "while_partie_trois",
  "for_partie_une", "for_partie_deux", "for_partie_trois", "corps",
  "elsebloc", "elsestatement", "elifstatement", "elifkey", "call", "$@6",
  "parametresCall", "parametreCall", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-128)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-101)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -128,     7,  -128,  -128,     9,    -4,   -15,    82,  -128,  -128,
    -128,  -128,  -128,    14,  -128,     4,  -128,  -128,  -128,    -2,
     346,   355,  -128,    82,    32,    34,   -20,  -128,    82,    35,
      36,     0,    33,  -128,   245,    47,  -128,  -128,  -128,   346,
      61,    73,    53,  -128,  -128,    54,    86,    87,    90,    92,
      63,  -128,  -128,   107,    48,    10,    55,  -128,  -128,  -128,
    -128,    96,  -128,  -128,   259,  -128,    63,   108,    97,  -128,
     259,  -128,   123,  -128,    63,   103,   136,  -128,  -128,   -16,
     118,    63,  -128,  -128,    63,  -128,  -128,  -128,  -128,   -21,
    -128,  -128,   163,    63,   132,   120,   298,   138,  -128,  -128,
    -128,  -128,  -128,  -128,    37,   163,   140,   105,   259,  -128,
    -128,   139,   309,    81,  -128,  -128,   -19,   143,   144,   306,
     127,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,   141,   168,  -128,    -3,   163,
    -128,   117,   355,  -128,  -128,  -128,   259,   259,   146,   259,
    -128,   172,  -128,  -128,   170,  -128,  -128,   152,   164,   153,
     162,  -128,   163,   191,   320,    52,   320,   163,    52,   -23,
     306,   306,   306,   306,   163,   163,  -128,  -128,   176,   190,
    -128,  -128,   -10,  -128,   296,  -128,    37,    63,  -128,   155,
    -128,  -128,   203,  -128,  -128,  -128,  -128,  -128,   117,  -128,
    -128,   177,   214,   204,  -128,  -128,   229,  -128,  -128
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,    48,     1,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,    49,     0,    41,    53,     9,     0,
      54,    60,    52,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,    42,     0,     0,    59,    57,    50,     0,
       0,     0,    43,    45,    46,     0,     0,     0,     0,     0,
       0,    64,    65,     0,    20,     0,     0,    62,    61,    68,
      69,     0,    63,    66,     0,    67,     0,     0,     0,    58,
       0,    55,     0,    41,     0,     0,     0,    86,    89,     0,
       0,     0,    16,    17,     0,    14,    15,    12,    13,    20,
      21,    22,    77,     0,     0,     0,     0,     0,    39,    40,
      70,    71,    41,    93,    94,    87,     0,     0,     0,    72,
      51,     0,     0,     0,    44,     3,     0,     0,     0,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,    20,    78,
      80,   102,    60,    97,    99,    82,     0,     0,     0,     0,
      84,     0,    85,    91,     0,    11,    47,     0,     0,     0,
       0,    30,    38,    37,    23,    26,    24,    25,    27,    28,
      31,    32,    33,    34,    35,    36,    83,    18,     0,     0,
     105,   106,     0,   104,     0,    95,    94,     0,    88,     0,
      10,    76,     0,    75,    73,    79,    81,   101,     0,    92,
      96,     0,     0,     0,   103,    98,     0,    74,    90
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,  -128,    15,  -128,  -128,  -127,   -34,   -64,  -128,
     -65,   -18,  -128,  -128,  -128,  -128,  -128,  -128,   220,   119,
     -33,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,   -66,    74,  -128,  -128,
    -128,   167,  -128,  -128,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    30,    31,    32,    90,    91,    92,    56,
      21,    57,    75,     4,    14,    70,    15,    26,    27,    34,
     103,    59,    60,    61,    62,   178,   179,    63,    64,    65,
      66,   106,   150,    67,   108,   152,   104,   145,   146,   147,
     148,    68,    97,   182,   183
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      55,    58,   105,    33,   110,    38,   157,     3,   112,    94,
     113,   116,     5,    95,   180,   197,    16,   119,     6,    13,
     120,  -100,    18,    19,   126,   117,    17,    94,    20,   135,
      55,    95,   139,   133,   134,    25,    55,   142,    35,    22,
      39,   158,   153,    40,     7,     8,     9,    10,    11,    12,
     198,    43,    98,    99,    25,   143,   144,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   180,  -100,    36,    55,    37,    41,    42,    94,    81,
     185,   186,    95,   188,    44,    82,    83,    84,    69,    85,
      86,    87,    88,    73,    33,   121,   122,   124,    96,   126,
     127,   128,    72,    74,    89,    76,   100,   181,   133,   134,
      77,    78,    55,    55,    79,    55,    80,     7,     8,     9,
      10,    11,    12,   201,    33,   123,   124,   125,   126,   127,
     128,    93,   156,   129,   130,   131,   132,   133,   134,    82,
      83,   121,   122,    85,    86,    87,    88,   101,   109,   107,
      55,    58,   161,   111,   114,   121,   122,   115,    89,   118,
     136,   137,   141,   151,   181,   149,   176,   154,   159,   160,
     187,   123,   124,   125,   126,   127,   128,   121,   122,   129,
     130,   131,   132,   133,   134,   123,   124,   125,   126,   127,
     128,   121,   122,   129,   130,   131,   132,   133,   134,   177,
     189,   190,   205,   191,   193,   192,   122,   123,   124,   125,
     126,   127,   128,   194,   202,   129,   130,   131,   132,   133,
     134,   123,   124,   125,   126,   127,   128,   195,   203,   129,
     130,   131,   132,   133,   134,   123,   124,   125,   126,   127,
     128,   196,   206,   129,   130,   131,   132,   133,   134,    45,
      46,    47,    48,    49,   208,   207,    50,    51,    52,    71,
     200,   184,    53,   140,    46,    47,    48,    49,   204,     0,
      50,    51,    52,     0,     0,     0,    53,    28,    29,   102,
       7,     8,     9,    10,    11,    12,    54,     0,     0,     0,
       0,    28,    29,     0,     7,     8,     9,    10,    11,    12,
      54,    46,    47,    48,    49,     0,     0,    50,    51,    52,
       0,     0,     0,    53,    81,     0,     0,   199,     0,     0,
      82,    83,    84,     0,    85,    86,    87,    88,    28,    29,
     155,     7,     8,     9,    10,    11,    12,    54,     0,   138,
       0,    28,    29,     0,     7,     8,     9,    10,    11,    12,
     123,   124,   125,   126,   127,   128,     0,     0,   129,   130,
     131,   132,   133,   134,   123,   124,   125,   126,   127,   128,
       0,     0,     0,     0,     0,     0,   133,   134,    23,    24,
       0,     7,     8,     9,    10,    11,    12,    28,    29,     0,
       7,     8,     9,    10,    11,    12
};

static const yytype_int16 yycheck[] =
{
      34,    34,    66,    21,    70,    25,    25,     0,    73,    30,
      74,    27,     3,    34,   141,    25,    20,    81,     9,     4,
      84,    24,     7,     9,    47,    41,    41,    30,    24,    93,
      64,    34,    96,    56,    57,    20,    70,   102,    23,    41,
      60,    60,   108,    28,    35,    36,    37,    38,    39,    40,
      60,    51,    42,    43,    39,    18,    19,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   198,    24,    41,   108,    41,    41,    41,    30,    16,
     146,   147,    34,   149,    51,    22,    23,    24,    41,    26,
      27,    28,    29,    20,   112,    14,    15,    45,    50,    47,
      48,    49,    41,    50,    41,    51,    51,   141,    56,    57,
      24,    24,   146,   147,    24,   149,    24,    35,    36,    37,
      38,    39,    40,   187,   142,    44,    45,    46,    47,    48,
      49,    24,    51,    52,    53,    54,    55,    56,    57,    22,
      23,    14,    15,    26,    27,    28,    29,    51,    51,    41,
     184,   184,    25,    30,    51,    14,    15,    21,    41,    41,
      28,    41,    24,    58,   198,    25,    25,    28,    25,    25,
      24,    44,    45,    46,    47,    48,    49,    14,    15,    52,
      53,    54,    55,    56,    57,    44,    45,    46,    47,    48,
      49,    14,    15,    52,    53,    54,    55,    56,    57,    31,
      28,    31,    25,    51,    51,    41,    15,    44,    45,    46,
      47,    48,    49,    51,    59,    52,    53,    54,    55,    56,
      57,    44,    45,    46,    47,    48,    49,    51,    25,    52,
      53,    54,    55,    56,    57,    44,    45,    46,    47,    48,
      49,    51,    28,    52,    53,    54,    55,    56,    57,     4,
       5,     6,     7,     8,    25,    51,    11,    12,    13,    39,
     186,   142,    17,    96,     5,     6,     7,     8,   198,    -1,
      11,    12,    13,    -1,    -1,    -1,    17,    32,    33,    20,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,     5,     6,     7,     8,    -1,    -1,    11,    12,    13,
      -1,    -1,    -1,    17,    16,    -1,    -1,    21,    -1,    -1,
      22,    23,    24,    -1,    26,    27,    28,    29,    32,    33,
      21,    35,    36,    37,    38,    39,    40,    41,    -1,    41,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      54,    55,    56,    57,    44,    45,    46,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    32,    33,    -1,
      35,    36,    37,    38,    39,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    65,    66,     0,    77,     3,     9,    35,    36,    37,
      38,    39,    40,    67,    78,    80,    20,    41,    67,     9,
      24,    74,    41,    32,    33,    67,    81,    82,    32,    33,
      67,    68,    69,    75,    83,    67,    41,    41,    25,    60,
      67,    41,    41,    51,    51,     4,     5,     6,     7,     8,
      11,    12,    13,    17,    41,    71,    73,    75,    84,    85,
      86,    87,    88,    91,    92,    93,    94,    97,   105,    41,
      79,    82,    41,    20,    50,    76,    51,    24,    24,    24,
      24,    16,    22,    23,    24,    26,    27,    28,    29,    41,
      70,    71,    72,    24,    30,    34,    50,   106,    42,    43,
      51,    51,    20,    84,   100,    72,    95,    41,    98,    51,
     100,    30,    74,    72,    51,    21,    27,    41,    41,    72,
      72,    14,    15,    44,    45,    46,    47,    48,    49,    52,
      53,    54,    55,    56,    57,    72,    28,    41,    41,    72,
     105,    24,    74,    18,    19,   101,   102,   103,   104,    25,
      96,    58,    99,   100,    28,    21,    51,    25,    60,    25,
      25,    25,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    25,    31,    89,    90,
      70,    71,   107,   108,    83,   100,   100,    24,   100,    28,
      31,    51,    41,    51,    51,    51,    51,    25,    60,    21,
     101,    72,    59,    25,   108,    25,    28,    51,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    64,    66,    65,    67,    67,    67,    67,    67,    67,
      68,    69,    70,    70,    70,    70,    70,    70,    71,    71,
      71,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    73,
      73,    74,    74,    76,    75,    75,    75,    75,    77,    77,
      79,    78,    80,    80,    81,    81,    81,    82,    82,    82,
      83,    83,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    86,    86,    86,    87,    89,    88,
      90,    88,    91,    92,    93,    93,    94,    95,    96,    97,
      98,    99,   100,   100,   101,   101,   101,   102,   103,   104,
     106,   105,   107,   107,   107,   108,   108
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     9,     1,     1,     1,     1,     1,     2,
       6,     5,     1,     1,     1,     1,     1,     1,     4,     3,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     0,     2,     0,     4,     2,     2,     5,     0,     2,
       0,     6,     3,     2,     0,     3,     1,     2,     3,     2,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     5,     7,     5,     5,     2,     0,     5,
       0,     5,     3,     4,     3,     3,     2,     1,     2,     2,
       6,     1,     4,     1,     0,     2,     3,     1,     4,     1,
       0,     5,     0,     3,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* $@1: %empty  */
#line 241 "parser.y"
    {
        TS = creerTableSymbole();  
        TQ = initialiserTQ() ;
        P = initialiserP();

    }
#line 1522 "parser.tab.c"
    break;

  case 3: /* programme: $@1 functions DEBUT DEB_CORPS declarations instructions FIN SEMICOLON FIN_CORPS  */
#line 247 "parser.y"
                                                                                {
        qC++;
        quad = creer_Q("fin", "", "", "", qC);
        inserer_TQ(TQ, quad); 
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);  // afficher TQ pour confirmer
        afficherTQDansFichier(TQ, "output.txt");
        printf("\nProgramme accepte.");
    }
#line 1536 "parser.tab.c"
    break;

  case 4: /* type: ENTIER  */
#line 259 "parser.y"
           {
        currentType.baseType = ENTIER;
        currentType.isArray = false;
        currentType.isConst = false;
        (yyval.type) = ENTIER;
    }
#line 1547 "parser.tab.c"
    break;

  case 5: /* type: FLOTTANT  */
#line 265 "parser.y"
               {
        currentType.baseType = FLOTTANT;
        currentType.isArray = false;
        currentType.isConst = false;
        (yyval.type) = FLOTTANT;
    }
#line 1558 "parser.tab.c"
    break;

  case 6: /* type: STRING  */
#line 271 "parser.y"
             {
        currentType.baseType = STRING;
        currentType.isArray = false;
        currentType.isConst = false;
        (yyval.type) = STRING;
    }
#line 1569 "parser.tab.c"
    break;

  case 7: /* type: CHAR  */
#line 277 "parser.y"
           {
        currentType.baseType = CHAR;
        currentType.isArray = false;
        currentType.isConst = false;
        (yyval.type) = CHAR;
    }
#line 1580 "parser.tab.c"
    break;

  case 8: /* type: BOOLEAN  */
#line 283 "parser.y"
              {
        currentType.baseType = BOOLEAN;
        currentType.isArray = false;
        currentType.isConst = false;
        (yyval.type) = BOOLEAN;
    }
#line 1591 "parser.tab.c"
    break;

  case 9: /* type: CONST type  */
#line 289 "parser.y"
                 {
        currentType.isConst = true;
        (yyval.type) = (yyvsp[0].type);
    }
#line 1600 "parser.tab.c"
    break;

  case 10: /* tableau: TABLE type ID DEB_TABLEAU INT FIN_TABLEAU  */
#line 297 "parser.y"
                                              {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[-3].str), &found)) {
            semanticError("Tableau Deja Déclaré", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch((yyvsp[-4].type)) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            (yyvsp[-3].str),         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            (yyvsp[-1].intv)         // memory address
        );
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
#line 1634 "parser.tab.c"
    break;

  case 12: /* valeur: INT  */
#line 331 "parser.y"
        { 
        (yyval.structure).type = ENTIER;
        char buffer[256];
        sprintf(buffer, "%d", (yyvsp[0].intv));
        (yyval.structure).valeur = strdup(buffer);
    }
#line 1645 "parser.tab.c"
    break;

  case 13: /* valeur: FLOAT  */
#line 337 "parser.y"
            {
        (yyval.structure).type = FLOTTANT;
        char buffer[256];
        sprintf(buffer, "%f", (yyvsp[0].flt));
        (yyval.structure).valeur = strdup(buffer);
    }
#line 1656 "parser.tab.c"
    break;

  case 14: /* valeur: CARACTERE  */
#line 343 "parser.y"
                {
        (yyval.structure).type = CHAR;
        char buffer[256];
        sprintf(buffer, "%c", (yyvsp[0].charv));
        (yyval.structure).valeur = strdup(buffer);
    }
#line 1667 "parser.tab.c"
    break;

  case 15: /* valeur: CHAINE  */
#line 349 "parser.y"
             {
        (yyval.structure).type = STRING;
        (yyval.structure).valeur = strdup((yyvsp[0].str));
    }
#line 1676 "parser.tab.c"
    break;

  case 16: /* valeur: TRUE  */
#line 353 "parser.y"
           {
        (yyval.structure).type = BOOLEAN;
        (yyval.structure).valeur = strdup("true");
    }
#line 1685 "parser.tab.c"
    break;

  case 17: /* valeur: FALSE  */
#line 357 "parser.y"
            {
        (yyval.structure).type = BOOLEAN;
        (yyval.structure).valeur = strdup("false");
    }
#line 1694 "parser.tab.c"
    break;

  case 18: /* variable: ID DEB_TABLEAU INT FIN_TABLEAU  */
#line 363 "parser.y"
                                   {
        Symbole* found;
        rechercherSymbole(TS, (yyvsp[-3].str), &found);
        if (found == NULL) {
            semanticError("Tableau non declaree", line);
        }
        else{
            if ((yyvsp[-1].intv) > found->taille) {
                
                semanticError("Index hors limites", line);
            }
            else{
                (yyval.structure).nom = (yyvsp[-3].str);
                switch(found->type[0]) {
                    case 'E': (yyval.structure).type = ENTIER; break;
                    case 'F': (yyval.structure).type = FLOTTANT; break;
                    case 'S': (yyval.structure).type = STRING; break;
                    case 'C': (yyval.structure).type = CHAR; break;
                    case 'B': (yyval.structure).type = BOOLEAN; break;
                    default: (yyval.structure).type = ENTIER;
                }
            }
       }
    }
#line 1723 "parser.tab.c"
    break;

  case 19: /* variable: ID POINTEUR ID  */
#line 387 "parser.y"
                     {
        Symbole* found;
        rechercherSymbole(TS, (yyvsp[-2].str), &found);
        if (found == NULL) {
            semanticError("Structure non declaree", line);
        }
        else{
            rechercherSymbole(TS, (yyvsp[0].str), &found);
            if (found == NULL) {
                semanticError("Variable non declaree", line);
            }
            else{
                (yyval.structure).nom = (yyvsp[0].str);
                switch(found->type[0]) {
                    case 'E': (yyval.structure).type = ENTIER; break;
                    case 'F': (yyval.structure).type = FLOTTANT; break;
                    case 'S': (yyval.structure).type = STRING; break;
                    case 'C': (yyval.structure).type = CHAR; break;
                    case 'B': (yyval.structure).type = BOOLEAN; break;
                    default: (yyval.structure).type = ENTIER;
                }
            }
        }

    }
#line 1753 "parser.tab.c"
    break;

  case 20: /* variable: ID  */
#line 412 "parser.y"
         {
        (yyval.structure).nom = strdup((yyvsp[0].str));
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[0].str), &found)) {
            switch(found->type[0]) {
                case 'E': (yyval.structure).type = ENTIER; break;
                case 'F': (yyval.structure).type = FLOTTANT; break;
                case 'S': (yyval.structure).type = STRING; break;
                case 'C': (yyval.structure).type = CHAR; break;
                case 'B': (yyval.structure).type = BOOLEAN; break;
                default: (yyval.structure).type = ENTIER;
            }
            (yyval.structure).valeur = strdup(found->valeur);
            }
                else {
            //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                //on check si la variable est un parametre de la fonction
                Parametre *param;
                Symbole* found;
                rechercherSymbole(TS, saveFunctionDec, &found);
                if (rechercherParametre(found, (yyvsp[0].str), &param)) {
                    char* exprType = NULL;
                    switch(param->type[0]) {
                    case 'E': (yyval.structure).type = ENTIER; break;
                    case 'F': (yyval.structure).type = FLOTTANT; break;
                    case 'S': (yyval.structure).type = STRING; break;
                    case 'C': (yyval.structure).type = CHAR; break;
                    case 'B': (yyval.structure).type = BOOLEAN; break;
                    default: (yyval.structure).type = ENTIER;
                }
                (yyval.structure).valeur = strdup("0");
                    
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
    }
            }
#line 1799 "parser.tab.c"
    break;

  case 23: /* expression: expression PLUS expression  */
#line 458 "parser.y"
                                 {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform addition based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) + atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-2].structure).type == FLOTTANT || (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) + atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) + atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for addition", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("+", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 1854 "parser.tab.c"
    break;

  case 24: /* expression: expression MOINS expression  */
#line 508 "parser.y"
                                  {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) - atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-2].structure).type == FLOTTANT || (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) - atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) - atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for subtraction", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("-", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 1909 "parser.tab.c"
    break;

  case 25: /* expression: expression MULT expression  */
#line 558 "parser.y"
                                 {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform multiplication based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) * atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-2].structure).type == FLOTTANT || (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) * atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) * atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for multiplication", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("*", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 1964 "parser.tab.c"
    break;

  case 26: /* expression: expression DIV expression  */
#line 608 "parser.y"
                                {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform division based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            if (atoi(val2) == 0) {
                semanticError("Division by zero", line);
            }
            int result = atoi(val1) / atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-2].structure).type == FLOTTANT || (yyvsp[0].structure).type == FLOTTANT) {
            if (atof(val2) == 0.0) {
                semanticError("Division by zero", line);
            }
            float result = atof(val1) / atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            if (atof(val2) == 0.0) {
                semanticError("Division by zero", line);
            }
            float result = atof(val1) / atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for division", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("/", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2028 "parser.tab.c"
    break;

  case 27: /* expression: expression MOD expression  */
#line 667 "parser.y"
                                {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform modulo based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            if (atoi(val2) == 0) {
                semanticError("Modulo by zero", line);
            }
            int result = atoi(val1) % atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else {
            semanticError("Invalid types for modulo", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("%", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2078 "parser.tab.c"
    break;

  case 28: /* expression: expression PUISS expression  */
#line 712 "parser.y"
                                  {
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;            
        // Perform power based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = pow(atoi(val1), atoi(val2));
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-2].structure).type == FLOTTANT || (yyvsp[0].structure).type == FLOTTANT) {
            float result = pow(atof(val1), atof(val2));
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for power", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("^", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2129 "parser.tab.c"
    break;

  case 29: /* expression: NOT expression  */
#line 758 "parser.y"
                     {
        printf("i am inside NOT operation\n");
        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = (yyvsp[0].structure).valeur;            
        // Perform NOT operation based on type
        if ((yyvsp[0].structure).type == BOOLEAN) {
            int result = !atoi(val1);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid type for NOT operation", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R", qC);
        (yyval.structure).nom = resultVarName;
        quad = creer_Q("!", 
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      "",
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2175 "parser.tab.c"
    break;

  case 30: /* expression: PAR_OUV expression PAR_FERM  */
#line 800 "parser.y"
    {
        (yyval.structure).nom = (yyvsp[-1].structure).nom;
        (yyval.structure).valeur = (yyvsp[-1].structure).valeur;
        (yyval.structure).type = (yyvsp[-1].structure).type;
    }
#line 2185 "parser.tab.c"
    break;

  case 31: /* expression: expression INF expression  */
#line 806 "parser.y"
                                {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) < atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) < atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) < atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) < 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q(">", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2247 "parser.tab.c"
    break;

  case 32: /* expression: expression INF_EGAL expression  */
#line 863 "parser.y"
                                     {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) <= atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) <= atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) <= atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) <= 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("=>", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
#line 2310 "parser.tab.c"
    break;

  case 33: /* expression: expression SUP expression  */
#line 921 "parser.y"
                                {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) > atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) > atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) > atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) > 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("<", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2372 "parser.tab.c"
    break;

  case 34: /* expression: expression SUPP_EGAL expression  */
#line 978 "parser.y"
                                      {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) >= atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) >= atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) >= atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) >= 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("<=", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2434 "parser.tab.c"
    break;

  case 35: /* expression: expression EQUAL expression  */
#line 1036 "parser.y"
                                  {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) == atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) == atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) == atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) == 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        }
        else if((yyvsp[-2].structure).type == BOOLEAN && (yyvsp[0].structure).type == BOOLEAN){
            int result = atoi(val1) == atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("==", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2501 "parser.tab.c"
    break;

  case 36: /* expression: expression NOT_EQUAL expression  */
#line 1099 "parser.y"
                                      {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == ENTIER) {
            int result = atoi(val1) != atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if ((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == FLOTTANT) {
            float result = atof(val1) != atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == FLOTTANT && (yyvsp[0].structure).type == ENTIER) || ((yyvsp[-2].structure).type == ENTIER && (yyvsp[0].structure).type == FLOTTANT)) {
            float result = atof(val1) != atof(val2);
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = BOOLEAN;
        } else if (((yyvsp[-2].structure).type == STRING && (yyvsp[0].structure).type == STRING) || ((yyvsp[-2].structure).type == CHAR && (yyvsp[0].structure).type == CHAR)) {
            int result = strcmp(val1, val2) != 0;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else if((yyvsp[-2].structure).type == BOOLEAN && (yyvsp[0].structure).type == BOOLEAN){
            int result = atoi(val1) != atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("!=", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2567 "parser.tab.c"
    break;

  case 37: /* expression: expression AND expression  */
#line 1160 "parser.y"
                                {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == BOOLEAN && (yyvsp[0].structure).type == BOOLEAN) {
            int result = atoi(val1) && atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("&&", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
#line 2618 "parser.tab.c"
    break;

  case 38: /* expression: expression OR expression  */
#line 1206 "parser.y"
                               {

        printf("i am inside comparaison\n");
        char bff[255]; 
        Symbole* found1;
        Symbole* found2;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get values for operands
        char *val1, *val2;
        val1 = (yyvsp[-2].structure).valeur;
        val2 = (yyvsp[0].structure).valeur;   

        // Perform subtraction based on types
        if ((yyvsp[-2].structure).type == BOOLEAN && (yyvsp[0].structure).type == BOOLEAN) {
            int result = atoi(val1) || atoi(val2);
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = BOOLEAN;
        } else {
            semanticError("Invalid types for comparaison", line);
        } 

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("||", 
                      (yyvsp[-2].structure).nom ? (yyvsp[-2].structure).nom : (yyvsp[-2].structure).valeur,
                      (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (yyvsp[0].structure).valeur,
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");

    }
#line 2669 "parser.tab.c"
    break;

  case 39: /* incrementation: variable INCREM  */
#line 1255 "parser.y"
                    {

        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = (yyvsp[-1].structure).valeur;        
        // Perform addition based on types
        if ((yyvsp[-1].structure).type == ENTIER ) {
            int result = atoi(val1) + 1;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-1].structure).type == FLOTTANT ) {
            float result = atof(val1) + 1;
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for addition", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("++", 
                      (yyvsp[-1].structure).nom ? (yyvsp[-1].structure).nom : (yyvsp[-1].structure).valeur,
                      "",
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2719 "parser.tab.c"
    break;

  case 40: /* incrementation: variable DECREM  */
#line 1301 "parser.y"
    {
        char bff[255]; 
        Symbole* found1;
        
        // Initialize result structure
        (yyval.structure).nom = NULL;
        (yyval.structure).valeur = malloc(255);
        if ((yyval.structure).valeur == NULL) {
            semanticError("Memory allocation failed", line);
        }

        // Get value for operand
        char *val1;
        val1 = (yyvsp[-1].structure).valeur;        
        // Perform subtraction based on types
        if ((yyvsp[-1].structure).type == ENTIER ) {
            int result = atoi(val1) - 1;
            sprintf((yyval.structure).valeur, "%d", result);
            (yyval.structure).type = ENTIER;
        } else if ((yyvsp[-1].structure).type == FLOTTANT ) {
            float result = atof(val1) - 1;
            sprintf((yyval.structure).valeur, "%f", result);
            (yyval.structure).type = FLOTTANT;
        } else {
            semanticError("Invalid types for subtraction", line);
        }

        // Generate quadruplet
        qC++;
        char resultVarName[20];
        sprintf(resultVarName, "%s%d", "R",qC);
        (yyval.structure).nom=resultVarName;
        quad = creer_Q("--", 
                      (yyvsp[-1].structure).nom ? (yyvsp[-1].structure).nom : (yyvsp[-1].structure).valeur,
                      "",
                      (yyval.structure).nom,
                      qC);        
        afficherQ(quad);        
        inserer_TQ(TQ, quad);

        afficherTableSymbole(TS);
        afficherTQ(TQ);
        afficherTQDansFichier(TQ, "output.txt");
    }
#line 2768 "parser.tab.c"
    break;

  case 43: /* $@2: %empty  */
#line 1353 "parser.y"
             {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[0].str), &found)) {
            semanticError("Variable already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch((yyvsp[-1].type)) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            (yyvsp[0].str),         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
#line 2803 "parser.tab.c"
    break;

  case 47: /* declaration: type ID ASSIGN expression SEMICOLON  */
#line 1385 "parser.y"
                                          {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[-3].str), &found)) {
            semanticError("Variable already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch((yyvsp[-4].type)) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        
        // Check if the expression is valid for the type
        if (!((yyvsp[-1].structure).valeur, typeStr)) {
            semanticError("Invalid literal for type", line);
        }
        
        Symbole* sym = creerSymbole(
            VARIABLE,    // category
            (yyvsp[-3].str),         // name
            typeStr,    // type
            (yyvsp[-1].structure).valeur,  // initial value
            line,       // line number
            0          // memory address
        );
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
#line 2842 "parser.tab.c"
    break;

  case 50: /* $@3: %empty  */
#line 1426 "parser.y"
                                                    {
        printf("je suis dans fonction1\n");
        printf("Function parameters:\n");
        if ((yyvsp[-1].parametresList)->head != NULL) {
            ParametreNode* current = (yyvsp[-1].parametresList)->head;
            printf("hilllo");
            Symbole* found;
            //ajouter les parametres de la fonction à la table des symboles
            printf("Parametre: %s %s\n", current->parametre.nom, current->parametre.type);
            printf("le nom de la fonction est : %s\n", (yyvsp[-3].parametreUnion).nom);
            if(rechercherSymbole(TS, (yyvsp[-3].parametreUnion).nom, &found)){
            }

            while (current) {            
            ajouterParametre(found, current->parametre.nom, current->parametre.type);
            current = current->next;
            }
            printf("\naffichage des infos de la fonction \n"); 
         afficherInfoFonction(found);
         

        }
        saveFunctionDec=strdup((yyvsp[-3].parametreUnion).nom);
         
    }
#line 2872 "parser.tab.c"
    break;

  case 51: /* fonction: declarationfonction PAR_OUV parametres PAR_FERM $@3 corps  */
#line 1450 "parser.y"
            {
        
        //on check si c'est une fonction (a un type != void ) et ne retourne pas de valeur
        if((yyvsp[-5].parametreUnion).type[0] != 'V' && !itReturn){
            semanticError("La fonction ne retourne pas de valeur", line);
        }
        printf("fonction correcte syntaxiquement\n"); afficherTableSymbole(TS);   
        saveFunctionDec=NULL;
        }
#line 2886 "parser.tab.c"
    break;

  case 52: /* declarationfonction: type FONCTION ID  */
#line 1461 "parser.y"
                     {        
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[0].str), &found)) {
            semanticError("Function already declared", line);
        }
        
        // Convert the type token to string
        char* typeStr;
        switch((yyvsp[-2].type)) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        //création du symbole de la fonction
        Symbole* sym = creerSymbole(
            FUNCTION,    // category
            (yyvsp[0].str),         // name
            typeStr,    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        (yyval.parametreUnion).nom = strdup((yyvsp[0].str));
        (yyval.parametreUnion).type = strdup(typeStr);
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);

    }
#line 2924 "parser.tab.c"
    break;

  case 53: /* declarationfonction: FONCTION ID  */
#line 1494 "parser.y"
                 {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[0].str), &found)) {
            semanticError("Function already declared", line);
        }
        //création du symbole de la fonction
        Symbole* sym = creerSymbole(
            FUNCTION,    // category
            (yyvsp[0].str),         // name
            "VOID",    // type
            "",         // initial value
            line,       // line number
            0          // memory address
        );
        (yyval.parametreUnion).nom = strdup((yyvsp[0].str));
        (yyval.parametreUnion).type = strdup("VOID");
        
        insererSymbole(TS, sym);
        afficherTableSymbole(TS); // afficher TS pour confirmer
        afficherTQ(TQ);
    }
#line 2950 "parser.tab.c"
    break;

  case 54: /* parametres: %empty  */
#line 1518 "parser.y"
                {
        (yyval.parametresList) = creerParametresList();  // Always create a new list for empty case
    }
#line 2958 "parser.tab.c"
    break;

  case 55: /* parametres: parametres VIRGULE parametre  */
#line 1521 "parser.y"
                                   {
        // Use the existing list from $1
        (yyval.parametresList) = (yyvsp[-2].parametresList);  
        // Add the new parameter to the list
        if (!ajouterParametreUnion((yyval.parametresList), (yyvsp[0].parametreUnion).nom, (yyvsp[0].parametreUnion).type)) {
            semanticError("Parametre deja declare", line);
        }
    }
#line 2971 "parser.tab.c"
    break;

  case 56: /* parametres: parametre  */
#line 1529 "parser.y"
                {
        // Create new list for single parameter
        (yyval.parametresList) = creerParametresList();
        // Add the parameter to the new list
        if (!ajouterParametreUnion((yyval.parametresList), (yyvsp[0].parametreUnion).nom, (yyvsp[0].parametreUnion).type)) {
            semanticError("Parametre deja declare", line);
        }
    }
#line 2984 "parser.tab.c"
    break;

  case 57: /* parametre: type ID  */
#line 1541 "parser.y"
            {
        //remplissage des champs du parametre 
        (yyval.parametreUnion).nom = strdup((yyvsp[0].str));
        char* typeStr;
        switch((yyvsp[-1].type)) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        (yyval.parametreUnion).type = strdup(typeStr);

    }
#line 3004 "parser.tab.c"
    break;

  case 58: /* parametre: TABLE type ID  */
#line 1556 "parser.y"
                    {
    
    }
#line 3012 "parser.tab.c"
    break;

  case 59: /* parametre: ENREGISTREMENT ID  */
#line 1559 "parser.y"
                        {printf("parametre correcte syntaxiquement\n");}
#line 3018 "parser.tab.c"
    break;

  case 73: /* read: INPUT PAR_OUV ID PAR_FERM SEMICOLON  */
#line 1579 "parser.y"
                                        {printf("read correcte syntaxiquement\n");}
#line 3024 "parser.tab.c"
    break;

  case 76: /* write: PRINT PAR_OUV CHAINE PAR_FERM SEMICOLON  */
#line 1585 "parser.y"
                                            {printf("write correcte syntaxiquement\n");}
#line 3030 "parser.tab.c"
    break;

  case 77: /* retourner: RETURN expression  */
#line 1589 "parser.y"
                      {
        //on check si le type de retour est compatible avec le type de la fonction
        
        Symbole* found;
        printf("voici le nom de la fonction %s\n", saveFunctionDec);
        if (rechercherSymbole(TS, saveFunctionDec, &found)) {
            printf("je suis dans retourner\n");
            if(found->type[0] == 'V'){
                semanticError("La fonction ne retourne pas de valeur", line);
            }
            char* exprType = NULL;
                switch((yyvsp[0].structure).type) {
                    case ENTIER: exprType = TYPE_ENTIER; break;
                    case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                    case CHAR: exprType = TYPE_CHAR; break;
                    case STRING: exprType = TYPE_STRING; break;
                    case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                    default: exprType = TYPE_ENTIER;
                }
            if (!areTypesCompatible(found->type, exprType)) {
                semanticError("Type de retour incompatible avec la fonction", line);
            }
            itReturn = true;
        } else {
          semanticError("Fonction non declaree", line);
        }
        }
#line 3062 "parser.tab.c"
    break;

  case 78: /* $@4: %empty  */
#line 1619 "parser.y"
                         {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[-2].str), &found)) { // is declared
            if (found->categorie == VARIABLE) {  
                qC++;
                
                // Create a buffer for the value
                char buffer[256];
                sprintf(buffer, "%s", (yyvsp[0].structure).valeur ? (yyvsp[0].structure).valeur : "");
                
                // Update the symbol's value

                SetValueSymbol(found, buffer);
                
                // Create quadruplet
                quad = creer_Q(":=", (yyvsp[-2].str), " ", buffer, qC);
                inserer_TQ(TQ, quad);
                 afficherTQ(TQ);
                
                // Convert expression type to string for comparison
                char* exprType = NULL;
                printf("Type de l'expression: %d\n", (yyvsp[0].structure).type);
                switch((yyvsp[0].structure).type) {
                    case ENTIER: exprType = TYPE_ENTIER; break;
                    case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                    case CHAR: exprType = TYPE_CHAR; break;
                    case STRING: exprType = TYPE_STRING; break;
                    case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                }
                printf("Type de l'expression: %s\n", exprType);
                printf("Type de la variable: %s\n", found->type);
                if (!areTypesCompatible(found->type, exprType)) {
                    semanticError("Type incompatible dans l'affectation.", line);
                }
            } else {
                semanticError("Identifier is not a variable", line);
            }
        } else {
            //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                //on check si la variable est un parametre de la fonction
                Parametre *param;
                Symbole* found;
                rechercherSymbole(TS, saveFunctionDec, &found);
                if (rechercherParametre(found, (yyvsp[-2].str), &param)) {
                    //on check si le type de retour est compatible avec le type de la variable
                    char* exprType = NULL;
                    switch((yyvsp[0].structure).type) {
                        case ENTIER: exprType = TYPE_ENTIER; break;
                        case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                        case CHAR: exprType = TYPE_CHAR; break;
                        case STRING: exprType = TYPE_STRING; break;
                        case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                        default: exprType = TYPE_ENTIER;
                    }
                    if (!areTypesCompatible(exprType, found->type)) {
                        semanticError("Type incompatible dans l'affectation.", line);
                    }
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
    } }
#line 3132 "parser.tab.c"
    break;

  case 80: /* $@5: %empty  */
#line 1684 "parser.y"
                    {
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[-2].str), &found)) { // is declared
        //on check si le type de retour est compatible avec le type de la variable
        char* typeStr;
        switch((yyvsp[0].structure).type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        if (!areTypesCompatible(found->type, typeStr)) {
            semanticError("Type incompatible dans l'affectation.", line);
        }
        qC++;        
        // Create a buffer for the value
        char buffer[256];
        sprintf(buffer, "%s", (yyvsp[0].structure).valeur ? (yyvsp[0].structure).valeur : "");
        // Update the symbol's value
        SetValueSymbol(found, buffer);        
        // Create quadruplet
        quad = creer_Q(":=", (yyvsp[-2].str), " ", buffer, qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);}
        else {
             //on check dabord si il ya une fonction en cours de traitement 
            if (saveFunctionDec != NULL) {
                Symbole* found;
                Parametre *param;
                rechercherSymbole(TS, saveFunctionDec, &found);
                //on check si la variable est un parametre de la fonction
                if (rechercherParametre(found, (yyvsp[-2].str), &param)) {
                    //on check si le type de retour est compatible avec le type de la variable
                    char* exprType = NULL;
                    switch((yyvsp[0].structure).type) {
                        case ENTIER: exprType = TYPE_ENTIER; break;
                        case FLOTTANT: exprType = TYPE_FLOTTANT; break;
                        case CHAR: exprType = TYPE_CHAR; break;
                        case STRING: exprType = TYPE_STRING; break;
                        case BOOLEAN: exprType = TYPE_BOOLEAN; break;
                        default: exprType = TYPE_ENTIER;
                    }
                    if (!areTypesCompatible(exprType, found->type)) {
                        semanticError("Type incompatible dans l'affectation.", line);
                    }
                } else {
                    semanticError("Variable non declaree", line);
                }
            } else {
            semanticError("Variable non declaree", line);
        }
        }
    }
#line 3192 "parser.tab.c"
    break;

  case 82: /* condition: ifstatement corps elsebloc  */
#line 1743 "parser.y"
        {
        while (!Pempty(P)) {
            quad = pop(P);
            updateLabel(quad, qC);
        }
        qC++;
        quad = creer_Q("finIf","", " ", "", qC);
        inserer_TQ(TQ, quad);
        
    }
#line 3207 "parser.tab.c"
    break;

  case 83: /* ifstatement: IF PAR_OUV expression PAR_FERM  */
#line 1755 "parser.y"
                                  {
        //vérifier si le resultat de l'expression est un boolean
        if((yyvsp[-1].structure).type != BOOLEAN){
            semanticError("L'expression doit être un boolean", line);
        }else{
            qC++;
            //branchement en cas de faux
            quad = creer_Q("BZ","temp", " ", (yyvsp[-1].structure).valeur, qC);
            inserer_TQ(TQ, quad);
            //sauvegarder qC pour la mise a jour de l'adresse de branchement dans la pile des adresses
            push(P, quad);
            
        }
    }
#line 3226 "parser.tab.c"
    break;

  case 86: /* while_partie_une: WHILE PAR_OUV  */
#line 1777 "parser.y"
                  {

        printf("I'm inside while\n");

        qC++;
        
        sauvDebut = qC;
    }
#line 3239 "parser.tab.c"
    break;

  case 87: /* while_partie_deux: expression  */
#line 1787 "parser.y"
              {

        printf("I'm inside while\n");

        if((yyvsp[0].structure).type != BOOLEAN) {
            semanticError("Condition de boucle invalide", line);
        }
        // Branchement vers fin si condition n'est pas vrai
        quad = creer_Q("BZ", "fin", " ", (yyvsp[0].structure).valeur, qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
        afficherTQ(TQ);
    }
#line 3257 "parser.tab.c"
    break;

  case 88: /* while_partie_trois: PAR_FERM corps  */
#line 1802 "parser.y"
                   {

        printf("I'm inside while\n");

        qC++;
        quad = pop(P); 
        updateLabel(quad, qC+1);
        // Branchement inconditionnel vers Debut
        
        char etiq[255];
        sprintf(etiq, "%d", sauvDebut);
        quad = creer_Q("BR", etiq, "", "", qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);

    }
#line 3278 "parser.tab.c"
    break;

  case 89: /* for_partie_une: FOR PAR_OUV  */
#line 1820 "parser.y"
                {

        printf("I'm inside for\n");
       
        qC++;
        sauvDebut = qC;
    }
#line 3290 "parser.tab.c"
    break;

  case 90: /* for_partie_deux: ID FROM INT TO INT PAR_FERM  */
#line 1830 "parser.y"
                                {

        printf("Entrée dans for_partie_deux\n");

        char temp[20]; 
        int tempCounter = 0; 

        sprintf(temp, "T%d", tempCounter++); 

        // Utilisation dans creer_Q
        char str5[20], str3[20];
        sprintf(str5, "%d", (yyvsp[-1].intv));
        sprintf(str3, "%d", (yyvsp[-3].intv));
        quad = creer_Q("-", str5, str3, temp, qC);

        inserer_TQ(TQ, quad);
        afficherTQ(TQ);

        // Vérifier si le résultat est inférieur à 0
        qC++;
        quad = creer_Q("BZ", "fin", " ", temp, qC); // Sauter si la condition est fausse
        inserer_TQ(TQ, quad);
        push(P, quad);
        afficherTQ(TQ);
    }
#line 3320 "parser.tab.c"
    break;

  case 91: /* for_partie_trois: corps  */
#line 1857 "parser.y"
          {

        printf("Entrée dans for_partie_trois\n");

        qC++;
        quad = pop(P); 
        updateLabel(quad, qC+1);

        // Branchement inconditionnel vers Debut
        char etiq[255];
        sprintf(etiq, "%d", sauvDebut);
        quad = creer_Q("BR", etiq, "", "", qC);
        inserer_TQ(TQ, quad);
        afficherTQ(TQ);
    }
#line 3340 "parser.tab.c"
    break;

  case 94: /* elsebloc: %empty  */
#line 1878 "parser.y"
                {
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC);
        printf("\nelsebloc correcte syntaxiquement qc = %d\n", qC);
    }
#line 3352 "parser.tab.c"
    break;

  case 95: /* elsebloc: elsestatement corps  */
#line 1885 "parser.y"
                          {
        qC++;
        //maj de l'adresse de branchement dans la pile des adresses
        quad = pop(P);
        updateLabel(quad, qC);
        //maj de l'adresse de branchement dans la pile des adresses
       
    }
#line 3365 "parser.tab.c"
    break;

  case 97: /* elsestatement: ELSE  */
#line 1897 "parser.y"
         {
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC+1);
        //branchement vers la fin
        
        quad = creer_Q("BR","temp", " ", "", qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
        
    }
#line 3382 "parser.tab.c"
    break;

  case 98: /* elifstatement: elifkey PAR_OUV expression PAR_FERM  */
#line 1910 "parser.y"
                                        {
        if((yyvsp[-1].structure).type != BOOLEAN){
            semanticError("L'expression doit être un boolean", line);
        }else{
            qC++;
            //branchement en cas de faux
            quad = creer_Q("BZ","temp", " ", (yyvsp[-1].structure).valeur, qC);
            inserer_TQ(TQ, quad);
            //sauvegarder qC pour la mise a jour de l'adresse de branchement dans la pile des adresses
            push(P, quad);
            
        }
    }
#line 3400 "parser.tab.c"
    break;

  case 99: /* elifkey: ELIF  */
#line 1924 "parser.y"
        {
        //maj de l'adresse de branchement dans la pile des adresses
        qC++;
        quad = pop(P);
        updateLabel(quad, qC+1);
        //branchement vers la fin
        
        quad = creer_Q("BR","temp", " ", "", qC);
        inserer_TQ(TQ, quad);
        push(P, quad);
    }
#line 3416 "parser.tab.c"
    break;

  case 100: /* $@6: %empty  */
#line 1940 "parser.y"
    {
        //affectation du nom de la fonction courrante 

        currentFunctionCalled = strdup((yyvsp[0].str));
        //initialisation de la liste des parametres
        currentParametresList = creerParametresList();
        
    }
#line 3429 "parser.tab.c"
    break;

  case 101: /* call: ID $@6 PAR_OUV parametresCall PAR_FERM  */
#line 1948 "parser.y"
                                        {
        (yyval.structure).nom = strdup((yyvsp[-4].str));
        //on check si la fonction existe déja dans la table des symboles
        Symbole* found;
        if (rechercherSymbole(TS, (yyvsp[-4].str), &found)) {
            if (found->categorie == FUNCTION) {                
                if (strcmp(found->type, TYPE_ENTIER) == 0) {
                    (yyval.structure).type = ENTIER;
                } else if (strcmp(found->type, TYPE_FLOTTANT) == 0) {
                    (yyval.structure).type = FLOTTANT;
                } else if (strcmp(found->type, TYPE_CHAR) == 0) {
                    (yyval.structure).type = CHAR;
                } else if (strcmp(found->type, TYPE_STRING) == 0) {
                    (yyval.structure).type = STRING;
                } else if (strcmp(found->type, TYPE_BOOLEAN) == 0) {
                    (yyval.structure).type = BOOLEAN;
                }
                (yyval.structure).valeur = strdup(found->valeur);
                //on vérifie le nombre de parametres est le meme 
                if (found->infoFonction->nbParametres != currentParametresList->count) {
                    semanticError("Nombre de parametres incorrect", line);
                }else
                {
                    //on check pour chaque parametre la compatibilité de type 
                    ParametreNode* current = currentParametresList->head;
                    Parametre* currentFonction = found->infoFonction->parametres;
                    while (current) {
                        if (!areTypesCompatible(current->parametre.type, currentFonction->type)) {
                            semanticError("Type incompatible dans l'appel de fonction", line);
                        }
                        current = current->next;
                        currentFonction = currentFonction->suivant;
                    }
                    // on crée le quadruplet pour l'appel de fonction
                    
                    //on fin , on rend la fonction courante et les parametres à NULL
                    currentFunctionCalled = NULL;
                    currentParametresList = NULL;
                    //on push la ligne suivante dans la pile des adresses
                }
            }
            else{
                semanticError("Identifier is not a function", line);
            }
        } else {
            semanticError("Function not declared", line);
        }
        
    }
#line 3483 "parser.tab.c"
    break;

  case 103: /* parametresCall: parametresCall VIRGULE parametreCall  */
#line 2004 "parser.y"
                                          {
        char* typeStr;
        switch((yyvsp[0].structure).type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        //ajouter le nouveau parametre à la liste des parametres
        ajouterParametreUnion(currentParametresList, (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (char*)(yyvsp[0].structure).valeur, typeStr);
            
    }
#line 3502 "parser.tab.c"
    break;

  case 104: /* parametresCall: parametreCall  */
#line 2018 "parser.y"
                   {
        char* typeStr;
        switch((yyvsp[0].structure).type) {
            case ENTIER: typeStr = TYPE_ENTIER; break;
            case FLOTTANT: typeStr = TYPE_FLOTTANT; break;
            case CHAR: typeStr = TYPE_CHAR; break;
            case STRING: typeStr = TYPE_STRING; break;
            case BOOLEAN: typeStr = TYPE_BOOLEAN; break;
            default: typeStr = TYPE_ENTIER; // default case
        }
        ajouterParametreUnion(currentParametresList, (yyvsp[0].structure).nom ? (yyvsp[0].structure).nom : (char*)(yyvsp[0].structure).valeur, typeStr) ;
    }
#line 3519 "parser.tab.c"
    break;

  case 106: /* parametreCall: variable  */
#line 2034 "parser.y"
               {printf("parametre correcte syntaxiquement\n");}
#line 3525 "parser.tab.c"
    break;


#line 3529 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 2037 "parser.y"


void yyerror(const char *s) {
    if (sauvline == line){
        line = line -1 ;
    }
    fprintf(stderr, "File \"%s\", line %d, character %d: %s\n", 
            yyin_filename, line,sauv - linecol, s);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    yyin_filename = argv[1];
    FILE *file = fopen(yyin_filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    yyin = file;
    yyparse();
    fclose(yyin);

    return 0;
}

