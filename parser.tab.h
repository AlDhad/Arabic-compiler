/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 166 "parser.y"

    #include "symbolTable.h"  // Include it again here to make sure types are available for the union

#line 53 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DEBUT = 258,                   /* DEBUT  */
    FIN = 259,                     /* FIN  */
    WHILE = 260,                   /* WHILE  */
    FOR = 261,                     /* FOR  */
    PRINT = 262,                   /* PRINT  */
    INPUT = 263,                   /* INPUT  */
    FONCTION = 264,                /* FONCTION  */
    VIDE = 265,                    /* VIDE  */
    RETURN = 266,                  /* RETURN  */
    COMMENT = 267,                 /* COMMENT  */
    COMMENT_PLUS = 268,            /* COMMENT_PLUS  */
    OR = 269,                      /* OR  */
    AND = 270,                     /* AND  */
    NOT = 271,                     /* NOT  */
    IF = 272,                      /* IF  */
    ELSE = 273,                    /* ELSE  */
    ELIF = 274,                    /* ELIF  */
    DEB_CORPS = 275,               /* DEB_CORPS  */
    FIN_CORPS = 276,               /* FIN_CORPS  */
    TRUE = 277,                    /* TRUE  */
    FALSE = 278,                   /* FALSE  */
    PAR_OUV = 279,                 /* PAR_OUV  */
    PAR_FERM = 280,                /* PAR_FERM  */
    CARACTERE = 281,               /* CARACTERE  */
    CHAINE = 282,                  /* CHAINE  */
    INT = 283,                     /* INT  */
    FLOAT = 284,                   /* FLOAT  */
    DEB_TABLEAU = 285,             /* DEB_TABLEAU  */
    FIN_TABLEAU = 286,             /* FIN_TABLEAU  */
    TABLE = 287,                   /* TABLE  */
    ENREGISTREMENT = 288,          /* ENREGISTREMENT  */
    POINTEUR = 289,                /* POINTEUR  */
    CONST = 290,                   /* CONST  */
    ENTIER = 291,                  /* ENTIER  */
    FLOTTANT = 292,                /* FLOTTANT  */
    STRING = 293,                  /* STRING  */
    CHAR = 294,                    /* CHAR  */
    BOOLEAN = 295,                 /* BOOLEAN  */
    ID = 296,                      /* ID  */
    INCREM = 297,                  /* INCREM  */
    DECREM = 298,                  /* DECREM  */
    PLUS = 299,                    /* PLUS  */
    DIV = 300,                     /* DIV  */
    MOINS = 301,                   /* MOINS  */
    MULT = 302,                    /* MULT  */
    MOD = 303,                     /* MOD  */
    PUISS = 304,                   /* PUISS  */
    ASSIGN = 305,                  /* ASSIGN  */
    SEMICOLON = 306,               /* SEMICOLON  */
    INF = 307,                     /* INF  */
    INF_EGAL = 308,                /* INF_EGAL  */
    SUP = 309,                     /* SUP  */
    SUPP_EGAL = 310,               /* SUPP_EGAL  */
    EQUAL = 311,                   /* EQUAL  */
    NOT_EQUAL = 312,               /* NOT_EQUAL  */
    FROM = 313,                    /* FROM  */
    TO = 314,                      /* TO  */
    VIRGULE = 315,                 /* VIRGULE  */
    EGALE = 316,                   /* EGALE  */
    PASEGALE = 317,                /* PASEGALE  */
    MUL = 318                      /* MUL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 170 "parser.y"

    int type;
    char str[255];
    char charv;
    int intv; 
    float flt;
    bool boolean;
    struct {
        char* valeur;
        char* nom;
        int type;
    } structure;
    ParametreUnion parametreUnion;
    ParametreNode* parametreNode;
    ParametresList* parametresList;

#line 150 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
