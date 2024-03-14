#include "../draw_utils.h"

void compare_fit_fractions(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TFile *fin_pythia = new TFile("../template_fit/histos/fitted_parameters_RooFit_data_aggrTMVA_XXT_"+observable+"_glued_jer_nom_jec_nom.root");
    TH2D *h_b_frac_pythia = (TH2D *) fin_pythia->Get("h_sig_fraction")->Clone("h_b_frac_pythia");
    TH2D *h_bb_frac_pythia = (TH2D *) fin_pythia->Get("h_bkg_bb_fraction")->Clone("h_bb_frac_pythia");
    TH2D *h_cl_frac_pythia = (TH2D *) fin_pythia->Get("h_bkg_rest_fraction")->Clone("h_cl_frac_pythia");

    TFile *fin_herwig = new TFile("../template_fit/histos/fitted_parameters_RooFit_data_herwig_aggrTMVA_XXT_"+observable+"_glued_jer_nom_jec_nom.root");
    TH2D *h_b_frac_herwig = (TH2D *) fin_herwig->Get("h_sig_fraction")->Clone("h_b_frac_herwig");
    TH2D *h_bb_frac_herwig = (TH2D *) fin_herwig->Get("h_bkg_bb_fraction")->Clone("h_bb_frac_herwig");
    TH2D *h_cl_frac_herwig = (TH2D *) fin_herwig->Get("h_bkg_rest_fraction")->Clone("h_cl_frac_herwig");

    int ibin_pt = 2;

    // Make projections
    TH1D *h_b_frac_pythia_1d = (TH1D *) h_b_frac_pythia->ProjectionX("h_b_frac_pythia_1d", ibin_pt, ibin_pt);
    h_b_frac_pythia_1d->SetMarkerStyle(kFullCircle);
    h_b_frac_pythia_1d->SetMarkerColor(kBlack);
    h_b_frac_pythia_1d->SetLineColor(kBlack);
    h_b_frac_pythia_1d->SetTitle("b frac pythia");
    h_b_frac_pythia_1d->GetYaxis()->SetTitle("fraction from template fit");

    TH1D *h_b_frac_herwig_1d = (TH1D *) h_b_frac_herwig->ProjectionX("h_b_frac_herwig_1d", ibin_pt, ibin_pt);
    h_b_frac_herwig_1d->SetMarkerStyle(kFullCircle);
    h_b_frac_herwig_1d->SetMarkerColor(kRed);
    h_b_frac_herwig_1d->SetLineColor(kRed);
    h_b_frac_herwig_1d->SetTitle("b frac herwig");
    h_b_frac_herwig_1d->GetYaxis()->SetTitle("fraction from template fit");

    TH1D *h_bb_frac_pythia_1d = (TH1D *) h_bb_frac_pythia->ProjectionX("h_bb_frac_pythia_1d", ibin_pt, ibin_pt);
    h_bb_frac_pythia_1d->SetMarkerStyle(kOpenCross);
    h_bb_frac_pythia_1d->SetMarkerColor(kBlack);
    h_bb_frac_pythia_1d->SetLineColor(kBlack);
    h_bb_frac_pythia_1d->SetTitle("bb frac pythia");
    h_bb_frac_pythia_1d->GetYaxis()->SetTitle("fraction from template fit");

    TH1D *h_bb_frac_herwig_1d = (TH1D *) h_bb_frac_herwig->ProjectionX("h_bb_frac_herwig_1d", ibin_pt, ibin_pt);
    h_bb_frac_herwig_1d->SetMarkerStyle(kOpenCross);
    h_bb_frac_herwig_1d->SetMarkerColor(kRed);
    h_bb_frac_herwig_1d->SetLineColor(kRed);
    h_bb_frac_herwig_1d->SetTitle("bb frac herwig");
    h_bb_frac_herwig_1d->GetYaxis()->SetTitle("fraction from template fit");

    TH1D *h_cl_frac_pythia_1d = (TH1D *) h_cl_frac_pythia->ProjectionX("h_cl_frac_pythia_1d", ibin_pt, ibin_pt);
    h_cl_frac_pythia_1d->SetMarkerStyle(kOpenStar);
    h_cl_frac_pythia_1d->SetMarkerColor(kBlack);
    h_cl_frac_pythia_1d->SetLineColor(kBlack);
    h_cl_frac_pythia_1d->SetTitle("cl frac pythia");
    h_cl_frac_pythia_1d->GetYaxis()->SetTitle("fraction from template fit");

    TH1D *h_cl_frac_herwig_1d = (TH1D *) h_cl_frac_herwig->ProjectionX("h_cl_frac_herwig_1d", ibin_pt, ibin_pt);
    h_cl_frac_herwig_1d->SetMarkerStyle(kOpenStar);
    h_cl_frac_herwig_1d->SetMarkerColor(kRed);
    h_cl_frac_herwig_1d->SetLineColor(kRed);
    h_cl_frac_herwig_1d->SetTitle("cl frac herwig");
    h_cl_frac_herwig_1d->GetYaxis()->SetTitle("fraction from template fit");

    h_b_frac_pythia_1d->GetYaxis()->SetRangeUser(0., 1.);
    h_b_frac_pythia_1d->GetXaxis()->SetTitle(xlabel);

    TCanvas *c_b_frac = new TCanvas("c_b_frac", "", 800, 600);
    h_b_frac_pythia_1d->Draw("pe1");
    h_b_frac_herwig_1d->Draw("pe1 same");
    h_bb_frac_pythia_1d->Draw("pe1 same");
    h_bb_frac_herwig_1d->Draw("pe1 same");
    h_cl_frac_pythia_1d->Draw("pe1 same");
    h_cl_frac_herwig_1d->Draw("pe1 same");
    // auto leg = c_b_frac->BuildLegend();

    auto leg = new TLegend(0.7, 0.35, 0.9, 0.7);
    leg->SetHeader("100 < p_{T}^{jet} < 120 (GeV)");
    leg->SetFillStyle(0);
    leg->AddEntry(h_b_frac_pythia_1d);
    leg->AddEntry(h_b_frac_herwig_1d);
    leg->AddEntry(h_bb_frac_pythia_1d);
    leg->AddEntry(h_bb_frac_herwig_1d);
    leg->AddEntry(h_cl_frac_pythia_1d);
    leg->AddEntry(h_cl_frac_herwig_1d);
    leg->Draw();
    drawHeader();

    c_b_frac->Draw();
    c_b_frac->Print("plots_an/pythia_vs_herwig_fractions_"+observable+".png");
}