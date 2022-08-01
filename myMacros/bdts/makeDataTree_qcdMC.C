#include "TFile.h"
#include "TTree.h"

#include "TreeAnalyzer.h"

void makeDataTree_qcdMC()
{
    std::string finname = "/data_CMS/cms/kalipoliti/qcdMC/trackTaggingTraining/merged_HiForestAOD.root";
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
    std::string foutName = "tmva_data_complex_qcdMC.root";
    TFile *fout = new TFile(foutName.c_str(), "recreate");
    std::cout << "(Re)creating " << foutName << " file." << std::endl;

    TTree *bkgLight = new TTree("bkgLight", "Background tree from inclusive jets for TMVA");

    Float_t ip3dSig;
    Int_t ipInSV;
    Float_t ipSvtxdls;
    Float_t ipSvtxdls2d;
    Float_t ipSvtxm;
    Float_t ipSvtxmcorr;
    Float_t weight;

    bkgLight->Branch("ip3dSig", &ip3dSig, "ip3dSig/F");
    bkgLight->Branch("ipInSV", &ipInSV, "ipInSV/I");
    bkgLight->Branch("ipSvtxdls", &ipSvtxdls, "ipSvtxdls/F");
    bkgLight->Branch("ipSvtxdls2d", &ipSvtxdls2d, "ipSvtxdls2d/F");
    bkgLight->Branch("ipSvtxm", &ipSvtxm, "ipSvtxm/F");
    bkgLight->Branch("ipSvtxmcorr", &ipSvtxmcorr, "ipSvtxmcorr/F");
    bkgLight->Branch("weight", &weight, "weight/F");

    Long64_t counts = 0;

    for (Long64_t ient = 0; ient < ta.nentries; ient++) {
            
        // for debugging purposes 
        //if (ient < 2) continue;
        //if (ient > 100) break;
            
        // Show progress
        if (ient % 1000000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        ta.GetEntry(ient);
        weight = ta.weight;

        Int_t itrackOffset = 0;

        for (Int_t ijet = 0; ijet < ta.nref; ijet++) {
            //std::cout << "New jet" << std::endl;
            // flavour & eta cut 
            if ((ta.jtHadFlav[ijet] > 0) || (std::abs(ta.jteta[ijet]) > 2)) {
                itrackOffset += ta.nselIPtrk[ijet];
                continue;
            }

            //std::cout << "Is a light jet with " << ta.nselIPtrk[ijet] << " tracks" << std::endl;

            // Go over jet tracks
            for (Int_t itrack = 0; itrack < ta.nselIPtrk[ijet]; itrack++) {
                // Get track properties
                
                ip3dSig = ta.ip3dSig[itrackOffset + itrack];
                ipInSV = ta.ipInSV[itrackOffset + itrack];
                Int_t sta = ta.ipMatchStatus[itrackOffset + itrack];

                ipSvtxdls = ta.ipSvtxdls[itrackOffset + itrack];
                ipSvtxdls2d = ta.ipSvtxdls2d[itrackOffset + itrack];
                ipSvtxm = ta.ipSvtxm[itrackOffset + itrack];
                ipSvtxmcorr = ta.ipSvtxmcorr[itrackOffset + itrack];

                weight = ta.weight;

                if (ipInSV == 0) continue;
                //std::cout << "Track in SV with status " << sta << std::endl;
                counts++;

                // safety if
                if (sta == 1) {
                    bkgLight->Fill();
                }

            } // end track loop
            itrackOffset += ta.nselIPtrk[ijet];
        } // end jet loop
    } // end entry loop

    //std::cout << counts << " tracks found in SV" << std::endl;
    bkgLight->Write("", TObject::kOverwrite);

    fout->Close();
    delete fout;
                
}