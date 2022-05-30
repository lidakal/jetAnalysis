#include "../TreeAnalyzer_pt2dscan.h"
#include "TH3.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void plot_aggregateB_ip3dSig()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig/merged_HiForestAOD.root";
    std::string foutname = "~/rootFiles/aggregateB_ip3dSig";

    //TreeAnalyzer_pt2dscan TAref(finname, true);
    //TAref.plot_pt2dscan(foutname, "ref", true);  // truth with GSP

    TreeAnalyzer_pt2dscan TAreco(finname, true);
    //TAreco.plot_pt2dscan(foutname, "reco", true);  // reco with GSP
    TAreco.plot_pt2dscan(foutname, "reco", false);  // reco no GSP
}
