#include "../TreeAnalyzer_pt2dscan.h"

#include <iostream>
#include <stdlib.h>


void plot_aggregateB_ip3dSigOnly()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSigOnly/merged_HiForestAOD.root";
    std::string foutname = "~/rootFiles/aggregateB_ip3dSigOnly";

    TreeAnalyzer_pt2dscan TAref(finname, true);
    TAref.plot_pt2dscan(foutname, "ref", true);  // truth with GSP
    TAref.plot_pt2dscan(foutname, "ref", false);  // truth no GSP

    TreeAnalyzer_pt2dscan TAreco(finname, true);
    //TAreco.plot_pt2dscan(foutname, "reco", true);  // reco with GSP
    //TAreco.plot_pt2dscan(foutname, "reco", false);  // reco no GSP
}
