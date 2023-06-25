#include "../hist_utils.h"

void draw_fractions_mc()
{
    // Setup 
    TString label_out = "pseudo_data";
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Load histograms
    TString method = "RooFit";
    TString fin_fitted_name = "./histos/fitted_parameters_" + method + "_" + label_out + ".root";
    TFile *fin_fitted = new TFile(fin_fitted_name);

    // Load true fractions based on pseudo data
    TH2D *h_sig_fraction_true_training = (TH2D *) fin_fitted->Get("h_sig_fraction_true_training");
    // TH2D *h_bkg_bb_fraction_true = (TH2D *) fin_fitted->Get("h_bkg_bb_fraction_true");
    // TH2D *h_bkg_rest_fraction_true = (TH2D *) fin_fitted->Get("h_bkg_rest_fraction_true");

    // Load fractions estimated from data
    TH2D *h_sig_fraction = (TH2D *) fin_fitted->Get("h_sig_fraction");
    // TH2D *h_bkg_bb_fraction = (TH2D *) fin_fitted->Get("h_bkg_bb_fraction");
    // TH2D *h_bkg_rest_fraction = (TH2D *) fin_fitted->Get("h_bkg_rest_fraction");

    // Load errors on fractions
    TH2D *h_sig_fraction_error = (TH2D *) fin_fitted->Get("h_sig_fraction_error");
    // TH2D *h_bkg_bb_fraction_error = (TH2D *) fin_fitted->Get("h_bkg_bb_fraction_error");
    // TH2D *h_bkg_rest_fraction_error = (TH2D *) fin_fitted->Get("h_bkg_rest_fraction_error");


    Int_t nbins_rg = h_sig_fraction->GetNbinsX();
    Int_t nbins_pt = h_sig_fraction->GetNbinsZ();

    TCanvas *c_fraction = new TCanvas("c_fraction", "", 1800, 600);
    c_fraction->Divide(3, 1, 0.0001, 0.0001);

    for (Int_t ibin_pt = 1; ibin_pt <= 3; ibin_pt++) {
        // if (ibin_pt != 2) continue;

        Double_t pt_min = h_sig_fraction->GetYaxis()->GetBinLowEdge(ibin_pt);
        Double_t pt_max = h_sig_fraction->GetYaxis()->GetBinUpEdge(ibin_pt);

        
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            Double_t sig_fraction_error = h_sig_fraction_error->GetBinContent(ibin_rg, ibin_pt);
            h_sig_fraction->SetBinError(ibin_rg, ibin_pt, sig_fraction_error);
        }

        // Make projections 
        TH1D *h_sig_fraction_true_per_pt = (TH1D *) h_sig_fraction_true_training->ProjectionX(Form("h_sig_fraction_true_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_sig_fraction_per_pt = (TH1D *) h_sig_fraction->ProjectionX(Form("h_sig_fraction_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);

        THStack *h_fractions = new THStack(Form("h_fractions_%d", ibin_pt), "");
        h_fractions->SetTitle("; ln(0.4/R_{g}); signal fraction");

        TLegend *leg_fractions = new TLegend(0.2, 0.2, 0.8, 0.4, "ndc");
        leg_fractions->SetFillStyle(0);
        leg_fractions->SetBorderSize(0);
        leg_fractions->SetMargin(0.15);
        leg_fractions->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f, k_{T} > 1 GeV", pt_min, pt_max));

        h_sig_fraction_true_per_pt->SetLineStyle(1);
        h_sig_fraction_true_per_pt->SetLineWidth(2);
        h_sig_fraction_true_per_pt->SetLineColor(kRed);
        h_sig_fraction_true_per_pt->SetMarkerStyle(1);
        h_fractions->Add(h_sig_fraction_true_per_pt, "hist e1");
        leg_fractions->AddEntry(h_sig_fraction_true_per_pt, "from MC", "l");

        h_sig_fraction_per_pt->SetMarkerStyle(kFullCircle);
        h_sig_fraction_per_pt->SetMarkerSize(1);
        h_sig_fraction_per_pt->SetMarkerColor(kBlack);
        h_fractions->Add(h_sig_fraction_per_pt, "pe1");
        leg_fractions->AddEntry(h_sig_fraction_per_pt, TString("from fit to pseudo data"), "pe1");

        c_fraction->cd(ibin_pt);
        h_fractions->Draw("nostack");
        leg_fractions->Draw();
    } // pt loop
    // Make decorations
    TPaveText *info_top_left = new TPaveText(-0.4, 0.865, 0.85, 0.94, "nb ndc");
    info_top_left->SetTextSize(text_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

    TPaveText *info_top_right = new TPaveText(0.2, 0.87, 1.6, 0.95, "nb ndc");
    info_top_right->SetTextSize(text_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");
    
    c_fraction->cd(1);
    info_top_left->Draw();

    c_fraction->cd(3);
    info_top_right->Draw();

    c_fraction->Draw();
}