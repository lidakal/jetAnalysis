#include "tTree.h"
#include "hist_utils.h"

void plot_trigger_eff()
{
    // ---- Load tree
    TString sample = "HighEGJet";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/"+sample+"_"+label+"_tree.root";
    TString fout_name = "./histos/" + sample + "_" + label + "_trg_eff.root"; 

    TFile *fin = new TFile(fin_name);
    TTree *tree_all = (TTree *) fin->Get("tree_all");

    Double_t jtpt;
    tree_all->SetBranchAddress("jtpt", &jtpt);

    Double_t weight;
    tree_all->SetBranchAddress("weight", &weight);


    // Create histograms

    // jtpt
    Int_t z1bins = 59;
    Float_t z1min = 10.;
    Float_t z1max = 600.;

    // if LowEGJet -> two histograms: 40 and 60 
    // if HighEGJet -> one histogram: 80

    TH1D *h40 = new TH1D("h40", "x=jtpt", z1bins, z1min, z1max);
    TH1D *h60 = new TH1D("h60", "x=jtpt", z1bins, z1min, z1max);
    TH1D *h80 = new TH1D("h80", "x=jtpt", z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;
    for (Long64_t ient = 0; ient < tree_all->GetEntries(); ient++) {
        // Print progress
        if (ient % 100000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }
 
        tree_all->GetEntry(ient);

        // Fill without weight and multiply histograms when combining them
        if (sample.Contains("HighEGJet")) {
            h80->Fill(jtpt);
        } else {
            if (weight>2) h40->Fill(jtpt);
            else h60->Fill(jtpt);
        }
    } // entry loop

    // Save histograms
    std::cout << "\n(Re)creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h40,h60,h80
                   }) {
        h->Write();
    }

    fout->Close();
}