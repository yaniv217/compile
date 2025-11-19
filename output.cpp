#include "output.hpp"
#include <iostream>
#include <cstring>

static const std::string token_names[] = {
        "__FILLER_FOR_ZERO",
        "VOID",
        "INT",
        "BYTE",
        "BOOL",
        "AND",
        "OR",
        "NOT",
        "TRUE",
        "FALSE",
        "RETURN",
        "IF",
        "ELSE",
        "WHILE",
        "BREAK",
        "CONTINUE",
        "SC",
        "COMMA",
        "LPAREN",
        "RPAREN",
        "LBRACE",
        "RBRACE",
        "LBRACK",
        "RBRACK",
        "ASSIGN",
        "RELOP",
        "BINOP",
        "COMMENT",
        "ID",
        "NUM",
        "NUM_B",
        "STRING"
};

void output::printToken(int lineno, enum tokentype token, const char *value) {
    if (token == COMMENT) {
        std::cout << lineno << " COMMENT //" << std::endl;
    }
    else {
        std::cout << lineno << " " << token_names[token] << " " << value << std::endl;
    }
}

void output::errorUnknownChar(char c) {
    std::cout << "ERROR: Unknown character " << c << std::endl;
    exit(0);
}

void output::errorUnclosedString() {
    std::cout << "ERROR: Unclosed string" << std::endl;
    exit(0);
}

void output::errorUndefinedEscape(const char *sequence) {
    std::cout << "ERROR: Undefined escape sequence " << sequence << std::endl;
    exit(0);
}


std::string output::unescape(const char *text) {
    std::string out;

    // Skip leading quote if present
    if (*text == '\"') {
        text++;
    }

    // Find the end (we stop *before* a trailing quote, if it exists)
    // First compute the real end
    const char *end = text + strlen(text);

    // If last char is a quote, ignore it
    if (end > text && *(end - 1) == '\"') {
        end--;
    }

    // Now process the content between text..end-1
    while (text < end) {
        if (*text == '\\') {
            text++; // move past backslash

            switch (*text) {
                case 'n':
                    out.push_back('\n');
                    break;
                case 't':
                    out.push_back('\t');
                    break;
                case 'r':
                    out.push_back('\r');
                    break;
                case '0':
                    out.push_back('\0');
                    break;
                case '\\':
                    out.push_back('\\');
                    break;
                case '\"':
                    out.push_back('\"');
                    break;

                case '\0':
                    return out; // malformed but safe

                default:
                    // Unknown escape → treat literally
                    out.push_back(*text);
                    break;
            }
        }
        else {
            out.push_back(*text);
        }

        text++;
    }

    return out;
}