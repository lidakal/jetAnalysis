#include "../TreeAnalyzer_pt2dscan.h"
#include "TH3.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void plot_chargedSJ_mergedSVtracks_gen_reco()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    std::string foutname = "~/rootFiles/chargedSJ_mergedSVtracks_gen_reco";

    //TreeAnalyzer_pt2dscan TAref(finname, true);
    //TAref.plot_pt2dscan(foutname, "ref", true);  // truth with GSP

    TreeAnalyzer_pt2dscan TAreco(finname, true);
    //TAreco.plot_pt2dscan(foutname, "reco", true);  // reco with GSP
	TAreco.plot_pt2dscan(foutname, "reco", false);  // reco no GSP
}
