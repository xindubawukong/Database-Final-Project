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
#include <cstring>
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
    parser::ColumnList* columnList;
    parser::Type* type;
    parser::Field* field;
    parser::FieldList* fieldList;
    parser::Constraint* cons;
    parser::ConstraintList* consList;
}

/* keyword */
%token QUIT
%token CREATE DROP USE SHOW TABLES DESC ADD CHANGE ALTER CLOSE
%token DATABASES DATABASE TABLE INDEX ON CONSTRAINT

/* COLUMN DESCPRITION */
%token T_INT T_FLOAT VARCHAR PRIMARY FOREIGN REFERENCES NOTNULL DEFAULT


/* number */
%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING IDENTIFIER VALUE_NULL

/* type */
%type <tree> dbStmt sysStmt tbStmt idxStmt alterStmt stmt
%type <string> tbName dbName idxName fkName columnName
%type <columnList> columnList
%type <string> value
%type <type> type
%type <consList> keyConstraints
%type <cons> keyConstrint
%type <field> field
%type <fieldList> fieldList
%%

program: %empty 
        {

        }
        | stmt program
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
            $$ = new parser::ShowDatabase();
            parser::Tree::setInstance($$);
            parser::Tree::run();
        }
        | SHOW DATABASE dbName 
        {
            $$ = new parser::ShowDatabase($3);
            parser::Tree::setInstance($$);
            parser::Tree::run();
        }
        ;

dbStmt: CREATE DATABASE dbName
        {
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
        | CLOSE 
        {
            $$ = new parser::CloseDatabase();
            parser::Tree::setInstance($$);
            parser::Tree::run();
        }
        | SHOW TABLE tbName
        {
            $$ = new parser::ShowTable($3);
            parser::Tree::setInstance($$);
            parser::Tree::run();
        }
        | SHOW TABLES
        {
            $$ = new parser::ShowTable();
            parser::Tree::setInstance($$);
            parser::Tree::run();
        }
        ;

tbStmt: CREATE TABLE tbName '(' fieldList ',' keyConstraints ')'
        {
            std::cout << $3 << std::endl;
            $$ = new parser::CreateTable($3, $5, $7);
            parser::Tree::setInstance($$);
            parser::Tree::run();
            delete $3;
        }
        | CREATE TABLE tbName '(' fieldList ')'
        {
            $$ = new parser::CreateTable($3, $5);
            parser::Tree::setInstance($$);
            parser::Tree::run();
            delete $3;
        }
        | DROP TABLE tbName
        {
            $$ = new parser::DropTable($3);
            parser::Tree::setInstance($$);
            parser::Tree::run();
            delete $3;
        }
        ;

keyConstraints: keyConstrint
                {
                   $$ = new parser::ConstraintList();
                   $$->add($1);
                }
                | keyConstraints ',' keyConstrint
                {
                   $$->add($3);
                }
                ;

keyConstrint: PRIMARY '(' columnList ')'
            {
              $$ = new parser::Constraint(true, $3, "PrimaryKey", "");
            }
            | FOREIGN fkName '(' columnList ')' REFERENCES tbName '(' columnList ')'
            {
              $$ = new parser::Constraint(false, $4, $2, $7, $9);
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
              $$ = new parser::FieldList();
              $$->add($1);
          }
          | fieldList ',' field
          {
              $$->add($3);
          }
          ;

field: columnName type
      {
          $$ = new parser::Field($1, $2->attrType, $2->attrLength);
          delete $1;
          delete $2;
      }
      | columnName type NOTNULL
      {
          $$ = new parser::Field($1, $2->attrType, $2->attrLength, true);
          delete $1;
          delete $2;
      }
      | columnName type DEFAULT value
      {
          $$ = new parser::Field($1, $2->attrType, $2->attrLength, false, $4);
          delete $1;
          delete $2;
      }
      | columnName type NOTNULL DEFAULT value
      {
          $$ = new parser::Field($1, $2->attrType, $2->attrLength, true, $5);
          delete $1;
          delete $2;
      }
      ;

type: T_INT 
      {
          $$ = new parser::Type();
          $$->attrType = AttrType::INT;
          $$->attrLength = 4;
      }
      | VARCHAR '(' VALUE_INT ')'
      {         
          $$ = new parser::Type();
          $$->attrType = AttrType::STRING;
          $$->attrLength = $3;
      }
      | T_FLOAT
      {
          $$ = new parser::Type();
          $$->attrType = AttrType::FLOAT;
          $$->attrLength = 4;
      }
      ;

value: VALUE_INT
      {
          $$ = new char[4];
          memcpy($$, &$1, 4);
      }
      | VALUE_STRING
      {
          memcpy($$, $1, std::strlen($1));
      }
      | VALUE_FLOAT
      {
          $$ = new char[4];
          memcpy($$, &$1, 4);
      }
      | VALUE_NULL
      {
          $$ = nullptr;
      }
      ;

columnList: columnName
            {
                $$ = new parser::ColumnList();
                $$->add($1);
            }
            | columnList ',' columnName
            {
                $$->add($3);
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