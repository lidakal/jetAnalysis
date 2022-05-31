#include "../HistDrawer_pt2dscan.h"

void draw_aggregateB_ip3dSig_looserCut()
{          
    std::string fname = "aggregateB_ip3dSig_looserCut";
    HistDrawer_pt2dscan HD(fname);
    HD.do_pt2dscan(true);
}
