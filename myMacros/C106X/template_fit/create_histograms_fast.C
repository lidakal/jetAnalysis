#include "../tTree.h"
#include "../hist_utils.h"

void create_histograms_fast()
{
    // TString sample = "dijet";
    // TString label = "aggrTMVA_withHLT_v2";
    // TString sample = "dijet";
    // TString label = "aggrTMVA_withHLT_v2_inclusive";
    // TString sample = "bjet";
    // TString label = "aggrTMVA_withHLT";
    // TString sample = "bjet";
    // TString label = "aggrTMVA_withHLT_inclusive";
    TString sample = "data";
    TString label = "aggrTMVA_v2";
    // TString sample = "data";
    // TString label = "aggrTMVA_v2_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree_template_fit.root";   
    TString fout_name = "./histos/" + sample + "_" + label + "_histograms.root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
    Double_t logrg;
    Double_t mb;
    Double_t weight;

    // jet pt bins : no underflow or overflow
    const Int_t z1bins = 3;
    Double_t z1min = 80.;
    Double_t z1max = 140.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 5;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 1.6; 

    // mBch bins : 
    const Int_t y1bins = 10;
    Double_t y1min = 0.;
    Double_t y1max = 7.;

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    bool mc = false;
    if (sample.Contains("data")) {
        tree_names = {"tree_all"};
        hist_names = {"h_data"};
    } else {
        mc = true;
        tree_names = {"tree_sig", "tree_bkg_bb", "tree_bkg_rest"};
        hist_names = {"h_sig", "h_bkg_bb", "h_bkg_rest"};
    }

    TFile *fout = new TFile(fout_name, "recreate");

    for (size_t i = 0; i < tree_names.size(); i++) {
        TString tree_name = tree_names[i];
        TString hist_name = hist_names[i];

        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("logrg", &logrg);
        tree->SetBranchAddress("mb", &mb);
        tree->SetBranchAddress("weight", &weight);

        TH3D *h_training = new TH3D(hist_name + "_training", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3D *h_testing = new TH3D(hist_name + "_testing", "x=rg, y=mBch, z=jtpt", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

        h_training->Sumw2();
        h_testing->Sumw2();

        if (!mc) h_training->SetName(hist_name);

        // sum of weights
        double ntest = 0;
        double ntrain = 0;

        // sum of squared weights
        double ntest2 = 0.; 
        double ntrain2 = 0.;

        // sum of entries 
        Long64_t nent_test = 0;
        Long64_t nent_train = 0;

        Long64_t nentries = tree->GetEntries();
        for (Long64_t ient = 0; ient < nentries; ient++) {
            tree->GetEntry(ient);

            // Check for logrg 
            if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
            // Check for mb
            if (mb < 0. || mb > 7.) continue;

            bool in_range = (jtpt >= z1min && jtpt < z1max) && (logrg >= x1min && logrg < x1max);

            if (mc) {
                // weight = 1.; //debug
                if (weight > 0.2) continue;
                if (ient > (nentries / 2)) {
                    // templates
                    h_training->Fill(logrg, mb, jtpt, weight);
                    if (in_range) {
                        ntrain += weight;
                        ntrain2 += weight * weight;
                        nent_train++;
                    }
                    
                }
                else {
                    // pseudo data
                    // weight = 1.; // debug
                    h_testing->Fill(logrg, mb, jtpt, weight);
                    if (in_range) {
                        nent_test++;
                        ntest += weight;
                        ntest2 += weight * weight;

                        if (weight > 0.2) {
                            std::cout << "weight = " << weight << ", jtpt = " << jtpt << std::endl; 
                        }
                    }
                    
                }
            } else {
                h_training->Fill(logrg, mb, jtpt);
            }
        } // tree entry loop
        h_training->Write();
        if (mc) h_testing->Write();
        // std::cout << tree_name 
        //           << " : ntest = " << ntest << " +- " << std::sqrt(ntest2)
        //           << ", ntrain = " << ntrain  << " +- " << std::sqrt(ntrain2) 
        //           << std::endl;
        // std::cout << tree_name 
        //           << " : nent_test = " << nent_test 
        //           << ", nent_train = " << nent_train   
        //           << std::endl;
    } // tree loop

    

    fout->Close();
    delete fout;
}