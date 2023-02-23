#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH3.h>

#include <limits>

#include "tTree.h"

void plot_mB()
{
    // ---- Load tree
    TString label = "aggrTMVA";
    TString indir = "/data_CMS/cms/kalipoliti/ttbarMC/highPU/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    // TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    // TString label = "HiForestMiniAOD_HighPU_10000events_conmatch";
    // TString suffix = "_bdt44_sig_vs_bkg";
    // TString fname = label + suffix + ".root";
    // TString rootf = fdir + fname;

    tTree t = tTree(fin);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "jtHadFlav", "jtpt", "jteta", "jtphi", 
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "jtmB", "refmB"};
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Create the histograms 

    // mB ratio
    Int_t x1bins = 20;
    Float_t x1min = 0.;
    Float_t x1max = 2.;

    // mB
    Int_t x2bins = 7*4;
    Float_t x2min = 0.;
    Float_t x2max = 7.;

    // nvtx
    Int_t y1bins = 20;
    Float_t y1min = 0.;
    Float_t y1max = 100.;

    // jet pt
    Int_t z1bins = 27 * 2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    TH3D *hB_mB_ratio = new TH3D("hB_mB_ratio", "x=mBreco/mBgen, y=nvtx, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_mB_ratio = new TH3D("hBtag_mB_ratio", "x=mBreco/mBgen, y=nvtx, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH2D *hB_mB = new TH2D("hB_mB", "x=mBreco, y=jtpt", x2bins, x2min, x2max, y1bins, z1min, z1max);
    TH2D *hBtag_mB = new TH2D("hBtag_mB", "x=mBreco, y=jtpt", x2bins, x2min, x2max, z1bins, z1min, z1max);

    TH2D *hB_mB_gen = new TH2D("hB_mB_gen", "x=mB_gen, y=jtpt", x2bins, x2min, x2max, y1bins, z1min, z1max);
    TH2D *hBtag_mB_gen = new TH2D("hBtag_mB_gen", "x=mB_gen, y=jtpt", x2bins, x2min, x2max, z1bins, z1min, z1max);

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        t.GetEntry(ient);  

        // Print progress
        if (ient % 100000 == 0) {
            std::cout << "ient = " << ient << std::endl;
            // std::cout << "nref = " << t.nref << std::endl;
        }

        for (int ijet = 0; ijet < t.nref; ijet++) {
            bool skipJet = false; 

            bool inEta = (std::abs(t.jteta[ijet]) <= 2);
            skipJet &= inEta;

            int flav = t.jtHadFlav[ijet];
            bool isBJet = (flav == 5);
            skipJet &= isBJet;

            if (skipJet) continue;

            float bTagWP = 0.9;
            bool passWP = ((t.discr_deepFlavour_b[ijet] + t.discr_deepFlavour_bb[ijet] + t.discr_deepFlavour_lepb[ijet]) > bTagWP);

            float mB = t.jtmB[ijet];
            float mB_gen = t.refmB[ijet];

            float ratio = -1.;
            if (mB_gen > 0) {
                ratio = mB / mB_gen;
            } else {
                if (mB > 0) {
                    ratio = -1.;
                } else {
                    ratio = 1.;
                }
            }

            Int_t nvtx = t.nvtx;
            Float_t jtpt = t.jtpt[ijet];            

            hB_mB_ratio->Fill(ratio, nvtx, jtpt);
            hB_mB->Fill(mB, jtpt);
            hB_mB_gen->Fill(mB_gen, jtpt);
            if (passWP) {
                hBtag_mB_ratio->Fill(ratio, nvtx, jtpt);
                hBtag_mB->Fill(mB, jtpt);
                hBtag_mB_gen->Fill(mB_gen, jtpt);
            }
        } // end jet loop
    } // end entries loop

    // Save histograms 
    TString rootf_out = "./histos/" + label + "_mB.root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    for (TH3D *h : {hB_mB_ratio, hBtag_mB_ratio}) {
        h->Write();
    }

    for (TH2D *h : {hB_mB, hBtag_mB,
                    hB_mB_gen, hBtag_mB_gen}) {
        h->Write();
    }

    fout->Close();
}

