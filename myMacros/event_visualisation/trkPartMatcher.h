#include "TFile.h"
#include "TTree.h"
#include "../TreeAnalyzer.h"

class trkPartMatcher : public TreeAnalyzer
{
    public: 
        TTree *hi;
        TTree *matches;

        trkPartMatcher(std::string filename = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root", bool init = true);
        void InitHi();
        void matchTrkGenPart(double ptCut = 1.);
        

        // hi tree leaves
        Int_t mult;
        std::vector<float> pt;
        std::vector<float> eta;
        std::vector<float> phi;
        std::vector<int> pdg;
        std::vector<int> sta;
};

trkPartMatcher::trkPartMatcher(std::string filename = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForest.root", bool init = true)
{
    fin = new TFile(filename.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("HiTree=hiEvtAnalyzer/HiTree");

    hi = (TTree *) fin->Get("bDecayAna/hi");

    if (init) {
        Init();
        InitHi();
    }
}

void trkPartMatcher::InitHi()
{
    hi->SetBranchAddress("mult", &mult);
    hi->SetBranchAddress("pt", &pt);
    hi->SetBranchAddress("eta", &eta);
    hi->SetBranchAddress("phi", &phi);
    hi->SetBranchAddress("pdg", &pdg);
    hi->SetBranchAddress("sta", &sta);
}

void trkPartMatcher::matchTrkGenPart(double ptCut = 1.)
{
    t->SetBranchStatus("*", 0);
    for (auto branchName : {"nref", "jtHadFlav", "jteta", "nselIPtrk", "nIP", "ipEta", "ipPhi"}) {
        t->SetBranchStatus(branchName, 1);
    }

    TFile *fout = new TFile("~/rootFiles/matches.root");

    matches = (TTree *) t->CloneTree();
    matches->SetName("matches");
    matches->AddFriend(hi);

    const Int_t nIPmax = 100;
    Int_t genPartMatchID[nIPmax];
    matches->Branch("genPartMatchID", genPartMatchID, "genPartMatchID[nIP]/I");

    for (Long64_t i = 0; i < nentries; i++) {
        matches->GetEntry(i);

        for (Int_t itrk = 0; itrk < nIP; itrk++) {
            double dRmin = 100;
            double dR = dRmin;

            int partMatchID = -1;

            for (int imult = 0; imult < mult; imult++) { 
                if (pt[imult] < ptCut) continue;

                double dEta = ipPt[itrk] - pt[imult];
                double dPhi = std::acos(std::cos(ipPhi[itrk] - phi[imult]));
                dR = std::sqrt(dEta * dEta + dPhi * dPhi);

                if (dR < dRmin) {
                    dRmin = dR;
                    partMatchID = imult;
                }
            }
            genPartMatchID[itrk] = partMatchID;
        }
        matches->Fill();
    }
    matches->Write();
    fout->Close();
}
