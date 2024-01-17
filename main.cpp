#include <iostream>
#include "Lexer.hpp"

int main()
{
    const std::string source = "+- */";
    Lexer lexer(source);

    Token token = lexer.getToken();
    try
    {
        while (token.kind != EOF_)
        {
            std::cout << token.kind << '\n';
            token = lexer.getToken();
            lexer.nextChar();
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Lexer error. " << e.what();
        return EXIT_FAILURE;
    }

    return 0;
}