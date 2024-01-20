#include <iostream>
#include "Lexer.hpp"

int main()
{
    const std::string source = "*12";
    Lexer lexer(source);
    std::cout << source.size() << '\n';

    Token token = lexer.getToken();

    while (token.kind != TokenType::EOF_)
    {
        std::string key = Token::getTokenKeyFromValue(token.kind);
        std::cout << key << '\n';
        try
        {
            token = lexer.getToken();
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "Lexer error. " << e.what();
            return EXIT_FAILURE;
        }
        lexer.nextChar();
    }

    return EXIT_SUCCESS;
}