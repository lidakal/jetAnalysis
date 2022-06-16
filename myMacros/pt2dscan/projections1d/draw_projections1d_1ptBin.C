#include "../HistDrawer_pt2dscan.h"

void draw_projections1d_1ptBin(bool GSPincl = false)
{
    Float_t ptrange[] = {50., 80.};
    Float_t ktrange[] = {-5., 5.};

    HistDrawer_pt2dscan HD;
    HD.draw_1d_projection(GSPincl, "rg", ptrange, ktrange);
    HD.draw_1d_projection(GSPincl, "zg", ptrange, ktrange);
}
