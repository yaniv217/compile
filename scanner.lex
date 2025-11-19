%{
#include "tokens.hpp"
#include <stdio.h>
%}

%option yylineno
%option noyywrap

digit   ([0-9])
letter  ([a-zA-Z])
whitespace  ([ \n\r\t])
relationalOP	("=="|"!="|"<="|"<"|">="|">")
binaryOP	("+"|"-"|"*"|"/")
identifier	({letter})+({digit}|{letter})*
number	((0)|([1-9])+)
binarynumber ({number}(b))
hexnumber (\\(x[0-7][0-9a-fA-F]))
LegalEscapes    \\([\\ntr\"0])
IllegalEscapes   \\[^"nrt0\\]
printablecharacters ([ !#-\[\]-~	])
followingIlegalEsacape ([!#-\[\]-~])
commentBreakers (\n|\r|\r\n)


%%
{whitespace} { /* ignore */ }
void    return VOID;
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
\;   {return SC;}
\,   {return COMMA;}
\(	{return LPAREN;}
\)	{return RPAREN;}
\{	{return LBRACE;}
\}	{return RBRACE;}
\[	{return LBRACK;}
\]	{return RBRACK;}
\=	{return ASSIGN;}
{relationalOP}	{return RELOP;}
{binaryOP}	{return BINOP;}
\/\/[^\n\r]*	{return COMMENT;}
\"({printablecharacters}|{LegalEscapes}|{hexnumber})*\" {return STRING;}
\"({printablecharacters}|{LegalEscapes}|{hexnumber})* {return UNCLOSEDSTRINGERROR;}
\"({printablecharacters}|{LegalEscapes}|{hexnumber})*\\[^\\ntr\"0]({followingIlegalEsacape})* {return UNDIFIENDESCAPEERROR;}
{identifier} {return ID;}
{number} {return NUM;}
{binarynumber} {return NUM_B;}
.	{return UNKOWNCHARERROR;}
%%	