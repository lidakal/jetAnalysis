#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>

#include <limits>

#include "tTree.h"

void plot_nvtx()
{
    // ---- Load tree
    TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_10000events_conmatch";
    TString suffix = "_truth";
    TString fname = label + suffix + ".root";
    TString rootf = fdir + fname;

    tTree t = tTree(rootf);

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                            //  "jtHadFlav", "jtpt", "jteta", "jtphi", 
                                            //  "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                            //  "jtmB", "refmB"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    // ---- Create the histograms 

    // nvtx
    Int_t y1bins = 20;
    Float_t y1min = 0;
    Float_t y1max = 100;

    TH1D *hNvtx = new TH1D("hNvtx", "x=nvtx", y1bins, y1min, y1max);

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        t.GetEntry(ient);  

        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
            std::cout << "nref = " << t.nref << std::endl;
        }

        float nvtx = (float) t.nvtx;
        hNvtx->Fill(nvtx);        
    } // end entries loop

    // Save histograms 
    TString rootf_out = "./histos/nvtx" + suffix + ".root";
    TFile *fout = new TFile(rootf_out, "recreate");
    std::cout << "(Re)creating " << rootf_out << " file." << std::endl;

    for (TH1D *h : {hNvtx}) {
        h->Write();
    }

    fout->Close();
}

