/* A Bison parser, made by GNU Bison 3.5.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 126 "/Users/xdbwk/Desktop/thu41/database/Database-Final-Project/parser/parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_USERS_XDBWK_DESKTOP_THU41_DATABASE_DATABASE_FINAL_PROJECT_PARSER_PARSER_TAB_HPP_INCLUDED  */
