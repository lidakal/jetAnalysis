void draw_fit_multiple()
{
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString label_in = "data";
    TString sample = "dijet";
    TString label = "aggrTMVA_withNb";
    TString fin_name = "histos/qcd_" + sample + "_" + label + "_fitted_parameters_RooFit_" + label_in + ".root";
    TFile *fin = new TFile(fin_name);

    TH3D *hdata = (TH3D *) fin->Get("hdata");
    TH3D *hsig = (TH3D *) fin->Get("hsig");
    TH3D *hbkg_bb = (TH3D *) fin->Get("hbkg_bb");
    TH3D *hbkg_rest = (TH3D *) fin->Get("hbkg_rest");

    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_sig_fraction_error = (TH2D *) fin->Get("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) fin->Get("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) fin->Get("h_bkg_rest_fraction_error");

    Int_t nbins_rg = hdata->GetNbinsX();
    Int_t nbins_pt = hdata->GetNbinsZ();

    for (Int_t ibin_pt = 2; ibin_pt <= 2; ibin_pt++) {
        TCanvas *c_mB = new TCanvas(Form("c_mB_%d", ibin_pt), "", 1600, 1000);
        c_mB->Divide(nbins_rg / 2, 2, 0., 0.);

        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
            Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

            Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

            TH1D *hdata_mB = (TH1D *) hdata->ProjectionY(Form("hdata_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hsig_mB = (TH1D *) hsig->ProjectionY(Form("hsig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hbkg_bb_mB = (TH1D *) hbkg_bb->ProjectionY(Form("hbkg_bb_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hbkg_rest_mB = (TH1D *) hbkg_rest->ProjectionY(Form("hbkg_rest_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            
            Int_t nbins_mB = hdata_mB->GetNbinsX();
            Double_t ndata = hdata_mB->Integral(1, nbins_mB);
            Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_rg, ibin_pt);

            THStack *hStack_mB = new THStack(Form("hStack_mB_%d_%d", ibin_pt, ibin_rg), "");
            hStack_mB->SetTitle("; m_{B}^{ch}; entries");

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

            // Format histos
            hdata_mB->Sumw2();
            hdata_mB->SetMarkerStyle(kFullCircle);
            hdata_mB->SetMarkerColor(kBlack);
            hdata_mB->SetMarkerSize(2);
            leg_mB->AddEntry(hdata_mB, label_in, "pe1");

            hsig_mB->Scale(ndata*sig_fraction/hsig_mB->Integral(1, nbins_mB, "width"));
            hsig_mB->SetFillStyle(1001);
            hsig_mB->SetFillColor(kRed-7);
            hsig_mB->SetMarkerStyle(1);
            hStack_mB->Add(hsig_mB);
            leg_mB->AddEntry(hsig_mB, "signal", "f");

            hbkg_bb_mB->Scale(ndata*bkg_bb_fraction/hbkg_bb_mB->Integral(1, nbins_mB, "width"));
            hbkg_bb_mB->SetFillStyle(1001);
            hbkg_bb_mB->SetFillColor(kBlue-3);
            hbkg_bb_mB->SetMarkerStyle(1);
            hStack_mB->Add(hbkg_bb_mB);
            leg_mB->AddEntry(hbkg_bb_mB, "bb bkg", "f");

            hbkg_rest_mB->Scale(ndata*bkg_rest_fraction/hbkg_rest_mB->Integral(1, nbins_mB, "width"));
            hbkg_rest_mB->SetFillStyle(1001);
            hbkg_rest_mB->SetFillColor(kGreen-6);
            hbkg_rest_mB->SetMarkerStyle(1);
            hStack_mB->Add(hbkg_rest_mB);
            leg_mB->AddEntry(hbkg_rest_mB, "light+c+PU bkg", "f");

            c_mB->cd(ibin_rg);
            hStack_mB->Draw("hist e1");
            // hStack_mB->SetMaximum(0.45);
            // hStack_mB->SetMaximum(0.05);
            hdata_mB->Draw("pe1 same");
            leg_mB->Draw();
            rg_range_text->Draw();
        }
        c_mB->Draw();
    }

}