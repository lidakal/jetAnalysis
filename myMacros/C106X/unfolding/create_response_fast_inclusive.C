void create_response_fast_inclusive()
{
    TString sample = "dijet";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";  

    TString split_option = "full";

    TString jer_opt="nom";
    TString jec_opt="nom";
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;  
    TString fout_name = "./histos/" + sample + "_" + label + "_response_" + split_option +  suffix + ".root";

    TFile *fin = new TFile(fin_name);
    TTree *tree_all = (TTree *) fin->Get("tree_all"); // b+bb+cl for inclusive

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
    Double_t jer_sf_nom;
    Double_t jer_sf_up;
    Double_t jer_sf_down;
    Double_t jec_unc;

    tree_all->SetBranchAddress("jtpt", &jtpt);
    tree_all->SetBranchAddress("jtptCh", &jtptCh);
    tree_all->SetBranchAddress("logrg", &logrg);
    tree_all->SetBranchAddress("zg", &zg);
    tree_all->SetBranchAddress("mb", &mb);
    tree_all->SetBranchAddress("bpt", &bpt);
    tree_all->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_all->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_all->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_all->SetBranchAddress("zg_gen", &zg_gen);
    tree_all->SetBranchAddress("mb_gen", &mb_gen);
    tree_all->SetBranchAddress("bpt_gen", &bpt_gen);
    tree_all->SetBranchAddress("weight", &weight);
    tree_all->SetBranchAddress("jer_sf_nom", &jer_sf_nom);
    tree_all->SetBranchAddress("jer_sf_up", &jer_sf_up);
    tree_all->SetBranchAddress("jer_sf_down", &jer_sf_down);
    tree_all->SetBranchAddress("jec_unc", &jec_unc);

    // NOTE : Need to use the same binning as b jets

    // jet pt bins : no underflow or overflow
    const Int_t z1binsVectorSize = 4;
    Int_t z1bins = z1binsVectorSize - 1;
    Double_t z1binsVector[z1binsVectorSize] = {80., 
                                               100., 
                                               120.,
                                               140.};
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
                                               2.5};
    Double_t x1min = x1binsVector[0];
    Double_t x1max = x1binsVector[x1bins];


    // zg bins : 1st bin untagged (unphysical)
    const Int_t x3binsVectorSize = 8;
    Int_t x3bins = x3binsVectorSize - 1;
    Double_t x3binsVector[x3binsVectorSize] = {-0.1, 
                                               0.1, 
                                               0.15,
                                               0.20,
                                               0.25,
                                               0.3,
                                               0.4,
                                               0.5};
    Double_t x3min = x3binsVector[0];
    Double_t x3max = x3binsVector[x3bins];

    TH2D *h_testing_reco_rgpt = new TH2D("h_testing_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_testing_true_rgpt = new TH2D("h_testing_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_testing_fake_rgpt = new TH2D("h_testing_fake_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_training_true_rgpt = new TH2D("h_training_true_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_training_reco_rgpt = new TH2D("h_training_reco_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_training_fake_rgpt = new TH2D("h_training_fake_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_testing_reco_zgpt = new TH2D("h_testing_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_testing_true_zgpt = new TH2D("h_testing_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_testing_fake_zgpt = new TH2D("h_testing_fake_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_training_true_zgpt = new TH2D("h_training_true_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_training_reco_zgpt = new TH2D("h_training_reco_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_training_fake_zgpt = new TH2D("h_training_fake_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    RooUnfoldResponse response_rgpt(h_training_reco_rgpt, h_training_true_rgpt, "response_rgpt", "response for 2d: rg and jet pt"); 
    RooUnfoldResponse response_zgpt(h_training_reco_zgpt, h_training_true_zgpt, "response_zgpt", "response for 2d: zg and jet pt"); 

    TH2D *h_training_purity_nominator_rgpt = new TH2D("h_training_purity_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_training_efficiency_nominator_rgpt = new TH2D("h_training_efficiency_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1binsVector, z1bins, z1binsVector);

    TH2D *h_training_purity_nominator_zgpt = new TH2D("h_training_purity_nominator_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_training_efficiency_nominator_zgpt = new TH2D("h_training_efficiency_nominator_zgpt", "x=zg, y=jtpt", x3bins, x3binsVector, z1bins, z1binsVector);

    for (auto h : {h_testing_reco_rgpt, h_testing_true_rgpt, h_testing_fake_rgpt,
                   h_training_true_rgpt, h_training_reco_rgpt, h_training_fake_rgpt,
                   h_training_purity_nominator_rgpt, h_training_efficiency_nominator_rgpt,
                   h_testing_reco_zgpt, h_testing_true_zgpt, h_testing_fake_zgpt,
                   h_training_true_zgpt, h_training_reco_zgpt, h_training_fake_zgpt,
                   h_training_purity_nominator_zgpt, h_training_efficiency_nominator_zgpt
                   }) {
                    h->Sumw2();
                   }

    Long64_t nentries = tree_all->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree_all->GetEntry(ient);

        if (jer_opt=="nom") jtpt = jtpt * jer_sf_nom;
        else if (jer_opt=="up") jtpt = jtpt * jer_sf_up;
        else if (jer_opt=="down") jtpt = jtpt * jer_sf_down;

        double jec_fact = 0; // nominal
        if (jec_opt=="up") jec_fact = 1;
        if (jec_opt=="down") jec_fact = -1;
        double newCorrection = 1 + (jec_fact * jec_unc);
        jtpt = jtpt * newCorrection;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        if (std::abs(logrg_gen - (-0.2)) > 1e-3 && logrg_gen < 0.) logrg_gen = -999;
        if (logrg>=2.5) logrg = 2.499; // overflow
        if (logrg_gen>=2.5) logrg_gen = 2.499; // overflow

        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Move zg=0.5 to last bin
        if (std::abs(zg - 0.5) < 1e-4) zg = 0.499;
        if (std::abs(zg_gen - 0.5) < 1e-4) zg_gen = 0.499;

        bool has_gen_match = (jtpt_gen > 0);
        bool reco_pass_cuts_rg = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
        bool true_pass_cuts_rg = (jtpt_gen < z1max && jtpt_gen >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

        bool reco_pass_cuts_zg = (jtpt < z1max && jtpt >= z1min && zg < x3max && zg >= x3min);
        bool true_pass_cuts_zg = (jtpt_gen < z1max && jtpt_gen >= z1min && zg_gen < x3max && zg_gen >= x3min);

        if (split_option=="full" || (split_option=="split" && ient > (nentries / 2))) {
            // training 
            if (has_gen_match) {                    
                if (true_pass_cuts_rg) {
                    h_training_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                    if (reco_pass_cuts_rg) {
                        h_training_purity_nominator_rgpt->Fill(logrg, jtpt, weight);
                        h_training_efficiency_nominator_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                        response_rgpt.Fill(logrg, jtpt, logrg_gen, jtpt_gen, weight);
                    }
                }

                if (true_pass_cuts_zg) {
                    h_training_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
                    if (reco_pass_cuts_zg) {
                        h_training_purity_nominator_zgpt->Fill(zg, jtpt, weight);
                        h_training_efficiency_nominator_zgpt->Fill(zg_gen, jtpt_gen, weight);
                        response_zgpt.Fill(zg, jtpt, zg_gen, jtpt_gen, weight);
                    }
                }

                if (reco_pass_cuts_rg) {
                    h_training_reco_rgpt->Fill(logrg, jtpt, weight); // does not include fakes
                } 

                if (reco_pass_cuts_zg) {
                    h_training_reco_zgpt->Fill(zg, jtpt, weight); // does not include fakes
                }                        
            } else {
                // Fakes = no gen match
                if (reco_pass_cuts_rg)
                    h_training_fake_rgpt->Fill(logrg, jtpt, weight); 

                if (reco_pass_cuts_zg)
                    h_training_fake_zgpt->Fill(zg, jtpt, weight); 
            }   
        } else if (split_option=="split") {
            // testing
            if (reco_pass_cuts_rg)
                h_testing_reco_rgpt->Fill(logrg, jtpt, weight); // includes fakes
            if (true_pass_cuts_rg)
                h_testing_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);

            if (reco_pass_cuts_zg)
                h_testing_reco_zgpt->Fill(zg, jtpt, weight); // includes fakes
            if (true_pass_cuts_zg)
                h_testing_true_zgpt->Fill(zg_gen, jtpt_gen, weight);
        } 
    } // tree entry loop

    TH2D *h_training_purity_rgpt = (TH2D *) h_training_purity_nominator_rgpt->Clone("h_training_purity_rgpt");
    h_training_purity_rgpt->Divide(h_training_purity_rgpt, h_training_reco_rgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_training_efficiency_rgpt = (TH2D *) h_training_efficiency_nominator_rgpt->Clone("h_training_efficiency_rgpt");
    h_training_efficiency_rgpt->Divide(h_training_efficiency_rgpt, h_training_true_rgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_training_purity_zgpt = (TH2D *) h_training_purity_nominator_zgpt->Clone("h_training_purity_zgpt");
    h_training_purity_zgpt->Divide(h_training_purity_zgpt, h_training_reco_zgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    TH2D *h_training_efficiency_zgpt = (TH2D *) h_training_efficiency_nominator_zgpt->Clone("h_training_efficiency_zgpt");
    h_training_efficiency_zgpt->Divide(h_training_efficiency_zgpt, h_training_true_zgpt, 1., 1., "b"); // correct division for when nominator is included in denominator

    // Create file
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_testing_reco_rgpt, h_testing_true_rgpt, h_testing_fake_rgpt,
                   h_training_true_rgpt, h_training_reco_rgpt, h_training_fake_rgpt,
                   h_training_purity_nominator_rgpt, h_training_efficiency_nominator_rgpt,
                   h_training_purity_rgpt, h_training_efficiency_rgpt,
                   h_testing_reco_zgpt, h_testing_true_zgpt, h_testing_fake_zgpt,
                   h_training_true_zgpt, h_training_reco_zgpt, h_training_fake_zgpt,
                   h_training_purity_nominator_zgpt, h_training_efficiency_nominator_zgpt,
                   h_training_purity_zgpt, h_training_efficiency_zgpt
                   }) {
                    h->Write();
                   }
    response_rgpt.Write();
    response_zgpt.Write();
    
    fout->Close();
    delete fout;
}