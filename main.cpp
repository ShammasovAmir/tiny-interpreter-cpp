#include "Lexer.h"

int main()
{
    std::cout << std::filesystem::current_path() << '\n';

    std::ifstream inputFile(std::filesystem::current_path().string() + "./test.bas");

    if (!inputFile.is_open()) {
        std::cerr
            << "Error opening the file! Place the .bas file in the same directory where your executable is located"
            << std::endl;
        return EXIT_FAILURE;
    }

    std::string source;
    std::string line;

    while (std::getline(inputFile, line))
    {
        source += line + "\n";
    }

    inputFile.close();

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