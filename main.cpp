#include <iostream>
#include "Lexer.h"

int main()
{
    const std::string source = "LET abc==++*12";
    Lexer lexer(source);

    Token token = lexer.getToken();

    for (int i = 1; i < source.length(); ++i)
    {
        std::string key = Token::getTokenKeyFromValue(token.kind);
        std::cout << "Key: " << key << '\n';

        try
        {
            token = lexer.getToken();
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "Lexer error. " << e.what();
            return EXIT_FAILURE;
        }

        if (token.kind == TokenType::EOF_) break;
    }

    return EXIT_SUCCESS;
}