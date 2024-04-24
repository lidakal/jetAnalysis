#include "../tTree.h"
#include "../hist_utils.h"

void create_histograms_fast_gen()
{
    TString sample = "bjet";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";   
    TString fout_name = "./histos/" + sample + "_" + label + "_histograms_gen.root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t mb;
    Double_t bpt;
    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;
    Int_t nb_gen;
    Int_t nc_gen;
    Double_t weight;

    // jet pt bins : no underflow or overflow
    const Int_t z1bins = 70;
    Double_t z1min = 30.;
    Double_t z1max = 730.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 13;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 4.8; 

    // mBch bins : 
    const Int_t y1bins = 10;
    Double_t y1min = 0.;
    Double_t y1max = 7.;

    // bpt/jtptCh bins :
    const Int_t x2bins = 20;
    Double_t x2min = 0.;
    Double_t x2max = 1.; 

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    tree_names = {"tree_sig", "tree_bkg_bb", "tree_bkg_rest"};
    hist_names = {"h_sig", "h_bkg_bb", "h_bkg_rest"};
    
    TFile *fout = new TFile(fout_name, "recreate");

    for (size_t i = 0; i < tree_names.size(); i++) {
        TString tree_name = tree_names[i];
        TString hist_name = hist_names[i];

        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("jtptCh", &jtptCh);
        tree->SetBranchAddress("logrg", &logrg);
        tree->SetBranchAddress("mb", &mb);
        tree->SetBranchAddress("bpt", &bpt);
        tree->SetBranchAddress("jtpt_gen", &jtpt_gen);
        tree->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
        tree->SetBranchAddress("logrg_gen", &logrg_gen);
        tree->SetBranchAddress("mb_gen", &mb_gen);
        tree->SetBranchAddress("bpt_gen", &bpt_gen);
        tree->SetBranchAddress("nb_gen", &nb_gen);
        tree->SetBranchAddress("nc_gen", &nc_gen);
        tree->SetBranchAddress("weight", &weight);

        TH3D *h_rg = new TH3D(hist_name + "_rg", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3D *h_zpt = new TH3D(hist_name + "_zpt", "x=zpt, y=mBch, z=jtpt", x2bins, x2min, x2max, y1bins, y1min, y1max, z1bins, z1min, z1max);

        // Create a separate histogram for c 
        TH3D *h_rg_c = new TH3D(hist_name + "_rg_c", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3D *h_zpt_c = new TH3D(hist_name + "_zpt_c", "x=zpt, y=mBch, z=jtpt", x2bins, x2min, x2max, y1bins, y1min, y1max, z1bins, z1min, z1max);

        for (auto h : {h_rg,
                       h_zpt,
                       h_rg_c, h_zpt_c
                       }) {
                        h->Sumw2();
                       }

        Long64_t nentries = tree->GetEntries();
        std::cout << nentries << std::endl;

        for (Long64_t ient = 0; ient < nentries; ient++) {
            tree->GetEntry(ient);

            // Check for logrg 
            if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
            // Check for mb
            // if (mb < 0. || mb > 7.) continue;

            double zpt_gen = -1.;
            if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
            
            if (weight > 0.005) continue;
            h_rg->Fill(logrg_gen, mb_gen, jtpt_gen, weight);
            h_zpt->Fill(zpt_gen, mb_gen, jtpt_gen, weight);

            if (nb_gen==0&&nc_gen>0) {
                h_rg_c->Fill(logrg_gen, mb_gen, jtpt_gen, weight);
                h_zpt_c->Fill(zpt_gen, mb_gen, jtpt_gen, weight);
            }
            
        } // tree entry loop

        h_rg->Write();
        h_zpt->Write();
        if (i==2) {
            h_rg_c->Write();
            h_zpt_c->Write();
        }
    } // tree type loop
    
    fout->Close();
    delete fout;
}