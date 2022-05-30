#include "../TreeAnalyzer_pt2dscan.h"

#include <iostream>
#include <stdlib.h>


void plot_aggregateB_ip3dSig_loosestCut()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_loosestCut/merged_HiForestAOD.root";
    std::string foutname = "~/rootFiles/aggregateB_ip3dSig_loosestCut";

    //TreeAnalyzer_pt2dscan TAref(finname, true);
    //TAref.plot_pt2dscan(foutname, "ref", true);  // truth with GSP

    TreeAnalyzer_pt2dscan TAreco(finname, true);
    TAreco.plot_pt2dscan(foutname, "reco", true);  // reco with GSP

    //TA.plot_pt2dscan(fname, "ref", false); // truth no GSP
    //TA.plot_pt2dscan(fname, "reco", false);  // reco no GSP
}
