void stat_unc(TString observable="rg")
{
     // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin_nom = new TFile("../unfolding/histos/aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();

    TCanvas *c_jer = new TCanvas("c_jer", "", 1600, 600);
    TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 0.35);
    TPad *pad12 = new TPad("pad12", "", 0.33, 0., 0.66, 0.35);
    TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 0.35);
    TPad *pad21 = new TPad("pad21", "", 0., 0.35, 0.33, 1.);
    TPad *pad22 = new TPad("pad22", "", 0.33, 0.35, 0.66, 1.);
    TPad *pad23 = new TPad("pad23", "", 0.66, 0.35, 0.99, 1.);

    std::vector<TPad *> bottom_pads = {pad11, pad12, pad13};
    std::vector<TPad *> top_pads = {pad21, pad22, pad23};
    
    TFile *fout = new TFile("./histos/"+observable+"_stat_unc_XXT.root", "recreate");
    
    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        int ipad = ibin_pt - 1;

        double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);

        TLegend *leg;
        if (observable=="rg") leg = new TLegend(0.25, 0.1, 0.65, 0.35);
        if (observable=="zg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zpt") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

        // Make projections 
        TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);

        int ibin_x_min = 1;
        int ibin_x_max = nbins_x;

        h_nom_1d->SetMarkerStyle(kFullCircle); 
        h_nom_1d->SetMarkerColor(kBlack);
        h_nom_1d->SetLineColor(kBlack);
        leg->AddEntry(h_nom_1d, "nominal", "pe1");

        top_pads[ipad]->cd();
        top_pads[ipad]->SetRightMargin(0.05);
        top_pads[ipad]->SetLeftMargin(0.2);
        top_pads[ipad]->SetBottomMargin(0.01);
        for (auto h : {h_nom_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
            h->GetYaxis()->SetTitle(ylabel);
            h->GetYaxis()->SetTitleOffset(2.5);
            h->Draw("pe1 same");
        }
        leg->Draw();

        // Extract uncertainties 
        TH1D *h_stat_unc_up = (TH1D *) h_nom_1d->Clone(Form("h_stat_unc_%d", ibin_pt));
        h_stat_unc_up->Reset();
        h_stat_unc_up->SetTitle("statistical unc per bin");

        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            h_stat_unc_up->SetBinContent(ibin_x, h_nom_1d->GetBinError(ibin_x));
        }

        TH1D *h_stat_unc_down = (TH1D *) h_stat_unc_up->Clone("h_stat_unc_down");
        h_stat_unc_down->Scale(-1.);

        bottom_pads[ipad]->cd();
        bottom_pads[ipad]->SetTopMargin(0.01);
        bottom_pads[ipad]->SetBottomMargin(0.35);
        bottom_pads[ipad]->SetRightMargin(0.05);
        bottom_pads[ipad]->SetLeftMargin(0.2);

        double ymax = std::max({h_stat_unc_up->GetBinContent(h_stat_unc_up->GetMaximumBin()), h_stat_unc_down->GetBinContent(h_stat_unc_down->GetMaximumBin())});
        double ymin = std::min({h_stat_unc_up->GetBinContent(h_stat_unc_up->GetMinimumBin()), h_stat_unc_down->GetBinContent(h_stat_unc_down->GetMinimumBin())});

        if (ymin<0) ymin*=1.1;
        else ymin*=0.9;

        if (ymax<0) ymax*=0.9;
        else ymax*=1.1;

        h_stat_unc_up->GetYaxis()->SetRangeUser(ymin, ymax);
        h_stat_unc_down->GetYaxis()->SetRangeUser(ymin, ymax);

        h_stat_unc_up->Draw("hist");
        h_stat_unc_down->Draw("hist same");

        TLine *line = new TLine(h_stat_unc_up->GetXaxis()->GetBinLowEdge(ibin_x_min), 0, h_stat_unc_up->GetXaxis()->GetBinUpEdge(ibin_x_max), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        c_jer->cd();
        top_pads[ipad]->Draw();
        bottom_pads[ipad]->Draw();

        h_stat_unc_up->Write();

        // save relative uncertainty 
        TH1D *h_stat_unc_rel = (TH1D *) h_stat_unc_up->Clone(Form("h_stat_unc_rel_%d", ibin_pt));
        h_stat_unc_rel->Divide(h_nom_1d);
        h_stat_unc_rel->Write();
    }
}
