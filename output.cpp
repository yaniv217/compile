#include "output.hpp"
#include <iostream>
#include <cstring>
#include <cctype>

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

int hexCharToInt(char c) {
    if (std::isdigit(c)) {
        return c - '0';
    }
    else if (std::isxdigit(c)) {
        return std::tolower(c) - 'a' + 10;
    }
    else {
        throw std::invalid_argument("Invalid hex character");
    }
}

// Convert a 2-character hex string to a char
char hexToAscii(const char *hex) {
    if (!hex) { throw std::invalid_argument("Null pointer"); }

    int high = hexCharToInt(hex[0]);
    int low = hexCharToInt(hex[1]);

    return static_cast<char>((high << 4) | low);
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
                case 'x':
                    text++;
                    out.push_back(hexToAscii(text));
                    text++;
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


const char *output::extractAfterSlash(const char *value) {
    // Find the first '/'
    const char *p = value;
    while (*p && *p != '\\') {
        p++;
    }

    // If no '/' found, return nullptr or "" depending on what you want
    if (*p != '\\') {
        return nullptr;  // or return "" if you prefer
    }

    const char *start = p + 1;  // start at the '/'
    p++; // move past '/'


    // Move until space or end
    while (*p && *p != ' ') {
        p++;
    }

    // Compute length
    size_t len = p - start;

    // Copy into a static buffer (safe for returning const char*)
    static char buffer[1024];
    if (len >= sizeof(buffer)) { len = sizeof(buffer) - 1; }

    for (size_t i = 0; i < 3; i++) {
        buffer[i] = start[i];
        if (buffer[i] != 'x' && i == 0) {
            break;
        }
    }
    buffer[len] = '\0';

    return buffer;
}
