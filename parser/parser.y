%require "3.2"
%language "c++"


%{
#include <cstdio>
#include "global.h"
#include "parser/ast.h"
#include "parser/parser.h"

int yylex();
void yyerror(const char *);

class Tree;
%}

// %union {
//     int ivalue;
//     AttrType attrType;
//     float fvalue;
//     char *string;
//     Tree *tree;
//     Field *field;
// }

/* keyword */
%token QUIT
%token CREATE DROP USE SHOW TABLES DESC ADD CHANGE ALTER 
%token DATABASES DATABASE TABLE INDEX ON CONSTRAINT

/* COLUMN DESCPRITION */
%token INT FLOAT VARCHAR PRIMARY FOREIGN REFERENCES NOTNULL DEFAULT


/* number */
%token <int> VALUE_INT
%token <float> VALUE_FLOAT
%token <char*> VALUE_STRING IDENTIFIER VALUE_NULL
%token END_OF_FILE 0

/* type */
%type <Tree*> dbStmt
%type <char*> tbName dbName idxName fkName columnName
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
            $$ = new CreateDatabase($3);
            Tree::setInstance($$);
            delete $3;
            Tree::run();
        }
        | DROP DATABASE dbName
        {
            $$ = new DropDatabase($3);
            Tree::setInstance($$);
            delete $3;
            Tree::run();
        }
        | USE dbName
        {
            $$ = new UseDatabase($2);
            Tree::setInstance($$);
            delete $2;
            Tree::run();
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

type: INT '(' VALUE_INT ')'
      {

      }
      | VARCHAR '(' VALUE_INT ')'
      {

      }
      | FLOAT
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
