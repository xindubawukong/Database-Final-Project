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
}

/* keyword */
%token QUIT
%token CREATE DROP USE SHOW TABLES DESC ADD CHANGE ALTER CLOSE
%token DATABASES DATABASE TABLE INDEX ON CONSTRAINT
%token SELECT FROM WHERE IS NOT AND LEX_GE LEX_LE LEX_NE INSERT
%token INTO VALUES DELETE SET UPDATE

/* COLUMN DESCPRITION */
%token T_INT T_FLOAT VARCHAR PRIMARY FOREIGN REFERENCES NOTNULL DEFAULT


/* number */
%token <ivalue> VALUE_INT
%token <fvalue> VALUE_FLOAT
%token <string> VALUE_STRING IDENTIFIER VALUE_NULL

/* type */
%type <string> tbName dbName idxName fkName columnName
%type <valueliststree> valueLists
%type <valuelisttree> valueList
%type <my_value> value
%type <compop> op
%type <my_col> col
%type <my_expr> expr
%type <whereclause> whereClause;
%type <tree> dbStmt sysStmt tbStmt idxStmt alterStmt stmt
%type <columnList> columnList
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
        | SELECT selector FROM tableList WHERE whereClause
        {

        }
        | INSERT INTO tbName VALUES valueLists
        {
          std::cout << "insert" << std::endl;
          $$ = new parser::Insert($3, $5);
          parser::Tree::setInstance($$);
          parser::Tree::run();
        }
        | DELETE FROM tbName WHERE whereClause
        {
          std::cout << "yacc delete" << std::endl;
          $$ = new parser::Delete($3, $5);
          parser::Tree::setInstance($$);
          parser::Tree::run();
        }
        | UPDATE tbName SET setClause
        {

        }
        ;

setClause:      columnName '=' value
                {

                }
                | setClause ',' columnName '=' value
                {

                }
                ;

valueLists:     '(' valueList ')'
                {
                  $$ = new parser::ValueLists($2);
                }
                | valueLists ',' '(' valueList ')'
                {
                  $$ = $1;
                  $$->AddValueList($4);
                }
                ;

valueList:      value
                {
                  $$ = new parser::ValueList($1);
                }
                | valueList ',' value
                {
                  $$ = $1;
                  $$->AddValue($3);
                }
                ;

selector:       '*'
                | cols
                {

                }
                ;

cols:   col
        {

        }
        | cols ',' col
        {

        }
        ;

col:    columnName
        {
          $$ = new parser::Col(NULL, $1);
        }
        | tbName '.' columnName
        {
          $$ = new parser::Col($1, $3);
        }
        ;

tableList:      tbName
                {

                }
                | tableList ',' tbName
                {

                }
                ;

whereClause:    col op expr
                {
                  $$ = new parser::WhereClause($1, $2, $3);
                }
                | col IS VALUE_NULL
                {
                  // TBD
                }
                | col IS NOT VALUE_NULL
                {
                  // TBD
                }
                | whereClause AND whereClause
                {
                  $$ = new parser::WhereClause($1, $3);
                }
                ;

op:     '='
        {
          $$ = CompOp::EQ_OP;
        }
        | LEX_GE
        {
          $$ = CompOp::GE_OP;
        }
        | LEX_LE
        {
          $$ = CompOp::LE_OP;
        }
        | LEX_NE
        {
          $$ = CompOp::NE_OP;
        }
        | '>'
        {
          $$ = CompOp::GT_OP;
        }
        | '<'
        {
          $$ = CompOp::LT_OP;
        }
        ;

expr:   col
        {
          $$ = new parser::Expr();
          $$->col = $1;
          $$->is_value = false;
        }
        | value
        {
          $$ = new parser::Expr();
          $$->value = $1;
          $$->is_value = true;
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
        $$ = new querylanguage::Value();
        $$->type = AttrType::INT;
        $$->data = new int($1);
      }
      | VALUE_STRING
      {
        $$ = new querylanguage::Value();
        $$->type = AttrType::STRING;
        $$->data = (void*)$1;
      }
      | VALUE_FLOAT
      {
        $$ = new querylanguage::Value();
        $$->type = AttrType::FLOAT;
        $$->data = new float($1);
      }
      | VALUE_NULL
      {
        $$ = new querylanguage::Value();
        $$->data = NULL;
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