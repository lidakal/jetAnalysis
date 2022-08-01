#include "TFile.h"
#include "TTree.h"

#include "TreeAnalyzer.h"

void makeDataTree()
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/trackTaggingTraining/merged_HiForestAOD.root";
    TreeAnalyzer ta(finname, true);

    // Activate branches of interest

    std::vector<std::string> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav", "jtNbHad", 
                                                "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB", 
                                                "nselIPtrk", "ipEta", "ipPhi", "ipMatchStatus", "ip3dSig", "ipInSV", 
                                                "ipSvtxdls", "ipSvtxdls2d", "ipSvtxm", "ipSvtxmcorr",
                                                "svtxTrEta", "svtxTrPhi", "svtxdls",
                                                "weight"};

    ta.SetBranchStatus("*", 0);
    ta.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal tree
    std::string foutName = "tmva_data_complex_bJetMC.root";
    TFile *fout = new TFile(foutName.c_str(), "recreate");
    std::cout << "(Re)creating " << foutName << " file." << std::endl;

    TTree *TreeS = new TTree("TreeS", "Signal tree for TMVA");
    TTree *TreeB = new TTree("TreeB", "Background tree for TMVA");

    Float_t ip3dSig;
    Int_t ipInSV;
    Float_t ipSvtxdls;
    Float_t ipSvtxdls2d;
    Float_t ipSvtxm;
    Float_t ipSvtxmcorr;
    Float_t weight;

    TreeS->Branch("ip3dSig", &ip3dSig, "ip3dSig/F");
    TreeS->Branch("ipInSV", &ipInSV, "ipInSV/I");
    TreeS->Branch("ipSvtxdls", &ipSvtxdls, "ipSvtxdls/F");
    TreeS->Branch("ipSvtxdls2d", &ipSvtxdls2d, "ipSvtxdls2d/F");
    TreeS->Branch("ipSvtxm", &ipSvtxm, "ipSvtxm/F");
    TreeS->Branch("ipSvtxmcorr", &ipSvtxmcorr, "ipSvtxmcorr/F");
    TreeS->Branch("weight", &weight, "weight/F");

    TreeB->Branch("ip3dSig", &ip3dSig, "ip3dSig/F");
    TreeB->Branch("ipInSV", &ipInSV, "ipInSV/I");
    TreeB->Branch("ipSvtxdls", &ipSvtxdls, "ipSvtxdls/F");
    TreeB->Branch("ipSvtxdls2d", &ipSvtxdls2d, "ipSvtxdls2d/F");
    TreeB->Branch("ipSvtxm", &ipSvtxm, "ipSvtxm/F");
    TreeB->Branch("ipSvtxmcorr", &ipSvtxmcorr, "ipSvtxmcorr/F");
    TreeB->Branch("weight", &weight, "weight/F");

    Long64_t counts = 0;

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
            
        // for debugging purposes 
        //if (ient < 217316) continue;
        //if (ient > 217316) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        weight = ta.weight;

        Int_t itrackOffset = 0;
        //std::cout << "nref " << ta.nref << std::endl;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
            //std::cout << "itrackOffset " << itrackOffset << std::endl;
            //std::cout << "nselIPtrk " << ta.nselIPtrk[ijet] << std::endl;
            // bjet & eta cut 
            // ignoring the btagging lowers the efficiency
            bool isBjet = (ta.jtDiscDeepFlavourB[ijet] + ta.jtDiscDeepFlavourBB[ijet] + ta.jtDiscDeepFlavourLEPB[ijet]) > 0.9;
            //bool isBjet = true; // forget about btagging for a bit 

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
                ipInSV = ta.ipInSV[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrackOffset + itrack];
                //Int_t signal = int(sta >= 100);

                ipSvtxdls = ta.ipSvtxdls[itrackOffset + itrack];
                ipSvtxdls2d = ta.ipSvtxdls2d[itrackOffset + itrack];
                ipSvtxm = ta.ipSvtxm[itrackOffset + itrack];
                ipSvtxmcorr = ta.ipSvtxmcorr[itrackOffset + itrack];

                weight = ta.weight;

                if (ipInSV != 0 && ipInSV != 1) {
                    counts++;
                    //std::cout << "Found weird track at event " << ient << ", jet " << ijet << " and track+offset " << itrack+itrackOffset << std::endl;
                }

                if (ipInSV == 0) continue;

                if (sta >= 100) {
                    TreeS->Fill();
                } else {
                    TreeB->Fill();
                }

            } // end track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // end jet loop
    } // end entry loop

    TreeS->Write("", TObject::kOverwrite);
    TreeB->Write("", TObject::kOverwrite);

    //std::cout << counts << std::endl;

    fout->Close();
    delete fout;
                
}