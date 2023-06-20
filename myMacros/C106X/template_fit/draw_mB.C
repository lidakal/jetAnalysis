#include "../hist_utils.h"

void draw_mB()
{
    // Setup 
    TString label_out = "data";
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Load MC
    TString sample = "dijet";
    TString label = "aggrTMVA_withNb";
    // TString fin_mc_name = "./histos/qcd_" + sample + "_" + label + "_data_for_template_fit.root";
    TString fin_mc_name = "./histos/qcd_" + sample + "_" + label + "_fitted_parameters_" + label_out + ".root";
    TFile *fin_mc = new TFile(fin_mc_name);

    TH3D *hsig_dijet = (TH3D *) fin_mc->Get("hsig");
    hsig_dijet->SetName("hsig_dijet");
    TH3D *hbkg_bb_dijet = (TH3D *) fin_mc->Get("hbkg_bb");
    hbkg_bb_dijet->SetName("hbkg_bb_dijet");
    TH3D *hbkg_rest_dijet = (TH3D *) fin_mc->Get("hbkg_rest");
    hbkg_rest_dijet->SetName("hbkg_rest_dijet");

    // Load true fractions based on pseudo data
    TH2D *h_sig_fraction_true = (TH2D *) fin_mc->Get("h_sig_fraction_true");
    TH2D *h_bkg_bb_fraction_true = (TH2D *) fin_mc->Get("h_bkg_bb_fraction_true");
    TH2D *h_bkg_rest_fraction_true = (TH2D *) fin_mc->Get("h_bkg_rest_fraction_true");

    // Load b enhanced MC
    TString fin_mc_b_name = "./histos/qcd_bjet_aggrTMVA_withHLT_data_for_template_fit.root";
    TFile *fin_mc_b = new TFile(fin_mc_b_name);
    TH3D *hsig_bjet = (TH3D *) fin_mc->Get("hsig");
    hsig_bjet->SetName("hsig_bjet");
    TH3D *hbkg_bb_bjet = (TH3D *) fin_mc->Get("hbkg_bb");
    hbkg_bb_bjet->SetName("hbkg_bb_bjet");
    TH3D *hbkg_rest_bjet = (TH3D *) fin_mc->Get("hbkg_rest");
    hbkg_rest_bjet->SetName("hbkg_rest_bjet");

    // Merge MC to increase stats
    TH3D *h_mc_sig = (TH3D *) hsig_dijet->Clone("h_mc_sig");
    h_mc_sig->Add(hsig_bjet);

    TH3D *h_mc_bkg_bb = (TH3D *) hbkg_bb_dijet->Clone("h_mc_bkg_bb");
    h_mc_bkg_bb->Add(hbkg_bb_bjet);

    TH3D *h_mc_bkg_rest = (TH3D *) hbkg_rest_dijet->Clone("h_mc_bkg_rest");
    h_mc_bkg_rest->Add(hbkg_rest_bjet);

    // Load data
    TString label_data = "aggrTMVA_v2";
    TString fin_name_data = "./histos/data_" + label_data + "_raw_full.root";
    TFile *fin_data = new TFile(fin_name_data);
    TH3D *hdata = (TH3D *) fin_data->Get("hdata");

    Int_t nbins_rg = hdata->GetNbinsX();
    Int_t nbins_pt = hdata->GetNbinsZ();

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt != 1) continue;
        TCanvas *c_mB = new TCanvas(Form("c_mB_%d", ibin_pt), "", 1600, 1000);
        c_mB->Divide(nbins_rg / 2, 2, 0., 0.);
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_rg != 3) continue;

            Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
            Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

            Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

            THStack *h_mc_total_mB = new THStack(Form("h_mc_total_mB_%d_%d", ibin_pt, ibin_rg), "");
            h_mc_total_mB->SetTitle("; m_{B}^{ch}; entries");

            TLegend *leg_mB = new TLegend(0.6, 0.7, 1., 0.9, "ndc");
            leg_mB->SetFillStyle(0);
            leg_mB->SetBorderSize(0);
            leg_mB->SetMargin(0.15);
            leg_mB->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

            TPaveText *rg_range_text = new TPaveText(0.2, 0.75, 0.5, 0.9, "ndc");
            rg_range_text->SetTextSize(text_size);
            rg_range_text->SetFillStyle(0);
            rg_range_text->SetBorderSize(0);
            rg_range_text->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
            rg_range_text->AddText(Form("%.2f < ln(0.4/R_{g}) < %.2f", rg_min, rg_max));

            // Make projections
            TH1D *hdata_mB = (TH1D *) hdata->ProjectionY(Form("hdata_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_mc_sig_mB = (TH1D *) h_mc_sig->ProjectionY(Form("h_mc_sig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_mc_bkg_bb_mB = (TH1D *) h_mc_bkg_bb->ProjectionY(Form("h_mc_bkg_bb_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_mc_bkg_rest_mB = (TH1D *) h_mc_bkg_rest->ProjectionY(Form("h_mc_bkg_rest_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            // Get true fractions
            Double_t sig_fraction = h_sig_fraction_true->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction_true->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction_true->GetBinContent(ibin_rg, ibin_pt);

            // Fix normalization
            Double_t ndata = hdata_mB->Integral();
            Double_t sig_integral = h_mc_sig_mB->Integral();
            Double_t bkg_bb_integral = h_mc_bkg_bb_mB->Integral();
            Double_t bkg_rest_integral = h_mc_bkg_rest_mB->Integral();
            Double_t mB_bin_width = hdata_mB->GetBinWidth(1);

            h_mc_sig_mB->Scale(ndata * sig_fraction / (sig_integral * mB_bin_width));
            h_mc_bkg_bb_mB->Scale(ndata * bkg_bb_fraction / (bkg_bb_integral * mB_bin_width));
            h_mc_bkg_rest_mB->Scale(ndata * bkg_rest_fraction / (bkg_rest_integral * mB_bin_width));

            // Combine MC
            h_mc_sig_mB->SetFillStyle(1001);
            h_mc_sig_mB->SetFillColor(kRed-7);
            h_mc_sig_mB->SetMarkerStyle(1);
            h_mc_total_mB->Add(h_mc_sig_mB, "hist e1");
            leg_mB->AddEntry(h_mc_sig_mB, "signal MC", "f");

            h_mc_bkg_bb_mB->SetFillStyle(1001);
            h_mc_bkg_bb_mB->SetFillColor(kBlue-3);
            h_mc_bkg_bb_mB->SetMarkerStyle(1);
            h_mc_total_mB->Add(h_mc_bkg_bb_mB, "hist e1");
            leg_mB->AddEntry(h_mc_bkg_bb_mB, "bb MC", "f");

            h_mc_bkg_rest_mB->SetFillStyle(1001);
            h_mc_bkg_rest_mB->SetFillColor(kGreen-6);
            h_mc_bkg_rest_mB->SetMarkerStyle(1);
            h_mc_total_mB->Add(h_mc_bkg_rest_mB, "hist e1");
            leg_mB->AddEntry(h_mc_bkg_rest_mB, "light+c+PU MC", "f");
            
            // Format data histogram
            // hdata_mB->Scale(1/hdata_mB->Integral("width"));
            hdata_mB->SetMarkerColor(kBlack);
            hdata_mB->SetMarkerStyle(kFullCircle);
            hdata_mB->SetMarkerSize(1);
            // hStack_mB->Add(hdata_mB, "pe1");
            leg_mB->AddEntry(hdata_mB, "raw data", "pe");

            // Plot MC and raw data
            c_mB->cd(ibin_rg);
            h_mc_total_mB->Draw();
            hdata_mB->Draw("same pe1");
            // hStack_mB->SetMaximum(0.45);
            // hStack_mB->SetMaximum(0.05);
            leg_mB->Draw();
            rg_range_text->Draw();
        } // rg loop 
        c_mB->Draw();
    } // pt loop
}