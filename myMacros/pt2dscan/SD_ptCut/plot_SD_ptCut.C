#include "../TreeAnalyzer_pt2dscan.h"

#include <iostream>
#include <stdlib.h>


void plot_SD_ptCut()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/SD_ptCut/merged_HiForestAOD.root";
    std::string foutname = "~/rootFiles/SD_ptCut";

    TreeAnalyzer_pt2dscan TAref(finname, true);
    //TAref.plot_pt2dscan(foutname, "ref", true);  // truth with GSP
    //TAref.plot_pt2dscan(foutname, "ref", false);  // truth no GSP

    TreeAnalyzer_pt2dscan TAreco(finname, true);
    //TAreco.plot_pt2dscan(foutname, "reco", true);  // reco with GSP
    //TAreco.plot_pt2dscan(foutname, "reco", false);  // reco no GSP

    TreeAnalyzer_pt2dscan TApar(finname, true);
    TApar.plot_pt2dscan(foutname, "par", true);  // reco with GSP
    TApar.plot_pt2dscan(foutname, "par", false);  // reco no GSP
}
