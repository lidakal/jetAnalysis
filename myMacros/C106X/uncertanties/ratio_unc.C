void ratio_unc(TString observable="rg")
{
    TFile *fin_jer_XXT = new TFile("histos/"+observable+"_jer_unc_XXT.root");
    TH1D *h_jer_XXT = (TH1D *) fin_jer_XXT->Get("h_unc_rel_2")->Clone("h_jer_XXT");

    TFile *fin_jec_XXT = new TFile("histos/"+observable+"_jec_unc_src_XXT.root");
    TH1D *h_jec_noflav_XXT = (TH1D *) fin_jec_XXT->Get("h_total_unc_rel_sym_noflav_2")->Clone("h_jec_noflav_XXT");
    TH1D *h_jec_flav_XXT = (TH1D *) fin_jec_XXT->Get("h_flav_unc_rel_sym_2")->Clone("h_jec_flav_XXT");

    TFile *fin_trk_inef_XXT = new TFile("histos/"+observable+"_trk_inef_XXT.root");
    TH1D *h_trk_inef_XXT = (TH1D *) fin_trk_inef_XXT->Get("h_unc_rel_2")->Clone("h_trk_inef_XXT");

    TFile *fin_model_XXT = new TFile("histos/"+observable+"_model_unc_XXT.root");
    TH1D *h_model_XXT = (TH1D *) fin_model_XXT->Get("h_unc_rel_2")->Clone("h_model_XXT");

    TFile *fin_stat_XXT = new TFile("histos/"+observable+"_stat_unc_XXT.root");
    TH1D *h_stat_XXT = (TH1D *) fin_stat_XXT->Get("h_stat_unc_rel_2")->Clone("h_stat_XXT");

    TFile *fin_mc_stat_XXT = new TFile("histos/"+observable+"_mc_stat_unc_XXT.root");
    TH1D *h_mc_stat_XXT = (TH1D *) fin_mc_stat_XXT->Get("h_mc_stat_unc_up_rel_2")->Clone("h_mc_stat_XXT");

    TFile *fin_model_fit_XXT = new TFile("histos/"+observable+"_model_fit_unc_XXT.root");
    TH1D *h_model_fit_XXT = (TH1D *) fin_model_fit_XXT->Get("h_unc_rel_2")->Clone("h_model_fit_XXT");

    TFile *fin_btag_XXT = new TFile("histos/"+observable+"_btag_bias.root");
    TH1D *h_btag_XXT = (TH1D *) fin_btag_XXT->Get("h_unc_rel_2")->Clone("h_btag_XXT");

    TFile *fin_cl_frac_XXT = new TFile("histos/"+observable+"_cl_frac_unc_XXT.root");
    TH1D *h_cl_frac_XXT = (TH1D *) fin_cl_frac_XXT->Get("h_unc_rel_2")->Clone("h_cl_frac_XXT");

    TFile *fin_jer_inclusive = new TFile("histos/"+observable+"_jer_unc_inclusive.root");
    TH1D *h_jer_inclusive = (TH1D *) fin_jer_inclusive->Get("h_unc_rel_2")->Clone("h_jer_inclusive");

    TFile *fin_jec_inclusive = new TFile("histos/"+observable+"_jec_unc_src_inclusive.root");
    TH1D *h_jec_noflav_inclusive = (TH1D *) fin_jec_inclusive->Get("h_total_unc_rel_sym_noflav_2")->Clone("h_jec_noflav_inclusive");
    TH1D *h_jec_flav_inclusive = (TH1D *) fin_jec_inclusive->Get("h_flav_unc_rel_sym_2")->Clone("h_jec_flav_inclusive");

    TFile *fin_trk_inef_inclusive = new TFile("histos/"+observable+"_trk_inef_inclusive.root");
    TH1D *h_trk_inef_inclusive = (TH1D *) fin_trk_inef_inclusive->Get("h_unc_rel_2")->Clone("h_trk_inef_inclusive");

    TFile *fin_model_inclusive = new TFile("histos/"+observable+"_model_unc_inclusive.root");
    TH1D *h_model_inclusive = (TH1D *) fin_model_inclusive->Get("h_unc_rel_2")->Clone("h_model_inclusive");

    TFile *fin_stat_inclusive = new TFile("histos/"+observable+"_stat_unc_inclusive.root");
    TH1D *h_stat_inclusive = (TH1D *) fin_stat_inclusive->Get("h_stat_unc_rel_2")->Clone("h_stat_inclusive");

    TFile *fin_mc_stat_inclusive = new TFile("histos/"+observable+"_mc_stat_unc_inclusive.root");
    TH1D *h_mc_stat_inclusive = (TH1D *) fin_mc_stat_inclusive->Get("h_mc_stat_unc_up_rel_2")->Clone("h_mc_stat_inclusive");

    int nbins_x = h_jer_XXT->GetNbinsX();
    int ibin_x_min = 2;
    int ibin_x_max = nbins_x; 
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    TH1D *h_ratio_unc = (TH1D *) h_jer_XXT->Clone("h_ratio_unc");
    h_ratio_unc->Reset();
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        // jer = correlated
        double jer_XXT = h_jer_XXT->GetBinContent(ibin_x);
        double jer_inclusive = h_jer_inclusive->GetBinContent(ibin_x);
        double jer_ratio2 = (jer_XXT*jer_XXT)+(jer_inclusive*jer_inclusive)-(2*jer_XXT*jer_inclusive); 

        // jec = correlated without flavor
        double jec_noflav_XXT = h_jec_noflav_XXT->GetBinContent(ibin_x);
        double jec_noflav_inclusive = h_jec_noflav_inclusive->GetBinContent(ibin_x);
        double jec_noflav_ratio2 = (jec_noflav_XXT*jec_noflav_XXT)+(jec_noflav_inclusive*jec_noflav_inclusive)-(2*jec_noflav_XXT*jec_noflav_inclusive); 

        // jer = correlated
        double trk_inef_XXT = h_trk_inef_XXT->GetBinContent(ibin_x);
        double trk_inef_inclusive = h_trk_inef_inclusive->GetBinContent(ibin_x);
        double trk_inef_ratio2 = (trk_inef_XXT*trk_inef_XXT)+(trk_inef_inclusive*trk_inef_inclusive)-(2*trk_inef_XXT*trk_inef_inclusive); 

        // model = correlated
        double model_XXT = h_model_XXT->GetBinContent(ibin_x);
        double model_inclusive = h_model_inclusive->GetBinContent(ibin_x);
        double model_ratio2 = (model_XXT*model_XXT)+(model_inclusive*model_inclusive)-(2*model_XXT*model_inclusive); 

        // jec_flav = uncorrelated
        double jec_flav_XXT = h_jec_flav_XXT->GetBinContent(ibin_x);
        double jec_flav_inclusive = h_jec_flav_inclusive->GetBinContent(ibin_x);
        double jec_flav_ratio2 = (jec_flav_XXT*jec_flav_XXT)+(jec_flav_inclusive*jec_flav_inclusive); 

        // mc_stat = uncorrelated
        double mc_stat_XXT = h_mc_stat_XXT->GetBinContent(ibin_x);
        double mc_stat_inclusive = h_mc_stat_inclusive->GetBinContent(ibin_x);
        double mc_stat_ratio2 = (mc_stat_XXT*mc_stat_XXT)+(mc_stat_inclusive*mc_stat_inclusive); 

        // model_fit = uncorrelated
        double model_fit_XXT = h_model_fit_XXT->GetBinContent(ibin_x);
        double model_fit_ratio2 = (model_fit_XXT*model_fit_XXT); 

        // btag = uncorrelated
        double btag_XXT = h_btag_XXT->GetBinContent(ibin_x);
        double btag_ratio2 = (btag_XXT*btag_XXT); 

        // cl_frac = uncorrelated
        double cl_frac_XXT = h_cl_frac_XXT->GetBinContent(ibin_x);
        double cl_frac_ratio2 = (cl_frac_XXT*cl_frac_XXT); 

        double ratio2 = jer_ratio2 + jec_noflav_ratio2 + trk_inef_ratio2 + model_ratio2 
                        + jec_flav_ratio2 + mc_stat_ratio2 
                        + model_fit_ratio2 + btag_ratio2 + cl_frac_ratio2;

        h_ratio_unc->SetBinContent(ibin_x, std::sqrt(ratio2));
    }

    TString fout_name = "histos/ratio_unc_"+observable+".root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_ratio_unc->Write();
}