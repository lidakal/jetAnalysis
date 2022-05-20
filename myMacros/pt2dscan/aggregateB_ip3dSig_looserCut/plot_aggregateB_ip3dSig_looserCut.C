#include "../TreeAnalyzer_pt2dscan.h"

#include <iostream>
#include <stdlib.h>


void plot_aggregateB_ip3dSig_looserCut()
{
    TreeAnalyzer_pt2dscan TA("/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut_fixedBugs/merged_HiForestAOD.root", true);
    
    std::string fname = "~/rootFiles/aggregateB_ip3dSig_looserCut";
    
    TA.plot_pt2dscan(fname, "ref", true);  // truth with GSP
    TA.plot_pt2dscan(fname, "reco", true);  // reco with GSP

    //TA.plot_pt2dscan(fname, "ref", false); // truth no GSP
    //TA.plot_pt2dscan(fname, "reco", false);  // reco no GSP
}
