#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>

#include <limits>

#include "tTree.h"

void plot_sv_tracks_out_of_jet()
{
    // ---- Load tree
    // TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    // TString label = "HiForestMiniAOD_HighPU_10000events_conmatch";
    // TString suffix = "_truth_withExtraTracks";
    TString fdir = "/data_CMS/cms/kalipoliti/qcdMC/bjet/aggrTMVA_newJP/";
    TString label = "merged_HiForestMiniAOD";
    TString suffix = "";
    TString fname = label + suffix + ".root";
    TString rootf = fdir + fname;

    tTree t = tTree(rootf);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "jtHadFlav", "jtpt", "jteta", "jtphi", 
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "nsvtx", "svtxJetId", "svtxNtrk", "svtxm",
                                             "ntrkInSvtxNotInJet", "trkInSvtxNotInJetSvId", "trkInSvtxNotInJetOtherJetId",
                                             "trkInSvtxNotInJetMatchSta", 
                                             "trkInSvtxNotInJetPt", "trkInSvtxNotInJetEta", "trkInSvtxNotInJetPhi"
                                            //  "jtmB", "refmB"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Create the histograms 

    // pt
    Int_t x1bins = 40;
    Float_t x1min = 0;
    Float_t x1max = 40;

    // y=0.1:not matched; y=1.:matched to bkg; y=1.9:matched to sig
    Int_t y1bins = 3;
    Float_t y1min = 0.; 
    Float_t y1max = 2.;

    TH2F *h_pt = new TH2F("h_pt", "x=trk pt, y=trk status", x1bins, x1min, x1max, y1bins, y1min, y1max);

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        t.GetEntry(ient);  

        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }
        Int_t nTrkOutOfSelectedJets_sig = 0;
        Int_t nTrkOutOfSelectedJets_bkg = 0;
        Int_t nTrkOutOfSelectedJets_nomatch = 0;

        Int_t ntrkInSvtxNotInJet = t.ntrkInSvtxNotInJet;
        for (Int_t itrk = 0; itrk < ntrkInSvtxNotInJet; itrk++) {
            // keep only tracks in selected jets
            bool skipTrk = false;

            Int_t svId = t.trkInSvtxNotInJetSvId[itrk];
            Int_t jetId = t.svtxJetId[svId];

            Float_t jteta = t.jteta[jetId];
            skipTrk |= (std::abs(jteta) > 2);

            Float_t jtpt = t.jtpt[jetId];
            skipTrk |= (jtpt < 30.);

            Int_t jtHadFlav = t.jtHadFlav[jetId];
            skipTrk |= (jtHadFlav != 5);

            Float_t discr_deepFlavour_b = t.discr_deepFlavour_b[jetId];
            Float_t discr_deepFlavour_bb = t.discr_deepFlavour_bb[jetId];
            Float_t discr_deepFlavour_lepb = t.discr_deepFlavour_lepb[jetId];
            bool passWP = ((discr_deepFlavour_b + discr_deepFlavour_bb + discr_deepFlavour_lepb) > 0.9);
            skipTrk |= (!passWP);

            if (skipTrk) continue;
            Float_t trkInSvtxNotInJetPt = t.trkInSvtxNotInJetPt[itrk];
            Float_t yclass = -1.;
            Int_t trkInSvtxNotInJetMatchSta = t.trkInSvtxNotInJetMatchSta[itrk];
            if (trkInSvtxNotInJetMatchSta >= 100) {
                yclass = 1.9;
                nTrkOutOfSelectedJets_sig++;
            } else if (trkInSvtxNotInJetMatchSta > 0) {
                yclass = 1.;
                nTrkOutOfSelectedJets_bkg++;
            } else {
                yclass = 0.1;
                nTrkOutOfSelectedJets_nomatch++;
            }
            
            h_pt->Fill(trkInSvtxNotInJetPt, yclass);
        } // end tracks loop
    } // end entries loop

    // Save histograms 
    TString rootf_out = "./histos/sv_tracks_out_of_jet.root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    for (TH2F *h : {h_pt}) {
        h->Write();
    }

    fout->Close();
}

