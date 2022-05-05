// TreeAnalyzer_chargedSJ_mergedSVtracks_gen_reco class for .../chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root
// chargedSJ_mergedSVtracks_gen_reco ntuples find jets using the full list of final state particles
// and then perform SD with aggregated tracks from all SV's in a jet, chraged SJ

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_chargedSJ_mergedSVtracks_gen_reco : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_chargedSJ_mergedSVtracks_gen_reco(bool init = true);
};

TreeAnalyzer_chargedSJ_mergedSVtracks_gen_reco::TreeAnalyzer_chargedSJ_mergedSVtracks_gen_reco(bool init = true)
{    
    string path_bJet = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root";
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

