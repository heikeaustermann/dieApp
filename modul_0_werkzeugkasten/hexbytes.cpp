#include "hexbytes.h"
#include <math.h>

std::string Hexbytes::hex2bytes(uint32_t zahl) {
    uint32_t eins = floor(zahl/4096);
    eins = eins%16;
    uint32_t zwei = floor(zahl/256);
    uint32_t drei = floor(zahl/16);
    uint32_t vier = zahl%16;
    std::string rueckgabe = hexzahl(eins).append(hexzahl(zwei)).append(hexzahl(drei)).append(hexzahl(vier));
    return rueckgabe;
}

std::string Hexbytes::hexbyte(uint8_t zahl) {
    uint32_t eins = floor(zahl/16);
    eins = eins%16;
    uint32_t zwei = zahl%16;
    std::string rueckgabe = hexzahl(eins).append(hexzahl(zwei));
    return rueckgabe;
}

std::string Hexbytes::hexbyte(int zahl) {
    uint32_t eins = floor(zahl/16);
    eins = eins%16;
    uint32_t zwei = zahl%16;
    std::string rueckgabe = hexzahl(eins).append(hexzahl(zwei));
    return rueckgabe;
}

std::string Hexbytes::hexbyte(uint32_t zahl) {
    uint32_t eins = floor(zahl/16);
    eins = eins%16;
    uint32_t zwei = zahl%16;
    std::string rueckgabe = hexzahl(eins).append(hexzahl(zwei));
    return rueckgabe;
}

std::string Hexbytes::hexzahl(uint32_t zahl) {
    uint32_t wert = zahl%16;
    std::string rueckgabe;
    switch (wert)
    {
    case 0:
        rueckgabe = "0";
        break;
    case 1:
        rueckgabe = "1";
        break;
    case 2:
        rueckgabe = "2";
        break;
    case 3:
        rueckgabe = "3";
        break;
    case 4:
        rueckgabe = "4";
        break;
    case 5:
        rueckgabe = "5";
        break;
    case 6:
        rueckgabe = "6";
        break;
    case 7:
        rueckgabe = "7";
        break;
    case 8:
        rueckgabe = "8";
        break;
    case 9:
        rueckgabe = "9";
        break;
    case 10:
        rueckgabe = "a";
        break;
    case 11:
        rueckgabe = "b";
        break;
    case 12:
        rueckgabe = "c";
        break;
    case 13:
        rueckgabe = "d";
        break;
    case 14:
        rueckgabe = "e";
        break;
    case 15:
        rueckgabe = "f";
        break;
    default:
        rueckgabe = "0";
        break;
    }
    return rueckgabe;
}