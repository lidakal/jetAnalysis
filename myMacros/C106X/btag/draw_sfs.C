void draw_sfs(TString observable="rg", int ibin_pt=2) 
{
    gStyle->SetPaintTextFormat(".3f"); 
    gStyle->SetErrorX(0.5);
    
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch} #equiv p_{T}^{B,ch}/p_{T}^{jet,ch}";

    TFile *fin_nom = new TFile("histos/aggrTMVA_inclusive_"+observable+"_sfs.root");
    TH2D *h_sf_nom = (TH2D *) fin_nom->Get("h_eff_sf")->Clone("h_sf_nom");

    TFile *fin_dataWithMC = new TFile("histos/aggrTMVA_inclusive_"+observable+"_DataWithMCJPCalibration_sfs.root");
    TH2D *h_sf_dataWithMC = (TH2D *) fin_dataWithMC->Get("h_eff_sf")->Clone("h_sf_dataWithMC");

    double pt_min = h_sf_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_sf_nom->GetYaxis()->GetBinUpEdge(ibin_pt);

    // Make projections 
    TH1D *h_sf_nom_1d = (TH1D *) h_sf_nom->ProjectionX("h_sf_nom_1d", ibin_pt, ibin_pt);
    h_sf_nom_1d->SetLineColor(kBlack);
    h_sf_nom_1d->SetMarkerColor(kBlack);
    h_sf_nom_1d->SetMarkerStyle(kFullCircle);
    h_sf_nom_1d->SetTitle("nominal");
    h_sf_nom_1d->GetXaxis()->SetTitle(xlabel);
    h_sf_nom_1d->GetYaxis()->SetTitle("SF_{b}");
    h_sf_nom_1d->GetYaxis()->SetRangeUser(0.9, 3.);

    TH1D *h_sf_dataWithMC_1d = (TH1D *) h_sf_dataWithMC->ProjectionX("h_sf_dataWithMC_1d", ibin_pt, ibin_pt);
    h_sf_dataWithMC_1d->SetLineColor(kRed);
    h_sf_dataWithMC_1d->SetMarkerColor(kRed);
    h_sf_dataWithMC_1d->SetMarkerStyle(kFullCross);
    h_sf_dataWithMC_1d->SetTitle("JP Swap Data");

    TCanvas *c_sf = new TCanvas("c_sf", "", 800, 600);
    h_sf_nom_1d->Draw("pe1");
    h_sf_dataWithMC_1d->Draw("pe1 same");
    auto leg_sf = c_sf->BuildLegend();
    leg_sf->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));
}
