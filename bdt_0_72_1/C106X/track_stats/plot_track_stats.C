#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH2F.h"

#include "../tTree.h"

#include <iostream>

void plot_track_stats()
{
    // Setup 
    float ptMin = 30.;
    float ptMax = 700.;

    const float missing_value = -1000000.;

    TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_100000events_conmatch";
    TString fin = indir + label + ".root";

    TString odir = "./histos/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + "track_stats.root";    

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

    int x1bins = 25*2;
    float x1min = -100.;
    float x1max = 150.;

    int y1bins = 3;
    float y1min = 0.; 
    float y1max = 2.;

    // y=0.1 : not matched; y=1. : matched to bkg; y=1.9 : matched to sig 
    TH2F *hIp3dSig = new TH2F("hIp3dSig", "x=ip3dSig, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *hIp3dSig_bjet = new TH2F("hIp3dSig_bjet", "x=ip3dSig, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);

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
            // skipJet |= (!isBjet);
            bool passBtag = (t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet]) > 0.9;
            // skipJet |= (!passBtag);

            if (skipJet) continue;

            float trkIp3dSig = t.trkIp3dSig[itrk];
            // int trkSvtxId = t.trkSvtxId[itrk];
            // trkInSV = int(trkSvtxId >= 0);
            Int_t sta = t.trkMatchSta[itrk];
            // Int_t sta = 100;

            if (sta == 1) {
                hIp3dSig->Fill(trkIp3dSig, 1.);
                if (isBjet && passBtag) {
                    hIp3dSig_bjet->Fill(trkIp3dSig, 1.);
                }
            } else if (sta >= 100) {
                hIp3dSig->Fill(trkIp3dSig, 1.9);
                if (isBjet && passBtag) {
                    hIp3dSig_bjet->Fill(trkIp3dSig, 1.9);
                }
            } else {
                hIp3dSig->Fill(trkIp3dSig, 0.1);
                if (isBjet && passBtag) {
                    hIp3dSig_bjet->Fill(trkIp3dSig, 0.1);
                }
            }
        } // end track loop
    } // end entry loop

    for (auto h : {hIp3dSig, hIp3dSig_bjet}) {
        h->Write();
    }

    foutPtr->Close();
    delete foutPtr;         
}