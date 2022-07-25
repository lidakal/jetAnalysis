#include "TFile.h"
#include "TTree.h"

#include "TreeAnalyzer.h"

void makeDataTree()
{
    std::string finname = "/home/llr/cms/kalipoliti/CMSSW_9_4_10/src/HeavyIonsAnalysis/JetAnalysis/test/HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest

    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                                "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                                "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", "inSV", 
                                                "svtxTrEta", "svtxTrPhi", "svtxdls",
                                                "weight"};

    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal tree
    std::string foutName = "tmva_data.root";
    TFile *fout = new TFile(foutName.c_str(), "recreate");
    std::cout << "(Re)creating " << foutName << " file." << std::endl;

    TTree *TreeS = new TTree("TreeS", "Signal tree for TMVA");
    TTree *TreeB = new TTree("TreeB", "Background tree for TMVA");

    Float_t ip3dSig;
    Int_t inSV;
    Float_t weight;

    TreeS->Branch("ip3dSig", &ip3dSig, "ip3dSig/F");
    TreeS->Branch("inSV", &inSV, "inSV/I");
    TreeS->Branch("weight", &weight, "weight/F");

    TreeB->Branch("ip3dSig", &ip3dSig, "ip3dSig/F");
    TreeB->Branch("inSV", &inSV, "inSV/I");
    TreeB->Branch("weight", &weight, "weight/F");

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
            
        // for debugging purposes 
        //if (ient < 2) continue;
        //if (ient > 10) break;
            
        // Show progress
        if (ient % 100 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        weight = ta.weight;

        Int_t itrackOffset = 0;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
            // bjet & eta cut 
            bool isBjet = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;

            if ((!isBjet) || (std::abs(ta.jteta[ijet]) > 2)) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }

            // remove GSP 
            if (ta.jtNbHad[ijet] != 1) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }

            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                // Get track properties
                
                ip3dSig = ta.ip3dSig[itrackOffset + itrack];
                inSV = ta.inSV[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrackOffset + itrack];
                //Int_t signal = int(sta >= 100);

                weight = ta.weight;

                if (sta >= 100) {
                    TreeS->Fill();
                } else {
                    TreeB->Fill();
                }

            } // end track loop
        } // end jet loop
    } // end entry loop
    TreeS->Write();
    TreeB->Write();
    fout->Close();
    delete fout;
                
}