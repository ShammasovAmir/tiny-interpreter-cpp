//
// Created by Amir on 16.04.2024.
//

#include "Emitter.h"

void Emitter::emit(const std::string& newCode)
{
    this->code += newCode;
}

void Emitter::emitLine(const std::string& newCode)
{
    this->code += newCode + '\n';
}

void Emitter::headerLine(const std::string& newCode)
{
    this->code += newCode + '\n';
}

void Emitter::writeFile() const
{
    std::ofstream outputFile(this->fullPath);
    if (outputFile.is_open())
    {
        outputFile << this->header;
        outputFile << this->code;

        outputFile.close();

        std::cout << "Code has been generated." << std::endl;
    }
    else
    {
        std::cout << "Failed to generate code." << std::endl;  // Display an error message if file creation failed
    }
}
