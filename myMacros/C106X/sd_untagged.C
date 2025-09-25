void sd_untagged(TString observable="rg")
{
    bool inclusive = false;
    TString pythia_sample = inclusive ? "dijet_PF40" : "pythia_PF40";
    TString herwig_sample = inclusive ? "herwig_dijet_official_PF40" : "herwig_official_PF40";
    TString label = inclusive ? "aggrTMVA_inclusive" : "aggrTMVA_XXT";

    // Grab nominal unfolded data
    TString fin_data_name = "unfolding/histos/"+pythia_sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom" + TString(inclusive ? "" : "_withSF") +".root";
    std::cout << "Reading data from : " << fin_data_name << std::endl;
    TFile *fin_data = new TFile(fin_data_name);
    TH2D *h_data = (TH2D *) fin_data->Get("h_data_unfolded")->Clone("h_data");

    // Grab truth MC 
    TString fin_pythia_name = "unfolding/histos/"+pythia_sample+"_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading pythia from : " << fin_pythia_name << std::endl;
    TFile *fin_pythia = new TFile(fin_pythia_name);
    TH2D *h_pythia = (TH2D *) fin_pythia->Get("h_full_efficiency_denominator_"+observable+"pt");

    TString fin_herwig_name = "unfolding/histos/"+herwig_sample+"_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading herwig from : " << fin_herwig_name << std::endl;
    TFile *fin_herwig = new TFile(fin_herwig_name);
    TH2D *h_herwig = (TH2D *) fin_herwig->Get("h_full_efficiency_denominator_"+observable+"pt");

    // Make projections -- DO NOT NORMALIZE
    int ibin_pt = 2;
    double min_pt = h_data->GetYaxis()->GetBinLowEdge(ibin_pt);
    double max_pt = h_data->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_data_1d = (TH1D *) h_data->ProjectionX("h_data_1d", ibin_pt, ibin_pt);
    TH1D *h_pythia_1d = (TH1D *) h_pythia->ProjectionX("h_pythia_1d", ibin_pt, ibin_pt);
    TH1D *h_herwig_1d = (TH1D *) h_herwig->ProjectionX("h_herwig_1d", ibin_pt, ibin_pt);

    // Get uncertainties -- THESE ARE RELATIVE UNCERTAINTIES
    TString fin_unc_name = "uncertanties/histos/total_unc_"+TString(inclusive ? "incl_" : "XXT_")+observable+"_wU.root";
    std::cout << "Reading uncertainties from : " << fin_unc_name << std::endl;
    TFile *fin_unc = new TFile(fin_unc_name);
    TH1D *h_stat_unc = (TH1D *) fin_unc->Get("h_stat_unc")->Clone("h_stat_unc");
    TH1D *h_syst_unc = (TH1D *) fin_unc->Get("h_syst_unc_up_2")->Clone("h_syst_unc");

    // Calculate SD-untagged fraction with uncertainty
    double data_untagged = h_data_1d->Integral(1,1)/h_data_1d->Integral();
    double stat_unc = h_stat_unc->GetBinContent(1)*data_untagged;
    double syst_unc = h_syst_unc->GetBinContent(1)*data_untagged;
    std::cout << "Data untagged fraction: " << data_untagged << " +/- " << stat_unc << " (stat) +/- " << syst_unc << " (syst)" << std::endl;

    double pythia_untagged = h_pythia_1d->Integral(1,1)/h_pythia_1d->Integral();
    std::cout << "Pythia untagged fraction: " << pythia_untagged << std::endl;

    double herwig_untagged = h_herwig_1d->Integral(1,1)/h_herwig_1d->Integral();
    std::cout << "Herwig untagged fraction: " << herwig_untagged << std::endl;
}