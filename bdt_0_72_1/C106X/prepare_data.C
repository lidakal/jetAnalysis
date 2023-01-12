#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "tTree.h"

#include <iostream>

void prepare_data()
{
    // Setup 
    float ptMin = 50.;
    float ptMax = 80.;

    const float missing_value = -1000000.;

    TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_100000events_conmatch";
    TString fin = indir + label + ".root";

    TString odir = "./ntuples/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + ".root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nref", "jteta", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "ntrk", "trkJetId", "trkSvtxId", "trkPt", "trkEta", "trkPhi",
                                           "trkIp3d", "trkIp3dSig", "trkDistToAxis", "trkDistToAxisSig", "trkMatchSta", 
                                           "nsvtx", "svtxJetId", "svtxNtrk", "svtxm", "svtxpt",
                                           "svtxdl", "svtxdls", "svtxdl2d", "svtxdls2d",
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    TTree *TreeS = new TTree("TreeS", "Signal tree for MVA");
    TTree *TreeB = new TTree("TreeB", "Background tree for MVA");

    Float_t trkIp3dSig;
    Int_t trkInSV;
    Float_t svtxdls;
    Float_t svtxdls2d;
    Float_t svtxm;

    TreeS->Branch("trkIp3dSig", &trkIp3dSig, "trkIp3dSig/F");
    TreeS->Branch("trkInSV", &trkInSV, "trkInSV/I");
    TreeS->Branch("svtxdls", &svtxdls, "svtxdls/F");
    TreeS->Branch("svtxdls2d", &svtxdls2d, "svtxdls2d/F");
    TreeS->Branch("svtxm", &svtxm, "svtxm/F");

    TreeB->Branch("trkIp3dSig", &trkIp3dSig, "trkIp3dSig/F");
    TreeB->Branch("trkInSV", &trkInSV, "trkInSV/I");
    TreeB->Branch("svtxdls", &svtxdls, "svtxdls/F");
    TreeB->Branch("svtxdls2d", &svtxdls2d, "svtxdls2d/F");
    TreeB->Branch("svtxm", &svtxm, "svtxm/F");

    for (Long64_t ient = 0; ient < t.GetEntries(); ient++) {
        // for debugging purposes 
        //if (ient < 217316) continue;
        //if (ient > 217316) break;
            
        // Show progress
        if (ient % 1000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        t.GetEntry(ient);
        for (Int_t itrk = 0; itrk < t.ntrk; itrk++) {
            int ijet = t.trkJetId[itrk];
            bool skipJet = false;
            skipJet |= (std::abs(t.jteta[ijet]) > 2);
            skipJet |= (t.jtpt[ijet] < ptMin || t.jtpt[ijet] > ptMax);
            bool isBjet = (t.jtHadFlav[ijet] == 5);
            skipJet |= (!isBjet);
            bool passBtag = (t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet]) > 0.9;
            skipJet |= (!passBtag);

            if (skipJet) continue;

            trkIp3dSig = t.trkIp3dSig[itrk];
            if (trkIp3dSig != trkIp3dSig) {
                // if is nan
                trkIp3dSig = missing_value;
            }

            int trkSvtxId = t.trkSvtxId[itrk];
            trkInSV = int(trkSvtxId >= 0);
            if (trkSvtxId >= 0) {
                svtxdls = t.svtxdls[trkSvtxId];
                svtxdls2d = t.svtxdls2d[trkSvtxId];
                svtxm = t.svtxm[trkSvtxId];
            } else {
                svtxdls = missing_value;
                svtxdls2d = missing_value;
                svtxm = missing_value;
            }

            Int_t sta = t.trkMatchSta[itrk];
            if (sta >= 100) {
                TreeS->Fill();
            } else if (sta == 1) {
                TreeB->Fill();
            }

        } // end track loop
    } // end entry loop

    TreeS->Write("", TObject::kOverwrite);
    TreeB->Write("", TObject::kOverwrite);

    foutPtr->Close();
    delete foutPtr;         
}