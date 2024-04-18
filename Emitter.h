//
// Created by Amir on 16.04.2024.
//

#ifndef EMITTER_H
#define EMITTER_H

#include "common.h"

struct Emitter
{
    std::string fullPath;
    std::string header;
    std::string code;

    explicit Emitter(const std::string& fullPath)
    {
        this->fullPath = fullPath;
    }

    void emit(const std::string& newCode);

    void emitLine(const std::string& newCode);

    void headerLine(const std::string& newCode);

    void writeFile() const;
};


#endif //EMITTER_H
