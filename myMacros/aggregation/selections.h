#include <iostream>

bool passSelection(int Selection, bool inSV, float ip3dSig)
{
    if (Selection == 1) {
        if (inSV) return true;
        return false;
    }

    if (Selection == 2) {
        if (inSV) return true;
        if (ip3dSig > 9.) return true;
        return false; 
    }

    if (Selection == 3) {

    }

    std::cout << "Selection out of bounds, returning false." << std::endl;
    return false;
}