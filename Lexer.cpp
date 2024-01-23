#include "Lexer.h"

#pragma region Token
std::string Token::getTokenKeyFromValue(const TokenType& value)
{
    const auto iterator = std::find_if(mapTokenType.begin(), mapTokenType.end(),
        [&](const std::pair<std::string, TokenType>& element)
        {
            return element.second == value;
        });
    if (iterator != mapTokenType.end()) return iterator->first;
    // If the value is not found in the map, return an empty key
    return {};
}

TokenType Token::checkIfKeyword(
    const std::variant<char, std::string>& tokenText)
{
    for (auto& [name, tokenType] : mapTokenType)
    {
        if (name == std::get<std::string>(tokenText)
            && Lexer::getTokenValue(tokenType) >= 100
            && Lexer::getTokenValue(tokenType) < 200)
            return tokenType;
    }
    return TokenType::NOT_A_KEYWORD;
}
#pragma endregion

#pragma region Lexer
void Lexer::nextChar()
{
    this->curPos += 1;
    if (this->curPos > this->source.size())
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

void Lexer::skipComment()
{
    if (this->curChar == '#')
    {
        while (this->curChar != '\n')
        {
            this->nextChar();
        }
    }
}

template <typename Enumeration>
auto Lexer::getTokenValue(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

Token Lexer::getToken()
{
    this->skipWhitespace();
    this->skipComment();

    switch (this->curChar)
    {
    case '+':
        this->nextChar();
        return Token(this->curChar, TokenType::PLUS);
    case '-':
        this->nextChar();
        return Token(this->curChar, TokenType::MINUS);
    case '*':
        this->nextChar();
        return Token(this->curChar, TokenType::ASTERISK);
    case '/':
        this->nextChar();
        return Token(this->curChar, TokenType::SLASH);
    case '=':
        {
            if (this->peek() == '=')
            {
                const char lastChar = this->curChar;
                this->nextChar();
                this->nextChar();
                return Token(std::to_string(lastChar + this->curChar), TokenType::EQEQ);
            }
            this->nextChar();
            return Token(this->curChar, TokenType::EQ);
        }
    case '>':
        {
            if (this->peek() == '=')
            {
                const char lastChar = this->curChar;
                this->nextChar();
                this->nextChar();
                return Token(std::to_string(lastChar + this->curChar), TokenType::GTEQ);
            }
            this->nextChar();
            return Token(this->curChar, TokenType::GT);
        }
    case '<':
        {
            if (this->peek() == '=')
            {
                const char lastChar = this->curChar;
                this->nextChar();
                this->nextChar();
                return Token(std::to_string(lastChar + this->curChar), TokenType::LTEQ);
            }
            this->nextChar();
            return Token(this->curChar, TokenType::LT);
        }
    case '!':
        {
            if (this->peek() == '=')
            {
                const char lastChar = this->curChar;
                this->nextChar();
                this->nextChar();
                return Token(std::to_string(lastChar + this->curChar), TokenType::NOTEQ);
            }
            throw std::runtime_error("Expected !=, got !" + std::to_string(this->peek()));
        }
    case '\n': this->nextChar();
return Token(this->curChar, TokenType::NEWLINE);
    case '\0': this->nextChar();
return Token(this->curChar, TokenType::EOF_);
    case '\"':
        {
            // Get the chars between quotations
            this->nextChar();
            const int startPos = this->curPos;

            while (this->curChar != '\"')
            {
                // Don't allow special characters in the string. No escape characters, newlines, tabs, or %.
                // We will be using C's printf on this string.
                if (this->curChar == '\r' || this->curChar == '\n' ||
                    this->curChar == '\t' || this->curChar == '\\' ||
                    this->curChar == '%')
                    throw std::runtime_error("Illegal character in string.");
                this->nextChar();
            }

            // Get the substring.
            std::string tokText = this->source.substr(startPos, this->curPos - startPos);
            this->nextChar();
            return Token(tokText, TokenType::STRING);
        }
    default:
        {
            if (std::isdigit(this->curChar))
            {
                // Leading character is a digit, so this must be a number.
                // //Get all consecutive digits and decimal if there is one.
                const int startPos = this->curPos;
                while (std::isdigit(this->peek())) this->nextChar();
                if (this->peek() == '.')
                {
                    this->nextChar();

                    // Must have at least one digit after decimal.
                    if (!std::isdigit(this->peek()))
                        throw std::runtime_error("Illegal character in number.");
                    while (std::isdigit(this->peek())) this->nextChar();
                }

                std::string tokText = this->source.substr(startPos, this->curPos - startPos + 1);
                this->nextChar();
                return Token(tokText, TokenType::NUMBER);
            }
            if (std::isalpha(this->curChar))
            {
                // Leading character is a letter, so this must be an identifier or a keyword.
                // Get all consecutive alpha numeric characters.
                const int startPos = this->curPos;
                while (std::isalpha(this->peek())) this->nextChar();

                // Check if the token is in the list of keywords.
                std::string tokText     = this->source.substr(startPos, this->curPos - startPos + 1);
                const TokenType keyword = Token::checkIfKeyword(tokText);

                if (keyword == TokenType::NOT_A_KEYWORD)
                {
                    this->nextChar();
                    return Token(tokText, TokenType::IDENT);
                }
                this->nextChar();
                return Token(tokText, keyword);
            }

            // Handle unknown token
            throw std::runtime_error("Unknown Token: " + std::to_string(this->curChar));
        }
    }
}
#pragma endregion

