void create_response_fast()
{
    TString sample = "bjet";
    TString label = "aggrTMVA_withHLT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree_template_fit.root";   
    TString fout_name = "./histos/" + sample + "_" + label + "_response.root";

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TTree *tree_sig = (TTree *) fin->Get("tree_sig");

    Double_t jtpt;
    Double_t logrg;
    Double_t mb;
    Double_t jtpt_gen;
    Double_t logrg_gen;
    Double_t mb_gen;
    Double_t weight;

    tree_sig->SetBranchAddress("jtpt", &jtpt);
    tree_sig->SetBranchAddress("logrg", &logrg);
    tree_sig->SetBranchAddress("mb", &mb);
    tree_sig->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_sig->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_sig->SetBranchAddress("mb_gen", &mb_gen);
    tree_sig->SetBranchAddress("weight", &weight);

    // jet pt bins : no underflow or overflow
    const Int_t z1bins = 3;
    Double_t z1min = 60.;
    Double_t z1max = 120.; 

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1bins = 5;
    Double_t x1min = -0.4; // if untagged or kT < cut : ln(R/Rg) = -0.2
    Double_t x1max = 1.6; 
    
    TH2D *h_sig_testing_reco_rgpt = new TH2D("h_sig_testing_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_testing_true_rgpt = new TH2D("h_sig_testing_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_testing_fake_rgpt = new TH2D("h_sig_testing_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_training_true_rgpt = new TH2D("h_sig_training_true_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_training_reco_rgpt = new TH2D("h_sig_training_reco_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_training_fake_rgpt = new TH2D("h_sig_training_fake_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    RooUnfoldResponse response_rgpt(h_sig_training_reco_rgpt, h_sig_training_true_rgpt, "response_rgpt", "response for 2d: rg and jet pt"); 

    TH2D *h_sig_training_purity_nominator_rgpt = new TH2D("h_sig_training_purity_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);
    TH2D *h_sig_training_efficiency_nominator_rgpt = new TH2D("h_sig_training_efficiency_nominator_rgpt", "x=rg, y=jtpt", x1bins, x1min, x1max, z1bins, z1min, z1max);

    for (auto h : {h_sig_testing_reco_rgpt, h_sig_testing_true_rgpt, h_sig_testing_fake_rgpt,
                   h_sig_training_true_rgpt, h_sig_training_reco_rgpt, h_sig_training_fake_rgpt,
                   h_sig_training_purity_nominator_rgpt, h_sig_training_efficiency_nominator_rgpt}) {
                    h->Sumw2();
                   }

    Long64_t nentries = tree_sig->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        tree_sig->GetEntry(ient);

        bool has_gen_match = (jtpt_gen > 0);
        bool reco_pass_cuts = (jtpt < z1max && jtpt >= z1min && logrg < x1max && logrg >= x1min);
        bool true_pass_cuts = (jtpt_gen < z1max && jtpt_gen >= z1min && logrg_gen < x1max && logrg_gen >= x1min);

        if (ient > (nentries / 2)) {
            if (has_gen_match) {                    
                if (true_pass_cuts) {
                    h_sig_training_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                    if (reco_pass_cuts) {
                        h_sig_training_purity_nominator_rgpt->Fill(logrg, jtpt, weight);
                        h_sig_training_efficiency_nominator_rgpt->Fill(logrg_gen, jtpt_gen, weight);
                        response_rgpt.Fill(logrg, jtpt, logrg_gen, jtpt_gen, weight);
                    }
                }

                if (reco_pass_cuts) {
                    h_sig_training_reco_rgpt->Fill(logrg, jtpt, weight); // does not include fakes
                }                        
            } else {
                // Fakes = no gen match
                if (reco_pass_cuts)
                    h_sig_training_fake_rgpt->Fill(logrg, jtpt, weight); 
            }   
        } else {
            if (reco_pass_cuts)
                h_sig_testing_reco_rgpt->Fill(logrg, jtpt, weight); // includes fakes
            if (true_pass_cuts)
                h_sig_testing_true_rgpt->Fill(logrg_gen, jtpt_gen, weight);
        }
        
    } // tree entry loop
    
    // Create file
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_sig_testing_reco_rgpt, h_sig_testing_true_rgpt, h_sig_testing_fake_rgpt,
                   h_sig_training_true_rgpt, h_sig_training_reco_rgpt, h_sig_training_fake_rgpt,
                   h_sig_training_purity_nominator_rgpt, h_sig_training_efficiency_nominator_rgpt}) {
                    h->Write();
                   }
    response_rgpt.Write();
    
    fout->Close();
    delete fout;
}