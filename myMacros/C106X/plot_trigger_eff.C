#include "tTree.h"
#include "hist_utils.h"

void plot_trigger_eff()
{
    // ---- Load tree
    TString sample = "HighEGJet_PF80and100_v2";
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
    Int_t z1bins = 29;
    Float_t z1min = 10.;
    Float_t z1max = 300.;

    TH1D *h_high = new TH1D("h_high", "x=jtpt", z1bins, z1min, z1max);
    TH1D *h_low = new TH1D("h_low", "x=jtpt", z1bins, z1min, z1max);
    TH1D *h_lower = new TH1D("h_lower", "x=jtpt", z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;
    for (Long64_t ient = 0; ient < tree_all->GetEntries(); ient++) {
        // Print progress
        if (ient % 100000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }
 
        tree_all->GetEntry(ient);
        if (sample.Contains("HighEGJet")) {
            h_high->Fill(jtpt);
        } else if (sample.Contains("LowEGJet")) {
            if (weight<2) h_low->Fill(jtpt);
            else h_lower->Fill(jtpt);
        }
    } // entry loop

    // Save histograms
    std::cout << "\n(Re)creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_high, h_low, h_lower
                   }) {
        h->Write();
    }

    fout->Close();
}