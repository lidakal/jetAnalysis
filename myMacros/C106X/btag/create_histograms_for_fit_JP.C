void create_histograms_for_fit_JP(TString sample="bjet", TString label="aggrTMVA_inclusive", bool tagged=true)
{
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";
    label += (tagged ? "_tagged" : "");
    // label += (tagged ? "_deepJetT" : "");
    TString fout_name = "histos/" + sample + "_" + label + "_histograms_for_fit_JP.root";

    TFile *fin = new TFile(fin_name);

    std::vector<TString> tree_names; 
    std::vector<TString> hist_names;
    bool mc = false;
    if (sample.Contains("data") || sample.Contains("SingleMuon") || sample.Contains("HighEGJet")) {
        tree_names = {"tree_all"};
        hist_names = {"h_data"};
    } else {
        mc = true;
        tree_names = {"tree_sig", "tree_bkg_bb", "tree_bkg_c", "tree_bkg_l"};
        hist_names = {"h_b", "h_bb", "h_c", "h_l"};
    }

    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t zg;
    Double_t mb;
    Double_t bpt;
    Double_t weight;
    Double_t jer_sf_nom;
    Double_t discr_pfJP;
    Double_t discr_particleNet_BvsAll;
    Double_t discr_b;
    Double_t discr_bb;
    Double_t discr_lepb;
    // TTree will be defined in the loop

    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;

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
    double x1max = x1binsVector[x1bins];

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

    // pfJP bins: last bin overflow
    const Int_t y1binsVectorSize = 61;
    int y1bins = y1binsVectorSize - 1;
    Double_t y1binsVector[y1binsVectorSize];
    double y1step = 0.035;
    // y1binsVector[0] = -y1step;
    // y1binsVector[1] = 0;
    y1binsVector[0] = 0.;
    y1binsVector[1] = y1step;
    // double y1step = (tagged) ? 0.1 : 0.05;
    
    for (size_t i=2; i<y1binsVectorSize; i++) {
        y1binsVector[i] = y1binsVector[i-1] + y1step;
    }
    Double_t y1min = y1binsVector[0];
    Double_t y1max = y1binsVector[y1bins];

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (size_t i = 0; i < tree_names.size(); i++) {
        TString tree_name = tree_names[i];
        TString hist_name = hist_names[i];

        TTree *tree = (TTree *) fin->Get(tree_name);
        tree->SetBranchAddress("jtpt", &jtpt);
        tree->SetBranchAddress("jtptCh", &jtptCh);
        tree->SetBranchAddress("logrg", &logrg);
        tree->SetBranchAddress("zg", &zg);
        tree->SetBranchAddress("bpt", &bpt);
        tree->SetBranchAddress("discr_pfJP", &discr_pfJP);
        tree->SetBranchAddress("discr_particleNet_BvsAll", &discr_particleNet_BvsAll);
        tree->SetBranchAddress("discr_b", &discr_b);
        tree->SetBranchAddress("discr_bb", &discr_bb);
        tree->SetBranchAddress("discr_lepb", &discr_lepb);
        tree->SetBranchAddress("weight", &weight);
        tree->SetBranchAddress("jer_sf_nom", &jer_sf_nom);

        tree->SetBranchAddress("jtpt_gen", &jtpt_gen);
        tree->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
        tree->SetBranchAddress("logrg_gen", &logrg_gen);
        tree->SetBranchAddress("zg_gen", &zg_gen);
        tree->SetBranchAddress("bpt_gen", &bpt_gen);

        TH3D *h_rg_training = new TH3D(hist_name + "_rg_training", "x=rg, y=pfJP, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_rg_testing = new TH3D(hist_name + "_rg_testing", "x=rg, y=pfJP, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_training = new TH3D(hist_name + "_zg_training", "x=zg, y=pfJP, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_testing = new TH3D(hist_name + "_zg_testing", "x=zg, y=pfJP, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_training = new TH3D(hist_name + "_zpt_training", "x=zpt, y=pfJP, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_testing = new TH3D(hist_name + "_zpt_testing", "x=zpt, y=pfJP, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);

        TH3D *h_rg_training_gen = new TH3D(hist_name + "_rg_training_gen", "x=rg, y=pfJP, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_rg_testing_gen = new TH3D(hist_name + "_rg_testing_gen", "x=rg, y=pfJP, z=jtpt", x1bins, x1binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_training_gen = new TH3D(hist_name + "_zg_training_gen", "x=zg, y=pfJP, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zg_testing_gen = new TH3D(hist_name + "_zg_testing_gen", "x=zg, y=pfJP, z=jtpt", x3bins, x3binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_training_gen = new TH3D(hist_name + "_zpt_training_gen", "x=zpt, y=pfJP, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);
        TH3D *h_zpt_testing_gen = new TH3D(hist_name + "_zpt_testing_gen", "x=zpt, y=pfJP, z=jtpt", x2bins, x2binsVector, y1bins, y1binsVector, z1bins, z1binsVector);

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
        for (Long64_t ient = 0; ient < nentries; ient++) {
            tree->GetEntry(ient);

            // if (ient>1000) break;

            if (tagged && discr_particleNet_BvsAll<0.99) continue;
            // if (tagged && (discr_b+discr_bb+discr_lepb)<0.55) continue;

            if (!mc) jer_sf_nom = 1;
            jtpt = jtpt * jer_sf_nom;

            // Check for logrg 
            if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
            if (logrg>=x1max) logrg = x1max - 1e-4; // overflow
            if (logrg_gen>=x1max) logrg_gen = x1max - 1e-4; // overflow

            // Check for jtptCh
            if (std::abs(jtptCh) < 1e-4) continue;

            // Check for zg
            if (std::abs(zg-0.5)<1e-4) zg = 0.499; // fix max value
            if (std::abs(zg_gen-0.5)<1e-4) zg_gen = 0.499; // fix max value

            // Check jtpt
            if (jtpt<z1min || jtpt>=z1max) continue;

            // Check for JP
            if (discr_pfJP>=y1max) discr_pfJP = y1max - 1e-4; // overflow
            // if (!(discr_pfJP>0)) discr_pfJP = y1min + 1e-4; // JP-untagged
            if (!(discr_pfJP>0)) discr_pfJP = -1.;

            double zpt = -1.;
            if (jtptCh > 0) zpt = bpt / jtptCh;
            if (std::abs(zpt-1)<1e-3) zpt = 0.999; // fix max value

            double zpt_gen = -1.;
            if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
            if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999; // fix max value
            
            if (mc) {
                if (weight > 0.01) continue;
                if (ient > (nentries / 2)) {
                    // templates
                    h_rg_training->Fill(logrg, discr_pfJP, jtpt, weight);
                    h_zpt_training->Fill(zpt, discr_pfJP, jtpt, weight);
                    h_zg_training->Fill(zg, discr_pfJP, jtpt, weight);

                    h_rg_training_gen->Fill(logrg_gen, discr_pfJP, jtpt_gen, weight);
                    h_zpt_training_gen->Fill(zpt_gen, discr_pfJP, jtpt_gen, weight);
                    h_zg_training_gen->Fill(zg_gen, discr_pfJP, jtpt_gen, weight);
                }
                else {
                    // pseudo data
                    // weight = 1.; // debug
                    h_rg_testing->Fill(logrg, discr_pfJP, jtpt, weight);
                    h_zpt_testing->Fill(zpt, discr_pfJP, jtpt, weight);   
                    h_zg_testing->Fill(zg, discr_pfJP, jtpt, weight);   

                    h_rg_testing_gen->Fill(logrg_gen, discr_pfJP, jtpt_gen, weight);
                    h_zpt_testing_gen->Fill(zpt_gen, discr_pfJP, jtpt_gen, weight);   
                    h_zg_testing_gen->Fill(zg_gen, discr_pfJP, jtpt_gen, weight);                    
                }
            } else {
                h_rg_training->Fill(logrg, discr_pfJP, jtpt, weight);
                h_zpt_training->Fill(zpt, discr_pfJP, jtpt, weight);
                h_zg_training->Fill(zg, discr_pfJP, jtpt, weight);
            }
        } // tree entry loop
        fout->cd();
        h_rg_training->Write();
        h_zpt_training->Write();
        h_zg_training->Write();
        if (mc) {
            h_rg_testing->Write();
            h_zpt_testing->Write();
            h_zg_testing->Write();

            h_rg_training_gen->Write();
            h_zpt_training_gen->Write();
            h_zg_training_gen->Write();

            h_rg_testing_gen->Write();
            h_zpt_testing_gen->Write();
            h_zg_testing_gen->Write();
        }
    } // tree loop
    fout->Close();
}