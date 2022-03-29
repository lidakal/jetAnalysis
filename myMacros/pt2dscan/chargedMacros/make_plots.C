#include "draw_pt2dscan_undecayed.C"
#include "draw_pt2dscan_GSP_undecayed.C"
#include "draw_pt2dscan_ratio_undecayed.C"
#include "draw_pt2dscan_GSP_ratio_undecayed.C"
#include <initializer_list>
    
int make_plots() 
{
    for (auto qtype : {'B', 'C', 'L'}) {
        
        draw_pt2dscan_undecayed(qtype, true, true);
        draw_pt2dscan_undecayed(qtype, true, false);
        draw_pt2dscan_undecayed(qtype, false, true);
        draw_pt2dscan_undecayed(qtype, false, false);
        
        draw_pt2dscan_ratio_undecayed(qtype, true, true);
        draw_pt2dscan_ratio_undecayed(qtype, true, false);
        draw_pt2dscan_ratio_undecayed(qtype, false, true);
        draw_pt2dscan_ratio_undecayed(qtype, false, false);
        
        if (qtype == 'L') {
            continue;
        }
        
        draw_pt2dscan_GSP_undecayed(qtype, true, true, true);
        draw_pt2dscan_GSP_undecayed(qtype, true, false, true);
        draw_pt2dscan_GSP_undecayed(qtype, true, true, false);
        draw_pt2dscan_GSP_undecayed(qtype, true, false, false);
        
        draw_pt2dscan_GSP_undecayed(qtype, false, true, true);
        draw_pt2dscan_GSP_undecayed(qtype, false, false, true);
        draw_pt2dscan_GSP_undecayed(qtype, false, true, false);
        draw_pt2dscan_GSP_undecayed(qtype, false, false, false);
        
        draw_pt2dscan_GSP_ratio_undecayed(qtype, true, true, true);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, true, false, true);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, true, true, false);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, true, false, false);
        
        draw_pt2dscan_GSP_ratio_undecayed(qtype, false, true, true);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, false, false, true);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, false, true, false);
        draw_pt2dscan_GSP_ratio_undecayed(qtype, false, false, false);
    }
    return 0;
}