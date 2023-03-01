#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH2F.h"

#include "../tTree.h"

#include <iostream>
#include <cmath>

void plot_newVars()
{
    // Setup 
    float ptMin = 50.;
    float ptMax = 80.;

    // Float_t ptMin = 30.;
    // Float_t ptMax = 700.;

    const Float_t missing_value = -1000000.;

    TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_10000events_conmatch_newVars_truth";
    TString fin = indir + label + ".root";

    TString odir = "./histos/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + "_newVars.root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nvtx",
                                           "nref", "jteta", "jtphi", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "ntrk", "trkJetId", "trkSvtxId", "trkPt", "trkEta", "trkPhi", "trkMatchSta", 
                                           "trkIp3d", "trkIp3dSig", "trkIp2d", "trkIp2dSig", "trkDistToAxis", "trkDistToAxisSig", 
                                           "nsvtx", "svtxJetId", "svtxNtrk", "svtxm", "svtxmcorr", "svtxpt",
                                           "svtxdl", "svtxdls", "svtxdl2d", "svtxdls2d",
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    // svtxm, svtxmcorr
    Int_t x1bins = 25*2;
    Float_t x1min = 0.;
    Float_t x1max = 7.;

    // svtxdls, svtxdls2d
    Int_t x2bins = 25*2;
    Float_t x2min = 0.;
    Float_t x2max = 100.;

    // trkIp2dSig
    Int_t x3bins = 25*2;
    Float_t x3min = -80.;
    Float_t x3max = 80.;

    // nvtx
    Int_t x4bins = 25*2;
    Float_t x4min = 0.;
    Float_t x4max = 60.;

    // trkPt / svtxpt
    Int_t x5bins = 25*5;
    Float_t x5min = 0.;
    Float_t x5max = 1.;

    // svtxNtrk
    Int_t x6bins = 25*5;
    Float_t x6min = 0.;
    Float_t x6max = 10.;

    // y=0.1:not matched; y=1.:matched to bkg; y=1.9:matched to sig
    Int_t y1bins = 3;
    Float_t y1min = 0.; 
    Float_t y1max = 2.;

    
    TH2F *h_svtxm = new TH2F("h_svtxm", "x=svtxm, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *h_svtxm_bjet = new TH2F("h_svtxm_bjet", "x=svtxm, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);

    TH2F *h_svtxmcorr = new TH2F("h_svtxmcorr", "x=svtxmcorr, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *h_svtxmcorr_bjet = new TH2F("h_svtxmcorr_bjet", "x=svtxmcorr, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);

    TH2F *h_svtxdls = new TH2F("h_svtxdls", "x=svtxdls, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *h_svtxdls_bjet = new TH2F("h_svtxdls_bjet", "x=svtxdls, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);

    TH2F *h_svtxdls2d = new TH2F("h_svtxdls2d", "x=svtxdls2d, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *h_svtxdls2d_bjet = new TH2F("h_svtxdls2d_bjet", "x=svtxdls2d, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);

    TH2F *h_trkIp2dSig = new TH2F("h_trkIp2dSig", "x=trkIp2dSig, y=class", x3bins, -40., x3max, y1bins, y1min, y1max);
    TH2F *h_trkIp2dSig_bjet = new TH2F("h_trkIp2dSig_bjet", "x=trkIp2dSig, y=class", x3bins, -40., x3max, y1bins, y1min, y1max);

    TH2F *h_nvtx = new TH2F("h_nvtx", "x=nvtx, y=class", x4bins, x4min, x4max, y1bins, y1min, y1max);
    TH2F *h_nvtx_bjet = new TH2F("h_nvtx_bjet", "x=nvtx, y=class", x4bins, x4min, x4max, y1bins, y1min, y1max);

    TH2F *h_ptFrac = new TH2F("h_ptFrac", "x=ptFrac, y=class", x5bins, x5min, x5max, y1bins, y1min, y1max);
    TH2F *h_ptFrac_bjet = new TH2F("h_ptFrac_bjet", "x=ptFrac, y=class", x5bins, x5min, x5max, y1bins, y1min, y1max);

    TH2F *h_svtxNtrk = new TH2F("h_svtxNtrk", "x=svtxNtrk, y=class", x6bins, x6min, x6max, y1bins, y1min, y1max);
    TH2F *h_svtxNtrk_bjet = new TH2F("h_svtxNtrk_bjet", "x=svtxNtrk, y=class", x6bins, x6min, x6max, y1bins, y1min, y1max);


    for (Long64_t ient = 0; ient < t.GetEntries(); ient++) {
        // for debugging purposes 
        //if (ient < 217316) continue;
        // if (ient > 0) break;
            
        // Show progress
        if (ient % 1000 == 0) {
            std::cout << "i = " << ient << std::endl;
        }

        t.GetEntry(ient);
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
            // skipJet |= (!isBjet);
            bool passBtag = (deepFlavour_discr > 0.9);
            // skipJet |= (!passBtag);

            if (skipJet) continue;

            // Calculate quantities
            Int_t nvtx = t.nvtx;

            Float_t trkPt = t.trkPt[itrk];
            Float_t trkIp2dSig = t.trkIp2dSig[itrk];
            if (trkIp2dSig != trkIp2dSig) trkIp2dSig = missing_value; // nan

            Int_t isvtx = t.trkSvtxId[itrk];
            Float_t svtxpt = missing_value;
            Float_t svtxm = missing_value;
            Float_t svtxmcorr = missing_value;
            Float_t svtxdls = missing_value;
            Float_t svtxdls2d = missing_value;
            Float_t svtxNtrk = missing_value;
            Float_t ptFrac = missing_value;
            if (isvtx >= 0) {
                svtxpt = t.svtxpt[isvtx];
                svtxm = t.svtxm[isvtx];
                svtxmcorr = t.svtxmcorr[isvtx];
                svtxdls = t.svtxdls[isvtx];
                svtxdls2d = t.svtxdls2d[isvtx];
                svtxNtrk = t.svtxNtrk[isvtx];
                ptFrac = trkPt / svtxpt;
            }

            // Get class
            Int_t sta = t.trkMatchSta[itrk];
            Float_t yclass = -1.;
            if (sta == 1) { 
                // match to bkg
                yclass = 1.;
            } else if (sta >= 100) { 
                // match to sig
                yclass = 1.9;
            } else { 
                // no match
                yclass = 0.1;
            }

            // Fill histos
            h_svtxm->Fill(svtxm, yclass);
            h_svtxmcorr->Fill(svtxmcorr, yclass);
            h_svtxdls->Fill(svtxdls, yclass);
            h_svtxdls2d->Fill(svtxdls2d, yclass);
            h_svtxNtrk->Fill(svtxNtrk, yclass);
            h_trkIp2dSig->Fill(trkIp2dSig, yclass);
            h_nvtx->Fill(nvtx, yclass);
            h_ptFrac->Fill(ptFrac, yclass);
            if (isBjet && passBtag) {
                h_svtxm_bjet->Fill(svtxm, yclass);
                h_svtxmcorr_bjet->Fill(svtxmcorr, yclass);
                h_svtxdls_bjet->Fill(svtxdls, yclass);
                h_svtxdls2d_bjet->Fill(svtxdls2d, yclass);
                h_svtxNtrk_bjet->Fill(svtxNtrk, yclass);
                h_trkIp2dSig_bjet->Fill(trkIp2dSig, yclass);
                h_nvtx_bjet->Fill(nvtx, yclass);
                h_ptFrac_bjet->Fill(ptFrac, yclass);
            }
        } // end track loop
    } // end entry loop

    for (auto h : {h_svtxm, h_svtxm_bjet,
                   h_svtxmcorr, h_svtxmcorr_bjet,
                   h_svtxdls, h_svtxdls_bjet,
                   h_svtxdls2d, h_svtxdls2d_bjet,
                   h_trkIp2dSig, h_trkIp2dSig_bjet,
                   h_nvtx, h_nvtx_bjet,
                   h_ptFrac, h_ptFrac_bjet,
                   h_svtxNtrk, h_svtxNtrk_bjet
                   }) {
        h->Write();
    }

    foutPtr->Close();
    delete foutPtr;         
}