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

    // Since some newlines are required in our grammar, need to skip the excess.
    while (this->checkToken(TokenType::NEWLINE))
    {
        this->nextToken();
    }

    // Parse all the statements in the program.
    while (!this->checkToken(TokenType::EOF_))
    {
        this->statement();
    }

    for (auto label = this->labelsGotoed.begin(); label != this->labelsGotoed.end(); label++)
    {
        if (!this->labelsDeclared.contains(*label))
        {
            const std::string& expectsString{ *label };
            throw std::runtime_error("Attempting to GOTO to undeclared label: " + expectsString);
        }
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
        else this->expression();
    }
    // "IF" comparison "THEN" {statement} "ENDIF"
    else if (this->checkToken(TokenType::IF))
    {
        std::cout << "STATEMENT-IF" << '\n';
        this->nextToken();
        this->comparison();

        this->match(TokenType::THEN);
        this->newline();

        // Zero or more statements in the body.
        while (!this->checkToken(TokenType::ENDIF))
        {
            this->statement();
        }
        this->match(TokenType::ENDIF);
    }
    // "WHILE" comparison "REPEAT" {statement} "ENDWHILE"
    else if (this->checkToken(TokenType::WHILE))
    {
        std::cout << "STATEMENT-WHILE" << '\n';
        this->nextToken();
        this->comparison();

        this->match(TokenType::REPEAT);
        this->newline();

        // Zero or more statements in the loop body.
        while (!this->checkToken(TokenType::ENDWHILE))
        {
            this->statement();
        }
        this->match(TokenType::ENDWHILE);
    }
    // "LABEL" ident
    else if (this->checkToken(TokenType::LABEL))
    {
        std::cout << "STATEMENT-LABEL" << '\n';
        this->nextToken();
        if (this->labelsDeclared.contains(std::get<std::string>(this->currentToken.text)))
            throw std::runtime_error("Label already exists: " +
                 std::get<std::string>(this->currentToken.text));
        this->labelsDeclared.insert(std::get<std::string>(this->currentToken.text));
        this->match(TokenType::IDENT);
    }
    // "GOTO" ident
    else if (this->checkToken(TokenType::GOTO))
    {
        std::cout << "STATEMENT-GOTO" << '\n';
        this->nextToken();
        this->labelsGotoed.insert(std::get<std::string>(this->currentToken.text));
        this->match(TokenType::IDENT);
    }
    // "LET" ident "=" expression
    else if (this->checkToken(TokenType::LET))
    {
        std::cout << "STATEMENT-LET" << '\n';
        this->nextToken();
        if (!this->symbols.contains(std::get<std::string>(this->currentToken.text)))
        {
            this->symbols.insert(std::get<std::string>(this->currentToken.text));
        }
        this->match(TokenType::IDENT);
        this->match(TokenType::EQ);
        this->expression();
    }
    // "INPUT" ident
    else if (this->checkToken(TokenType::INPUT))
    {
        std::cout << "STATEMENT-INPUT" << '\n';
        this->nextToken();
        if (!this->symbols.contains(std::get<std::string>(this->currentToken.text)))
        {
            this->symbols.insert(std::get<std::string>(this->currentToken.text));
        }
        this->match(TokenType::IDENT);
    }
    // This is not a valid statement. Error!
    else throw std::runtime_error("Invalid statement at " +
        std::get<std::string>(this->currentToken.text) + " (" +
        Token::getTokenKeyFromValue(this->currentToken.kind) + ")\n");

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

void Parser::comparison()
{
    std::cout << "COMPARISON" << '\n';

    this->expression();
    // Must be at least one comparison operator and another expression.
    if (this->isComparisonOperator())
    {
        this->nextToken();
        this->expression();
    }
    else throw std::runtime_error("Expected comparison operator at: " +
        Token::getTokenKeyFromValue(this->currentToken.kind));

    // Can have 0 or more comparison operator and expressions.
    while (this->isComparisonOperator())
    {
        this->nextToken();
        this->expression();
    }
}

bool Parser::isComparisonOperator() const
{
    return this->checkToken(TokenType::GT) || this->checkToken(TokenType::GTEQ)
        || this->checkToken(TokenType::LT) || this->checkToken(TokenType::LTEQ)
        || this->checkToken(TokenType::EQEQ) || this->checkToken(TokenType::NOTEQ);
}

void Parser::expression()
{
    std::cout << "EXPRESSION" << '\n';

    this->term();
    // Can have 0 or more +/- and expressions.
    while (this->checkToken(TokenType::PLUS) || this->checkToken(TokenType::MINUS))
    {
        this->nextToken();
        this->term();
    }
}

void Parser::term()
{
    std::cout << "TERM" << '\n';

    this->unary();
    // Can have 0 or more *// and expressions.
    while (this->checkToken(TokenType::ASTERISK) || this->checkToken(TokenType::SLASH))
    {
        this->nextToken();
        this->unary();
    }
}

void Parser::unary()
{
    std::cout << "UNARY" << '\n';

    // Optional unary +/-
    if (this->checkToken(TokenType::PLUS) || this->checkToken(TokenType::MINUS))
        this->nextToken();
    this->primary();
}

void Parser::primary()
{
    std::cout << "PRIMARY (" <<
        Token::getTokenKeyFromValue(this->currentToken.kind) <<
        ": " << std::get<std::string>(this->currentToken.text) << ")" << '\n';

    if (this->checkToken(TokenType::NUMBER))
        this->nextToken();
    else if (this->checkToken(TokenType::IDENT))
    {
        if (!this->symbols.contains(std::get<std::string>(this->currentToken.text)))
        {
            throw std::runtime_error("Referencing variable before assignment: " +
                std::get<std::string>(this->currentToken.text));
        }
        this->nextToken();
    }
    else throw std::runtime_error(&"Unexpected token at " [
        std::get<char>(this->currentToken.text)]);
}
