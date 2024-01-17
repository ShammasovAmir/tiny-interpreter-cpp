#include "Lexer.hpp"

void Lexer::nextChar()
{
    this->curPos += 1;
    if (this->curPos >= this->source.size())
    {
        this->curChar = '\0'; // EOF
    }
    else
    {
        this->curChar = this->source[this->curPos];
    }
}

char Lexer::peek()
{
    if (this->curPos + 1 >= this->source.size())
        return '\0';
    return this->source[this->curPos + 1];
}

void Lexer::skipWhitespace()
{
    while (this->curChar == ' ' || this->curChar == '\t' || this->curChar == '\r')
    {
        this->nextChar();
    }
}


Token Lexer::getToken()
{
    this->skipWhitespace();

    switch (this->curChar)
    {
    case '+': return Token(this->curChar, TokenType::PLUS);
    case '-': return Token(this->curChar, TokenType::MINUS);
    case '*': return Token(this->curChar, TokenType::ASTERISK);
    case '/': return Token(this->curChar, TokenType::SLASH);
    case '\n': return Token(this->curChar, TokenType::NEWLINE);
    case '\0': return Token(this->curChar, TokenType::EOF_);
    default:
        throw std::runtime_error("Unknown Token: " + std::to_string(this->curChar));
        return Token(this->curChar, TokenType::INVALID_); // Unknown token!
    }
}

