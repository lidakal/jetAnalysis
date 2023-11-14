void draw_fit_multiple(TString observable = "rg")
{
    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString option = "_glued";
    TString sample = "aggrTMVA_XXT";
    TString label_in = "data";
    label_in += "_" + sample;
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + "_" + observable + option + ".root";
    std::cout << "fin = " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get(TString("h_data"));
    TH3D *h_sig_training_dijet = (TH3D *) fin->Get("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin->Get("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin->Get("h_bkg_rest_training_dijet");
    TH3D *h_sig_training_bjet = (TH3D *) fin->Get("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin->Get("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin->Get("h_bkg_rest_training_bjet");
    TH3D *h_sig_testing_dijet = (TH3D *) fin->Get("h_sig_testing_dijet");
    TH3D *h_bkg_bb_testing_dijet = (TH3D *) fin->Get("h_bkg_bb_testing_dijet");
    TH3D *h_bkg_rest_testing_dijet = (TH3D *) fin->Get("h_bkg_rest_testing_dijet");
    TH3D *h_sig_testing_bjet = (TH3D *) fin->Get("h_sig_testing_bjet");
    TH3D *h_bkg_bb_testing_bjet = (TH3D *) fin->Get("h_bkg_bb_testing_bjet");
    TH3D *h_bkg_rest_testing_bjet = (TH3D *) fin->Get("h_bkg_rest_testing_bjet");

    // Note: combine after making the projections

    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");

    TH2D *h_sig_fraction_true_training = (TH2D *) fin->Get("h_sig_fraction_true_training");
    TH2D *h_bkg_bb_fraction_true_training = (TH2D *) fin->Get("h_bkg_bb_fraction_true_training");

    Int_t nbins_x = h_data->GetNbinsX();
    Int_t nbins_pt = h_data->GetNbinsZ();

    Int_t npads = std::round((nbins_x + 1) / 2) * 2;

    Int_t choose_bin = 2;
    for (Int_t ibin_pt = choose_bin; ibin_pt <= choose_bin; ibin_pt++) {
        TCanvas *c_mb = new TCanvas(Form("c_mb_%d", ibin_pt), "", 1600, 1000);
        // c_mb->Divide(npads / 2, 2, 0.0001, 0.0001);

        TPad *pad1 = new TPad("pad1", "",0.0,0.0,0.33,0.2);
        TPad *pad2 = new TPad("pad2", "",0.0,0.2,0.33,0.5);
        TPad *pad3 = new TPad("pad3", "",0.0,0.5,0.33,0.7);
        TPad *pad4 = new TPad("pad4", "",0.0,0.7,0.33,1.0);
        TPad *pad5 = new TPad("pad5", "",0.33,0.0,0.66,0.2);
        TPad *pad6 = new TPad("pad6", "",0.33,0.2,0.66,0.5);
        TPad *pad7 = new TPad("pad7", "",0.33,0.5,0.66,0.7);
        TPad *pad8 = new TPad("pad8", "",0.33,0.7,0.66,1.0);
        TPad *pad9 = new TPad("pad9", "",0.66,0.5,0.99,0.7);
        TPad *pad10 = new TPad("pad10", "",0.66,0.7,0.99,1.0);
        TPad *pad11 = new TPad("pad11", "",0.66,0.0,0.99,0.5);

        std::vector<TPad *> dist_pads = {pad4, pad8, pad10, pad2, pad6};
        std::vector<TPad *> ratio_pads = {pad3, pad7, pad9, pad1, pad5};

        Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
        Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

        for (Int_t ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            if (ibin_x>5) break;
            int ipad = ibin_x - 1;

            // if (ibin_x<=5) continue;
            // int ipad = ibin_x - 6;

            Double_t x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
            Double_t x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

            // Make projections
            TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Compile dijet + bjet training and testing - no need to add them all together
            TH1D *h_sig_mb = (TH1D *) h_sig_training_dijet_mb->Clone(Form("h_sig_mb_%d_%d", ibin_pt, ibin_x));
            h_sig_mb->Add(h_sig_testing_dijet_mb);
            h_sig_mb->Add(h_sig_training_bjet_mb);
            h_sig_mb->Add(h_sig_testing_bjet_mb);

            TH1D *h_bkg_bb_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone(Form("h_bkg_bb_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_bb_mb->Add(h_bkg_bb_testing_dijet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_training_bjet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_testing_bjet_mb);

            TH1D *h_bkg_rest_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone(Form("h_bkg_rest_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_rest_mb->Add(h_bkg_rest_testing_dijet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_training_bjet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_testing_bjet_mb);

            Int_t nbins_mb = h_data_mb->GetNbinsX();
            Double_t ndata = h_data_mb->Integral(1, nbins_mb);
            Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_x, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_x, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_x, ibin_pt);

            THStack *h_stack_mb = new THStack(Form("h_stack_mb_%d_%d", ibin_pt, ibin_x), "");
            h_stack_mb->SetTitle("; m_{B}^{ch}; entries");

            TLegend *leg_mb = new TLegend(0.3, 0.3, 0.7, 0.6);
            leg_mb->SetFillStyle(0);
            leg_mb->SetBorderSize(0);
            leg_mb->SetMargin(0.15);
            // leg_mb->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

            TPaveText *rg_range_text = new TPaveText(0.4, 0.7, 1., 0.8, "ndc");
            rg_range_text->SetTextSize(text_size);
            rg_range_text->SetFillStyle(0);
            rg_range_text->SetBorderSize(0);
            // rg_range_text->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
            TString xlabel;
            if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
            else if (observable=="zg") xlabel = "z_{g}";
            else if (observable=="zpt") xlabel = "z";
            if (ibin_x > 1) rg_range_text->AddText(Form("%.3f < %s < %.3f", x_min, xlabel.Data(), x_max));
            else rg_range_text->AddText("fail SD or k_{T} < 1 GeV");

            // Format histos
            // for (auto h : {h_data_mb, h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb}) {
            //     h->Sumw2();
            // }
            // h_data_mb->Sumw2();
            h_data_mb->SetMarkerStyle(kFullCircle);
            h_data_mb->SetMarkerColor(kBlack);
            h_data_mb->SetMarkerSize(1);
            leg_mb->AddEntry(h_data_mb, label_in, "pe1");

            h_sig_mb->Scale(ndata*sig_fraction/h_sig_mb->Integral(1, nbins_mb));
            h_sig_mb->SetFillStyle(1001);
            h_sig_mb->SetFillColor(kRed-7);
            h_sig_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_sig_mb);
            leg_mb->AddEntry(h_sig_mb, "signal", "f");

            h_bkg_bb_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_mb->Integral(1, nbins_mb));
            h_bkg_bb_mb->SetFillStyle(1001);
            h_bkg_bb_mb->SetFillColor(kBlue-3);
            h_bkg_bb_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_bb_mb);
            leg_mb->AddEntry(h_bkg_bb_mb, "bb bkg", "f");

            h_bkg_rest_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_mb->Integral(1, nbins_mb));
            h_bkg_rest_mb->SetFillStyle(1001);
            h_bkg_rest_mb->SetFillColor(kGreen-6);
            h_bkg_rest_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_rest_mb);
            leg_mb->AddEntry(h_bkg_rest_mb, "light+c bkg", "f");

            // make sure the fractions are correct 
            double int0 = h_sig_mb->Integral(1, nbins_mb);
            double int1 = h_bkg_bb_mb->Integral(1, nbins_mb);
            double int2 = h_bkg_rest_mb->Integral(1, nbins_mb);

            std::cout << "(" << ibin_pt << ", " << ibin_x << "):" << std::endl;
            std::cout << "\tsignal fraction = " << int0/(int0+int1+int2) << std::endl;
            std::cout << "\tbb fraction = " << int1/(int0+int1+int2) << std::endl;
            std::cout << "\tlight+c fraction = " << int2/(int0+int1+int2) << std::endl;

            dist_pads[ipad]->cd();
            // std::cout << "Top: " << dist_pads[ipad]->GetTopMargin() << std::endl;
            dist_pads[ipad]->SetTopMargin(0.15);
            dist_pads[ipad]->SetBottomMargin(0.);
            dist_pads[ipad]->SetLeftMargin(0.15);
            dist_pads[ipad]->SetRightMargin(0.08);
            h_stack_mb->Draw("hist e1");
            dist_pads[ipad]->Draw();
            TAxis *yaxis = h_stack_mb->GetYaxis();
            yaxis->SetTitleOffset(2.5);
            // h_stack_mb->SetMaximum(0.45);
            // h_stack_mb->SetMaximum(0.05);
            h_data_mb->Draw("pe1 same");
            // leg_mb->Draw();
            rg_range_text->Draw();

            // Legend and jet info in empty pad
            TPaveText *info_jets = new TPaveText(0.3, 0.65, 0.7, 0.85);
            info_jets->SetTextSize(text_size);
            info_jets->SetFillStyle(0);
            info_jets->SetBorderSize(0);
            info_jets->AddText("b tagged jets, k_{T} > 1 GeV");
            info_jets->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));

            pad11->cd();
            leg_mb->Draw();
            info_jets->Draw();

            // Plot ratio
            TLine *line = new TLine(0., 1., 7., 1.);
            line->SetLineColor(kGray);
            line->SetLineStyle(kDashed);

            TH1D *h_fit_mb = (TH1D *) h_sig_mb->Clone(Form("h_fit_mb_%d_%d", ibin_pt, ibin_x));
            h_fit_mb->Add(h_bkg_bb_mb);
            h_fit_mb->Add(h_bkg_rest_mb);
            h_fit_mb->Sumw2();

            TH1D *h_ratio = (TH1D *) h_data_mb->Clone(Form("h_ratio_%d_%d", ibin_pt, ibin_x));
            h_ratio->Divide(h_fit_mb);

            h_ratio->GetXaxis()->SetTitle("m_{B}^{ch}");
            h_ratio->GetYaxis()->SetTitle("data / fit");

            h_ratio->GetYaxis()->SetNdivisions(6);
            
            // c_mb_ratio->cd(ibin_x);
            ratio_pads[ipad]->cd();
            ratio_pads[ipad]->SetTopMargin(0.);
            ratio_pads[ipad]->SetBottomMargin(0.4);
            ratio_pads[ipad]->SetLeftMargin(0.15);
            ratio_pads[ipad]->SetRightMargin(0.08);
            // std::cout << ratio_pads[ipad]->GetLeftMargin() << std::endl;
            // std::cout << ratio_pads[ipad]->GetRightMargin() << std::endl;
            h_ratio->Draw("pe1");
            h_ratio->GetYaxis()->SetTitleOffset(2.);
            h_ratio->GetXaxis()->SetTitleOffset(4.5);
            line->Draw();



        }
        // Make decorations
        TPaveText *info_top_left = new TPaveText(0., 8.1, 4.1, 9.2, "nb ndc");
        info_top_left->SetTextSize(text_size);
        info_top_left->SetFillStyle(0);
        info_top_left->SetLineWidth(0);
        info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

        TPaveText *info_top_right = new TPaveText(2.2, 0.72, 7., 0.8, "nb ndc");
        info_top_right->SetTextSize(text_size);
        info_top_right->SetFillStyle(0);
        info_top_right->SetLineWidth(0);
        info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

        pad4->cd();
        // info_top_left->Draw();

        pad10->cd();
        // info_top_right->Draw();

        c_mb->cd();
        pad1->Draw();
        pad2->Draw();
        pad3->Draw();
        pad4->Draw();
        pad5->Draw();
        pad6->Draw();
        pad7->Draw();
        pad8->Draw();
        pad9->Draw();
        pad10->Draw();
        pad11->Draw();
        c_mb->Draw();

        TString c_mb_name = Form("./plots/data_fit_mb_%.0f_pt_%.0f.png", pt_min, pt_max);
        c_mb->Print(c_mb_name);
    }
}