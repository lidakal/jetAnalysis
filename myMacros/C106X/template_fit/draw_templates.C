#include "../hist_utils.h"

void draw_templates()
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
        if (ibin_pt != 2) continue;

        // Plot integrated over rg
        Int_t n_bins_rg = hdata->GetNbinsX();
        TH1D *hdata_per_pt = (TH1D *) hdata->ProjectionY(Form("hdata_per_pt_%d", ibin_pt), 1, n_bins_rg, ibin_pt, ibin_pt);
        TH1D *h_mc_sig_per_pt = (TH1D *) h_mc_sig->ProjectionY(Form("h_mc_sig_per_pt_%d", ibin_pt), 1, n_bins_rg, ibin_pt, ibin_pt);
        TH1D *h_mc_bkg_bb_per_pt = (TH1D *) h_mc_bkg_bb->ProjectionY(Form("h_mc_bkg_bb_per_pt_%d", ibin_pt), 1, n_bins_rg, ibin_pt, ibin_pt);
        TH1D *h_mc_bkg_rest_per_pt = (TH1D *) h_mc_bkg_rest->ProjectionY(Form("h_mc_bkg_rest_per_pt_%d", ibin_pt), 1, n_bins_rg, ibin_pt, ibin_pt);

        for (auto h : {hdata_per_pt, 
                       h_mc_sig_per_pt,
                       h_mc_bkg_bb_per_pt,
                       h_mc_bkg_rest_per_pt}) {
                        h->Scale(1/h->Integral("width"));
                       }

        THStack *h_per_pt = new THStack("h_per_pt", "");
        h_per_pt->SetTitle("; m_{B}^{ch}; 1/N_{jets} dN/dm_{B}^{ch}");

        TLegend *leg_per_pt = new TLegend(0.6, 0.7, 1., 0.9, "ndc");
        leg_per_pt->SetFillStyle(0);
        leg_per_pt->SetBorderSize(0);
        leg_per_pt->SetMargin(0.15);
        leg_per_pt->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

        hdata_per_pt->SetMarkerStyle(kFullStar);
        hdata_per_pt->SetMarkerColor(kGreen);
        hdata_per_pt->SetMarkerSize(2);
        h_per_pt->Add(hdata_per_pt, "pe1");
        leg_per_pt->AddEntry(hdata_per_pt, "data", "pe");

        h_mc_sig_per_pt->SetMarkerStyle(kFullCircle);
        h_mc_sig_per_pt->SetMarkerColor(kBlack);
        h_mc_sig_per_pt->SetMarkerSize(1);
        h_per_pt->Add(h_mc_sig_per_pt, "pe1");
        leg_per_pt->AddEntry(h_mc_sig_per_pt, "b", "pe");

        h_mc_bkg_bb_per_pt->SetMarkerStyle(kFullDiamond);
        h_mc_bkg_bb_per_pt->SetMarkerColor(kBlue);
        h_mc_bkg_bb_per_pt->SetMarkerSize(2);
        h_per_pt->Add(h_mc_bkg_bb_per_pt, "pe1");
        leg_per_pt->AddEntry(h_mc_bkg_bb_per_pt, "bb", "pe");

        h_mc_bkg_rest_per_pt->SetMarkerStyle(kFullCross);
        h_mc_bkg_rest_per_pt->SetMarkerColor(kRed);
        h_mc_bkg_rest_per_pt->SetMarkerSize(2);
        h_per_pt->Add(h_mc_bkg_rest_per_pt, "pe1");
        leg_per_pt->AddEntry(h_mc_bkg_rest_per_pt, "l+c", "pe");

        TCanvas *c_per_pt = new TCanvas(Form("c_per_pt_%d", ibin_pt), "", 1000, 800);
        h_per_pt->Draw("nostack");
        leg_per_pt->Draw();
        c_per_pt->Draw();

        // PLOT RATIOS
        THStack *h_ratio = new THStack("h_ratio", "");
        h_ratio->SetTitle("; m_{B}^{ch}; ratio");

        TLegend *leg_ratio = new TLegend(0.6, 0.7, 1., 0.9, "ndc");
        leg_ratio->SetFillStyle(0);
        leg_ratio->SetBorderSize(0);
        leg_ratio->SetMargin(0.15);
        leg_ratio->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));
        
        TH1D *h_b_ratio = (TH1D *) h_mc_sig_per_pt->Clone("h_b_ratio");
        h_b_ratio->Divide(hdata_per_pt);
        h_b_ratio->SetMarkerStyle(kFullCircle);
        h_b_ratio->SetMarkerColor(kBlack);
        h_b_ratio->SetMarkerSize(1);
        h_ratio->Add(h_b_ratio, "pe1");
        leg_ratio->AddEntry(h_b_ratio, "b / data", "pe");

        TH1D *h_bb_ratio = (TH1D *) h_mc_bkg_bb_per_pt->Clone("h_bb_ratio");
        h_bb_ratio->Divide(hdata_per_pt);
        h_bb_ratio->SetMarkerStyle(kFullDiamond);
        h_bb_ratio->SetMarkerColor(kBlue);
        h_bb_ratio->SetMarkerSize(2);
        h_ratio->Add(h_bb_ratio, "pe1");
        leg_ratio->AddEntry(h_bb_ratio, "bb / data", "pe");

        TH1D *h_lc_ratio = (TH1D *) h_mc_bkg_rest_per_pt->Clone("h_lc_ratio");
        h_lc_ratio->Divide(hdata_per_pt);
        h_lc_ratio->SetMarkerStyle(kFullCross);
        h_lc_ratio->SetMarkerColor(kRed);
        h_lc_ratio->SetMarkerSize(2);
        h_ratio->Add(h_lc_ratio, "pe1");
        leg_ratio->AddEntry(h_lc_ratio, "l+c / data", "pe");

        TCanvas *c_ratio_per_pt = new TCanvas(Form("c_ratio_per_pt_%d", ibin_pt), "", 1000, 800);
        h_ratio->Draw("nostack");
        leg_ratio->Draw();
        c_ratio_per_pt->Draw();

        TCanvas *c_mB = new TCanvas(Form("c_mB_%d", ibin_pt), "", 1600, 1000);
        c_mB->Divide(nbins_rg / 2, 2, 0., 0.);
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_rg != 3) continue;

            Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
            Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

            Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

            THStack *h_ratio_mB = new THStack(Form("h_ratio_mB_%d_%d", ibin_pt, ibin_rg), "");
            h_ratio_mB->SetTitle("; m_{B}^{ch}; ratio");

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
            hdata_mB->Scale(1/hdata_mB->Integral("width"));
            h_mc_sig_mB->Scale(1/h_mc_sig_mB->Integral("width"));
            h_mc_bkg_bb_mB->Scale(1/h_mc_bkg_bb_mB->Integral("width"));
            h_mc_bkg_rest_mB->Scale(1/h_mc_bkg_rest_mB->Integral("width"));

            // Take ratios 
            TH1D *h_b_ratio = (TH1D *) h_mc_sig_mB->Clone("h_b_ratio");
            h_b_ratio->Divide(hdata_mB);
            h_b_ratio->SetMarkerStyle(kFullCircle);
            h_b_ratio->SetMarkerColor(kBlack);
            h_b_ratio->SetMarkerSize(1);
            h_ratio_mB->Add(h_b_ratio, "pe1");
            leg_mB->AddEntry(h_b_ratio, "b / data", "pe");

            TH1D *h_bb_ratio = (TH1D *) h_mc_bkg_bb_mB->Clone("h_bb_ratio");
            h_bb_ratio->Divide(hdata_mB);
            h_bb_ratio->SetMarkerStyle(kFullDiamond);
            h_bb_ratio->SetMarkerColor(kBlue);
            h_bb_ratio->SetMarkerSize(2);
            h_ratio_mB->Add(h_bb_ratio, "pe1");
            leg_mB->AddEntry(h_bb_ratio, "bb / data", "pe");

            TH1D *h_lc_ratio = (TH1D *) h_mc_bkg_rest_mB->Clone("h_lc_ratio");
            h_lc_ratio->Divide(hdata_mB);
            h_lc_ratio->SetMarkerStyle(kFullCross);
            h_lc_ratio->SetMarkerColor(kRed);
            h_lc_ratio->SetMarkerSize(2);
            h_ratio_mB->Add(h_lc_ratio, "pe1");
            leg_mB->AddEntry(h_lc_ratio, "l+c / data", "pe");

            // Plot 
            c_mB->cd(ibin_rg);
            h_ratio_mB->Draw("nostack");
            // hdata_mB->Draw("same pe1");
            // hStack_mB->SetMaximum(0.45);
            // hStack_mB->SetMaximum(0.05);
            leg_mB->Draw();
            rg_range_text->Draw();
        } // rg loop 
        c_mB->Draw();
    } // pt loop
}