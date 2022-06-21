#include "../HistDrawer_pt2dscan.h"

void draw_SD_ptCut()
{          
    std::string fname = "SD_ptCut";
    HistDrawer_pt2dscan HD(fname);
    HD.draw_pt2dscan(false);
}
