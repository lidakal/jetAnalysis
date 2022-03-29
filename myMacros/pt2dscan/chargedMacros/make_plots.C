#include "draw_pt2dscan_charged.C"
#include "draw_pt2dscan_GSP_charged.C"
#include "draw_pt2dscan_ratio_charged.C"
#include "draw_pt2dscan_GSP_ratio_charged.C"
#include <initializer_list>
    
int make_plots() 
{
    for (auto qtype : {'B', 'C', 'L'}) {
        
        draw_pt2dscan_charged(qtype, true, true);
        draw_pt2dscan_charged(qtype, true, false);
        draw_pt2dscan_charged(qtype, false, true);
        draw_pt2dscan_charged(qtype, false, false);
        
        draw_pt2dscan_ratio_charged(qtype, true, true);
        draw_pt2dscan_ratio_charged(qtype, true, false);
        draw_pt2dscan_ratio_charged(qtype, false, true);
        draw_pt2dscan_ratio_charged(qtype, false, false);
        
        if (qtype == 'L') {
            continue;
        }
        
        draw_pt2dscan_GSP_charged(qtype, true, true, true);
        draw_pt2dscan_GSP_charged(qtype, true, false, true);
        draw_pt2dscan_GSP_charged(qtype, true, true, false);
        draw_pt2dscan_GSP_charged(qtype, true, false, false);
        
        draw_pt2dscan_GSP_charged(qtype, false, true, true);
        draw_pt2dscan_GSP_charged(qtype, false, false, true);
        draw_pt2dscan_GSP_charged(qtype, false, true, false);
        draw_pt2dscan_GSP_charged(qtype, false, false, false);
        
        draw_pt2dscan_GSP_ratio_charged(qtype, true, true, true);
        draw_pt2dscan_GSP_ratio_charged(qtype, true, false, true);
        draw_pt2dscan_GSP_ratio_charged(qtype, true, true, false);
        draw_pt2dscan_GSP_ratio_charged(qtype, true, false, false);
        
        draw_pt2dscan_GSP_ratio_charged(qtype, false, true, true);
        draw_pt2dscan_GSP_ratio_charged(qtype, false, false, true);
        draw_pt2dscan_GSP_ratio_charged(qtype, false, true, false);
        draw_pt2dscan_GSP_ratio_charged(qtype, false, false, false);
    }
    return 0;
}