#include "TFile.h"
#include "TTree.h"

#include "TreeAnalyzer.h"

#include <cmath>
#include <iostream>

void genPartMatching(std::string finname = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root", Float_t ptCut = 1.)
{
    // Read data
    std::cout << "Reading trees from " << finname << std::endl;
    TreeAnalyzer ta = TreeAnalyzer(finname);

    std::cout << "Running with pt cut of " << ptCut << " GeV" << std::endl;
    
    ta.t->SetBranchStatus("*", 0);
    for (auto branchName : {"nref", "jtHadFlav", "jteta", 
                       "nselIPtrk", "nIP", "ipEta", "ipPhi",
                       "nsvtx", "svtxntrk", "svtxdls", "svtxdls2d", "svtxm", "svtxpt", "svtxmcorr",
                       "svtxTrPt", "svtxTrEta", "svtxTrPhi"}) { 
        ta.t->SetBranchStatus(branchName, 1);
    }
    
    // Create new file for match tree
    std::string foutname = "matchedInfo.root";
    std::cout <<  "Creating file " << foutname << std::endl;
    TFile *fout = new TFile(foutname.c_str(), "recreate");
    TTree *matches = new TTree("matches", "tree with matches");
    
    // Add the new branches to the matches
    Int_t nIPmax = 500;
    Int_t nIP;
    matches->Branch("nIP", nIP, "nIP/I");
    
    Int_t genPartMatchID[nIPmax];
    matches->Branch("genPartMatchID", genPartMatchID, "genPartMatchID[nIP]/I");

    // Loop over t entries
    Long64_t nentries = ta.t->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        
        if (ient > 10) break;
            
        // Print progress
        if (ient % 100000 == 0) {
            std::cout << "Entry: " << ient << std::endl;
        }
        
        ta.GetEntry(ient);

        // All tracks per event
        nIP = ta.nIP;
        
        for (Int_t itrack = 0; itrack < nIP; itrack++) {
            Float_t dRmin = 100;
            Float_t dR = dRmin;
            Int_t partMatchID = -1;

            Float_t trkEta = ta.ipEta[itrack];
            Float_t trkPhi = ta.ipPhi[itrack];

            Int_t mult = ta.mult;

            for (Int_t imult = 0; imult < mult; imult++) {
                Float_t partPt = (*ta.pt)[imult];
                Float_t partEta = (*ta.eta)[imult];
                Float_t partPhi = (*ta.phi)[imult];

                // Apply pt cut
                if (partPt < ptCut) continue;

                // Match track to gen level particle
                Float_t dEta = trkEta - partEta;
                Float_t dPhi = std::acos(std::cos(trkPhi - partPhi));
                dR = std::sqrt((dEta * dEta) + (dPhi * dPhi));

                if (dR < dRmin) {
                    dRmin = dR;
                    partMatchID = imult;
                }
            }
            genPartMatchID[itrack] = partMatchID;
        matches->Fill();
        }
    }
    std::cout << "Writing in file " << foutname << std::endl;
    matches->Write("matches");
    fout->Close();
}