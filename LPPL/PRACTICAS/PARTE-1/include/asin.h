/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_ASIN_H_INCLUDED
# define YY_YY_ASIN_H_INCLUDED
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
    SEMI_COLON_ = 258,
    BOOL_ = 259,
    INT_ = 260,
    TRUE_ = 261,
    FALSE_ = 262,
    IGUAL_ = 263,
    MAS_IGUAL_ = 264,
    MENOS_IGUAL_ = 265,
    POR_IGUAL_ = 266,
    DIVIDIDO_IGUAL_ = 267,
    AND_DOBLE_ = 268,
    OR_DOBLE_ = 269,
    IGUAL_DOBLE_ = 270,
    DISTINTO_ = 271,
    MAYOR_ = 272,
    MENOR_ = 273,
    MAYOR_IGUAL_ = 274,
    MENOR_IGUAL = 275,
    MAS_ = 276,
    MENOS_ = 277,
    POR_ = 278,
    DIVIDIDO_ = 279,
    MODULO_ = 280,
    NEGADO_ = 281,
    INCREMENTO_ = 282,
    DECREMENTO_ = 283,
    CORCHETE_1_ = 284,
    CORCHETE_2_ = 285,
    PARENTESIS_1_ = 286,
    PARENTESIS_2_ = 287,
    LLAVE_1_ = 288,
    LLAVE_2_ = 289,
    FOR_ = 290,
    READ_ = 291,
    PRINT_ = 292,
    WHILE_ = 293,
    IF_ = 294,
    ELSE_ = 295,
    CTE_ = 296,
    ID_ = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ASIN_H_INCLUDED  */
