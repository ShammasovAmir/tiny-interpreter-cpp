//
// Created by Amir on 20.01.2024.
//

#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "Lexer.h"
#include "Emitter.h"

struct Parser
{
    // properties
    Lexer&                lexer;
    Emitter&              emitter;
    Token                 currentToken;
    Token                 peekToken;
    std::set<std::string> symbols; // Variables declared so far.
    std::set<std::string> labelsDeclared; // Labels declared so far.
    std::set<std::string> labelsGotoed; // Labels goto'ed so far.

    // constructor
    explicit Parser(Lexer& lexerInstance, Emitter& emitterInstance)
        : lexer(lexerInstance),
        emitter(emitterInstance),
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

    // comparison ::= expression (("==" | "!=" | ">" | ">=" | "<" | "<=") expression)+
    void comparison();

    //  Return true if the current token is a comparison operator.
    bool isComparisonOperator() const;

    // expression ::= term {( "-" | "+" ) term}
    void expression();

    // term ::= unary {( "/" | "*" ) unary}
    void term();

    // unary ::= ["+" | "-"] primary
    void unary();

    // primary ::= number | ident
    void primary();
};

#endif //PARSER_H
