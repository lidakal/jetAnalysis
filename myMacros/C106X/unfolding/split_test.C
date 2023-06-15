// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../hist_utils.h"

void split_test()
{
    // Split test: apply the unfolding to pseudo-data, independent of the
    //             MC used to fill the matrix

    // ---- Setup 
    gSystem->Load("libRooUnfold.so");

    Double_t ptMin = 100.;
    Double_t ptMax = 120.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TString sample = "bjet";
    TString label = "aggrTMVA_withNb";
    TString fname = "./histos/qcd_" + sample + "_" + label + "_unfold_2d_square.root";

    TFile *fin = new TFile(fname);
    TH2D *h_mc_true_rgpt = (TH2D *) fin->Get("h_mc_true_rgpt");
    TH2D *h_mc_reco_rgpt = (TH2D *) fin->Get("h_mc_reco_rgpt");
    TH2D *h_mc_purity_nominator_rgpt = (TH2D *) fin->Get("h_mc_purity_nominator_rgpt");
    TH2D *h_mc_efficiency_nominator_rgpt = (TH2D *) fin->Get("h_mc_efficiency_nominator_rgpt");
    RooUnfoldResponse *response_rgpt = (RooUnfoldResponse *) fin->Get("response_rgpt");

    TH2D *h_data_reco_rgpt = (TH2D *) fin->Get("h_data_reco_rgpt");
    TH2D *h_data_true_rgpt = (TH2D *) fin->Get("h_data_true_rgpt");

    // Note: Result = unfold(raw * purity) * 1 / (efficiency)

    // ---- Purity correction 
    TH2D *h_purity = (TH2D *) h_mc_purity_nominator_rgpt->Clone("h_purity");
    h_purity->Divide(h_mc_reco_rgpt);

    TH2D *h_data_reco_purity_corrected_rgpt = (TH2D *) h_data_reco_rgpt->Clone("h_data_reco_purity_corrected_rgpt");
    h_data_reco_purity_corrected_rgpt->Multiply(h_purity);

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    // Int_t niter = 5;
    // RooUnfoldBayes unfold(response_rgpt, h_data_reco_rgpt, niter);
    RooUnfoldInvert unfold(response_rgpt, h_data_reco_purity_corrected_rgpt);
    TH2D *h_data_reco_unfolded_rgpt = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_data_reco_refolded_rgpt = (TH2D *) response_rgpt->ApplyToTruth(h_data_reco_unfolded_rgpt, "h_data_reco_refolded_rgpt"); // to be compared to purity corrected data

    // ---- Efficiency correction
    TH2D *h_efficiency = (TH2D *) h_mc_efficiency_nominator_rgpt->Clone("h_efficiency");
    h_efficiency->Divide(h_mc_true_rgpt);

    TH2D *h_data_reco_efficiency_corrected_rgpt = (TH2D *) h_data_reco_unfolded_rgpt->Clone("h_data_reco_efficiency_corrected_rgpt");
    h_data_reco_efficiency_corrected_rgpt->Divide(h_efficiency);

    // ---- Make projections
    Int_t iptmin = h_data_reco_rgpt->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_data_reco_rgpt->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_data_reco_rgpt->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_data_reco_rgpt->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_data_reco_rg = (TH1D *) h_data_reco_rgpt->ProjectionX("h_data_reco_rg", iptmin, iptmax);
    TH1D *h_data_true_rg = (TH1D *) h_data_true_rgpt->ProjectionX("h_data_true_rg", iptmin, iptmax);
    TH1D *h_data_reco_purity_corrected_rg = (TH1D *) h_data_reco_purity_corrected_rgpt->ProjectionX("h_data_reco_purity_corrected_rg", iptmin, iptmax);
    TH1D *h_data_reco_unfolded_rg = (TH1D *) h_data_reco_unfolded_rgpt->ProjectionX("h_data_reco_unfolded_rg", iptmin, iptmax);
    TH1D *h_data_reco_refolded_rg = (TH1D *) h_data_reco_refolded_rgpt->ProjectionX("h_data_reco_refolded_rg", iptmin, iptmax);
    TH1D *h_data_reco_efficiency_corrected_rg = (TH1D *) h_data_reco_efficiency_corrected_rgpt->ProjectionX("h_data_reco_efficiency_corrected_rg", iptmin, iptmax);
    TH1D *h_mc_true_rg = (TH1D *) h_mc_true_rgpt->ProjectionX("h_mc_true_rg", iptmin, iptmax);
    TH1D *h_mc_reco_rg = (TH1D *) h_mc_reco_rgpt->ProjectionX("h_mc_reco_rg", iptmin, iptmax);

    // ---- Normalize
    Int_t nbins = h_data_reco_rg->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    for (auto h : {h_data_reco_rg, h_data_true_rg,
                   h_data_reco_purity_corrected_rg, 
                   h_data_reco_unfolded_rg,
                   h_data_reco_refolded_rg,
                   h_data_reco_efficiency_corrected_rg,
                   h_mc_true_rg, h_mc_reco_rg}) {
                    normalize_histo(h, ibin_min, ibin_max);
                    h->Sumw2();
                    h->SetBinContent(1, h->GetBinContent(1) / 10);
                   }


    // ---- Format histos
    THStack *hStack_data_rg = new THStack("hStack_data_rg", "");
    hStack_data_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    THStack *hStack_mc_rg = new THStack("hStack_mc_rg", "");
    hStack_mc_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    TLegend *leg_rg = new TLegend(0.4, 0.7, 0.8, 0.9);
    leg_rg->SetFillStyle(0);
    leg_rg->SetBorderSize(0);
    leg_rg->SetMargin(0.15);
    leg_rg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real));

    format_histo(h_mc_true_rg, mykRed);
    h_mc_true_rg->SetLineStyle(kDashed);
    hStack_mc_rg->Add(h_mc_true_rg);
    leg_rg->AddEntry(h_mc_true_rg, "MC true", "l");

    format_histo(h_mc_reco_rg, mykRedLight);
    h_mc_reco_rg->SetLineStyle(kDashed);
    hStack_mc_rg->Add(h_mc_reco_rg);
    leg_rg->AddEntry(h_mc_reco_rg, "MC reco", "l");

    h_data_reco_rg->SetMarkerColor(mykBlueLight);
    h_data_reco_rg->SetMarkerStyle(kFullCircle);
    h_data_reco_rg->SetMarkerSize(3);
    hStack_data_rg->Add(h_data_reco_rg);
    leg_rg->AddEntry(h_data_reco_rg, "pseudo data reco", "p");

    h_data_true_rg->SetMarkerColor(mykBlueLight);
    h_data_true_rg->SetMarkerStyle(kCircle);
    h_data_true_rg->SetMarkerSize(3);
    hStack_data_rg->Add(h_data_true_rg);
    leg_rg->AddEntry(h_data_true_rg, "pseudo data true", "p");

    h_data_reco_efficiency_corrected_rg->SetMarkerColor(mykBlue);
    h_data_reco_efficiency_corrected_rg->SetMarkerStyle(kFullDiamond);
    h_data_reco_efficiency_corrected_rg->SetMarkerSize(4);
    hStack_data_rg->Add(h_data_reco_efficiency_corrected_rg);
    leg_rg->AddEntry(h_data_reco_efficiency_corrected_rg, "pseudo data unfolded + corrected", "p");

    h_data_reco_refolded_rg->SetMarkerColor(mykGreen);
    h_data_reco_refolded_rg->SetMarkerStyle(30);
    h_data_reco_refolded_rg->SetMarkerSize(4);
    hStack_data_rg->Add(h_data_reco_refolded_rg);
    leg_rg->AddEntry(h_data_reco_refolded_rg, "pseudo data re-folded", "p");

    TPaveText *underflow_text = new TPaveText(-0.4, 0.02, 0., 0.07);
    underflow_text->SetTextSize(text_size);
    underflow_text->SetFillStyle(0);
    underflow_text->SetBorderSize(0);
    underflow_text->AddText("untagged/10");

    // TPaveText *overflow_text = new TPaveText(1.6, 0.02, 2., 0.07);
    // overflow_text->SetTextSize(text_size);
    // overflow_text->SetFillStyle(0);
    // overflow_text->SetBorderSize(0);
    // overflow_text->AddText("overflow");

    TPaveText *test_info_text = new TPaveText(1.2, 0.2, 1.7, 0.23);
    test_info_text->SetTextSize(text_size);
    test_info_text->SetFillStyle(0);
    test_info_text->SetBorderSize(0);
    test_info_text->AddText("SPLIT TEST");

    TCanvas *c_rg = new TCanvas("c_rg", "", 1200, 1000);
    hStack_mc_rg->Draw("nostack hist");
    // h_mc_true_rg->Draw("noclear hist");
    hStack_data_rg->Draw("p same nostack");
    underflow_text->Draw();
    // overflow_text->Draw();
    // hStack_mc_rg->SetMinimum(0.1);
    // hStack_data_rg->SetMinimum(0.1);
    double maximum = 0.4;
    hStack_mc_rg->SetMaximum(maximum);
    hStack_data_rg->SetMaximum(maximum);
    leg_rg->Draw();
    test_info_text->Draw();
    c_rg->Draw();

    // RATIO PLOTS
    THStack *h_ratio = new THStack("h_ratio", "");
    h_ratio->SetTitle("; ln(0.4/R_{g}); ratio");

    TLegend *leg_ratio = new TLegend(0.2, 0.2, 0.8, 0.4);
    leg_ratio->SetFillStyle(0);
    leg_ratio->SetBorderSize(1);
    // leg_ratio->SetMargin(0.15);
    leg_ratio->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real));

    TLine *line = new TLine(-0.4, 1., 2., 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);

    TH1D *h_efficiency_corrected_true_ratio = (TH1D *) h_data_reco_efficiency_corrected_rg->Clone("h_efficiency_corrected_true_ratio");
    h_efficiency_corrected_true_ratio->Divide(h_mc_true_rg);
    h_efficiency_corrected_true_ratio->SetMarkerStyle(33);
    h_efficiency_corrected_true_ratio->SetMarkerColor(mykBlue);
    h_efficiency_corrected_true_ratio->SetMarkerSize(4);
    h_ratio->Add(h_efficiency_corrected_true_ratio);
    leg_ratio->AddEntry(h_efficiency_corrected_true_ratio, "unfolded+corrected / true", "p");

    TH1D *h_refolded_reco_purity_corrected_ratio = (TH1D *) h_data_reco_refolded_rg->Clone("h_refolded_reco_purity_corrected_ratio");
    h_refolded_reco_purity_corrected_ratio->Divide(h_data_reco_purity_corrected_rg);
    h_refolded_reco_purity_corrected_ratio->SetMarkerStyle(30);
    h_refolded_reco_purity_corrected_ratio->SetMarkerColor(mykGreen);
    h_refolded_reco_purity_corrected_ratio->SetMarkerSize(4);
    h_ratio->Add(h_refolded_reco_purity_corrected_ratio);
    leg_ratio->AddEntry(h_refolded_reco_purity_corrected_ratio, "refolded / reco+purity corrected", "p");

    TPaveText *test_info_text_ratio = new TPaveText(0., 1.3, 1., 1.4);
    test_info_text_ratio->SetTextSize(text_size);
    test_info_text_ratio->SetFillStyle(0);
    test_info_text_ratio->SetBorderSize(0);
    test_info_text_ratio->AddText("SPLIT TEST");

    TCanvas *c_ratio = new TCanvas("c_ratio", "", 1000, 800);
    h_ratio->Draw("nostack p");
    h_ratio->SetMaximum(2.5);
    h_ratio->SetMinimum(0.);
    line->Draw();
    leg_ratio->Draw();
    test_info_text_ratio->Draw();
    c_ratio->Draw();
    TString c_ratio_name = "plots/split_test_ratio.png";
    c_ratio->Print(c_ratio_name);
}