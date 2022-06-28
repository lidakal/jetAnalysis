#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_truthInfo_fixedBugs()
{          
    std::string fname = "aggregateB_truthInfo_fixedBugs";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan("B", "#it{b}-jets with charged subjets", false);
}
