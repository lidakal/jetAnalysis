#include "../cms_palette.h"

void drawHeader(void) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.15, 0.965, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.965, "pp 301^{} pb^{-1} (5.02 TeV)");
}

void model_fit_unc(TString observable="rg")
{
    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";
    TString ylabel;
    if (observable=="rg") ylabel = "1/N dN/dln(R/R_{g})";
    else if (observable=="zg") ylabel = "1/N dN/dz_{g}";
    else if (observable=="zpt") ylabel = "1/N dN/dz_{b,ch}";

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA_XXT";

    TFile *fin_nom = new TFile("../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_up = new TFile("../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_herwigFit_"+observable+"_jer_nom_jec_nom_withSF.root");
    TH2D *h_up = (TH2D *) fin_up->Get("h_data_unfolded")->Clone("h_up");

    // Make projections 
    Int_t ibin_pt = 2; 
    Double_t pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
    TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX("h_nom_1d", ibin_pt, ibin_pt);
    TH1D *h_up_1d = (TH1D *) h_up->ProjectionX("h_up_1d", ibin_pt, ibin_pt);

    // Normalize
    Int_t nbins_x = h_nom_1d->GetNbinsX();
    Int_t ibin_x_min = 1;
    Int_t ibin_x_max = nbins_x;
    if (observable!="zpt") ibin_x_min = 2;
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    for (auto h : {h_nom_1d, h_up_1d}) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(), "width");
    }

    // Format histograms
    h_nom_1d->SetMarkerStyle(kFullCrossX);
    h_nom_1d->SetMarkerColor(kBlack);
    h_nom_1d->SetLineColor(kBlack);
    h_nom_1d->SetMarkerSize(2);

    h_up_1d->SetMarkerStyle(kFullTriangleUp);
    h_up_1d->SetMarkerColor(cmsBlue);
    h_up_1d->SetLineColor(cmsBlue);
    h_up_1d->SetMarkerSize(2);

    // Uncertainties 
    TH1D * h_unc_up = (TH1D *) h_up_1d->Clone("h_unc_up");
    h_unc_up->Add(h_nom_1d, -1);
    TH1D *h_unc_up_rel = (TH1D *) h_unc_up->Clone("h_unc_up_rel");
    h_unc_up_rel->Divide(h_nom_1d);

    // Symmetrize relative uncertainties
    TH1D *h_unc_rel_sym_up = (TH1D *) h_unc_up_rel->Clone("h_unc_rel_sym_up"); // 2-point variation
    for (int i=1; i<=h_unc_up_rel->GetNbinsX(); i++) {
        double unc1 = h_unc_up_rel->GetBinContent(i);
        h_unc_rel_sym_up->SetBinContent(i, std::abs(unc1));
    }

    TH1D *h_unc_rel_sym_down = (TH1D *) h_unc_rel_sym_up->Clone("h_unc_rel_sym_down");
    h_unc_rel_sym_down->Scale(-1.);

    for (auto h : {h_unc_rel_sym_up, h_unc_rel_sym_down}) {
        h->SetFillColor(kGray);
        h->SetFillStyle(1001);
        h->SetLineColor(kGray);
    }

    // Legend
    TLegend *leg = new TLegend(0.55, 0.3, 0.9, 0.5);
    if (observable=="zpt") leg = new TLegend(0.2, 0.3, 0.43, 0.5);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_nom_1d, "Nominal (PYTHIA8)", "pe1");
    leg->AddEntry(h_up_1d, "Variation (HERWIG7)", "pe1");

    // Draw
    TCanvas *c_unc = new TCanvas("c_unc", "", 700, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.33);

    top_pad->SetLeftMargin(0.15);
    top_pad->SetRightMargin(0.05);
    top_pad->SetTopMargin(0.09);
    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetLeftMargin(0.15);
    bottom_pad->SetRightMargin(0.05);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.35);

    top_pad->cd();
    h_nom_1d->GetYaxis()->SetTitle(ylabel);
    h_nom_1d->GetYaxis()->SetTitleSize(text_size);
    h_nom_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_nom_1d->GetYaxis()->SetTitleOffset(1.3);
    if (observable=="rg") h_nom_1d->GetYaxis()->SetRangeUser(0., 2.);
    else if (observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 10.);
    else if (observable=="zpt") h_nom_1d->GetYaxis()->SetRangeUser(0., 4.);
    h_nom_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_nom_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_nom_1d->Draw("pe1");
    h_up_1d->Draw("pe1 same");
    leg->Draw();
    drawHeader();
    // Jets text
    if (observable!="zpt") {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(32);
        jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.9, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
        jet_info->DrawLatex(0.9, 0.67, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.59, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");
    } else {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.2, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.2, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    }

    bottom_pad->cd();
    h_unc_up_rel->GetXaxis()->SetTitle(xlabel);
    h_unc_up_rel->GetXaxis()->SetTitleSize(text_size);
    h_unc_up_rel->GetXaxis()->SetLabelSize(text_size-4);
    h_unc_up_rel->GetXaxis()->SetTitleOffset(3.);
    h_unc_up_rel->GetYaxis()->SetTitle("#splitline{Relative}{uncertainty}");
    h_unc_up_rel->GetYaxis()->SetTitleSize(text_size);
    h_unc_up_rel->GetYaxis()->SetLabelSize(text_size-4);
    h_unc_up_rel->GetYaxis()->SetTitleOffset(1.4);
    if (observable=="rg") h_unc_up_rel->GetYaxis()->SetRangeUser(-0.4, 0.4);
    else if (observable=="zg") h_unc_up_rel->GetYaxis()->SetRangeUser(-0.6, 0.6);
    else if (observable=="zpt") h_unc_up_rel->GetYaxis()->SetRangeUser(-0.4, 0.4);
    h_unc_up_rel->GetYaxis()->SetNdivisions(-4);

    h_unc_up_rel->Draw("p hist");
    h_unc_rel_sym_up->Draw("hist same");
    h_unc_rel_sym_down->Draw("hist same");
    h_unc_up_rel->Draw("p hist same");

    c_unc->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    c_unc->Draw();
    c_unc->Print("../plots_thesis/"+sample+"_"+label+"_model_fit_unc_"+observable+".pdf");

    // Save histograms 
    TString fout_name = "histos/" + observable + "_model_fit_unc_XXT.root";
    TFile *fout = new TFile(fout_name, "RECREATE");
    h_nom_1d->Write("h_nom_1d");
    h_up_1d->Write("h_up_1d");
    h_unc_up->Write("h_unc_up");
    h_unc_up_rel->Write("h_unc_up_rel");
    h_unc_rel_sym_up->Write("h_unc_rel_sym_up");
    h_unc_rel_sym_down->Write("h_unc_rel_sym_down");
    fout->Close();
}