void draw_fit()
{
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString sample = "bjet";
    TString label = "aggrTMVA_withNb";
    TString fin_name = "histos/qcd_" + sample + "_" + label + "_signal_fraction.root";
    TFile *fin = new TFile(fin_name);
    TH3D *hdata = (TH3D *) fin->Get("hdata");
    TH3D *hsig = (TH3D *) fin->Get("hsig");
    TH3D *hbkg = (TH3D *) fin->Get("hbkg");
    TH2D *h_total_normalization = (TH2D *) fin->Get("h_total_normalization");
    TH2D *h_signal_fraction = (TH2D *) fin->Get("h_signal_fraction");

    Int_t ibin_pt = 2;
    Int_t ibin_rg = 3;

    Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

    Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
    Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

    TH1D *hdata_mB = (TH1D *) hdata->ProjectionY(Form("hdata_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
    TH1D *hsig_mB = (TH1D *) hsig->ProjectionY(Form("hsig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
    TH1D *hbkg_mB = (TH1D *) hbkg->ProjectionY(Form("hbkg_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
    Double_t normalization = h_total_normalization->GetBinContent(ibin_rg, ibin_pt);
    Double_t fraction = h_signal_fraction->GetBinContent(ibin_rg, ibin_pt);

    THStack *hStack_mB = new THStack("hStack_mB", "");
    hStack_mB->SetTitle("; m_{B}^{ch}; entries");

    TLegend *leg_mB = new TLegend(0.6, 0.7, 1., 0.9, "ndc");
    leg_mB->SetFillStyle(0);
    leg_mB->SetBorderSize(0);
    leg_mB->SetMargin(0.15);
    leg_mB->SetHeader(Form("b tagged jets"));

    TPaveText *rg_range_text = new TPaveText(0.2, 0.75, 0.5, 0.9, "ndc");
    rg_range_text->SetTextSize(text_size);
    rg_range_text->SetFillStyle(0);
    rg_range_text->SetBorderSize(0);
    rg_range_text->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
    rg_range_text->AddText(Form("%.2f < ln(0.4/R_{g}) < %.2f", rg_min, rg_max));
    rg_range_text->AddText("k_{T} > 1 GeV");

    // Format histos
    hdata_mB->Sumw2();
    hdata_mB->SetMarkerStyle(kFullCircle);
    hdata_mB->SetMarkerColor(kBlack);
    hdata_mB->SetMarkerSize(2);
    leg_mB->AddEntry(hdata_mB, "pseudo data", "pe");

    hsig_mB->Scale(normalization*fraction);
    hsig_mB->SetFillStyle(1001);
    hsig_mB->SetFillColor(kRed-7);
    hStack_mB->Add(hsig_mB);
    leg_mB->AddEntry(hsig_mB, "signal", "f");

    hbkg_mB->Scale(normalization*(1-fraction));
    hbkg_mB->SetFillStyle(1001);
    hbkg_mB->SetFillColor(kBlue-3);
    hStack_mB->Add(hbkg_mB);
    leg_mB->AddEntry(hbkg_mB, "bkg", "f");

    TCanvas *c_mB = new TCanvas("c_mB", "", 1000, 800);
    hStack_mB->Draw("hist");
    hStack_mB->SetMaximum(0.45);
    // hStack_mB->SetMaximum(0.05);
    hdata_mB->Draw("pe1 same");
    leg_mB->Draw();
    rg_range_text->Draw();
    c_mB->Draw();

}