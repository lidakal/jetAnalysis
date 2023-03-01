#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH2F.h"

#include "../tTree.h"

#include <iostream>
#include <cmath>

void plot_zdr()
{
    // Setup 
    // float ptMin = 50.;
    // float ptMax = 80.;

    Float_t ptMin = 30.;
    Float_t ptMax = 700.;

    const Float_t missing_value = -1000000.;

    TString indir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_100000events_conmatch";
    TString fin = indir + label + ".root";

    TString odir = "./histos/";
    TString fout = odir + label + "_" + ptMin + "_pt_" + ptMax + "_zdr.root";    

    // Load tree
    tTree t = tTree(fin);

    // Activate branches of interest
    std::vector<TString> activeBranches = {"nref", "jteta", "jtphi", "jtpt", "jtHadFlav",
                                           "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb", 
                                           "ntrk", "trkJetId", "trkSvtxId", "trkPt", "trkEta", "trkPhi",
                                           "trkIp3d", "trkDistToAxis", "trkDistToAxis", "trkDistToAxisSig", "trkMatchSta", 
                                           "nsvtx", "svtxJetId", "svtxNtrk", "svtxm", "svtxpt",
                                           "svtxdl", "svtxdls", "svtxdl2d", "svtxdls2d",
                                           };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches, 1);

    // Create new file to store signal and background trees
    TFile *foutPtr = new TFile(fout, "recreate");
    std::cout << "(Re)creating " << fout << " file." << std::endl;

    // |trkDistToAxis|
    Int_t x1bins = 25*2;
    Float_t x1min = 0.;
    Float_t x1max = 0.2;

    // deltaR/R
    Int_t x2bins = 25*2;
    Float_t x2min = 0.;
    Float_t x2max = 1.;

    // relpt=trkpt/jtpt
    Int_t x3bins = 25*2;
    Float_t x3min = 0.;
    Float_t x3max = 1.;

    // z*dr/R = relpt * dr/R
    Int_t x4bins = 25*2;
    Float_t x4min = 0.;
    Float_t x4max = 0.4;

    // y=0.1:not matched; y=1.:matched to bkg; y=1.9:matched to sig
    Int_t y1bins = 3;
    Float_t y1min = 0.; 
    Float_t y1max = 2.;

    
    TH2F *hTrkDistToAxis = new TH2F("hTrkDistToAxis", "x=trkDistToAxis, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);
    TH2F *hTrkDistToAxis_bjet = new TH2F("hTrkDistToAxis_bjet", "x=trkDistToAxis, y=class", x1bins, x1min, x1max, y1bins, y1min, y1max);

    TH2F *hDeltaR = new TH2F("hDeltaR", "x=deltaR/R, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);
    TH2F *hDeltaR_bjet = new TH2F("hDeltaR_bjet", "x=deltaR/R, y=class", x2bins, x2min, x2max, y1bins, y1min, y1max);

    TH2F *hRelpt = new TH2F("hRelpt", "x=relpt, y=class", x3bins, x3min, x3max, y1bins, y1min, y1max);
    TH2F *hRelpt_bjet = new TH2F("hRelpt_bjet", "x=relpt, y=class", x3bins, x3min, x3max, y1bins, y1min, y1max);

    TH2F *hzdr = new TH2F("hzdr", "x=zdr, y=class", x4bins, x4min, x4max, y1bins, y1min, y1max);
    TH2F *hzdr_bjet = new TH2F("hzdr_bjet", "x=zdr, y=class", x4bins, x4min, x4max, y1bins, y1min, y1max);

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
            Float_t trkDistToAxis = std::abs(t.trkDistToAxis[itrk]);
            Float_t trkEta = t.trkEta[itrk];
            Float_t trkPhi = t.trkPhi[itrk];
            Float_t trkPt = t.trkPt[itrk];
            // int trkSvtxId = t.trkSvtxId[itrk];
            // trkInSV = int(trkSvtxId >= 0);
            Int_t sta = t.trkMatchSta[itrk];
            // Int_t sta = 100;

            Float_t relpt = -1.;
            if (jtpt > 0.) relpt = trkPt / jtpt;
            // std::cout << "relpt " << relpt << std::endl;

            ROOT::Math::PtEtaPhiMVector trkVector;
            trkVector.SetPt(trkPt);
            trkVector.SetEta(trkEta);
            trkVector.SetPhi(trkPhi);

            ROOT::Math::PtEtaPhiMVector jetVector;
            jetVector.SetPt(jtpt);
            jetVector.SetEta(jteta);
            jetVector.SetPhi(jtphi);

            Float_t deltaR = ROOT::Math::VectorUtil::DeltaR(trkVector, jetVector);
            Float_t Rjet = 0.4;

            Float_t zdr = -1.;
            if (relpt > 0.) zdr = std::pow(relpt, 1) * std::pow(deltaR / Rjet, 1);
            // if (relpt > 0.) zdr = std::pow(trkPt / deltaR, 2);

            // Get class
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
            hTrkDistToAxis->Fill(trkDistToAxis, yclass);
            hDeltaR->Fill(deltaR/Rjet, yclass);
            hRelpt->Fill(relpt, yclass);
            hzdr->Fill(zdr, yclass);
            if (isBjet && passBtag) {
                hTrkDistToAxis_bjet->Fill(trkDistToAxis, yclass);
                hDeltaR_bjet->Fill(deltaR/Rjet, yclass);
                hRelpt_bjet->Fill(relpt, yclass);
                hzdr_bjet->Fill(zdr, yclass);
            }
        } // end track loop
    } // end entry loop

    for (auto h : {hTrkDistToAxis, hTrkDistToAxis_bjet,
                   hDeltaR, hDeltaR_bjet,
                   hRelpt, hRelpt_bjet,
                   hzdr, hzdr_bjet
                   }) {
        h->Write();
    }

    foutPtr->Close();
    delete foutPtr;         
}