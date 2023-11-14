void create_response_fast()
{
    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";   
    TString fout_name = "./histos/" + sample + "_" + label + "_response.root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TTree *tree_sig = (TTree *) fin->Get("tree_sig");

    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t zg;
    Double_t mb;
    Double_t bpt;
    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;
    Double_t weight;

    tree_sig->SetBranchAddress("jtpt", &jtpt);
    tree_sig->SetBranchAddress("jtptCh", &jtptCh);
    tree_sig->SetBranchAddress("logrg", &logrg);
    tree_sig->SetBranchAddress("zg", &zg);
    tree_sig->SetBranchAddress("mb", &mb);
    tree_sig->SetBranchAddress("bpt", &bpt);
    tree_sig->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_sig->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_sig->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_sig->SetBranchAddress("zg_gen", &zg_gen);
    tree_sig->SetBranchAddress("mb_gen", &mb_gen);
    tree_sig->SetBranchAddress("bpt_gen", &bpt_gen);
    tree_sig->SetBranchAddress("weight", &weight);

    // NOTE : Need to use the same binning as the template fit 

    // jet pt bins : no underflow or overflow
    const Int_t z1binsVectorSize = 4;
    Int_t z1bins = z1binsVectorSize - 1;
    Double_t z1binsVector[z1binsVectorSize] = {80., 
                                               100., 
                                               120.,
                                               140.};
    Double_t z1min = z1binsVector[0];
    Double_t z1max = z1binsVector[z1bins];

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1binsVectorSize = 8;
    Int_t x1bins = x1binsVectorSize - 1;
    Double_t x1binsVector[x1binsVectorSize] = {-1.2, 
                                               0., 
                                               0.3,
                                               0.6,
                                               0.9,
                                               1.2,
                                               1.6,
                                               2.1};
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

    // zpt bins (bpt/jtptCh)
    const Int_t x2binsVectorSize = 8;
    Int_t x2bins = x2binsVectorSize - 1;
    Double_t x2binsVector[x2binsVectorSize] = {0., 
                                               0.4,
                                               0.5,
                                               0.6,
                                               0.7,
                                               0.8,
                                               0.9,
                                               1.};
    Double_t x2min = x2binsVector[0];
    Double_t x2max = x2binsVector[x2bins];
    
    TH2D *h_sig_testing_reco_rgpt = new TH2D("h_sig_testing_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_true_rgpt = new TH2D("h_sig_testing_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_fake_rgpt = new TH2D("h_sig_testing_fake_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_true_rgpt = new TH2D("h_sig_training_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_reco_rgpt = new TH2D("h_sig_training_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_fake_rgpt = new TH2D("h_sig_training_fake_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_sig_testing_reco_zgpt = new TH2D("h_sig_testing_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_true_zgpt = new TH2D("h_sig_testing_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_fake_zgpt = new TH2D("h_sig_testing_fake_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_true_zgpt = new TH2D("h_sig_training_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_reco_zgpt = new TH2D("h_sig_training_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_fake_zgpt = new TH2D("h_sig_training_fake_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    TH2D *h_sig_testing_reco_zptpt = new TH2D("h_sig_testing_reco_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_true_zptpt = new TH2D("h_sig_testing_true_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_testing_fake_zptpt = new TH2D("h_sig_testing_fake_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_true_zptpt = new TH2D("h_sig_training_true_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_reco_zptpt = new TH2D("h_sig_training_reco_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_fake_zptpt = new TH2D("h_sig_training_fake_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);

    RooUnfoldResponse response_rgpt(h_sig_training_reco_rgpt, h_sig_training_true_rgpt, "response_rgpt", "response for 2d: rg and jet pt"); 
    RooUnfoldResponse response_zgpt(h_sig_training_reco_zgpt, h_sig_training_true_zgpt, "response_zgpt", "response for 2d: zg and jet pt"); 
    RooUnfoldResponse response_zptpt(h_sig_training_reco_zptpt, h_sig_training_true_zptpt, "response_zptpt", "response for 2d: zpt and jet pt"); 

    TH2D *h_sig_training_purity_nominator_rgpt = new TH2D("h_sig_training_purity_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_efficiency_nominator_rgpt = new TH2D("h_sig_training_efficiency_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_sig_training_purity_nominator_zgpt = new TH2D("h_sig_training_purity_nominator_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_efficiency_nominator_zgpt = new TH2D("h_sig_training_efficiency_nominator_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    TH2D *h_sig_training_purity_nominator_zptpt = new TH2D("h_sig_training_purity_nominator_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_sig_training_efficiency_nominator_zptpt = new TH2D("h_sig_training_efficiency_nominator_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);

    for (auto h : {h_sig_testing_reco_rgpt, h_sig_testing_true_rgpt, h_sig_testing_fake_rgpt,
                   h_sig_training_true_rgpt, h_sig_training_reco_rgpt, h_sig_training_fake_rgpt,
                   h_sig_training_purity_nominator_rgpt, h_sig_training_efficiency_nominator_rgpt,
                   h_sig_testing_reco_zgpt, h_sig_testing_true_zgpt, h_sig_testing_fake_zgpt,
                   h_sig_training_true_zgpt, h_sig_training_reco_zgpt, h_sig_training_fake_zgpt,
                   h_sig_training_purity_nominator_zgpt, h_sig_training_efficiency_nominator_zgpt,
                   h_sig_testing_reco_zptpt, h_sig_testing_true_zptpt, h_sig_testing_fake_zptpt,
                   h_sig_training_true_zptpt, h_sig_training_reco_zptpt, h_sig_training_fake_zptpt,
                   h_sig_training_purity_nominator_zptpt, h_sig_training_efficiency_nominator_zptpt}) {
                    h->Sumw2();
                   }

    Long64_t nentries = tree_sig->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree_sig->GetEntry(ient);
        if (weight > 0.01) continue;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Move zg=0.5 to last bin
        if (std::abs(zg - 0.5) < 1e-4) zg = 0.499;
        if (std::abs(zg_gen - 0.5) < 1e-4) zg_gen = 0.499;

        // Move zpt=1 to last bin
        double zpt = -1.;
        if (jtptCh > 0) zpt = bpt / jtptCh;
        if (std::abs(zpt-1)<1e-3) zpt = 0.999;

        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999;


        bool has_gen_match = (jtpt_gen > 0);
        bool reco_pass_cuts_rg = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
        bool true_pass_cuts_rg = (jtpt_gen < z1max && jtpt_gen >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

        bool reco_pass_cuts_zg = (jtpt < z1max && jtpt >= z1min && zg < x2max && zg >= x2min);
        bool true_pass_cuts_zg = (jtpt_gen < z1max && jtpt_gen >= z1min && zg_gen < x2max && zg_gen >= x2min);

        bool reco_pass_cuts_zpt = (jtpt < z1max && jtpt >= z1min && zpt < x2max && zpt >= x2min);
        bool true_pass_cuts_zpt = (jtpt_gen < z1max && jtpt_gen >= z1min && zpt_gen < x2max && zpt_gen >= x2min);

        if (ient > (nentries / 2)) {
            // training
            if (has_gen_match) {                    
                if (true_pass_cuts_rg) {
                    h_sig_training_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                    if (reco_pass_cuts_rg) {
                        h_sig_training_purity_nominator_rgpt->Fill(logrg, jtpt, weight);
                        h_sig_training_efficiency_nominator_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                        response_rgpt.Fill(logrg, jtpt, logrg_gen, jtpt_gen, weight);
                    }
                }

                if (true_pass_cuts_zg) {
                    h_sig_training_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
                    if (reco_pass_cuts_zg) {
                        h_sig_training_purity_nominator_zgpt->Fill(zg, jtpt, weight);
                        h_sig_training_efficiency_nominator_zgpt->Fill(zg_gen, jtpt_gen, weight);
                        response_zgpt.Fill(zg, jtpt, zg_gen, jtpt_gen, weight);
                    }
                }

                if (true_pass_cuts_zpt) {
                    h_sig_training_true_zptpt->Fill(zpt_gen, jtpt_gen, weight);
                    if (reco_pass_cuts_zpt) {
                        h_sig_training_purity_nominator_zptpt->Fill(zpt, jtpt, weight);
                        h_sig_training_efficiency_nominator_zptpt->Fill(zpt_gen, jtpt_gen, weight);
                        response_zptpt.Fill(zpt, jtpt, zpt_gen, jtpt_gen, weight);
                    }
                }

                if (reco_pass_cuts_rg) {
                    h_sig_training_reco_rgpt->Fill(logrg, jtpt, weight); // does not include fakes
                }      

                if (reco_pass_cuts_zg) {
                    h_sig_training_reco_zgpt->Fill(zg, jtpt, weight); // does not include fakes
                }     

                if (reco_pass_cuts_zpt) {
                    h_sig_training_reco_zptpt->Fill(zpt, jtpt, weight); // does not include fakes
                }                        
            } else {
                // Fakes = no gen match
                if (reco_pass_cuts_rg) {
                    h_sig_training_fake_rgpt->Fill(logrg, jtpt, weight); 
                }
                if (reco_pass_cuts_zg) {
                    h_sig_training_fake_zgpt->Fill(zg, jtpt, weight); 
                }
                if (reco_pass_cuts_zpt) {
                    h_sig_training_fake_zptpt->Fill(zpt, jtpt, weight); 
                }
            }   
        } else {
            // testing
            if (reco_pass_cuts_rg) {
                h_sig_testing_reco_rgpt->Fill(logrg, jtpt, weight); // includes fakes
            }
            if (true_pass_cuts_rg) {
                h_sig_testing_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
            }
            if (reco_pass_cuts_zg) {
                h_sig_testing_reco_zgpt->Fill(zg, jtpt, weight); // includes fakes
            }
            if (true_pass_cuts_zg) {
                h_sig_testing_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
            }
            if (reco_pass_cuts_zpt) {
                h_sig_testing_reco_zptpt->Fill(zpt, jtpt, weight); // includes fakes
            }
            if (true_pass_cuts_zpt) {
                h_sig_testing_true_zptpt->Fill(zpt_gen, jtpt_gen, weight);
            }
        }
        
    } // tree entry loop

    // --------- Make testing histos for the bkg as well

    TH2D *h_bkg_bb_testing_reco_rgpt = new TH2D("h_bkg_bb_testing_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_bb_testing_true_rgpt = new TH2D("h_bkg_bb_testing_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_bkg_bb_testing_reco_zgpt = new TH2D("h_bkg_bb_testing_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_bb_testing_true_zgpt = new TH2D("h_bkg_bb_testing_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    TH2D *h_bkg_bb_testing_reco_zptpt = new TH2D("h_bkg_bb_testing_reco_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_bb_testing_true_zptpt = new TH2D("h_bkg_bb_testing_true_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    
    TTree *tree_bkg_bb = (TTree *) fin->Get("tree_bkg_bb");
    tree_bkg_bb->SetBranchAddress("jtpt", &jtpt);
    tree_bkg_bb->SetBranchAddress("jtptCh", &jtptCh);
    tree_bkg_bb->SetBranchAddress("logrg", &logrg);
    tree_bkg_bb->SetBranchAddress("zg", &zg);
    tree_bkg_bb->SetBranchAddress("mb", &mb);
    tree_bkg_bb->SetBranchAddress("bpt", &bpt);
    tree_bkg_bb->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_bkg_bb->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_bkg_bb->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_bkg_bb->SetBranchAddress("zg_gen", &zg_gen);
    tree_bkg_bb->SetBranchAddress("mb_gen", &mb_gen);
    tree_bkg_bb->SetBranchAddress("bpt_gen", &bpt_gen);
    tree_bkg_bb->SetBranchAddress("weight", &weight);

    nentries = tree_bkg_bb->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree_bkg_bb->GetEntry(ient);
        if (weight > 0.01) continue;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Move zg=0.5 to last bin
        if (std::abs(zg - 0.5) < 1e-4) zg = 0.499;
        if (std::abs(zg_gen - 0.5) < 1e-4) zg_gen = 0.499;

        // Move zpt=1 to last bin
        double zpt = -1.;
        if (jtptCh > 0) zpt = bpt / jtptCh;
        if (std::abs(zpt-1)<1e-3) zpt = 0.999;

        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999;

        bool has_gen_match = (jtpt_gen > 0);
        bool reco_pass_cuts_rg = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
        bool true_pass_cuts_rg = (jtpt_gen < z1max && jtpt_gen >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

        bool reco_pass_cuts_zg = (jtpt < z1max && jtpt >= z1min && zg < x3max && zg >= x3min);
        bool true_pass_cuts_zg = (jtpt_gen < z1max && jtpt_gen >= z1min && zg_gen < x3max && zg_gen >= x3min);

        bool reco_pass_cuts_zpt = (jtpt < z1max && jtpt >= z1min && zpt < x2max && zpt >= x2min);
        bool true_pass_cuts_zpt = (jtpt_gen < z1max && jtpt_gen >= z1min && zpt_gen < x2max && zpt_gen >= x2min);

        if (ient > (nentries / 2)) {
            // reserved for training sample - do nothing           
        } else {
            // testing
            if (reco_pass_cuts_rg)
                h_bkg_bb_testing_reco_rgpt->Fill(logrg, jtpt, weight); // includes fakes
            if (true_pass_cuts_rg)
                h_bkg_bb_testing_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
            if (reco_pass_cuts_zg)
                h_bkg_bb_testing_reco_zgpt->Fill(zg, jtpt, weight); // includes fakes
            if (true_pass_cuts_zg)
                h_bkg_bb_testing_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
            if (reco_pass_cuts_zpt)
                h_bkg_bb_testing_reco_zptpt->Fill(zpt, jtpt, weight); // includes fakes
            if (true_pass_cuts_zpt)
                h_bkg_bb_testing_true_zptpt->Fill(zpt_gen, jtpt_gen, weight);
        }
    } // bkg bb tree entry loop

    TH2D *h_bkg_rest_testing_reco_rgpt = new TH2D("h_bkg_rest_testing_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_rest_testing_true_rgpt = new TH2D("h_bkg_rest_testing_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_bkg_rest_testing_reco_zgpt = new TH2D("h_bkg_rest_testing_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_rest_testing_true_zgpt = new TH2D("h_bkg_rest_testing_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    TH2D *h_bkg_rest_testing_reco_zptpt = new TH2D("h_bkg_rest_testing_reco_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    TH2D *h_bkg_rest_testing_true_zptpt = new TH2D("h_bkg_rest_testing_true_zptpt", "x=zpt, y=jtpt", x2bins, x2binsVector, z1bins, z1binsVector);
    
    TTree *tree_bkg_rest = (TTree *) fin->Get("tree_bkg_rest");
    tree_bkg_rest->SetBranchAddress("jtpt", &jtpt);
    tree_bkg_rest->SetBranchAddress("jtptCh", &jtptCh);
    tree_bkg_rest->SetBranchAddress("logrg", &logrg);
    tree_bkg_rest->SetBranchAddress("zg", &zg);
    tree_bkg_rest->SetBranchAddress("mb", &mb);
    tree_bkg_rest->SetBranchAddress("bpt", &bpt);
    tree_bkg_rest->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_bkg_rest->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_bkg_rest->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_bkg_rest->SetBranchAddress("zg_gen", &zg_gen);
    tree_bkg_rest->SetBranchAddress("mb_gen", &mb_gen);
    tree_bkg_rest->SetBranchAddress("bpt_gen", &bpt_gen);
    tree_bkg_rest->SetBranchAddress("weight", &weight);

    nentries = tree_bkg_rest->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree_bkg_rest->GetEntry(ient);
        if (weight > 0.01) continue;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Move zg=0.5 to last bin
        if (std::abs(zg - 0.5) < 1e-4) zg = 0.499;
        if (std::abs(zg_gen - 0.5) < 1e-4) zg_gen = 0.499;

        // Move zpt=1 to last bin
        double zpt = -1.;
        if (jtptCh > 0) zpt = bpt / jtptCh;
        if (std::abs(zpt-1)<1e-3) zpt = 0.999;

        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999;

        bool has_gen_match = (jtpt_gen > 0);

        bool reco_pass_cuts_rg = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
        bool true_pass_cuts_rg = (jtpt_gen < z1max && jtpt_gen >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

        bool reco_pass_cuts_zg = (jtpt < z1max && jtpt >= z1min && zg < x3max && zg >= x3min);
        bool true_pass_cuts_zg = (jtpt_gen < z1max && jtpt_gen >= z1min && zg_gen < x3max && zg_gen >= x3min);

        bool reco_pass_cuts_zpt = (jtpt < z1max && jtpt >= z1min && zpt < x2max && zpt >= x2min);
        bool true_pass_cuts_zpt = (jtpt_gen < z1max && jtpt_gen >= z1min && zpt_gen < x2max && zpt_gen >= x2min);

        if (ient > (nentries / 2)) {
            // reserved for training sample - do nothing           
        } else {
            // testing
            if (reco_pass_cuts_rg)
                h_bkg_rest_testing_reco_rgpt->Fill(logrg, jtpt, weight); // includes fakes
            if (true_pass_cuts_rg)
                h_bkg_rest_testing_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
            if (reco_pass_cuts_zg)
                h_bkg_rest_testing_reco_zgpt->Fill(zg, jtpt, weight); // includes fakes
            if (true_pass_cuts_zg)
                h_bkg_rest_testing_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
            if (reco_pass_cuts_zpt)
                h_bkg_rest_testing_reco_zptpt->Fill(zpt, jtpt, weight); // includes fakes
            if (true_pass_cuts_zpt)
                h_bkg_rest_testing_true_zptpt->Fill(zpt_gen, jtpt_gen, weight);
        }
    } // bkg rests tree entry loop

    TH2D *h_sig_training_purity_rgpt = (TH2D *) h_sig_training_purity_nominator_rgpt->Clone("h_sig_training_purity_rgpt");
    h_sig_training_purity_rgpt->Divide(h_sig_training_purity_rgpt, h_sig_training_reco_rgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_sig_training_efficiency_rgpt = (TH2D *) h_sig_training_efficiency_nominator_rgpt->Clone("h_sig_training_efficiency_rgpt");
    h_sig_training_efficiency_rgpt->Divide(h_sig_training_efficiency_rgpt, h_sig_training_true_rgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_sig_training_purity_zgpt = (TH2D *) h_sig_training_purity_nominator_zgpt->Clone("h_sig_training_purity_zgpt");
    h_sig_training_purity_zgpt->Divide(h_sig_training_purity_zgpt, h_sig_training_reco_zgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_sig_training_efficiency_zgpt = (TH2D *) h_sig_training_efficiency_nominator_zgpt->Clone("h_sig_training_efficiency_zgpt");
    h_sig_training_efficiency_zgpt->Divide(h_sig_training_efficiency_zgpt, h_sig_training_true_zgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_sig_training_purity_zptpt = (TH2D *) h_sig_training_purity_nominator_zptpt->Clone("h_sig_training_purity_zptpt");
    h_sig_training_purity_zptpt->Divide(h_sig_training_purity_zptpt, h_sig_training_reco_zptpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_sig_training_efficiency_zptpt = (TH2D *) h_sig_training_efficiency_nominator_zptpt->Clone("h_sig_training_efficiency_zptpt");
    h_sig_training_efficiency_zptpt->Divide(h_sig_training_efficiency_zptpt, h_sig_training_true_zptpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    // Create file
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_sig_testing_reco_rgpt, h_sig_testing_true_rgpt, h_sig_testing_fake_rgpt,
                   h_sig_training_true_rgpt, h_sig_training_reco_rgpt, h_sig_training_fake_rgpt,
                   h_sig_training_purity_nominator_rgpt, h_sig_training_efficiency_nominator_rgpt,
                   h_bkg_bb_testing_reco_rgpt, h_bkg_bb_testing_true_rgpt,
                   h_bkg_rest_testing_reco_rgpt, h_bkg_rest_testing_true_rgpt,
                   h_sig_training_purity_rgpt, h_sig_training_efficiency_rgpt,
                   h_sig_testing_reco_zgpt, h_sig_testing_true_zgpt, h_sig_testing_fake_zgpt,
                   h_sig_training_true_zgpt, h_sig_training_reco_zgpt, h_sig_training_fake_zgpt,
                   h_sig_training_purity_nominator_zgpt, h_sig_training_efficiency_nominator_zgpt,
                   h_bkg_bb_testing_reco_zgpt, h_bkg_bb_testing_true_zgpt,
                   h_bkg_rest_testing_reco_zgpt, h_bkg_rest_testing_true_zgpt,
                   h_sig_training_purity_zgpt, h_sig_training_efficiency_zgpt,
                   h_sig_testing_reco_zptpt, h_sig_testing_true_zptpt, h_sig_testing_fake_zptpt,
                   h_sig_training_true_zptpt, h_sig_training_reco_zptpt, h_sig_training_fake_zptpt,
                   h_sig_training_purity_nominator_zptpt, h_sig_training_efficiency_nominator_zptpt,
                   h_bkg_bb_testing_reco_zptpt, h_bkg_bb_testing_true_zptpt,
                   h_bkg_rest_testing_reco_zptpt, h_bkg_rest_testing_true_zptpt,
                   h_sig_training_purity_zptpt, h_sig_training_efficiency_zptpt
                   }) {
                    h->Write();
                   }
    response_rgpt.Write();
    response_zgpt.Write();
    response_zptpt.Write();
    
    fout->Close();
    delete fout;
}