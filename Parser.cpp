//
// Created by Amir on 20.01.2024.
//

#include "Parser.h"

bool Parser::checkToken(TokenType kind) const
{
    return kind == this->currentToken.kind;
}

bool Parser::checkPeek(TokenType kind) const
{
    return kind == this->peekToken.kind;
}

void Parser::nextToken()
{
    this->currentToken = this->peekToken;
    this->peekToken = this->lexer.getToken();
}

void Parser::match(TokenType kind)
{
    if (!this->checkToken(kind))
        throw std::runtime_error("Expected: " + Token::getTokenKeyFromValue(kind) +
            ", got " + Token::getTokenKeyFromValue(this->currentToken.kind));

    this->nextToken();
}

void Parser::program()
{
    std::cout << "PROGRAM" << '\n';

    // Parse all the statements in the program.
    while (!this->checkToken(TokenType::EOF_))
    {

    }
}

void Parser::statement()
{
    // Check the first token to see what kind of statement this is.

    // "PRINT" (expression | string)
    if (this->checkToken(TokenType::PRINT))
    {
        std::cout << "STATEMENT-PRINT" << '\n';
        this->nextToken();

        if (this->checkToken(TokenType::STRING))
        {
            // Simple string
            this->nextToken();
        }
        else
        {
            // Expect an expression.
            //TODO:
        }
    }

    this->newline();
}

void Parser::newline()
{
    std::cout << "NEWLINE" << '\n';

    // Require at least one newline.
    this->match(TokenType::NEWLINE);
    // But we will allow extra newlines too, of course.
    while (this->checkToken(TokenType::NEWLINE))
    {
        this->nextToken();
    }
}


