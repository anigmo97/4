/*****************************************************************************/
/**  alex.l   PARTE 1                       2018-2019                       **/
/*****************************************************************************/
%{
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "asin.h"

%}
/*----------------------------------------------------------------------
  Dado que las funciones "input()" y "unput(c)" son costosas y no las
  utilizaremos, evitaremos que se generen automaticamente sus rutinas
  correspondientes desactivandolas mediante las siguientes opciones:
----------------------------------------------------------------------*/
%option noinput
%option nounput
/*----------------------------------------------------------------------
  Para mantener el numero de linea actual en la variable global yylineno
----------------------------------------------------------------------*/
%option yylineno


delimitador     	      [ \t\n]+
digito          	      [0-9]
entero          	      {digito}+
comentario			        \/\/.*\n
comentario_bloque	      \/\*.*\*\/
letra                   [a-zA-Z]
id                      {letra}({letra}|{digito}|_)*
cte_real_1              {digito}*\.({digito})+ 
cte_real_2              {digito}+\.({digito})*
cte_entera              {digito}+


%%

{delimitador}   		      {ECHO; }
{comentario}			        {ECHO;  }
{comentario_bloque}		    {ECHO;  }
";"                       {ECHO; return(SEMI_COLON_); }
"bool"                    {ECHO; return(BOOL_); }
"int"                     {ECHO; return(INT_); }
"="                       {ECHO; return(IGUAL_); }
"+="                      {ECHO; return(MAS_IGUAL_); }
"-="                      {ECHO; return(MENOS_IGUAL_); }
"*="                      {ECHO; return(POR_IGUAL_); }
"/="                      {ECHO; return(DIVIDIDO_IGUAL_); }
"&&"                      {ECHO; return(AND_DOBLE_); }
"||"                      {ECHO; return(OR_DOBLE_); }
"=="                      {ECHO; return(IGUAL_DOBLE_); }
"!="                      {ECHO; return(DISTINTO_); }
">"                       {ECHO; return(MAYOR_); }
"<"                       {ECHO; return(MENOR_); }
">="                      {ECHO; return(MAYOR_IGUAL_); }
"<="                      {ECHO; return(MENOR_IGUAL_); }
"+"                       {ECHO; return(MAS_); }
"-"                       {ECHO; return(MENOS_); }
"*"                       {ECHO; return(POR_); }
"/"                       {ECHO; return(DIVIDIDO_); }
"%"                       {ECHO; return(MODULO_); }
"!"                       {ECHO; return(NEGADO_); }
"++"                      {ECHO; return(INCREMENTO_); }
"--"                      {ECHO; return(DECREMENTO_); }
"["                       {ECHO; return(CORCHETE_1_); }
"]"                       {ECHO; return(CORCHETE_2_); }
"{"                       {ECHO; return(LLAVE_1_); }
"}"                       {ECHO; return(LLAVE_2_); }
"("             		      {ECHO; return(PARENTESIS_1_); }
")"             		      {ECHO; return(PARENTESIS_2_); }
"for"                     {ECHO; return(FOR_); }
"read"                    {ECHO; return(READ_); }
"print"                   {ECHO; return(PRINT_); }
"while"                   {ECHO; return(WHILE_); }
"if"                      {ECHO; return(IF_); }
"else"                    {ECHO; return(ELSE_); }
"true"                    {ECHO; return(TRUE_); }
"false"                   {ECHO; return(FALSE_); }
{cte_real_1}              {ECHO; yylval.cent=atoi(yytext); return(CTE_);}
{cte_real_2}              {ECHO; yylval.cent=atoi(yytext); return(CTE_);}
{cte_entera}              {ECHO; yylval.cent=atoi(yytext); return(CTE_);}
{id}                      {ECHO; yylval.ident=strdup(yytext); return(ID_); }
.               		      { yyerror("Caracter desconocido -- ANALIZADOR LEXICO alex.l");}

%%

/*****************************************************************************/
