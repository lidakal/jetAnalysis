void draw_fit_multiple()
{
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString label_in = "data";
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + ".root";
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get(TString("h_" + label_in));
    TH3D *h_sig_training_dijet = (TH3D *) fin->Get("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin->Get("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin->Get("h_bkg_rest_training_dijet");
    TH3D *h_sig_training_bjet = (TH3D *) fin->Get("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin->Get("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin->Get("h_bkg_rest_training_bjet");

    // Note: combine after making the projections

    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_sig_fraction_error = (TH2D *) fin->Get("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    // TH2D *h_bkg_bb_fraction_error = (TH2D *) fin->Get("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");
    // TH2D *h_bkg_rest_fraction_error = (TH2D *) fin->Get("h_bkg_rest_fraction_error");

    Int_t nbins_rg = h_data->GetNbinsX();
    Int_t nbins_pt = h_data->GetNbinsZ();

    Int_t npads = std::round((nbins_rg + 1) / 2) * 2;

    Int_t choose_bin = 2;
    for (Int_t ibin_pt = choose_bin; ibin_pt <= choose_bin; ibin_pt++) {
        TCanvas *c_mb = new TCanvas(Form("c_mb_%d", ibin_pt), "", 1600, 1000);
        c_mb->Divide(npads / 2, 2, 0.0001, 0.0001);

        TCanvas *c_mb_ratio = new TCanvas(Form("c_mb_ratio_%d", ibin_pt), "", 1600, 1000);
        c_mb_ratio->Divide(npads / 2, 2, 0.0001, 0.0001);

        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            Double_t pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
            Double_t pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

            Double_t rg_min = h_data->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = h_data->GetXaxis()->GetBinUpEdge(ibin_rg);

            TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            // Compile dijet + bjet training - no need to add them all together
            TH1D *h_sig_training_mb = (TH1D *) h_sig_training_dijet_mb->Clone("h_sig_training_mb");
            h_sig_training_mb->Add(h_sig_training_bjet_mb);

            TH1D *h_bkg_bb_training_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone("h_bkg_bb_training_mb");
            h_bkg_bb_training_mb->Add(h_bkg_bb_training_bjet_mb);

            TH1D *h_bkg_rest_training_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone("h_bkg_rest_training_mb");
            h_bkg_rest_training_mb->Add(h_bkg_rest_training_bjet_mb);

            Int_t nbins_mb = h_data_mb->GetNbinsX();
            Double_t ndata = h_data_mb->Integral(1, nbins_mb);
            Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_rg, ibin_pt);

            THStack *hStack_mb = new THStack(Form("hStack_mb_%d_%d", ibin_pt, ibin_rg), "");
            hStack_mb->SetTitle("; m_{B}^{ch}; entries");

            TLegend *leg_mb = new TLegend(0.6, 0.7, 1., 0.9, "ndc");
            leg_mb->SetFillStyle(0);
            leg_mb->SetBorderSize(0);
            leg_mb->SetMargin(0.15);
            leg_mb->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

            TPaveText *rg_range_text = new TPaveText(0.2, 0.75, 0.5, 0.9, "ndc");
            rg_range_text->SetTextSize(text_size);
            rg_range_text->SetFillStyle(0);
            rg_range_text->SetBorderSize(0);
            rg_range_text->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
            rg_range_text->AddText(Form("%.2f < ln(0.4/R_{g}) < %.2f", rg_min, rg_max));

            // Format histos
            // for (auto h : {h_data_mb, h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb}) {
            //     h->Sumw2();
            // }
            // h_data_mb->Sumw2();
            h_data_mb->SetMarkerStyle(kFullCircle);
            h_data_mb->SetMarkerColor(kBlack);
            h_data_mb->SetMarkerSize(1);
            leg_mb->AddEntry(h_data_mb, label_in, "pe1");

            h_sig_training_mb->Scale(ndata*sig_fraction/h_sig_training_mb->Integral(1, nbins_mb));
            h_sig_training_mb->SetFillStyle(1001);
            h_sig_training_mb->SetFillColor(kRed-7);
            h_sig_training_mb->SetMarkerStyle(1);
            hStack_mb->Add(h_sig_training_mb);
            leg_mb->AddEntry(h_sig_training_mb, "signal", "f");

            h_bkg_bb_training_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_training_mb->Integral(1, nbins_mb));
            h_bkg_bb_training_mb->SetFillStyle(1001);
            h_bkg_bb_training_mb->SetFillColor(kBlue-3);
            h_bkg_bb_training_mb->SetMarkerStyle(1);
            hStack_mb->Add(h_bkg_bb_training_mb);
            leg_mb->AddEntry(h_bkg_bb_training_mb, "bb bkg", "f");

            h_bkg_rest_training_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_training_mb->Integral(1, nbins_mb));
            h_bkg_rest_training_mb->SetFillStyle(1001);
            h_bkg_rest_training_mb->SetFillColor(kGreen-6);
            h_bkg_rest_training_mb->SetMarkerStyle(1);
            hStack_mb->Add(h_bkg_rest_training_mb);
            leg_mb->AddEntry(h_bkg_rest_training_mb, "light+c bkg", "f");

            c_mb->cd(ibin_rg);
            hStack_mb->Draw("hist e1");
            // hStack_mb->SetMaximum(0.45);
            // hStack_mb->SetMaximum(0.05);
            h_data_mb->Draw("pe1 same");
            leg_mb->Draw();
            rg_range_text->Draw();

            // Plot ratio
            TLine *line = new TLine(0., 1., 7., 1.);
            line->SetLineColor(kGray);
            line->SetLineStyle(kDashed);

            TH1D *h_fit_mb = (TH1D *) h_sig_training_mb->Clone("h_fit_mb");
            h_fit_mb->Add(h_bkg_bb_training_mb);
            h_fit_mb->Add(h_bkg_rest_training_mb);
            h_fit_mb->Sumw2();

            TH1D *h_ratio = (TH1D *) h_data_mb->Clone("h_ratio");
            h_ratio->Divide(h_fit_mb);

            h_ratio->GetXaxis()->SetTitle("m_{B}^{ch}");
            h_ratio->GetYaxis()->SetTitle(TString(label_in + " / fit"));
            
            c_mb_ratio->cd(ibin_rg);
            h_ratio->Draw("pe1");
            line->Draw();
            // hStack_mb->SetMaximum(0.45);
            // hStack_mb->SetMaximum(0.05);
            rg_range_text->Draw();
        }
        c_mb->Draw();
        c_mb_ratio->Draw();
    }

}