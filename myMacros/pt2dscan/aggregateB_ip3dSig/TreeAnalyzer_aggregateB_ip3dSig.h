// TreeAnalyzer_aggregateB_ip3dSig class for .../aggregateB_ip3dSig/merged_HiForestAOD.root
// aggregateB_ip3dSig ntuples find jets using the full list of final state particles
// and then perform SD with aggregated tracks from all SV's in a jet, chraged SJ

#include "TFile.h"
#include "TTree.h"
#include "../TreeAnalyzer_pt2dscan.h"

using namespace std;

class TreeAnalyzer_aggregateB_ip3dSig : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_aggregateB_ip3dSig(bool init = true);
};

TreeAnalyzer_aggregateB_ip3dSig::TreeAnalyzer_aggregateB_ip3dSig(bool init = true)
{    
    string path_bJet = "/data_CMS/cms/kalipoliti/aggregateB_ip3dSig/merged_HiForestAOD.root";
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

