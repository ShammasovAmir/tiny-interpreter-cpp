//
// Created by Amir on 17.01.2024.
//

#ifndef LEXER_H
#define LEXER_H

#include "common.h"

#pragma region Token
enum class TokenType
{
    NULL_ = -7,
    NOT_A_KEYWORD = -3,
    EOF_ = -1, // put underscore at the end cause EOF is reserved
    NEWLINE = 0,
    NUMBER = 1,
    IDENT = 2,
    STRING = 3,
    // Keywords. Also add them in Lexer.cpp Lexer::checkIfKeyword method in keywords array
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
};

static std::map<std::string ,TokenType> mapTokenType = {
    {"NOT_A_KEYWORD", TokenType::NOT_A_KEYWORD},
    {"EOF_", TokenType::EOF_}, // put underscore at the end cause EOF is reserved
    {"NEWLINE", TokenType::NEWLINE},
    {"NUMBER", TokenType::NUMBER},
    {"IDENT", TokenType::IDENT},
    {"STRING", TokenType::STRING},
    // Keywords. Also add them in Lexer.cpp Lexer::checkIfKeyword method in keywords array
    {"LABEL", TokenType::LABEL},
    {"GOTO", TokenType::GOTO},
    {"PRINT", TokenType::PRINT},
    {"INPUT", TokenType::INPUT},
    {"LET", TokenType::LET},
    {"IF", TokenType::IF},
    {"THEN", TokenType::THEN},
    {"ENDIF", TokenType::ENDIF},
    {"WHILE", TokenType::WHILE},
    {"REPEAT", TokenType::REPEAT},
    {"ENDWHILE", TokenType::ENDWHILE},
    // Operators.
    {"EQ", TokenType::EQ},
    {"PLUS", TokenType::PLUS},
    {"MINUS", TokenType::MINUS},
    {"ASTERISK", TokenType::ASTERISK},
    {"SLASH", TokenType::SLASH},
    {"EQEQ", TokenType::EQEQ},
    {"NOTEQ", TokenType::NOTEQ},
    {"LT", TokenType::LT},
    {"LTEQ", TokenType::LTEQ},
    {"GT", TokenType::GT},
    {"GTEQ", TokenType::GTEQ},
};

// Move to another file later
struct Token
{
    // properties
    std::variant<char, std::string> text;
    TokenType                       kind;

    // constructor
    Token(const std::variant<char, std::string>& tokenText, const TokenType tokenKind)
    {
        this->text = tokenText;
        this->kind = tokenKind;
    }

    static std::string getTokenKeyFromValue(const TokenType& value);

    static TokenType checkIfKeyword(const std::variant<char, std::string>& tokenText);
};
#pragma endregion

#pragma region Lexer
struct Lexer
{
    // properties
    std::string source;
    char        curChar;
    int         curPos;

    // constructor
    explicit Lexer(const std::string& source)
    {
        this->source  = source;
        this->curChar = '\0';
        this->curPos  = -1;
        this->nextChar();
    }

    // methods
    void nextChar();

    char peek();

    void skipWhitespace();

    void skipComment();

    template <typename Enumeration>
    static auto getTokenValue(Enumeration value) -> typename std::underlying_type<Enumeration>::type;

    Token getToken();
};
#pragma endregion

#endif //LEXER_H
