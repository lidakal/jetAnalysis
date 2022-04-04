// TreeAnalyzer_mergedB class for .../mergedB/mergedHiForestAOD.root
// mergedB ntuples find jets using the full list of final state particles
// and then perform SD with aggregated charged B and neutral B parts
// (as one merged particle), excluding neutrinos alltogether

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_mergedB : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_mergedB(bool init = true);
};

TreeAnalyzer_mergedB::TreeAnalyzer_mergedB(bool init = true)
{    
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/mergedB/merged_HiForestAOD.root";
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

