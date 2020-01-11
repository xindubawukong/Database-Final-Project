%option caseless

%{
#include "ast.h"
#include "parser.tab.hpp"
#include <cstdio>
#include <cstring>

char *get_text(const char *text, bool strip = false) {
    int length = strlen(text);
    char *buffer;
    if(not strip) {
        buffer = new char[length + 1];
        memcpy(buffer, text, (length + 1) * sizeof(char));
    } else {
        buffer = new char[length - 1];
        memcpy(buffer, text + 1, (length - 1) * sizeof(char));
        buffer[length - 2] = 0;
    }

    return buffer;
}

void yyerror(const char *);

%}


DIGIT           [0-9]
VALUE_INT       {DIGIT}+
FLOAT           {DIGIT}+\.{DIGIT}*
IDENTIFIER      [a-z][_a-z0-9]*
WHITESPACE      [ \t\n]
DATE_FORMAT     '[0-9]{4}-[0-9]{1,2}-[0-9]{1,2}'
VALUE_STRING    '(\\\\|\\n|\\r|\\\"|\\0|\\'|[^'\\])*'


%x COMMENT

%%

"--"            {BEGIN(COMMENT);}
<COMMENT>"\n"   {BEGIN(INITIAL);}
<COMMENT>.      {}

","             { /*printf(", "); */ return yytext[0]; }
"("             { /*printf("( "); */ return yytext[0]; }
")"             { /*printf(") "); */ return yytext[0]; }
"."             { /*printf(". "); */ return yytext[0]; }
";"             { /*printf("; "); */ return yytext[0]; }
"+"             { /*printf("+ "); */ return yytext[0]; }
"-"             { /*printf("- "); */ return yytext[0]; }
"/"             { /*printf("/ "); */ return yytext[0]; }
"*"             { /*printf("* "); */ return yytext[0]; }
QUIT            { /*printf("QUIT");*/ return QUIT; }
CREATE          { /*printf("CREATE "); */ return CREATE; }
DROP            { /*printf("DROP "); */ return DROP; }
USE             { /*printf("USE "); */ return USE; }
SHOW            { /*printf("SHOW "); */ return SHOW; }
DATABASES       { /*printf("DATABASES "); */ return DATABASES; }
DATABASE        { /*printf("DATABASE "); */ return DATABASE; }
TABLE           { /*printf("TABLE "); */ return TABLE; }
TABLES          { /*printf("TABLES "); */ return TABLES; }
REFERENCES      { /*printf("REFERENCES "); */ return REFERENCES; }
"NOT NULL"      { /*printf("NOT_NULL "); */ return NOTNULL; }
"PRIMARY KEY"   { /*printf("PRIMARY_KEY "); */ return PRIMARY; }
"FOREIGN KEY"   { /*printf("FOREIGN_KEY "); */ return FOREIGN; }
CHANGE          { return CHANGE; }
DESC            { return DESC; }
CONSTRAINT      { return CONSTRAINT; }
INDEX           { /*printf("INDEX "); */ return INDEX; }
NULL            { /*printf("NULL "); */ return VALUE_NULL; }
int             { /*printf("int "); */ return INT; }
float           { /*printf("float "); */ return FLOAT; }
varchar         { /*printf("varchar "); */ return VARCHAR; }
{FLOAT}         { /*printf("FLOAT "); */ sscanf(yytext, "%f", &yylval.fvalue); return VALUE_FLOAT; }
{VALUE_INT}       { /*printf("INTEGER "); */ sscanf(yytext, "%d", &yylval.ivalue); return VALUE_INT; }
{IDENTIFIER}    { yylval.string = get_text(yytext, false); /*printf("IDENTIFIER(%s) ", yylval.string);*/ return IDENTIFIER; }

{VALUE_STRING}  { yylval.string = get_text(yytext, false); /*printf("STRING(%s) ", yylval.string);*/ return VALUE_STRING; }
{WHITESPACE}    {}
.               { yyerror((std::string("invalid char ") + yytext).c_str()); }

%%

int yywrap() {
    return 1;
}