// TreeAnalyzer_aggregateB_ip3dSig_looserCut class for .../aggregateB_ip3dSig_looserCut/merged_HiForestAOD.root
// aggregateB_ip3dSig_looserCut ntuples find jets using the full list of final state particles
// and then perform SD with aggregated tracks from all SV's in a jet, chraged SJ

#include "TFile.h"
#include "TTree.h"
#include "../TreeAnalyzer_pt2dscan.h"

using namespace std;

class TreeAnalyzer_aggregateB_ip3dSig_looserCut : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_aggregateB_ip3dSig_looserCut(bool init = true);
};

TreeAnalyzer_aggregateB_ip3dSig_looserCut::TreeAnalyzer_aggregateB_ip3dSig_looserCut(bool init = true)
{    
    string path_bJet = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut/merged_HiForestAOD.root";
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

