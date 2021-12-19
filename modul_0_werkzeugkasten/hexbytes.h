#include <string>

#ifndef HEXBYTES_H
#define HEXBYTES_H

class Hexbytes {
    public:
    static std::string hex2bytes(uint32_t zahl);
    static std::string hexbyte(uint32_t zahl);
    static std::string hexbyte(uint8_t zahl);
    static std::string hexbyte(int zahl);
    static std::string hexzahl(uint32_t zahl);
};

#endif