#include "../HistDrawer_pt2dscan.h"

void draw_SD_ptCut_qcdMC()
{          
    std::string fname = "SD_ptCut_qcdMC";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan_parVSref("C", "Normal SD #it{c}-jets", false);
}
