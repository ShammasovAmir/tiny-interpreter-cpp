//
// Created by Amir on 20.01.2024.
//

#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "Lexer.h"

struct Parser
{
    // properties
    Lexer&    lexer;
    Token currentToken;
    Token peekToken;

    // constructor
    explicit Parser(Lexer& lexerInstance)
        : lexer(lexerInstance),
        currentToken("NULL", TokenType::NULL_),
        peekToken("NULL", TokenType::NULL_)
    {
        this->nextToken();
        this->nextToken(); // Call this twice to initialize current and peek.
    }

    // Return true if the current token matches.
    bool checkToken(TokenType kind) const;

    // Return true if the next token matches.
    bool checkPeek(TokenType kind) const;

    // Try to match current token. If not, error. Advances the current token.
    void match(TokenType kind);

    // Advance the current token
    void nextToken();

    /* Producion rules */

    // program ::= {statement}
    void program();

    // One of the following statements...
    void statement();

    // Newline
    // nl ::= '\n'+
    void newline();
};

#endif //PARSER_H
