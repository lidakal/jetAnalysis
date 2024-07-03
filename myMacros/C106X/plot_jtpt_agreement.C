#include "binning.h"

void plot_jtpt_agreement() 
{
    // ---- Load tree
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";
    TString fout_name = "./histos/" + sample + "_" + label + "_jtpt_agreement.root"; 

    TFile *fin = new TFile(fin_name);
    std::vector<TString> tree_names = {};

    if (sample.Contains("dijet")) {
        tree_names = {"tree_all"}; 
    } else {
        tree_names = {"tree_sig", "tree_bkg_bb"}; 
    }

    // jtpt_gen
    Int_t x1bins = 10;
    Double_t x1min = 80.;
    Double_t x1max = 140.;

    // jtpt/jtpt_gen
    Int_t y1bins = 40;
    Double_t y1min = 0.;
    Double_t y1max = 2.;

    Double_t jtpt;
    Double_t jtpt_gen;
    Double_t weight;
    Double_t pthat;

    TH2D *h_jtpt = new TH2D("h_jtpt", "x=jtpt_gen, y=jtpt/jtpt_gen", x1bins, x1min, x1max, y1bins, y1min, y1max);
    std::vector<TH2D *> histos = {};

    for (TString tree_name : tree_names) {
        std::cout << "Looping through " << tree_name << std::endl;
        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("jtpt_gen", &jtpt_gen);
        tree->SetBranchAddress("weight", &weight);
        tree->SetBranchAddress("pthat", &pthat);

        TH2D *h_jtpt_t = new TH2D(Form("h_jtpt_%s", tree_name.Data()), "x=jtpt_gen, y=jtpt/jtpt_gen", x1bins, x1min, x1max, y1bins, y1min, y1max);

        for (Long64_t ient = 0; ient < tree->GetEntries(); ient++) {
            // Print progress
            if (ient % 100000000 == 0) {
                std::cout << "ient = " << ient << std::endl;
            }
    
            tree->GetEntry(ient);
            if (skipMC(jtpt, jtpt_gen, pthat)) continue;

            if (jtpt_gen>0) h_jtpt_t->Fill(jtpt_gen, jtpt/jtpt_gen, weight);
        } // entry loop
        histos.push_back(h_jtpt_t);
    }

    for (auto h : histos) {
        h_jtpt->Add(h);
    }

    // Save histograms
    std::cout << "\n(Re)creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_jtpt,
                   }) {
        h->Write();
    }

    fout->Close();

}
