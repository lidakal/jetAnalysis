// TreeAnalyzer_chargedSJ_partialB class for .../chargedSJ_partialB/mergedHiForestAOD.root

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_undecayHF_SD : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_undecayHF_SD(bool parORref = true, bool init = true);
};

TreeAnalyzer_undecayHF_SD::TreeAnalyzer_undecayHF_SD(bool bJetORqcd = true, bool init = true)
{
    string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/undecayHF_SD/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/undecayHF_SD/merged_HiForestAOD.root";
    
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

