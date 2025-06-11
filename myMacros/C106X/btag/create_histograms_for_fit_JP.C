#include "../binning.h"

void create_histograms_for_fit_JP(TString sample="bjet_PF40", TString label="aggrTMVA_inclusive")
{
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";
    TString fout_name = "histos/" + sample + "_" + label + "_histograms_for_fit_JP.root";

    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    bool mc = false;
    if (sample.Contains("data") || sample.Contains("SingleMuon") || sample.Contains("HighEGJet")) {
        tree_names = {"tree_all"};
        hist_names = {"h_data"};
    } else {
        mc = true;
        tree_names = {"tree_sig", "tree_bkg_bb", "tree_bkg_cc", "tree_bkg_singlec", "tree_bkg_l"};
        hist_names = {"h_b", "h_bb", "h_cc", "h_c", "h_l"};
    }

    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t logkt;
    Double_t zg;
    Double_t mb;
    Double_t bpt;

    Double_t jer_sf_nom;

    Double_t discr_pfJP;
    Double_t discr_particleNet_BvsAll;
    Double_t discr_b;
    Double_t discr_bb;
    Double_t discr_lepb;

    Double_t weight;
    Double_t pthat;

    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t logkt_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;

    // TTree will be defined in the loop

    // pfJP bins: last bin overflow
    const Int_t y1binsVectorSize = 61;
    int y1bins = y1binsVectorSize - 1;
    Double_t y1binsVector[y1binsVectorSize];
    double y1step = 0.035;
    // y1binsVector[0] = -y1step;
    // y1binsVector[1] = 0;
    y1binsVector[0] = 0.;
    y1binsVector[1] = y1step;
    
    for (size_t i=2; i<y1binsVectorSize; i++) {
        y1binsVector[i] = y1binsVector[i-1] + y1step;
    }
    Double_t y1min = y1binsVector[0];
    Double_t y1max = y1binsVector[y1bins];

    const Int_t y2binsVectorSize = 71;
    int y2bins = y2binsVectorSize - 1;
    Double_t y2binsVector[y2binsVectorSize];
    double y2step = 0.035;
    // y2binsVector[0] = -y2step;
    // y2binsVector[1] = 0;
    y2binsVector[0] = 0.;
    y2binsVector[1] = y2step;
    
    for (size_t i=2; i<y2binsVectorSize; i++) {
        y2binsVector[i] = y2binsVector[i-1] + y2step;
    }
    Double_t y2min = y2binsVector[0];
    Double_t y2max = y2binsVector[y2bins];

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (size_t i = 0; i < tree_names.size(); i++) {
        TString tree_name = tree_names[i];
        TString hist_name = hist_names[i];

        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("jtptCh", &jtptCh);
        tree->SetBranchAddress("logrg", &logrg);
        tree->SetBranchAddress("logkt", &logkt);
        tree->SetBranchAddress("zg", &zg);
        tree->SetBranchAddress("bpt", &bpt);
        tree->SetBranchAddress("discr_pfJP", &discr_pfJP);
        tree->SetBranchAddress("discr_particleNet_BvsAll", &discr_particleNet_BvsAll);
        tree->SetBranchAddress("discr_b", &discr_b);
        tree->SetBranchAddress("discr_bb", &discr_bb);
        tree->SetBranchAddress("discr_lepb", &discr_lepb);

        tree->SetBranchAddress("jer_sf_nom", &jer_sf_nom);

        tree->SetBranchAddress("jtpt_gen", &jtpt_gen);
        tree->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
        tree->SetBranchAddress("logrg_gen", &logrg_gen);
        tree->SetBranchAddress("logkt_gen", &logkt_gen);
        tree->SetBranchAddress("zg_gen", &zg_gen);
        tree->SetBranchAddress("bpt_gen", &bpt_gen);

        tree->SetBranchAddress("weight", &weight);
        tree->SetBranchAddress("pthat", &pthat);

        TH3D *h_rg = new TH3D(hist_name + "_rg", "x=rg, y=pfJP, z=jtpt", logrg_bins, logrg_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_rg_tagged = new TH3D(hist_name + "_rg_tagged", "x=rg, y=pfJP, z=jtpt", logrg_bins, logrg_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zg = new TH3D(hist_name + "_zg", "x=zg, y=pfJP, z=jtpt", zg_bins, zg_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zg_tagged = new TH3D(hist_name + "_zg_tagged", "x=zg, y=pfJP, z=jtpt", zg_bins, zg_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zpt = new TH3D(hist_name + "_zpt", "x=zpt, y=pfJP, z=jtpt", zpt_bins, zpt_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zpt_tagged = new TH3D(hist_name + "_zpt_tagged", "x=zpt, y=pfJP, z=jtpt", zpt_bins, zpt_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);

        // mc only
        TH3D *h_rg_gen = new TH3D(hist_name + "_rg_gen", "x=rg, y=pfJP, z=jtpt", logrg_bins, logrg_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_rg_tagged_gen = new TH3D(hist_name + "_rg_tagged_gen", "x=rg, y=pfJP, z=jtpt", logrg_bins, logrg_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zg_gen = new TH3D(hist_name + "_zg_gen", "x=zg, y=pfJP, z=jtpt", zg_bins, zg_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zg_tagged_gen = new TH3D(hist_name + "_zg_tagged_gen", "x=zg, y=pfJP, z=jtpt", zg_bins, zg_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zpt_gen = new TH3D(hist_name + "_zpt_gen", "x=zpt, y=pfJP, z=jtpt", zpt_bins, zpt_binsVector, y1bins, y1binsVector, jtpt_bins, jtpt_binsVector);
        TH3D *h_zpt_tagged_gen = new TH3D(hist_name + "_zpt_tagged_gen", "x=zpt, y=pfJP, z=jtpt", zpt_bins, zpt_binsVector, y2bins, y2binsVector, jtpt_bins, jtpt_binsVector);

        Long64_t nentries = tree->GetEntries();
        for (Long64_t ient = 0; ient < nentries; ient++) {
            tree->GetEntry(ient);

            // std::cout << "logkt=" << logkt << std::endl;
            // std::cout << "logkt_gen=" << logkt_gen << std::endl;
            // if (ient>10) break;

            if (mc && skipMC(jtpt, jtpt_gen, pthat)) continue; // skip large weight events in MC 
            // weight = 1; // FOR YIELD STUDIES

            bool passTag = (discr_particleNet_BvsAll>0.99);

            if (!mc) jer_sf_nom = 1;
            jtpt = jtpt * jer_sf_nom;

            // Checks for logrg 
            if (logrg < 0. && logrg > -900.) continue; // rg>0.4 -> skip for now
            if (logrg < -900.) logrg = -0.2; // SD-untagged bin range
            if (logkt < 0. && logkt > -900.) logrg = -0.2; // kt<1 -> SD-untagged bin range
            if (logrg >= 2.5) logrg = 2.499; // overflow bin range
            if (logrg_gen < 0. && logrg_gen>-900) logrg_gen = -900; // rg_gen>0.4 -> out of range but not SD-untagged
            if (logrg_gen < -900.) logrg_gen = -0.2; // SD-untagged bin range
            if (logkt_gen < 0. && logkt_gen > -900.) logrg_gen = -0.2; // kt_gen<1 -> SD-untagged bin range
            if (logrg_gen >= 2.5) logrg_gen = 2.499; // overflow bin range

            // Checks for zg
            if (zg < -900.) zg = 0.; // SD-untagged bin range
            if (logkt < 0. && logkt > -900.) zg = 0.; // kt<1 -> SD-untagged bin range
            if (zg >= 0.5) zg = 0.499; // zg=0.5 included in last bin
            if (zg_gen < -900.) zg_gen = 0.; // SD-untagged bin range
            if (logkt_gen < 0. && logkt_gen > -900.) zg_gen = 0.; // kt<1 -> SD-untagged bin range
            if (zg_gen >= 0.5) zg_gen = 0.499; // zg=0.5 included in last bin

            // Checks for zpt
            if (std::abs(jtptCh) < 1e-4) continue;
            double zpt = -1.;
            if (jtptCh > 0) zpt = bpt / jtptCh;
            if (zpt>=1.) zpt = 0.999; // zpt=1 included in last bin
            double zpt_gen = -1.;
            if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
            if (zpt_gen>=1.) zpt_gen = 0.999; // zpt_gen=1 included in last bin

            // Check jtpt
            if (jtpt<jtpt_min || jtpt>=jtpt_max) continue;

            // Check for JP
            Double_t discr_pfJP_tagged = discr_pfJP;

            if (discr_pfJP>=y1max) discr_pfJP = y1max - 1e-4; // overflow
            // if (!(discr_pfJP>0)) discr_pfJP = y1min + 1e-4; // JP-untagged
            if (!(discr_pfJP>0)) discr_pfJP = -1.;

            if (discr_pfJP_tagged>=y2max) discr_pfJP_tagged = y2max - 1e-4; // overflow
            if (!(discr_pfJP_tagged>0)) discr_pfJP_tagged = -1.;
            
            h_rg->Fill(logrg, discr_pfJP, jtpt, weight);
            h_zpt->Fill(zpt, discr_pfJP, jtpt, weight);
            h_zg->Fill(zg, discr_pfJP, jtpt, weight);

            if (passTag) {
                h_rg_tagged->Fill(logrg, discr_pfJP_tagged, jtpt, weight);
                h_zpt_tagged->Fill(zpt, discr_pfJP_tagged, jtpt, weight);   
                h_zg_tagged->Fill(zg, discr_pfJP_tagged, jtpt, weight);   
            }

            if (mc) {       
                // std::cout << "hi" << std::endl;
        
                h_rg_gen->Fill(logrg_gen, discr_pfJP, jtpt_gen, weight);
                h_zpt_gen->Fill(zpt_gen, discr_pfJP, jtpt_gen, weight);
                h_zg_gen->Fill(zg_gen, discr_pfJP, jtpt_gen, weight);
            
                if (passTag) {
                    h_rg_tagged_gen->Fill(logrg_gen, discr_pfJP_tagged, jtpt_gen, weight);
                    h_zpt_tagged_gen->Fill(zpt_gen, discr_pfJP_tagged, jtpt_gen, weight);   
                    h_zg_tagged_gen->Fill(zg_gen, discr_pfJP_tagged, jtpt_gen, weight);              
                }      
            }
        } // tree entry loop
        fout->cd();
        h_rg->Write();
        h_zpt->Write();
        h_zg->Write();

        h_rg_tagged->Write();
        h_zpt_tagged->Write();
        h_zg_tagged->Write();

        if (mc) {
            h_rg_gen->Write();
            h_zpt_gen->Write();
            h_zg_gen->Write();

            h_rg_tagged_gen->Write();
            h_zpt_tagged_gen->Write();
            h_zg_tagged_gen->Write();
        }
    } // tree loop
    fout->Close();
}