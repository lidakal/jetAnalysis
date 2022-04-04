// TreeAnalyzer_chargedSJ_partialB class for .../chargedSJ_partialB/mergedHiForestAOD.root

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_chargedSJ_partialB : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_chargedSJ_partialB(bool init = true);
};

TreeAnalyzer_chargedSJ_partialB::TreeAnalyzer_chargedSJ_partialB(bool init = true)
{
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/chargedSJ_partialB/merged_HiForestAOD.root";
    
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

