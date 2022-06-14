#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_ip3dSigOnly()
{          
    std::string fname = "aggregateB_ip3dSigOnly";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan(false);
}
