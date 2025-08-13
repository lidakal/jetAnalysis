void pythia_var_weights(TString observable="rg", bool inclusive=true)
{
    TString fin_nom_inclusive_name = "histos/dijet_aggrTMVA_fixedMassBug_gen_histograms.root";
    std::cout << "fin nom inclusive: " << fin_nom_inclusive_name << std::endl;
    TFile *fin_nom_inclusive = new TFile(fin_nom_inclusive_name);
    TH2D *h_nom_inclusive_b = (TH2D *) fin_nom_inclusive->Get("h_"+observable+"_b")->Clone("h_nom_inclusive_b");
    TH2D *h_nom_inclusive_all = (TH2D *) fin_nom_inclusive->Get("h_"+observable+"_all")->Clone("h_nom_inclusive_all");

    TString fin_nom_bjet_name = "histos/bjet_aggrTMVA_fixedMassBug_gen_histograms.root";
    std::cout << "fin nom bjet: " << fin_nom_bjet_name << std::endl;
    TFile *fin_nom_bjet = new TFile(fin_nom_bjet_name);
    TH2D *h_nom_bjet_b = (TH2D *) fin_nom_bjet->Get("h_"+observable+"_b")->Clone("h_nom_bjet_b");

    TH2D *h_nom; 
    if (inclusive) {
        h_nom = (TH2D *) h_nom_inclusive_all->Clone("h_nom");
    } else {
        h_nom = (TH2D *) h_nom_inclusive_b->Clone("h_nom"); 
        h_nom->Add(h_nom_bjet_b);
    }
    h_nom->Scale(1/h_nom->Integral(), "width");

    std::vector<TString> variations = {
        "FSRup", "FSRdown",
        "ISRup", "ISRdown",
        "Bothup", "Bothdown",
    };

    for (TString var : variations) {
        TString fin_var_inclusive_name = "histos/inclusive_"+var+"_gen_histograms.root";
        std::cout << "\nfin " << var << " inclusive: " << fin_var_inclusive_name << std::endl;
        TFile *fin_var_inclusive = new TFile(fin_var_inclusive_name);
        TH2D *h_var_inclusive_b = (TH2D *) fin_var_inclusive->Get("h_"+observable+"_b")->Clone("h_var_inclusive_b");
        TH2D *h_var_inclusive_all = (TH2D *) fin_var_inclusive->Get("h_"+observable+"_all")->Clone("h_var_inclusive_all");
      
        TString fin_var_bjet_name = "histos/bjet_"+var+"_gen_histograms.root";
        std::cout << "fin " << var << " bjet: " << fin_var_bjet_name << std::endl;
        TFile *fin_var_bjet = new TFile(fin_var_bjet_name);
        TH2D *h_var_bjet_b = (TH2D *) fin_var_bjet->Get("h_"+observable+"_b")->Clone("h_var_bjet_b");

        TH2D *h_var; 
        if (inclusive) {
            h_var = (TH2D *) h_var_inclusive_all->Clone("h_var");
        } else {
            h_var = (TH2D *) h_var_inclusive_b->Clone("h_var");
            h_var->Add(h_var_bjet_b);
        }
        h_var->Scale(1/h_var->Integral(), "width");

        TH2D *h_weights = (TH2D *) h_var->Clone("h_weights");
        h_weights->Divide(h_nom);

        TString fout_name = "histos/"+TString(inclusive ? "inclusive" : "bjet")+"_"+var+"_weights_"+observable+".root";
        std::cout << "fout: " << fout_name << std::endl;
        TFile *fout = new TFile(fout_name, "recreate");
        h_weights->Write();
        fout->Close();
        delete fout;
    }

    gApplication->Terminate();
}