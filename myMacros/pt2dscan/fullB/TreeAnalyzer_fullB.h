// TreeAnalyzer_fullB class for .../fullB/mergedHiForestAOD.root
// fullB ntuples find jets using the full list of final state particles
// and then perform SD with aggregated charged B and neutral B parts
// (two separate particles), excluding neutrinos alltogether

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_fullB : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_fullB(bool parORref = true, bool init = true);
};

TreeAnalyzer_fullB::TreeAnalyzer_fullB(bool bJetORqcd = true, bool init = true)
{
    string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/fullB/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/fullB/merged_HiForestAOD.root";
    
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

