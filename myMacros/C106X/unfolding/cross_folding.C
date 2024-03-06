#include "../draw_utils.h"

void cross_folding(TString observable="rg")
{
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin_pythia = new TFile("histos/bjet_aggrTMVA_XXT_response_full_jer_nom_jec_nom.root");
    TH2D *h_pythia_reco = (TH2D *) fin_pythia->Get("h_sig_training_reco_"+observable+"pt")->Clone("h_pythia_reco");
    TH2D *h_pythia_true = (TH2D *) fin_pythia->Get("h_sig_training_true_"+observable+"pt")->Clone("h_pythia_true");
    TH2D *h_pythia_purity = (TH2D *) fin_pythia->Get("h_sig_training_purity_"+observable+"pt")->Clone("h_pythia_purity");
    TH2D *h_pythia_efficiency = (TH2D *) fin_pythia->Get("h_sig_training_efficiency_"+observable+"pt")->Clone("h_pythia_efficiency");
    RooUnfoldResponse *response_pythia = (RooUnfoldResponse *) fin_pythia->Get("response_" + observable + "pt")->Clone("response_pythia");

    TFile *fin_herwig = new TFile("histos/herwig_bjet_aggrTMVA_XXT_response_full_jer_nom_jec_nom.root");
    TH2D *h_herwig_reco = (TH2D *) fin_herwig->Get("h_sig_training_reco_"+observable+"pt")->Clone("h_herwig_reco");
    TH2D *h_herwig_true = (TH2D *) fin_herwig->Get("h_sig_training_true_"+observable+"pt")->Clone("h_herwig_true");
    TH2D *h_herwig_purity = (TH2D *) fin_herwig->Get("h_sig_training_purity_"+observable+"pt")->Clone("h_herwig_purity");
    TH2D *h_herwig_efficiency = (TH2D *) fin_herwig->Get("h_sig_training_efficiency_"+observable+"pt")->Clone("h_herwig_efficiency");
    RooUnfoldResponse *response_herwig = (RooUnfoldResponse *) fin_herwig->Get("response_" + observable + "pt")->Clone("response_herwig");

    // Unfold pythia with herwig response
    TH2D *h_pythia_purity_corrected = (TH2D *) h_pythia_reco->Clone("h_pythia_purity_corrected");
    h_pythia_purity_corrected->Multiply(h_herwig_purity);

    RooUnfoldInvert unfold_herwig(response_herwig, h_pythia_purity_corrected);
    TH2D *h_pythia_unfolded = (TH2D *) unfold_herwig.Hreco();
    h_pythia_unfolded->SetName("h_pythia_unfolded");

    TH2D *h_pythia_efficiency_corrected = (TH2D *) h_pythia_unfolded->Clone("h_pythia_efficiency_corrected");
    h_pythia_efficiency_corrected->Divide(h_pythia_unfolded, h_herwig_efficiency, 1, 1);

     // Unfold herwig with pythia response
    TH2D *h_herwig_purity_corrected = (TH2D *) h_herwig_reco->Clone("h_herwig_purity_corrected");
    h_herwig_purity_corrected->Multiply(h_pythia_purity);

    RooUnfoldInvert unfold_pythia(response_pythia, h_herwig_purity_corrected);
    TH2D *h_herwig_unfolded = (TH2D *) unfold_pythia.Hreco();
    h_herwig_unfolded->SetName("h_herwig_unfolded");

    TH2D *h_herwig_efficiency_corrected = (TH2D *) h_herwig_unfolded->Clone("h_herwig_efficiency_corrected");
    h_herwig_efficiency_corrected->Divide(h_herwig_unfolded, h_pythia_efficiency, 1, 1);

    // Draw the distributions
    int ibin_pt = 2;

    // pythia with herwig response
    TH1D *h_pythia_efficiency_corrected_1d = (TH1D *) h_pythia_efficiency_corrected->ProjectionX("h_pythia_efficiency_corrected_1d", ibin_pt, ibin_pt);
    h_pythia_efficiency_corrected_1d->SetMarkerStyle(kFullCircle);
    h_pythia_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_pythia_efficiency_corrected_1d->SetLineColor(kBlack);
    h_pythia_efficiency_corrected_1d->GetXaxis()->SetTitle(xlabel);
    h_pythia_efficiency_corrected_1d->GetYaxis()->SetTitle(ylabel);
    h_pythia_efficiency_corrected_1d->SetTitle("pythia unfolded with herwig");
    h_pythia_efficiency_corrected_1d->Scale(1/h_pythia_efficiency_corrected_1d->Integral(), "width");

    TH1D *h_pythia_true_1d = (TH1D *) h_pythia_true->ProjectionX("h_pythia_true_1d", ibin_pt, ibin_pt);
    h_pythia_true_1d->SetMarkerStyle(kFullCircle);
    h_pythia_true_1d->SetMarkerColor(kRed);
    h_pythia_true_1d->SetLineColor(kRed);
    h_pythia_true_1d->SetTitle("pythia true");
    h_pythia_true_1d->Scale(1/h_pythia_true_1d->Integral(), "width");

    TH1D *h_ratio_pythia = (TH1D *) h_pythia_efficiency_corrected_1d->Clone("h_ratio_pythia");
    h_ratio_pythia->Divide(h_pythia_true_1d);
    h_ratio_pythia->GetYaxis()->SetTitle("unfolded/true");

    TCanvas *c_pythia = new TCanvas("c_pythia", "pythia unfolded with herwig", 800, 600);
    TPad *top_pad_pythia = new TPad("top_pad_pythia", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad_pythia = new TPad("top_pad_pythia", "", 0., 0., 1., 0.33);

    top_pad_pythia->cd(); 
    top_pad_pythia->SetBottomMargin(0.01);
    h_pythia_efficiency_corrected_1d->GetXaxis()->SetLabelOffset(999);
    h_pythia_efficiency_corrected_1d->GetXaxis()->SetLabelSize(0);
    h_pythia_efficiency_corrected_1d->GetXaxis()->SetTitleOffset(999);
    h_pythia_efficiency_corrected_1d->GetXaxis()->SetTitleSize(0);
    h_pythia_efficiency_corrected_1d->Draw("pe1");
    h_pythia_true_1d->Draw("pe1 same");
    top_pad_pythia->BuildLegend();
    drawHeaderSimulation();

    bottom_pad_pythia->cd();
    bottom_pad_pythia->SetTopMargin(0.05);
    bottom_pad_pythia->SetBottomMargin(0.3);
    h_ratio_pythia->GetXaxis()->SetTitleOffset(3.);
    h_ratio_pythia->Draw("pe1");
    TLine *line = new TLine(h_ratio_pythia->GetXaxis()->GetBinLowEdge(1), 1., h_ratio_pythia->GetXaxis()->GetBinUpEdge(h_ratio_pythia->GetNbinsX()), 1.);
    line->Draw();

    c_pythia->cd();
    top_pad_pythia->Draw("pe1");
    bottom_pad_pythia->Draw("pe1 same");

    c_pythia->Draw();
    c_pythia->Print("plots_an/bjet_aggrTMVA_XXT_xfolding_test_"+observable+".png");

    
    // herwig with pythia response
    TH1D *h_herwig_efficiency_corrected_1d = (TH1D *) h_herwig_efficiency_corrected->ProjectionX("h_herwig_efficiency_corrected_1d", ibin_pt, ibin_pt);
    h_herwig_efficiency_corrected_1d->SetMarkerStyle(kFullCircle);
    h_herwig_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_herwig_efficiency_corrected_1d->SetLineColor(kBlack);
    h_herwig_efficiency_corrected_1d->GetXaxis()->SetTitle(xlabel);
    h_herwig_efficiency_corrected_1d->GetYaxis()->SetTitle(ylabel);
    h_herwig_efficiency_corrected_1d->SetTitle("herwig unfolded with pythia");
    h_herwig_efficiency_corrected_1d->Scale(1/h_herwig_efficiency_corrected_1d->Integral(), "width");

    TH1D *h_herwig_true_1d = (TH1D *) h_herwig_true->ProjectionX("h_herwig_true_1d", ibin_pt, ibin_pt);
    h_herwig_true_1d->SetMarkerStyle(kFullCircle);
    h_herwig_true_1d->SetMarkerColor(kRed);
    h_herwig_true_1d->SetLineColor(kRed);
    h_herwig_true_1d->SetTitle("herwig true");
    h_herwig_true_1d->Scale(1/h_herwig_true_1d->Integral(), "width");

    TH1D *h_ratio_herwig = (TH1D *) h_herwig_efficiency_corrected_1d->Clone("h_ratio_herwig");
    h_ratio_herwig->Divide(h_herwig_true_1d);
    h_ratio_herwig->GetYaxis()->SetTitle("unfolded/true");

    TCanvas *c_herwig = new TCanvas("c_herwig", "herwig unfolded with herwig", 800, 600);
    TPad *top_pad_herwig = new TPad("top_pad_herwig", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad_herwig = new TPad("top_pad_herwig", "", 0., 0., 1., 0.33);

    top_pad_herwig->cd(); 
    top_pad_herwig->SetBottomMargin(0.01);
    h_herwig_efficiency_corrected_1d->GetXaxis()->SetLabelOffset(999);
    h_herwig_efficiency_corrected_1d->GetXaxis()->SetLabelSize(0);
    h_herwig_efficiency_corrected_1d->GetXaxis()->SetTitleOffset(999);
    h_herwig_efficiency_corrected_1d->GetXaxis()->SetTitleSize(0);
    h_herwig_efficiency_corrected_1d->Draw("pe1");
    h_herwig_true_1d->Draw("pe1 same");
    top_pad_herwig->BuildLegend();
    drawHeaderHerwig();

    bottom_pad_herwig->cd();
    bottom_pad_herwig->SetTopMargin(0.05);
    bottom_pad_herwig->SetBottomMargin(0.3);
    h_ratio_herwig->GetXaxis()->SetTitleOffset(3.);
    h_ratio_herwig->Draw("pe1");
    // TLine *line = new TLine(h_ratio_herwig->GetXaxis()->GetBinLowEdge(1), 1., h_ratio_herwig->GetXaxis()->GetBinUpEdge(h_ratio_herwig->GetNbinsX()), 1.);
    line->Draw();

    c_herwig->cd();
    top_pad_herwig->Draw("pe1");
    bottom_pad_herwig->Draw("pe1 same");

    c_herwig->Draw();
    c_herwig->Print("plots_an/herwig_bjet_aggrTMVA_XXT_xfolding_test_"+observable+".png");


}