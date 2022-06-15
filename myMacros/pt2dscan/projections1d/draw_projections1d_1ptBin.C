#include "../HistDrawer_pt2dscan.h"

void draw_projections1d_1ptBin(bool GSPincl = false)
{
    HistDrawer_pt2dscan HD;
    //HD.draw_1d_projection(GSPincl, "rg");
    HD.draw_1d_projection(GSPincl, "zg");
}
