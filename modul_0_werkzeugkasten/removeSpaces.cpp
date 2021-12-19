# include "removeSpaces.h"
# include <algorithm>

// isspace() hängt von lokaler Implementierung ab
std::string RemoveSpaces::removeSpaces(std::string s)
{
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}