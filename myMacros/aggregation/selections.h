#include <iostream>

bool passSelection(int Selection, bool inSV, float ip3dSig)
{
    if (Selection == 1) {
        if (inSV) return true;
        return false;
    }

    if (Selection == 2) {
        if (inSV) return true;
        if (ip3dSig > 3.) return true;
        return false; 
    }

    if (Selection == 3) {
        if (inSV) return true;
        if (ip3dSig > 6.) return true;
        return false; 
    }

    if (Selection == 4) {
        if (inSV) return true;
        if (ip3dSig > 9.) return true;
        return false; 
    }

    if (Selection == 5) {
        if (inSV) return true;
        if (ip3dSig > 12.) return true;
        return false; 
    }

    if (Selection == 6) {
        if (ip3dSig > 3.) return true;
        return false; 
    }

    if (Selection == 7) {
        if (ip3dSig > 6.) return true;
        return false; 
    }

    if (Selection == 8) {
        if (ip3dSig > 9.) return true;
        return false; 
    }

    if (Selection == 9) {
        if (ip3dSig > 12.) return true;
        return false; 
    }

    if (Selection == 10) {
        if (!inSV) return false;
        if (ip3dSig > 3.) return true;
        return false; 
    }

    if (Selection == 11) {
        if (!inSV) return false;
        if (ip3dSig > 6.) return true;
        return false; 
    }

    if (Selection == 12) {
        if (!inSV) return false;
        if (ip3dSig > 9.) return true;
        return false; 
    }

    if (Selection == 13) {
        if (!inSV) return false;
        if (ip3dSig > 12.) return true;
        return false; 
    }

	if (Selection == 14) {
	    if ((ip3dSig > 3.) || (ip3dSig < -3.)) return true;
	    return false;
	}


    std::cout << "Selection out of bounds, returning false." << std::endl;
    return false;
}
