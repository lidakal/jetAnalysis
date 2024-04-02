#include "../draw_utils.h"

void sf_unc(TString observable="rg")
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

    // Load histograms
    TFile *fin_sf = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_sf = (TH2D *) fin_sf->Get("h_data_unfolded")->Clone("h_sf");

    TFile *fin_sfUp = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSFUp.root");
    TH2D *h_sfUp = (TH2D *) fin_sfUp->Get("h_data_unfolded")->Clone("h_sfUp");

    TFile *fin_sfDown = new TFile("../unfolding/histos/bjet_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSFDown.root");
    TH2D *h_sfDown = (TH2D *) fin_sfDown->Get("h_data_unfolded")->Clone("h_sfDown");

    int nbins_x = h_sf->GetNbinsX();
    int nbins_pt = h_sf->GetNbinsY();

    TCanvas *c_corr = new TCanvas("c_corr", "", 800, 600);
    // TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 0.35);
    TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 0.33);
    // TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 0.35);
    // TPad *pad21 = new TPad("pad21", "", 0., 0.35, 0.33, 1.);
    TPad *pad22 = new TPad("pad22", "", 0., 0.33, 1., 1.);
    // TPad *pad23 = new TPad("pad23", "", 0.66, 0.35, 0.99, 1.);

    TPad *pad11, *pad13, *pad21, *pad23;

    std::vector<TPad *> bottom_pads = {pad11, pad12, pad13};
    std::vector<TPad *> top_pads = {pad21, pad22, pad23};

    TFile *fout = new TFile("./histos/"+observable+"_btag_bias.root", "recreate");

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;
        int ipad = ibin_pt - 1;

        double pt_min = h_sf->GetYaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_sf->GetYaxis()->GetBinUpEdge(ibin_pt);

        TLegend *leg;
        if (observable=="rg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zg") leg = new TLegend(0.6, 0.55, 0.9, 0.85);
        if (observable=="zpt") leg = new TLegend(0.3, 0.55, 0.6, 0.85);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

        TH1D *h_sf_1d = (TH1D *) h_sf->ProjectionX(Form("h_sf_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_sf_1d->SetMarkerColor(kBlack);
        h_sf_1d->SetLineColor(kBlack);
        h_sf_1d->SetMarkerStyle(kFullCircle);
        leg->AddEntry(h_sf_1d, "nominal", "pe1");

        TH1D *h_sfUp_1d = (TH1D *) h_sfUp->ProjectionX(Form("h_sfUp_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_sfUp_1d->SetMarkerColor(kRed);
        h_sfUp_1d->SetLineColor(kRed);
        h_sfUp_1d->SetMarkerStyle(kFullTriangleUp);
        leg->AddEntry(h_sfUp_1d, "SF up", "pe1");

        TH1D *h_sfDown_1d = (TH1D *) h_sfDown->ProjectionX(Form("h_sfDown_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_sfDown_1d->SetMarkerColor(kGreen);
        h_sfDown_1d->SetLineColor(kGreen);
        h_sfDown_1d->SetMarkerStyle(kFullTriangleDown);
        leg->AddEntry(h_sfDown_1d, "SF down", "pe1");

        int ibin_x_min = 1;
        int ibin_x_max = nbins_x;

        if (observable!="zpt") ibin_x_min = 2;
        if (observable=="rg") ibin_x_max = nbins_x - 1;

        top_pads[ipad]->cd();
        // top_pads[ipad]->SetRightMargin(0.05);
        // top_pads[ipad]->SetLeftMargin(0.2);
        top_pads[ipad]->SetBottomMargin(0.01);
        
        for (auto h : {h_sf_1d, h_sfUp_1d, h_sfDown_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
            h->GetYaxis()->SetTitle(ylabel);
            h->GetYaxis()->SetTitleOffset(2.);
            h->Draw("pe1 same");
        }
        leg->Draw();
        drawHeader();

        // Extract uncertainty
        TH1D *h_unc_up = (TH1D *) h_sfUp_1d->Clone(Form("h_unc_up_%d", ibin_pt));
        h_unc_up->Add(h_sf_1d, -1);
        h_unc_up->GetYaxis()->SetTitle("var-nom");
        h_unc_up->GetXaxis()->SetTitle(xlabel);
        h_unc_up->GetXaxis()->SetTitleOffset(3.);

         TH1D *h_unc_down = (TH1D *) h_sfDown_1d->Clone(Form("h_unc_down_%d", ibin_pt));
        h_unc_down->Add(h_sf_1d, -1);
        h_unc_down->GetYaxis()->SetTitle("var-nom");
        h_unc_down->GetXaxis()->SetTitle(xlabel);
        h_unc_down->GetXaxis()->SetTitleOffset(3.);

        h_unc_up->Write();
        h_unc_down->Write();

        TH1D *h_unc_up_rel = (TH1D *) h_unc_up->Clone(Form("h_unc_up_rel_%d", ibin_pt));
        h_unc_up_rel->Divide(h_sf_1d);
        h_unc_up_rel->GetYaxis()->SetTitle("(var-nom)/nom");
        h_unc_up_rel->GetYaxis()->SetTitleOffset(2.);
        h_unc_up_rel->GetYaxis()->SetRangeUser(-0.25, 0.25);
        h_unc_up_rel->Write();

        TH1D *h_unc_down_rel = (TH1D *) h_unc_down->Clone(Form("h_unc_down_rel_%d", ibin_pt));
        h_unc_down_rel->Divide(h_sf_1d);
        h_unc_down_rel->Write();

        bottom_pads[ipad]->cd();
        bottom_pads[ipad]->SetTopMargin(0.01);
        bottom_pads[ipad]->SetBottomMargin(0.3);
        // bottom_pads[ipad]->SetRightMargin(0.05);
        // bottom_pads[ipad]->SetLeftMargin(0.2);

        h_unc_up_rel->Draw("pe1");
        h_unc_down_rel->Draw("pe1 same");

        TLine *line = new TLine(h_unc_up->GetXaxis()->GetBinLowEdge(1), 0, h_unc_up->GetXaxis()->GetBinUpEdge(h_unc_up->GetNbinsX()), 0);
        line->SetLineStyle(kDashed);
        line->Draw();

        c_corr->cd();
        top_pads[ipad]->Draw();
        bottom_pads[ipad]->Draw();
    }

    c_corr->Draw();
    c_corr->Print("plots_an/sf_unc_XXT_"+observable+".png");

}