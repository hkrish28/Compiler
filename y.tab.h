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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    END = 258,
    PLUS = 259,
    MINUS = 260,
    DIV = 261,
    MUL = 262,
    NUM = 263,
    EQ = 264,
    READ = 265,
    WRITE = 266,
    BEG = 267,
    IF = 268,
    THEN = 269,
    ENDIF = 270,
    ELSE = 271,
    WHILE = 272,
    ENDWHILE = 273,
    LT = 274,
    LTE = 275,
    GT = 276,
    GTE = 277,
    NEQ = 278,
    ISEQ = 279,
    DO = 280,
    BREAK = 281,
    CONTINUE = 282,
    REPEAT = 283,
    UNTIL = 284,
    DECL = 285,
    ENDDECL = 286,
    INT = 287,
    STR = 288,
    ID = 289
  };
#endif
/* Tokens.  */
#define END 258
#define PLUS 259
#define MINUS 260
#define DIV 261
#define MUL 262
#define NUM 263
#define EQ 264
#define READ 265
#define WRITE 266
#define BEG 267
#define IF 268
#define THEN 269
#define ENDIF 270
#define ELSE 271
#define WHILE 272
#define ENDWHILE 273
#define LT 274
#define LTE 275
#define GT 276
#define GTE 277
#define NEQ 278
#define ISEQ 279
#define DO 280
#define BREAK 281
#define CONTINUE 282
#define REPEAT 283
#define UNTIL 284
#define DECL 285
#define ENDDECL 286
#define INT 287
#define STR 288
#define ID 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
