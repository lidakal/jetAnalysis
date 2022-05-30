#include "../TreeAnalyzer_pt2dscan.h"
#include "TH3.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void plot_chargedSJ()
{
    std::string finnameB = "/data_CMS/cms/kalipoliti/bJetMC/chargedSJ/merged_HiForestAOD.root";
    std::string finnameL = "/data_CMS/cms/kalipoliti/qcdMC/chargedSJ/merged_HiForestAOD.root";
    std::string foutnameB = "~/rootFiles/chargedSJ_new_bJetMC";
    std::string foutnameL = "~/rootFiles/chargedSJ_new_qcdMC";

    // -------- bJetMC --------
    std::cout << "Processing " << finnameB << std::endl;

    //TreeAnalyzer_pt2dscan TAb_ref(finnameB, true);
    //TAb_ref.plot_pt2dscan(foutnameB, "ref", true);  // b-jets truth with GSP

    TreeAnalyzer_pt2dscan TAb_reco(finnameB, true);
    //TAb_reco.plot_pt2dscan(foutnameB, "reco", true);  // b-jets reco with GSP
	TAb_reco.plot_pt2dscan(foutnameB, "reco", false);  // b-jets reco no GSP

    // ---------- qcdMC ---------
    //std::cout << "Processing " << finnameL << std::endl;

    //TreeAnalyzer_pt2dscan TAl_ref(finnameL, true);
    //TAl_ref.plot_pt2dscan(foutnameL, "ref", true);  // l-jets truth with GSP

    //TreeAnalyzer_pt2dscan TAl_reco(finnameL, true);
    //TAl_reco.plot_pt2dscan(foutnameL, "reco", true);  // l-jets reco with GSP

    //TA.plot_pt2dscan(fname, "ref", false); // truth no GSP
    //TA.plot_pt2dscan(fname, "reco", false);  // reco no GSP
}
