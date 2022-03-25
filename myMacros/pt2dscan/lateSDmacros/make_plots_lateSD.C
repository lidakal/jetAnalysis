#include "draw_pt2dscan_lateSD.C"
#include "draw_pt2dscan_GSP_lateSD.C"
#include "draw_pt2dscan_ratio_lateSD.C"
#include "draw_pt2dscan_GSP_ratio_lateSD.C"
#include <initializer_list>
    
int make_plots_lateSD() 
{
    for (auto qtype : {'B', 'C', 'L'}) {
        
        //draw_pt2dscan_lateSD(qtype, true, true);
        draw_pt2dscan_lateSD(qtype, true, false);
        //draw_pt2dscan_lateSD(qtype, false, true);
        //draw_pt2dscan_lateSD(qtype, false, false);
        
        //draw_pt2dscan_ratio_lateSD(qtype, true, true);
        draw_pt2dscan_ratio_lateSD(qtype, true, false);
        //draw_pt2dscan_ratio_lateSD(qtype, false, true);
        //draw_pt2dscan_ratio_lateSD(qtype, false, false);
        
        if (qtype == 'L') {
            continue;
        }
        
        //draw_pt2dscan_GSP_lateSD(qtype, true, true, true);
        //draw_pt2dscan_GSP_lateSD(qtype, true, false, true);
        draw_pt2dscan_GSP_lateSD(qtype, true, true, false);
        //draw_pt2dscan_GSP_lateSD(qtype, true, false, false);
        
        //draw_pt2dscan_GSP_lateSD(qtype, false, true, true);
        //draw_pt2dscan_GSP_lateSD(qtype, false, false, true);
        draw_pt2dscan_GSP_lateSD(qtype, false, true, false);
        //draw_pt2dscan_GSP_lateSD(qtype, false, false, false);
        
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, true, true, true);
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, true, false, true);
        draw_pt2dscan_GSP_ratio_lateSD(qtype, true, true, false);
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, true, false, false);
        
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, false, true, true);
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, false, false, true);
        draw_pt2dscan_GSP_ratio_lateSD(qtype, false, true, false);
        //draw_pt2dscan_GSP_ratio_lateSD(qtype, false, false, false);
        
    }
    return 0;
}