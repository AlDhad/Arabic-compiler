%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "parser.tab.h"
extern YYSTYPE yylval;

extern int line;
extern int linecol;
extern char *yyin_filename;
extern int sauv;
extern int sauvline;
%}

%option noyywrap

lettre    [قفكهعمنلءىوأ-ي]
quote "'"
dquote "\""

%%
"بداية"    { 
     printf("here is the begining of the program\n");
     linecol=linecol+5;
     return DEBUT;
}
"انتهى"   {
     printf("mot de fin\n");
     linecol=linecol+5; 
     return FIN; 
}
"صحيح"     { 
     yylval.type = 1; 
     linecol=linecol+4;
     return ENTIER; 
}

"عشري"     {
    yylval.type = 2; 
    linecol=linecol+4; 
    return FLOTTANT; }
"منطقي"   { 
     linecol=linecol+5;
     yylval.type = 5;
     return BOOLEAN; }
"نص"      { 
    linecol=linecol+2;
    yylval.type = 4;
    strcpy(yylval.str,yytext); 
    return STRING; }
"حرف"      { 
    linecol=linecol+3;
    yylval.type = 3;  
    return CHAR; }
"ثابت"     { 
     linecol=linecol+4;
     yylval.type = 0; 
     return CONST; }
"إذا"     { 
     linecol=linecol+3;
     return IF; }
"وإلا"  { 
     linecol=linecol+4;
     return ELSE; }
"وإلا إذا" { 
     linecol=linecol+8;
     return ELIF; }
"طالما"    { 
     linecol=linecol+5;
     return WHILE; }
"لكل"      { 
     linecol=linecol+3;
     return FOR; }
"اطبع"     { 
     linecol=linecol+4;
     return PRINT; }
"أدخل"     { 
     linecol=linecol+5;
     return INPUT; }
"جدول"     { 
    linecol=linecol+4;
    yylval.type = 6;
    return TABLE; }
"سجل"      {
     linecol=linecol+3;
     yylval.type = 7;
     return ENREGISTREMENT; }
"دالة"     { 
    linecol=linecol+4;
    return FONCTION; }
"صح"       { 
     linecol=linecol+2;
     yylval.boolean = true;
     return TRUE; }
"خطأ"      {
     linecol=linecol+3;
     yylval.boolean = false; 
     return FALSE; }
"لا شيء"  {
    linecol=linecol+6;
    return VIDE; }
"ارجع"     { 
     linecol=linecol+4;
     return RETURN; }
"من"        { 
     linecol=linecol+2;
     return FROM; }
"إلى"       { 
    linecol=linecol+3;
    return TO; }
"،"        { 
     linecol=linecol+1;
     return VIRGULE; }
"؛"  {
     printf("semi colon \n");
     linecol=linecol+1; 
     return SEMICOLON; } 

{quote}[^\"]*{quote}   { 
     linecol=linecol+yyleng;
     strcpy(yylval.str,yytext);
     return CHAINE; }
{dquote}[^\"]*{dquote}       {
     linecol=linecol+yyleng;
     strcpy(yylval.str,yytext);
     return CHAINE; }
[0-9]+      { 
    linecol=linecol+yyleng;
    yylval.intv = atoi(yytext);
    return INT; }
[0-9]+"."[0-9]* {
      linecol=linecol+yyleng; 
      yylval.flt = atof(yytext);
      return FLOAT; }
{lettre}{lettre}*  {
     strcpy(yylval.str,yytext);
     linecol=linecol+yyleng;
     return ID; }
"//".*          { 
     linecol=1;
     return COMMENT; }
"\"\"\""(\\.|[^\"])*"\"\"\""   {
      linecol=1; 
      return COMMENT_PLUS;  }
"."      {
    linecol=linecol+1;
    return POINTEUR;
} 
"["      {
    printf("debut de tableau \n");
    linecol=linecol+1;
    return DEB_TABLEAU;
}    

"]"    {
    linecol=linecol+1;
    return FIN_TABLEAU;
}  
"+"         {
     linecol=linecol+1;
     return PLUS; }
"-"         {
    linecol=linecol+1;
     return MOINS; }
"*"         { 
      linecol=linecol+1;
      return MULT; }
"/"         { 
     linecol=linecol+1;
     return DIV; }
"%"         { 
      linecol=linecol+1;
      return MOD; }
"^"         { 
      linecol=linecol+1;
      return PUISS; }
"++"        { 
      printf("operateur increment \n");
      linecol=linecol+2;
      return INCREM; }
"--"        { 
     linecol=linecol+2;
     return DECREM; }
"=="        { 
     linecol=linecol+2;
     return EQUAL; }
"!="        { 
     linecol=linecol+2;
      return NOT_EQUAL; }
"!"        { 
      linecol=linecol+1;
      return NOT; }
"="         { 
      linecol=linecol+1;
      return ASSIGN; }
"=<"         { 
    linecol=linecol+2;
     return INF_EGAL; }
"=>"         {
     linecol=linecol+2;
     return SUPP_EGAL; }
"<"         {
     linecol=linecol+1;
      return INF; }
">"         { 
      linecol=linecol+1;
      return SUP; }
"||"        { 
     linecol=linecol+2;
     return OR; } 
"&&"        { 
     linecol=linecol+2;
     return AND; } 
"("        { 
      linecol=linecol+1;
      return PAR_OUV; } 
")"        { 
      linecol=linecol+1;
      return PAR_FERM; } 
"{"        { 
      printf("accolade de debut \n");
      linecol=linecol+1;
      return DEB_CORPS; } 
"}"        { 
      printf("accolade de fin \n");
      linecol=linecol+1;
      return FIN_CORPS; } 

[ \t\r]+   {
     // Ignorer les espaces, tabulations 
    linecol=linecol+yyleng;

} 

\n {
    // Ignorer retours à la ligne
     sauvline=line;
     line++;
     sauv = linecol;
     linecol=0;
}
. {
    fprintf(stderr, "File \"%s\", line %d, character %d: Unrecognized symbol '%s'\n", yyin_filename, line, linecol, yytext);
    exit(EXIT_FAILURE);
}
%%


