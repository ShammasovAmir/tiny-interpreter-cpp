#include "Lexer.h"
#include "Emitter.h"
#include "Parser.h"

int main()
{
    std::cout << "Tiny Basic Compiler" << '\n';

    std::ifstream inputFile(std::filesystem::current_path().string() + "./test.bas");

    if (!inputFile.is_open())
    {
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

    try
    {
        Lexer lexer(source);
        Emitter emitter("out.c");
        Parser parser(lexer, emitter);

        parser.program();
        emitter.writeFile();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Lexer error. " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "Parsing completed" << '\n';

    return EXIT_SUCCESS;
}
