// TreeAnalyzer_aggregatedB_truthInfo class for .../aggregatedB_truthInfo/merged_HiForestAOD.root
// aggregatedB_truthInfo ntuples find jets using the full list of final state particles
// and then perform SD with aggregated charged B tagged with truth info (tracks matched to gen particles)
// excluding neutrinos alltogether, chraged SJ

#include "TFile.h"
#include "TTree.h"
#include "../../TreeAnalyzer.h"

using namespace std;

class TreeAnalyzer_aggregatedB_truthInfo : public TreeAnalyzer
{
    public: 
        TreeAnalyzer_aggregatedB_truthInfo(bool init = true);
};

TreeAnalyzer_aggregatedB_truthInfo::TreeAnalyzer_aggregatedB_truthInfo(bool init = true)
{    
    string path_bJet = "/data_CMS/cms/lkalipol//bJet2022/aggregatedB_truthInfo/merged_HiForestAOD.root";
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    if (init) {
        Init();
    }
}

