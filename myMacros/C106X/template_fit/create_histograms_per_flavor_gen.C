#include "../tTree.h"
#include "../hist_utils.h"
#include "../binning.h"

void create_histograms_per_flavor_gen()
{
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";   
    TString fout_name = "./histos/" + sample + "_" + label + "_histograms_per_refflavor_gen.root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;
    Double_t logkt_gen;
    Int_t nb_gen;
    Int_t nc_gen;
    Int_t parton_flavor;
    Double_t weight;
    Double_t pthat;

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    
    TFile *fout = new TFile(fout_name, "recreate");

    TString tree_name = "tree_all";

    TTree *tree = (TTree *) fin->Get(tree_name);
    tree->SetBranchAddress("jtpt", &jtpt);
    tree->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree->SetBranchAddress("logrg_gen", &logrg_gen);
    tree->SetBranchAddress("zg_gen", &zg_gen);
    tree->SetBranchAddress("mb_gen", &mb_gen);
    tree->SetBranchAddress("bpt_gen", &bpt_gen);
    tree->SetBranchAddress("logkt_gen", &logkt_gen);
    tree->SetBranchAddress("nb_gen", &nb_gen);
    tree->SetBranchAddress("nc_gen", &nc_gen);
    tree->SetBranchAddress("refparton_flavor", &parton_flavor);
    tree->SetBranchAddress("weight", &weight);
    tree->SetBranchAddress("pthat", &pthat); 
    
    TH2D *h_rg_b = new TH2D("h_rg_b", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_bb = new TH2D("h_rg_bb", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_c = new TH2D("h_rg_c", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_u = new TH2D("h_rg_u", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_d = new TH2D("h_rg_d", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_s = new TH2D("h_rg_s", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_g = new TH2D("h_rg_g", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_rg_undef = new TH2D("h_rg_undef", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_zg_b = new TH2D("h_zg_b", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_bb = new TH2D("h_zg_bb", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_c = new TH2D("h_zg_c", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_u = new TH2D("h_zg_u", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_d = new TH2D("h_zg_d", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_s = new TH2D("h_zg_s", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_g = new TH2D("h_zg_g", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_undef = new TH2D("h_zg_undef", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);

    Long64_t nentries = tree->GetEntries();
    std::cout << nentries << std::endl;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree->GetEntry(ient);
        if (skipMC(jtpt, jtpt_gen, pthat)) continue;

        // Checks for jtptCh
        if (jtptCh_gen < 1e-4) continue;

        // Checks for logrg 
        if (logrg_gen < 0. && logrg_gen>-900) logrg_gen = -800; // rg_gen>0.4 -> out of range but not SD-untagged
        if (logrg_gen < -900.) logrg_gen = -0.2; // SD-untagged bin range
        if (logkt_gen < 0.) logrg_gen = -0.2; // kt_gen<1 -> SD-untagged bin range // if logkt=-999, already SD-untagged
        if (logrg_gen >= 2.5) logrg_gen = 2.499; // overflow bin range

        // Checks for zg
        if (zg_gen < -900.) zg_gen = 0.; // SD-untagged bin range
        if (logkt_gen < 0.) zg_gen = 0.; // kt<1 -> SD-untagged bin range
        if (zg_gen >= 0.5) zg_gen = 0.499; // zg=0.5 included in last bin

        // FILL HISTOGRAMS
        if (nb_gen>1) {
            h_rg_bb->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_bb->Fill(zg_gen, jtpt_gen, weight);
        } else if (nb_gen==1) {
            h_rg_b->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_b->Fill(zg_gen, jtpt_gen, weight);
        } else if (nc_gen>0) {
            h_rg_c->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_c->Fill(zg_gen, jtpt_gen, weight);
        } else if (std::abs(parton_flavor)==3) {
            h_rg_s->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_s->Fill(zg_gen, jtpt_gen, weight);
        } else if (std::abs(parton_flavor)==2) {
            h_rg_d->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_d->Fill(zg_gen, jtpt_gen, weight);
        } else if (std::abs(parton_flavor)==1) {
            h_rg_u->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_u->Fill(zg_gen, jtpt_gen, weight);
        } else if (std::abs(parton_flavor)==21) {
            h_rg_g->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_g->Fill(zg_gen, jtpt_gen, weight);
        } else {
            h_rg_undef->Fill(logrg_gen, jtpt_gen, weight);
            h_zg_undef->Fill(zg_gen, jtpt_gen, weight);
        }
    } // tree entry loop

    for (auto h : {
        h_rg_b, h_rg_bb, h_rg_c, h_rg_u, h_rg_d, h_rg_s, h_rg_g, h_rg_undef,
        h_zg_b, h_zg_bb, h_zg_c, h_zg_u, h_zg_d, h_zg_s, h_zg_g, h_zg_undef
    }) { 
        h->Write();
    }
    
    fout->Close();
    delete fout;
}