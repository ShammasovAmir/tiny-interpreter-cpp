#ifndef TINY_INTERPRETER_H
#define TINY_INTERPRETER_H

#endif //TINY_INTERPRETER_H

#include <iostream>
#include <string>

enum TokenType
{
    EOF_ = -1, // put underscore at the end cause EOF is reserved
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,
    // Keywords.
    LABEL = 101,
    GOTO = 102,
    PRINT = 103,
    INPUT = 104,
    LET = 105,
    IF = 106,
    THEN = 107,
    ENDIF = 108,
    WHILE = 109,
    REPEAT = 110,
    ENDWHILE = 111,
    // Operators.
    EQ = 201,
    PLUS = 202,
    MINUS = 203,
    ASTERISK = 204,
    SLASH = 205,
    EQEQ = 206,
    NOTEQ = 207,
    LT = 208,
    LTEQ = 209,
    GT = 210,
    GTEQ = 211,

    // !!!INVALID
    INVALID_,
};

// Move to another file later
class Token
{
public:
    // properties
    char      text;
    TokenType kind;

    // constructor
    Token(const char tokenText, const TokenType tokenKind)
    {
        this->text = tokenText;
        this->kind = tokenKind;
    }
};

class Lexer
{
public:
    // properties
    std::string source;
    char        curChar;
    int         curPos;

    // constructor
    explicit Lexer(const std::string& source)
    {
        this->source  = source + std::to_string('/n');
        this->curChar = '\0';
        this->curPos  = -1;
        this->nextChar();
    }

    // methods
    void nextChar();

    char peek();

    void skipWhitespace();

    Token getToken();
};