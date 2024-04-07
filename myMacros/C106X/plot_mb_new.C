#include "tTree.h"
#include "hist_utils.h"

void plot_mb_new()
{
    // Plot mb templates 

    double pt_min = 100;
    double pt_max = 120;

    // ---- Load tree
    TString sample = "dijet";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";
    TString fout_name = "./histos/" + sample + "_" + label + "_mb_jer_nom_jec_nom.root"; 

    TFile *fin = new TFile(fin_name);
    TTree *tree_all = (TTree *) fin->Get("tree_all");

    Double_t mb;
    tree_all->SetBranchAddress("mb", &mb);

    Double_t jtpt;
    tree_all->SetBranchAddress("jtpt", &jtpt);

    Int_t nb_gen;
    tree_all->SetBranchAddress("nb_gen", &nb_gen);

    Int_t nc_gen;
    tree_all->SetBranchAddress("nc_gen", &nc_gen);

    Double_t weight;
    tree_all->SetBranchAddress("weight", &weight);

    Double_t jer_sf_nom;
    tree_all->SetBranchAddress("jer_sf_nom", &jer_sf_nom);

    // Create histograms

    // mb
    Int_t x1bins = 20;
    Float_t x1min = 0.;
    Float_t x1max = 7.;

    TH1D *hSingleB_mb = new TH1D("hSingleB_mb", "x=mb", x1bins, x1min, x1max);
    TH1D *hBB_mb = new TH1D("hBB_mb", "x=mb", x1bins, x1min, x1max);
    TH1D *hC_mb = new TH1D("hC_mb", "x=mb", x1bins, x1min, x1max);
    TH1D *hL_mb = new TH1D("hL_mb", "x=mb", x1bins, x1min, x1max);


    std::cout << "Creating histograms ..." << std::endl;
    for (Long64_t ient = 0; ient < tree_all->GetEntries(); ient++) {
        // Print progress
        if (ient % 100000000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        tree_all->GetEntry(ient);
        jtpt *= jer_sf_nom;

        if (mb>x1max) mb = x1max-0.001;

        if (nb_gen==1) {
            hSingleB_mb->Fill(mb, weight);
        } else if (nb_gen>1) {
            hBB_mb->Fill(mb, weight);
        } else if (nc_gen>1) {
            hC_mb->Fill(mb, weight);
        } else {
            hL_mb->Fill(mb, weight);
        }        
    } // entry loop

    // Save histograms
    std::cout << "\n(Re)creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {hSingleB_mb,hBB_mb,hC_mb,hL_mb
                   }) {
        h->Write();
    }

    fout->Close();
}