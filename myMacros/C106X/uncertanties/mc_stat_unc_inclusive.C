void mc_stat_unc_inclusive(TString observable="rg")
{
    TFile *fin = new TFile("../unfolding/histos/mc_stat_unc_incl_"+observable+".root");
    TH2D *h_mc_stat_unc_rel = (TH2D *) fin->Get("h_mc_stat_unc_rel");

    int nbins_x = h_mc_stat_unc_rel->GetNbinsX();
    int nbins_pt = h_mc_stat_unc_rel->GetNbinsY();

    TFile *fout = new TFile("./histos/"+observable+"_mc_stat_unc_inclusive.root", "recreate");
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        int ipad = ibin_pt - 1;

        double pt_min = h_mc_stat_unc_rel->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_mc_stat_unc_rel->GetYaxis()->GetBinUpEdge(ibin_pt);

        // Extract uncertainties 
        TH1D *h_mc_stat_unc_up_rel = (TH1D *) h_mc_stat_unc_rel->ProjectionX(Form("h_mc_stat_unc_up_rel_%d", ibin_pt));
        TH1D *h_mc_stat_unc_down_rel = (TH1D *) h_mc_stat_unc_up_rel->Clone(Form("h_mc_stat_unc_down_rel_%d", ibin_pt));
        h_mc_stat_unc_down_rel->Scale(-1.);


        h_mc_stat_unc_up_rel->Write();
        h_mc_stat_unc_down_rel->Write();
    }
}
