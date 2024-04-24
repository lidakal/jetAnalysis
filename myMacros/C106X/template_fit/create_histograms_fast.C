#include "../tTree.h"
#include "../hist_utils.h"

void create_histograms_fast(TString sample="", TString label="", 
                            TString jer_opt="nom", TString jec_opt="nom")
{
    if (sample=="") sample = "herwig_PF40";
    if (label=="") label = "aggrTMVA_XXT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";  
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt; 

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    bool mc = false;
    if (sample.Contains("data")) {
        tree_names = {"tree_all"};
        hist_names = {"h_data"};
        suffix = "";
    } else {
        mc = true;
        tree_names = {"tree_sig", "tree_bkg_bb", "tree_bkg_rest"};
        hist_names = {"h_sig", "h_bkg_bb", "h_bkg_rest"};
    }

    TString fout_name = "./histos/" + sample + "_" + label + "_histograms" + suffix + ".root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);

    // even in MC we only use reco for template fit
    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t zg;
    Double_t mb;
    Double_t bpt;
    Double_t weight;
    Double_t jer_sf_nom;
    Double_t jer_sf_up;
    Double_t jer_sf_down;
    Double_t jec_unc;

    // jet pt bins : no underflow or overflow
    const Int_t z1binsVectorSize = 4;
    Int_t z1bins = z1binsVectorSize - 1;
    Double_t z1binsVector[z1binsVectorSize] = {80., 
                                               100., 
                                               120.,
                                               140.
                                               };
    Double_t z1min = z1binsVector[0];
    Double_t z1max = z1binsVector[z1bins];

    // ln(0.4/rg) bins : 1st bin untagged (unphysical), last bin overflow
    const Int_t x1binsVectorSize = 9;
    Int_t x1bins = x1binsVectorSize - 1;
    Double_t x1binsVector[x1binsVectorSize] = {-1.2, 
                                               0., 
                                               0.3,
                                               0.6,
                                               0.9,
                                               1.2,
                                               1.6,
                                               2.1,
                                               2.5
                                               };
    Double_t x1min = x1binsVector[0];
    Double_t x1max = x1binsVector[x1bins];

    // zg bins : 1st bin untagged (unphysical)
    const Int_t x3binsVectorSize = 8;
    Int_t x3bins = x3binsVectorSize - 1;
    Double_t x3binsVector[x3binsVectorSize] = {-0.1, 
                                               0.1, 
                                               0.15,
                                               0.2,
                                               0.25,
                                               0.3,
                                               0.4,
                                               0.5};
    Double_t x3min = x3binsVector[0];
    Double_t x3max = x3binsVector[x3bins];


    // mBch bins : 
    // const Int_t y1bins = 20;
    // Double_t y1min = 0.;
    // Double_t y1max = 7.;
    // last bin overflow
    const Int_t y1binsVectorSize = 21;
    Int_t y1bins = y1binsVectorSize - 1;
    Double_t y1binsVector[y1binsVectorSize];
    y1binsVector[0] = 0.;
    double y1step = 7./y1bins;
    for (size_t i=1; i<y1binsVectorSize; i++) {
        y1binsVector[i] = y1binsVector[i-1] + y1step;
    }
    Double_t y1min = y1binsVector[0];
    Double_t y1max = y1binsVector[y1bins];

    // bpt/jtptCh bins :
    const Int_t x2binsVectorSize = 7;
    Int_t x2bins = x2binsVectorSize - 1;
    Double_t x2binsVector[x2binsVectorSize] = {0., 
                                               0.35,
                                               0.55,
                                               0.7,
                                               0.8,
                                               0.9,
                                               1.};
    Double_t x2min = x2binsVector[0];
    Double_t x2max = x2binsVector[x2bins];

    TFile *fout = new TFile(fout_name, "recreate");

    for (size_t i = 0; i < tree_names.size(); i++) {
        TString tree_name = tree_names[i];
        TString hist_name = hist_names[i];

        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("jtptCh", &jtptCh);
        tree->SetBranchAddress("logrg", &logrg);
        tree->SetBranchAddress("zg", &zg);
        tree->SetBranchAddress("mb", &mb);
        tree->SetBranchAddress("bpt", &bpt);
        tree->SetBranchAddress("weight", &weight);
        tree->SetBranchAddress("jer_sf_nom", &jer_sf_nom);
        tree->SetBranchAddress("jer_sf_up", &jer_sf_up);
        tree->SetBranchAddress("jer_sf_down", &jer_sf_down);
        tree->SetBranchAddress("jec_unc", &jec_unc);

        TH3D *h_rg_training = new TH3D(hist_name + "_rg_training", "x=rg, y=mBch, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_rg_testing = new TH3D(hist_name + "_rg_testing", "x=rg, y=mBch, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_training = new TH3D(hist_name + "_zg_training", "x=zg, y=mBch, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_testing = new TH3D(hist_name + "_zg_testing", "x=zg, y=mBch, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_training = new TH3D(hist_name + "_zpt_training", "x=zpt, y=mBch, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_testing = new TH3D(hist_name + "_zpt_testing", "x=zpt, y=mBch, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);

        for (auto h : {h_rg_training, h_rg_testing,
                       h_zpt_training, h_zpt_testing}) {
                        h->Sumw2();
                       }

        if (!mc) {
            h_rg_training->SetName(hist_name + "_rg");
            h_zpt_training->SetName(hist_name + "_zpt");
            h_zg_training->SetName(hist_name + "_zg");
        }

        Long64_t nentries = tree->GetEntries();
        std::cout << tree_name << "->GetEntries() = " << nentries << std::endl;

        double hentries = 0;

        TRandom *random = new TRandom();
        for (Long64_t ient = 0; ient < nentries; ient++) {
            tree->GetEntry(ient);

            if (!mc) jer_sf_nom = 1;

            ///////////////////////////////////////////////////
            // ---- skip while jer SF nom not working ---- 
            if (jer_opt=="nom") jtpt = jtpt * jer_sf_nom;
            else if (jer_opt=="up") jtpt = jtpt * jer_sf_up;
            else if (jer_opt=="down") jtpt = jtpt * jer_sf_down;

            double jec_fact = 0; // nominal
            if (jec_opt=="up") jec_fact = 1;
            if (jec_opt=="down") jec_fact = -1;
            double newCorrection = 1 + (jec_fact * jec_unc);
            jtpt = jtpt * newCorrection;
            ///////////////////////////////////////////////////

            // Check for logrg 
            if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
            if (logrg>=2.5) logrg = 2.499; // overflow

            // Check for jtptCh
            if (std::abs(jtptCh) < 1e-4) continue;

            // Check for mb
            if (mb >= 7.) mb = 6.999; // overflow

            // Check for zg
            if (std::abs(zg-0.5)<1e-4) zg = 0.499; // fix max value

            // Check jtpt
            if (jtpt<z1min || jtpt>=z1max) continue;

            double zpt = -1.;
            if (jtptCh > 0) zpt = bpt / jtptCh;
            if (std::abs(zpt-1)<1e-3) zpt = 0.999; // fix max value
            
            if (mc) {
                if (weight > 0.01) continue;
                hentries++;
                if (ient > (nentries / 2)) {
                    // templates
                    h_rg_training->Fill(logrg, mb, jtpt, weight);
                    h_zpt_training->Fill(zpt, mb, jtpt, weight);
                    h_zg_training->Fill(zg, mb, jtpt, weight);
                }
                else {
                    // pseudo data
                    // weight = 1.; // debug
                    h_rg_testing->Fill(logrg, mb, jtpt, weight);
                    h_zpt_testing->Fill(zpt, mb, jtpt, weight);   
                    h_zg_testing->Fill(zg, mb, jtpt, weight);                    
                }
            } else {
                // double uniform = random->Uniform();
                // if (uniform > 0.345) {
                    // continue;
                // }
                if (weight > 2.) weight = 6.32142; // until I remake the trees
                h_rg_training->Fill(logrg, mb, jtpt, weight);
                h_zpt_training->Fill(zpt, mb, jtpt, weight);
                h_zg_training->Fill(zg, mb, jtpt, weight);
            }
        } // tree entry loop

        h_rg_training->Write();
        h_zpt_training->Write();
        h_zg_training->Write();
        if (mc) {
            h_rg_testing->Write();
            h_zpt_testing->Write();
            h_zg_testing->Write();

            std::cout << hist_name << " = " << hentries << std::endl;
        }
    } // tree type loop
    
    fout->Close();
    delete fout;
}