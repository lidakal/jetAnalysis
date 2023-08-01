void draw_fit_multiple_mc()
{
    // MADE FOR 5 RG BINS (INCLUDING UNTAGGED)

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString label_in = "pseudo_data";
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + ".root";
    TFile *fin = new TFile(fin_name);

    // Load dijet MC
    TString fin_mc_dijet_name = "./histos/dijet_aggrTMVA_withHLT_v2_histograms.root";
    TFile *fin_mc_dijet = new TFile(fin_mc_dijet_name);

    TH3D *h_sig_training_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_training")->Clone("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_dijet");

    TH3D *h_sig_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_testing")->Clone("h_sig_testing_dijet");
    TH3D *h_bkg_bb_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_dijet");
    TH3D *h_bkg_rest_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_dijet");

    // Load bjet MC 
    TString fin_mc_bjet_name = "./histos/bjet_aggrTMVA_withHLT_histograms.root";
    TFile *fin_mc_bjet = new TFile(fin_mc_bjet_name);

    TH3D *h_sig_training_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_training")->Clone("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_bjet");

    TH3D *h_sig_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_testing")->Clone("h_sig_testing_bjet");
    TH3D *h_bkg_bb_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_bjet");
    TH3D *h_bkg_rest_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_bjet");

    // Fix statistical uncertainties on all histograms
    for (auto h : {
                   h_sig_training_dijet, h_sig_testing_dijet,
                   h_bkg_bb_training_dijet, h_bkg_bb_testing_dijet,
                   h_bkg_rest_training_dijet, h_bkg_rest_testing_dijet,
                   h_sig_training_bjet, h_sig_testing_bjet,
                   h_bkg_bb_training_bjet, h_bkg_bb_testing_bjet,
                   h_bkg_rest_training_bjet, h_bkg_rest_testing_bjet,
                   }) {
                    h->Sumw2();
                   }

    // Merge MC after making projections -> it's easier

    // Load fractions
    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_sig_fraction_error = (TH2D *) fin->Get("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) fin->Get("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) fin->Get("h_bkg_rest_fraction_error");

    Int_t nbins_rg = h_sig_training_dijet->GetNbinsX();
    Int_t nbins_mb = h_sig_training_dijet->GetNbinsY();
    Int_t nbins_pt = h_sig_training_dijet->GetNbinsZ();

    Int_t npads = std::round((nbins_rg + 1) / 2) * 2;

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

        // TCanvas *c_mb_ratio = new TCanvas(Form("c_mb_ratio_%d", ibin_pt), "", 1600, 1000);
        // c_mb_ratio->Divide(npads / 2, 2, 0.0001, 0.0001);

        Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
        Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            Double_t rg_min = h_sig_training_dijet->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = h_sig_training_dijet->GetXaxis()->GetBinUpEdge(ibin_rg);

            // Make projections
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            // Compile dijet + bjet training - no need to add them all together
            TH1D *h_sig_training_mb = (TH1D *) h_sig_training_dijet_mb->Clone("h_sig_training_mb");
            h_sig_training_mb->Add(h_sig_training_bjet_mb);

            TH1D *h_bkg_bb_training_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone("h_bkg_bb_training_mb");
            h_bkg_bb_training_mb->Add(h_bkg_bb_training_bjet_mb);

            TH1D *h_bkg_rest_training_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone("h_bkg_rest_training_mb");
            h_bkg_rest_training_mb->Add(h_bkg_rest_training_bjet_mb);

            // Compile dijet + bjet testing - add them all together with original fractions for pseudo_data
            double int1 = h_sig_testing_dijet_mb->Integral(1, nbins_mb);
            double int2 = h_bkg_bb_testing_dijet_mb->Integral(1, nbins_mb);
            double int3 = h_bkg_rest_testing_dijet_mb->Integral(1, nbins_mb);
            double total = int1 + int2 + int3;
            double c1 = int1 / total;
            double c2 = int2 / total;
            double c3 = int3 / total;
            // h_testing_mb = total * (c1 * h_sig + c2 * h_bkg_bb + c3 * h_bkg_rest)
            TH1D *h_sig_testing_mb = (TH1D *) h_sig_testing_dijet_mb->Clone("h_sig_testing_mb");
            h_sig_testing_mb->Add(h_sig_testing_bjet_mb);
            h_sig_testing_mb->Scale(1 / h_sig_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_bb_testing_mb = (TH1D *) h_bkg_bb_testing_dijet_mb->Clone("h_bkg_bb_testing_mb");
            h_bkg_bb_testing_mb->Add(h_bkg_bb_testing_bjet_mb);
            h_bkg_bb_testing_mb->Scale(1 / h_bkg_bb_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_rest_testing_mb = (TH1D *) h_bkg_rest_testing_dijet_mb->Clone("h_bkg_rest_testing_mb");
            h_bkg_rest_testing_mb->Add(h_bkg_rest_testing_bjet_mb);
            h_bkg_rest_testing_mb->Scale(1 / h_bkg_rest_testing_mb->Integral(1, nbins_mb));

            TH1D *h_testing_mb = (TH1D *) h_sig_testing_mb->Clone("h_testing_mb");
            h_testing_mb->Scale(c1);
            h_testing_mb->Add(h_bkg_bb_testing_mb, c2);
            h_testing_mb->Add(h_bkg_rest_testing_mb, c3);

            h_testing_mb->Scale(total / h_testing_mb->Integral(1, nbins_mb));

            // Get fractions from fit for this pt, rg bin
            Double_t ndata = h_testing_mb->Integral(1, nbins_mb);
            std::cout << "total = " << total << ", ndata = " << ndata << std::endl;
            Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_rg, ibin_pt);
            Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_rg, ibin_pt);

            std::cout << "total fractions = " << sig_fraction+bkg_bb_fraction+bkg_rest_fraction << std::endl;

            THStack *h_stack_mb = new THStack(Form("h_stack_mb_%d_%d", ibin_pt, ibin_rg), "");
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
            if (ibin_rg > 1) rg_range_text->AddText(Form("%.2f < ln(#frac{0.4}{R_{g}}) < %.2f", rg_min, rg_max));
            else rg_range_text->AddText("fail SD or k_{T} < 1 GeV");

            // Format histos
            for (auto h : {h_testing_mb, h_sig_training_mb, h_bkg_bb_training_mb, h_bkg_rest_training_mb}) {
                h->Sumw2();
            }
            // h_testing_mb->Sumw2();
            h_testing_mb->SetMarkerStyle(kFullCircle);
            h_testing_mb->SetMarkerColor(kBlack);
            h_testing_mb->SetMarkerSize(1);
            leg_mb->AddEntry(h_testing_mb, label_in, "pe1");

            h_sig_training_mb->Scale(ndata*sig_fraction/h_sig_training_mb->Integral(1, nbins_mb));
            h_sig_training_mb->SetFillStyle(1001);
            h_sig_training_mb->SetFillColor(kRed-7);
            h_sig_training_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_sig_training_mb);
            leg_mb->AddEntry(h_sig_training_mb, "signal", "f");

            h_bkg_bb_training_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_training_mb->Integral(1, nbins_mb));
            h_bkg_bb_training_mb->SetFillStyle(1001);
            h_bkg_bb_training_mb->SetFillColor(kBlue-3);
            h_bkg_bb_training_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_bb_training_mb);
            leg_mb->AddEntry(h_bkg_bb_training_mb, "bb bkg", "f");

            h_bkg_rest_training_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_training_mb->Integral(1, nbins_mb));
            h_bkg_rest_training_mb->SetFillStyle(1001);
            h_bkg_rest_training_mb->SetFillColor(kGreen-6);
            h_bkg_rest_training_mb->SetMarkerStyle(1);
            h_stack_mb->Add(h_bkg_rest_training_mb);
            leg_mb->AddEntry(h_bkg_rest_training_mb, "guds+c bkg", "f");

            // c_mb->cd(ibin_rg);

            dist_pads[ibin_rg-1]->cd();
            // std::cout << "Top: " << dist_pads[ibin_rg-1]->GetTopMargin() << std::endl;
            dist_pads[ibin_rg-1]->SetTopMargin(0.15);
            dist_pads[ibin_rg-1]->SetBottomMargin(0.);
            dist_pads[ibin_rg-1]->SetLeftMargin(0.15);
            dist_pads[ibin_rg-1]->SetRightMargin(0.08);
            h_stack_mb->Draw("hist e1");
            dist_pads[ibin_rg-1]->Draw();
            TAxis *yaxis = h_stack_mb->GetYaxis();
            yaxis->SetTitleOffset(2.5);
            // h_stack_mb->SetMaximum(0.45);
            // h_stack_mb->SetMaximum(0.05);
            h_testing_mb->Draw("pe1 same");
            // leg_mb->Draw();
            rg_range_text->Draw();

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

            TH1D *h_fit_mb = (TH1D *) h_sig_training_mb->Clone("h_fit_mb");
            h_fit_mb->Add(h_bkg_bb_training_mb);
            h_fit_mb->Add(h_bkg_rest_training_mb);
            h_fit_mb->Sumw2();

            TH1D *h_ratio = (TH1D *) h_testing_mb->Clone("h_ratio");
            h_ratio->Divide(h_fit_mb);

            h_ratio->GetXaxis()->SetTitle("m_{B}^{ch}");
            h_ratio->GetYaxis()->SetTitle("pseudo data / fit");

            h_ratio->GetYaxis()->SetNdivisions(6);
            
            // c_mb_ratio->cd(ibin_rg);
            ratio_pads[ibin_rg-1]->cd();
            ratio_pads[ibin_rg-1]->SetTopMargin(0.);
            ratio_pads[ibin_rg-1]->SetBottomMargin(0.4);
            ratio_pads[ibin_rg-1]->SetLeftMargin(0.15);
            ratio_pads[ibin_rg-1]->SetRightMargin(0.08);
            // std::cout << ratio_pads[ibin_rg-1]->GetLeftMargin() << std::endl;
            // std::cout << ratio_pads[ibin_rg-1]->GetRightMargin() << std::endl;
            h_ratio->Draw("pe1");
            h_ratio->GetYaxis()->SetTitleOffset(2.);
            h_ratio->GetXaxis()->SetTitleOffset(4.5);
            line->Draw();
            // // h_stack_mb->SetMaximum(0.45);
            // // h_stack_mb->SetMaximum(0.05);
            // rg_range_text->Draw();
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
        info_top_left->Draw();

        pad10->cd();
        info_top_right->Draw();

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

        TString c_mb_name = Form("./plots/pseudo_data_fit_mb_%.0f_pt_%.0f.png", pt_min, pt_max);
        c_mb->Print(c_mb_name);
    }

}