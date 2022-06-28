#include "../HistDrawer_pt2dscan.h"

void draw_chargedSJ()
{          
    std::string fname = "chargedSJ_new";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan("B", "#it{b}-jets with charged subjets", false);
}
