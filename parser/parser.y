%{
#include <cstdio>
#include <stdlib.h>

#include "global.h"
#include "ast.h"
#include "parser.yy.cpp"

int yylex();
void yyerror(const char *);

class Tree;
%}

%union {
    int ivalue;
    AttrType attrType;
    float fvalue;
    char *string;
    Tree *tree;
    Field *field;
    

}

/* keyword */
%token QUIT
%token CREATE DROP USE SHOW TABLES DESC
%token DATABASES DATABASE TABLE INDEX ON

/* COLUMN DESCPRITION */
%token INT FLOAT VARCHAR PRIMARY FOREIGN REFERENCES NOTNULL DEFAULT


/* number */
%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING IDENTIFIER VALUE_NULL

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
      ;

sysStmt: SHOW DATABASES 
        {

        }
        ;

dbStmt: CREATE DATABASE dbName
        {

        }
        | DROP DATABASE dbName
        {

        }
        | USE dbName
        {

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

        }
        ;
tbName: IDENTIFIER
        {

        }
        ;
columnName: IDENTIFIER
            {

            }
            ;

