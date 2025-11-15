#include "tokens.hpp"
#include "output.hpp"

int main() {
    enum tokentype token;

    // read tokens until the end of file is reached
    while ((token = static_cast<tokentype>(yylex()))) {
        switch (token) {
            case UNKOWNCHARERROR:
                output::errorUnknownChar(*yytext);
                break;
            case UNCLOSEDSTRINGERROR:
                output::errorUnclosedString();
                break;
            case UNDIFIENDESCAPEERROR:
                output::errorUndefinedEscape(yytext);
                break;
            default:
                output::printToken(yylineno, token, yytext);
                break;
        }

    }
    return 0;
}