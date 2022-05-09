// TreeAnalyzer_chargedSJ class for .../chargedSJ/merged_HiForestAOD.root
// chargedSJ ntuples find jets using the full list of final state particles
// and then perform SD with only charged particles with pt > 1 GeV

#include "TFile.h"
#include "TTree.h"
#include "../TreeAnalyzer_pt2dscan.h"

using namespace std;

class TreeAnalyzer_chargedSJ : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_chargedSJ(bool bJetORqcd = true, bool init = true);
};

TreeAnalyzer_chargedSJ::TreeAnalyzer_chargedSJ(bool bJetORqcd = true, bool init = true)
{    
    string path_bJet = "/data_CMS/cms/kalipoliti/bJetMC/chargedSJ/merged_HiForestAOD.root";
    string path_qcd = "/data_CMS/cms/kalipoliti/qcdMC/chargedSJ/merged_HiForestAOD.root";

    string path = "";
    if (bJetORqcd) {
        path = path_bJet;
    } else {
        path = path_qcd;
    }
    
    fin = new TFile(path.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

