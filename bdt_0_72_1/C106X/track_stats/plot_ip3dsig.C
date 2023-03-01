#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH2F.h"

#include "../tTree.h"

#include <iostream>
#include <cmath>

void plot_ip3dsig()
{
    // Setup 
    // float ptMin = 50.;
    // float ptMax = 80.;

    Float_t ptMin = 30.;
    Float_t ptMax = 700.;

    const Float_t missing_value = -1000000.;

    TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_newVars_truth";
    // TString label = "HiForestMiniAOD_LowPU_10000events_conmatch_newVars_truth";
    TString fin = indir + label + ".root";

    TString odir = "./histos/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + "_ip3dsig.root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nvtx",
                                           "nref", "jteta", "jtphi", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "ntrk", "trkJetId", "trkSvtxId", "trkPt", "trkEta", "trkPhi", "trkMatchSta", 
                                           "trkIp3d", "trkIp3dSig", "trkIp2d", "trkIp2dSig", "trkDistToAxis", "trkDistToAxisSig", "trkDz",
                                           "nsvtx", "svtxJetId", "svtxNtrk", "svtxm", "svtxmcorr", "svtxpt",
                                           "svtxdl", "svtxdls", "svtxdl2d", "svtxdls2d",
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    // ip3dsig
    Int_t x1bins = 25*4;
    Float_t x1min = -100.;
    Float_t x1max = 100.;

    // ip3d
    Int_t x2bins = 25*4;
    Float_t x2min = -1.;
    Float_t x2max = 1.;

    // y=0.1:not matched; y=1.:matched to bkg; y=1.9:matched to sig
    Int_t y1bins = 3;
    Float_t y1min = 0.; 
    Float_t y1max = 2.;
    
    TH2F *h_ip3dsig = new TH2F("h_ip3dsig", "x=ip3dsig, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *h_ip3dsig_bjet = new TH2F("h_ip3dsig_bjet", "x=ip3dsig, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);

    TH2F *h_ip3d = new TH2F("h_ip3d", "x=ip3d, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *h_ip3d_bjet = new TH2F("h_ip3d_bjet", "x=ip3d, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);

    int countSigIn = 0;
    int countSigOut = 0;
    int countBkgIn = 0;
    int countBkgOut = 0;
    int countPUIn = 0;
    int countPUOut = 0;

    float maxDz = 0.2;

    for (Long64_t ient = 0; ient < t.GetEntries(); ient++) {
        // for debugging purposes 
        //if (ient < 217316) continue;
        // if (ient > 0) break;
            
        // Show progress
        if (ient % 1000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        t.GetEntry(ient);
        Int_t nvtx = t.nvtx;
        // if (nvtx > 5) continue;
        for (Int_t itrk = 0; itrk < t.ntrk; itrk++) {
            Int_t ijet = t.trkJetId[itrk];
            Float_t jteta = t.jteta[ijet];
            Float_t jtphi = t.jtphi[ijet];
            Float_t jtpt = t.jtpt[ijet];
            Int_t jtHadFlav = t.jtHadFlav[ijet];
            Float_t deepFlavour_discr = t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet];

            bool skipJet = false;
            skipJet |= (std::abs(jteta) > 2);
            skipJet |= (jtpt < ptMin || jtpt > ptMax);
            bool isBjet = (jtHadFlav == 5);
            skipJet |= (!isBjet);
            bool passBtag = (deepFlavour_discr > 0.9);
            skipJet |= (!passBtag);

            if (skipJet) continue;

            // Calculate quantities
            Float_t trkDz = t.trkDz[itrk];
            Float_t trkIp3dSig = t.trkIp3dSig[itrk];
            Float_t trkIp3d = t.trkIp3d[itrk];
            if (trkIp3dSig != trkIp3dSig) trkIp3dSig = -1000.;

            // Get class
            Int_t sta = t.trkMatchSta[itrk];
            Float_t yclass = -1.;
            if (sta == 1) { 
                // match to bkg
                yclass = 1.;
                if (std::abs(trkDz) > maxDz) countBkgOut++;
                else countBkgIn++;
            } else if (sta >= 100) { 
                // match to sig
                yclass = 1.9;
                if (std::abs(trkDz) > maxDz) countSigOut++;
                else countSigIn++;
            } else { 
                // no match
                yclass = 0.1;
                if (std::abs(trkDz) > maxDz) countPUOut++;
                else countPUIn++;
            }

            // Fill histos
            // skip large dz tracks
            if (std::abs(trkDz) > maxDz) continue;
            h_ip3dsig->Fill(trkIp3dSig, yclass);
            h_ip3d->Fill(trkIp3d, yclass);
            if (isBjet && passBtag) {
                h_ip3dsig_bjet->Fill(trkIp3dSig, yclass);
                h_ip3d_bjet->Fill(trkIp3d, yclass);
            }
        } // end track loop
    } // end entry loop

    // std::cout << "sig in " << countSigIn << ", sig out " << countSigOut << std::endl;
    // std::cout << "bkg in " << countBkgIn << ", bkg out " << countBkgOut << std::endl;
    // std::cout << "pu in " << countPUIn << ", pu out " << countPUOut << std::endl;

    int sig = (countSigIn + countSigOut);
    int bkg = (countBkgIn + countBkgOut);
    int pu = (countPUIn + countPUOut);

    int total = sig + bkg + pu;

    std::cout << "sig / total : " << (float) sig / total << std::endl;
    std::cout << "bkg / total : " << (float) bkg / total << std::endl;
    std::cout << "pu / total : " << (float) pu / total << std::endl;

    std::cout << "bkg / sig : " << (float) bkg / sig << std::endl; 

    std::cout << "sig out / sig : " << (float) countSigOut / sig << std::endl;
    std::cout << "bkg out / bkg : " << (float) countBkgOut / bkg << std::endl;
    std::cout << "pu out / pu : " << (float) countPUOut / pu << std::endl;

    for (auto h : {h_ip3dsig, h_ip3dsig_bjet,
                   h_ip3d, h_ip3d_bjet
                   }) {
        h->Write();
    }

    foutPtr->Close();
    delete foutPtr;         
}