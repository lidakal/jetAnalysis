#include <TTree.h>
#include <TFile.h>
#include <TString.h>

#include <limits>

#include "hiTree.h"
#include "tTree.h"

void plot_mB()
{
    // Load tree
    TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_LowPU_10000events_conmatch";
    TString fname = label + ".root";
    TString rootf = fdir + fname;

    tTree t = tTree(rootf);

    std::vector<TString> activeBranches_t = {"nref", "jtHadFlav", "jtpt", "jteta", "jtphi", "jtmB",
                                             "refmB"};
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // Create the histograms 
    Int_t x1bins = 20;
    Float_t x1min = 0.;
    Float_t x1max = 2.;

    TH1F *hmB = new TH1F("hmB", "mBreco/mBgen for aggregated pseudoB", x1bins, x1min, x1max);
    TH1F *hmB_eta = new TH1F("hmB_eta", "mBreco/mBgen for aggregated pseudoB, -2 < jteta < 2", x1bins, x1min, x1max);

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        

        t.GetEntry(ient);  
        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
            std::cout << "nref = " << t.nref << std::endl;
        }
        for (int ijet = 0; ijet < t.nref; ijet++) {
            int flav = t.jtHadFlav[ijet];
            if (flav != 5) continue;

            float mB = t.jtmB[ijet];
            float mB_gen = t.refmB[ijet];

            float ratio = mB / mB_gen;

            hmB->Fill(ratio);
            if (std::abs(t.jteta[ijet]) <= 2) hmB_eta->Fill(ratio);
        } // end jet loop
    } // end entries loop

    // Save histograms 
    TString rootf_out = "./histos/" + label + "_histos.root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    for (TH1F *h : {hmB, hmB_eta}) {
        h->Write();
    }

    fout->Close();
}

