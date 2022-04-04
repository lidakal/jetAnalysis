// TreeAnalyzer_chargedSJ class for .../chargedSJ/mergedHiForestAOD.root

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_chargedSJ : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_chargedSJ(bool parORref = true, bool init = true);
};

TreeAnalyzer_chargedSJ::TreeAnalyzer_chargedSJ(bool bJetORqcd = true, bool init = true)
{
    string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/chargedSJ/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/chargedSJ/merged_HiForestAOD.root";
    
    if (bJetORqcd) {
        fin = new TFile(path_bJet.c_str());
    } else {
        fin = new TFile(path_qcd.c_str());
    }

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

