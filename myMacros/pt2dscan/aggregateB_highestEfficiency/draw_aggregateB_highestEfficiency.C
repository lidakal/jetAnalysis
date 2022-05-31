#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_highestEfficiency()
{          
    std::string fname = "aggregateB_highestEfficiency";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan(true);
}
