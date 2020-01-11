%{
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
#include "parser.yy.cpp"

int yylex();
void yyerror(const char *);

// class Tree;
%}

%union {
    int ivalue;
    AttrType attrType;
    float fvalue;
    char *string;
    parser::Tree* tree;
}

/* keyword */
%token QUIT
%token CREATE DROP USE SHOW TABLES DESC ADD CHANGE ALTER 
%token DATABASES DATABASE TABLE INDEX ON CONSTRAINT

/* COLUMN DESCPRITION */
%token T_INT T_FLOAT VARCHAR PRIMARY FOREIGN REFERENCES NOTNULL DEFAULT


/* number */
%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING IDENTIFIER VALUE_NULL

/* type */
%type <tree> dbStmt
%type <string> tbName dbName idxName fkName columnName
%%

program: %empty 
        {

        }
        | program stmt 
        {

        }
        ;

stmt: sysStmt ';'
      {

      }
      | dbStmt ';'
      {

      } 
      | tbStmt ';'
      {

      }
      | idxStmt ';'
      {

      }
      | alterStmt ';'
      {

      }
      | QUIT
      {
          YYACCEPT;
      }
      ;

sysStmt: SHOW DATABASES 
        {

        }
        ;

dbStmt: CREATE DATABASE dbName
        {
            std::cout << "Create" << std::endl;
            $$ = new parser::CreateDatabase($3);
            parser::Tree::setInstance($$);
            delete $3;
            parser::Tree::run();
        }
        | DROP DATABASE dbName
        {
            $$ = new parser::DropDatabase($3);
            parser::Tree::setInstance($$);
            delete $3;
            parser::Tree::run();
        }
        | USE dbName
        {
            $$ = new parser::UseDatabase($2);
            parser::Tree::setInstance($$);
            delete $2;
            parser::Tree::run();
        }
        | SHOW TABLES
        {

        }
        ;

tbStmt: CREATE TABLE tbName '(' fieldList keyConstraints ')'
        {

        }
        | DROP TABLE tbName
        {

        }
        | DESC tbName 
        {

        }
        ;

keyConstraints: %empty
                {

                }
                | ',' keyConstrint keyConstraints
                {

                }
                ;

keyConstrint: PRIMARY '(' columnList ')'
            {

            }
            | FOREIGN '(' columnList ')' REFERENCES tbName '(' columnList ')'
            {

            }
            ;

idxStmt: CREATE INDEX idxName ON tbName '(' columnList ')'
        {

        }
        | DROP INDEX idxName
        {

        }
        ;

alterStmt: ALTER TABLE tbName ADD PRIMARY '(' columnList ')'
        {

        }
        | ALTER TABLE tbName DROP PRIMARY
        {

        }
        | ALTER TABLE tbName ADD CONSTRAINT fkName FOREIGN '(' columnList ')' REFERENCES tbName '(' columnList ')'
        {

        }
        | ALTER TABLE tbName DROP FOREIGN fkName
        {

        }
        | ALTER TABLE tbName ADD columnName field
        {

        }
        | ALTER TABLE tbName DROP columnName
        {

        }
        | ALTER TABLE tbName CHANGE columnName field
        {

        }
        ;

fieldList: field
          {

          }
          | fieldList ',' field
          {

          }
          ;

field: columnName type
      {

      }
      | columnName type NOTNULL
      {

      }
      | columnName type DEFAULT value
      {

      }
      | columnName type NOTNULL DEFAULT value
      {

      }
      ;

type: T_INT '(' VALUE_INT ')'
      {

      }
      | VARCHAR '(' VALUE_INT ')'
      {

      }
      | T_FLOAT
      {

      }
      ;

value: VALUE_INT
      {

      }
      | VALUE_STRING
      {

      }
      | VALUE_FLOAT
      {

      }
      | VALUE_NULL
      {

      }
      ;

columnList: columnName
            {

            }
            | columnList ',' columnName
            {

            }
            ;

dbName: IDENTIFIER
        {
            $$ = $1;
        }
        ;
tbName: IDENTIFIER
        {
            $$ = $1;
        }
        ;
columnName: IDENTIFIER
            {
                $$ = $1;
            }
            ;

fkName: IDENTIFIER
        {
            $$ = $1;
        }
        ;

idxName: IDENTIFIER
        {
            $$ = $1;
        }
        ;

%%
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