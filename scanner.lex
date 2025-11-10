%{
#include "tokens.hpp"
void error(char type)
%}

%option yylineno
%option noyywrap

digit   ([0-9])
letter  ([a-zA-Z])
whitespace  ([\n\r\t])
relationalOP	("=="|"!="|"<="|"<"|">="|">")
binaryOP	("+"|"-"|"*"|"/")
ID 	((letter)+(number|letter)*)
NUM	((0)|([1-9])+)
NUM_B ((NUM)(b))
hexdigit ([0-9A-Fa-f])
hex ("\\x")(hexdigit)(hexdigit)
LegalEsacapes ("\\\"|"\\""|"\\n"|"\\r"|"\\t"|"\\0")
ILeagaleEscapes ("\\"|\n|\r)
printablecharacters ([\x20-\x7E])
commentBreakers (\n|\r|\r\n)

%x STRING
%x COMMENT

%%
void    {return VOID;}
int    {return INT;}
byte    {return BYTE;}
bool    {return BOOL;}
and     {return AND;}
not 	{return NOT;}
true	{return TRUE;}
false	{return FALSE;}
return	{return RETURN;}
if	{return IF;}
else	{return ELSE;}
while	{return WHILE;}
break	{return BREAK;}
continue	{return CONTINUE;}
";"   {return SC;}
","   {return COMMA;}
"("	{return LPAREN;}
")"	{return RPAREN;}
"{"	{return LBRACE;}
"}"	{return RBRACE;}
"["	{return LBRACK;}
"]"	{return RBRACK;}
"="	{return ASSIGN;}
relationalOP	{return RELOP;}
["]   {BEGIN(STRING);}
<STRING>ILeagaleEscapes 	{return error("e");}
<STRING>printablecharacters{0,1024}	{;}
<STRING>["]   {return STRING;}
<STRING>. {return errorUnclosedString();}
"//"  BEGIN(COMMENT);
<COMMENT>commentBreakers	{return errorUnknownChar();}
<COMMENT>whitespace   {BEGIN(INITIAL);}
%%

error(char type){
	if type =="e":
		errorUndefinedEscape(name);
}