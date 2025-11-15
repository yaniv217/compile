%{
#include "tokens.hpp"
%}

%option yylineno
%option noyywrap

digit   ([0-9])
letter  ([a-zA-Z])
whitespace  ([\n\r\t])
relationalOP	("=="|"!="|"<="|"<"|">="|">")
binaryOP	("+"|"-"|"*"|"/")
ID_ 	((letter)+(number|letter)*)
NUM_	((0)|([1-9])+)
NUM_B_ ((NUM_)(b))
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
(relationalOP)	{return RELOP;}
(binaryOP)	{return BINOP;}
["]   {BEGIN(STRING);}
<STRING>ILeagaleEscapes 	{return UNDIFIENDESCAPEERROR;}
<STRING>printablecharacters{0,1024}	{;}
<STRING>["]   {return STRING;}
<STRING>. {return UNCLOSEDSTRINGERROR;}
"//"  BEGIN(COMMENT);
<COMMENT>commentBreakers	{return UNKOWNCHARERROR;}
<COMMENT>whitespace   {BEGIN(INITIAL);}
(ID_) {return ID;}
(NUM_) {return NUM;}
(NUM_B_) {return NUM_B;}
%%