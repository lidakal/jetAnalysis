#include "draw_pt2dscan.C"
#include "draw_pt2dscan_GSP.C"
#include "draw_pt2dscan_ratio.C"
#include "draw_pt2dscan_GSP_ratio.C"
#include <initializer_list>
    
int make_plots() 
{
    for (auto qtype : {'B', 'C', 'L'}) {
        /*
        draw_pt2dscan(qtype, true, true);
        draw_pt2dscan(qtype, true, false);
        draw_pt2dscan(qtype, false, true);
        draw_pt2dscan(qtype, false, false);
        */
        draw_pt2dscan_ratio(qtype, true, true);
        draw_pt2dscan_ratio(qtype, true, false);
        draw_pt2dscan_ratio(qtype, false, true);
        draw_pt2dscan_ratio(qtype, false, false);
        
        if (qtype == 'L') {
            continue;
        }
        /*
        draw_pt2dscan_GSP(qtype, true, true, true);
        draw_pt2dscan_GSP(qtype, true, false, true);
        draw_pt2dscan_GSP(qtype, true, true, false);
        draw_pt2dscan_GSP(qtype, true, false, false);
        
        draw_pt2dscan_GSP(qtype, false, true, true);
        draw_pt2dscan_GSP(qtype, false, false, true);
        draw_pt2dscan_GSP(qtype, false, true, false);
        draw_pt2dscan_GSP(qtype, false, false, false);
        */
        draw_pt2dscan_GSP_ratio(qtype, true, true, true);
        draw_pt2dscan_GSP_ratio(qtype, true, false, true);
        draw_pt2dscan_GSP_ratio(qtype, true, true, false);
        draw_pt2dscan_GSP_ratio(qtype, true, false, false);
        
        draw_pt2dscan_GSP_ratio(qtype, false, true, true);
        draw_pt2dscan_GSP_ratio(qtype, false, false, true);
        draw_pt2dscan_GSP_ratio(qtype, false, true, false);
        draw_pt2dscan_GSP_ratio(qtype, false, false, false);
    }
    return 0;
}