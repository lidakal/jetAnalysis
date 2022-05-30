#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_ip3dSig_loosestCut()
{          
    std::string fname = "aggregateB_ip3dSig_loosestCut";
    HistDrawer_pt2dscan HD(fname);
    HD.do_pt2dscan(true);
}
