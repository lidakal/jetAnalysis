#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_SV_ip3dSig_tight()
{          
    std::string fname = "aggregateB_SV_ip3dSig_tight";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan(false);
}
