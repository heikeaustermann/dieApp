#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#ifndef STRINGEXEC_H
#define STRINGEXEC_H

class Stringexec {
    public:
    static std::string exec(const char* cmd);
};

#endif