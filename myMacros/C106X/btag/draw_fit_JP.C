void draw_fit_JP(TString observable="rg", bool tagged=false) 
{
    TString label = "aggrTMVA_inclusive";
    label += (tagged ? "_tagged" : "");

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    // Setup plot
    Float_t text_size = 15.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab histograms 
    TString fin_name = "histos/"+observable+"_fit_result_JP_" + label + ".root";
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get("h_data");
    TH3D *h_bbb = (TH3D *) fin->Get("h_bbb");
    TH3D *h_c = (TH3D *) fin->Get("h_c");
    TH3D *h_l = (TH3D *) fin->Get("h_l");

    TH2D *h_bbb_f = (TH2D *) fin->Get("h_bbb_f");
    TH2D *h_c_f = (TH2D *) fin->Get("h_c_f");
    TH2D *h_l_f = (TH2D *) fin->Get("h_l_f"); 

    for (auto h : {h_data, h_bbb, h_c, h_l}) {
        // h->RebinX(h->GetNbinsX()); // rebin only in x
    }

    TH2D *h_bbb_f_mc = (TH2D *) fin->Get("h_bbb_f_mc");

    // Draw fraction from fit + error
    gStyle->SetPaintTextFormat(".3f"); 
    TCanvas *c_bbb_f = new TCanvas("c_bbb_f", "", 800, 600);
    h_bbb_f->GetXaxis()->SetTitle(xlabel);
    h_bbb_f->GetYaxis()->SetTitle("p_{T}^{jet}");
    h_bbb_f->SetMarkerSize(800);
    h_bbb_f->Draw("colz texte");

    double x1 = h_bbb_f->GetXaxis()->GetBinLowEdge(h_bbb_f->GetNbinsX());
    double x2 = h_bbb_f->GetXaxis()->GetBinUpEdge(h_bbb_f->GetNbinsX());
    double y1 = h_bbb_f->GetYaxis()->GetBinLowEdge(1);
    double y2 = h_bbb_f->GetYaxis()->GetBinUpEdge(h_bbb_f->GetNbinsY());
    TPaveText *overflow = new TPaveText(x1, y1, x2, y2);
    overflow->SetBorderSize(1);
    overflow->SetFillStyle(3644);
    auto overflow_txt = overflow->AddText("overflow");
    overflow_txt->SetTextAngle(90);
    overflow_txt->SetTextSize(20);
    if (observable=="rg") overflow->Draw();

    // Draw fbincl + purity from mc
    TCanvas *c_purity = new TCanvas("c_purity", "", 1000, 600);
    c_purity->Divide(1,3);

    // Loop over jet pt+observable bins
    int nbins_x = h_bbb->GetNbinsX();
    int nbins_jp = h_bbb->GetNbinsY();
    int nbins_pt = h_bbb->GetNbinsZ();

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        double pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

        c_purity->cd(ibin_pt);
        TLegend *leg_purity = new TLegend(0.5, 0.6, 0.8, 0.9);
        leg_purity->SetFillStyle(0);
        leg_purity->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

        TH1D *h_bbb_f_1d = (TH1D *) h_bbb_f->ProjectionX(Form("h_bbb_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bbb_f_1d->SetMarkerStyle(kFullCircle);
        h_bbb_f_1d->SetMarkerColor(kBlack);
        h_bbb_f_1d->SetLineColor(kBlack);
        h_bbb_f_1d->GetYaxis()->SetTitle("b fraction");
        leg_purity->AddEntry(h_bbb_f_1d, "data (fit)", "pe1");
        h_bbb_f_1d->Draw("pe1 same");

        TH1D *h_bbb_f_mc_1d = (TH1D *) h_bbb_f_mc->ProjectionX(Form("h_bbb_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bbb_f_mc_1d->SetMarkerStyle(kFullCross);
        h_bbb_f_mc_1d->SetMarkerColor(kGreen);
        h_bbb_f_mc_1d->SetLineColor(kGreen);
        h_bbb_f_mc_1d->GetYaxis()->SetTitle("b fraction");
        leg_purity->AddEntry(h_bbb_f_mc_1d, "mc", "pe1");
        h_bbb_f_mc_1d->Draw("pe1 same");

        double ymin = std::min({h_bbb_f_1d->GetMinimum(), h_bbb_f_mc_1d->GetMinimum()});
        double ymax = std::max({h_bbb_f_1d->GetMaximum(), h_bbb_f_mc_1d->GetMaximum()});

        ymin *= 0.9;
        ymax *= 1.1;

        h_bbb_f_1d->GetYaxis()->SetRangeUser(ymin, ymax);

        leg_purity->Draw();

        // Create canvas + pads for data+fit
        TCanvas *c_jp = new TCanvas(Form("c_jp_%d", ibin_pt), "", 1200, 1000);

        TPad *pad11 = new TPad(Form("pad11_%d", ibin_pt),"",0.,0.,0.33,0.33);
        TPad *pad12 = new TPad(Form("pad12_%d", ibin_pt),"",0.33,0.,0.66,0.33);
        TPad *pad13 = new TPad(Form("pad13_%d", ibin_pt),"",0.66,0.,0.99,0.33);
        TPad *pad21 = new TPad(Form("pad21_%d", ibin_pt),"",0.,0.33,0.33,0.66);
        TPad *pad22 = new TPad(Form("pad22_%d", ibin_pt),"",0.33,0.33,0.66,0.66);
        TPad *pad23 = new TPad(Form("pad23_%d", ibin_pt),"",0.66,0.33,0.99,0.66);
        TPad *pad31 = new TPad(Form("pad31_%d", ibin_pt),"",0.,0.66,0.33,0.99);
        TPad *pad32 = new TPad(Form("pad32_%d", ibin_pt),"",0.33,0.66,0.66,0.99);
        TPad *pad33 = new TPad(Form("pad33_%d", ibin_pt),"",0.66,0.66,0.99,0.99);

        std::vector<TPad *> pads = {pad31, pad32, pad33, 
                                    pad21, pad22, pad23,
                                    pad11, pad12, pad13};
        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            // if (ibin_pt != 2) continue;
            // if (ibin_x != 3) continue;

            double x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
            double x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

            TString header = (observable!="zpt" && ibin_x==1) ? "SD-untagged OR k_{T} < 1 GeV" : Form("%.2f < %s < %.2f", x_min, xlabel.Data(), x_max);

            // Make projections 
            TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_1d = (TH1D *) h_bbb->ProjectionY(Form("h_bbb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_c_1d = (TH1D *) h_c->ProjectionY(Form("h_c_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_l_1d = (TH1D *) h_l->ProjectionY(Form("h_l_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            for (auto h : {h_data_1d, h_bbb_1d, h_c_1d, h_l_1d}) {
                h->GetXaxis()->SetRange(1, h->GetNbinsX());
            }
            // Get fractions
            double bbb_f = h_bbb_f->GetBinContent(ibin_x, ibin_pt);
            double c_f = h_c_f->GetBinContent(ibin_x, ibin_pt);
            double l_f = h_l_f->GetBinContent(ibin_x, ibin_pt);
            // std::cout << h_c_1d->Integral() << std::endl;

            // Normalize + multiply by fraction + stack 
            double ndata = h_data_1d->Integral(1, nbins_jp);
            THStack *h_fit = new THStack(Form("h_fit_%d_%d", ibin_pt, ibin_x), "");

            TLegend *leg_jp = new TLegend(0.4, 0.55, 0.8, 0.95);
            leg_jp->SetFillStyle(0);
            leg_jp->SetBorderSize(0);
            leg_jp->SetHeader(header);
            // leg_jp->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

            // std::cout << bbb_f << std::endl;
            h_bbb_1d->Scale(ndata*bbb_f/h_bbb_1d->Integral(1, nbins_jp));
            h_bbb_1d->SetFillColor(kRed);
            h_bbb_1d->SetFillStyle(1001);
            h_bbb_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_bbb_1d, "hist");
            leg_jp->AddEntry(h_bbb_1d, Form("b, %.2f%%", bbb_f*100), "f");

            h_c_1d->Scale(ndata*c_f/h_c_1d->Integral(1, nbins_jp));
            h_c_1d->SetFillColor(kGreen);
            h_c_1d->SetFillStyle(1001);
            h_c_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_c_1d, "hist");
            leg_jp->AddEntry(h_c_1d, Form("c, %.2f%%", c_f*100), "f");

            if (h_l_1d->Integral(1, nbins_jp) > 0) h_l_1d->Scale(ndata*l_f/h_l_1d->Integral(1, nbins_jp));
            h_l_1d->SetFillColor(kBlue);
            h_l_1d->SetFillStyle(1001);
            h_l_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_l_1d, "hist");
            leg_jp->AddEntry(h_l_1d, Form("l, %.2f%%", l_f*100), "f");

            h_data_1d->SetMarkerStyle(kFullCircle);
            h_data_1d->GetXaxis()->SetTitle("JP discr");
            leg_jp->AddEntry(h_data_1d, "data", "pe1");

            TPad *current_pad = pads[ibin_x-1];
            current_pad->SetRightMargin(0.1);
            current_pad->SetLeftMargin(0.12);
            current_pad->SetTopMargin(0.05);
            current_pad->SetBottomMargin(0.1);

            current_pad->cd();
            if (!tagged) current_pad->SetLogy();
            
            h_fit->Draw();
            h_data_1d->Draw("pe1 same");
            leg_jp->Draw();

            c_jp->cd();
            current_pad->Draw();
        } // observable bins

        TPaveText *pt_info_txt = new TPaveText(0.3, 0.3, 0.7, 0.7);
        pt_info_txt->SetTextSize(20);
        pt_info_txt->SetBorderSize(0);
        pt_info_txt->SetFillStyle(0);
        pt_info_txt->AddText(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

        pads[nbins_x]->cd();
        pt_info_txt->Draw();

        c_jp->cd();
        pads[nbins_x]->Draw();
    } // pt bins

    // Temporary (bins only in pt)
    // TH1D *h_bbb_f_pt = (TH1D *) h_bbb_f->ProjectionY();
    // h_bbb_f_pt->GetXaxis()->SetTitle("p_{T}^{jet}");
    // h_bbb_f_pt->GetYaxis()->SetTitle("b fraction");


    // TH1D *h_bbb_f_mc_pt = (TH1D *) h_bbb_f_mc->ProjectionY();
    // h_bbb_f_mc_pt->SetLineColor(kGreen);
    // h_bbb_f_mc_pt->SetMarkerColor(kGreen);

    // TCanvas *c_fraction = new TCanvas("c_fraction", "", 1000, 400);

    // TLegend *leg_fraction = new TLegend(0.6, 0.2, 0.9, 0.3);
    // leg_fraction->SetFillStyle(0);
    // leg_fraction->AddEntry(h_bbb_f_pt, "data", "pe1");
    // leg_fraction->AddEntry(h_bbb_f_mc_pt, "mc", "pe1");

    // h_bbb_f_pt->Draw("pe1");
    // h_bbb_f_mc_pt->Draw("pe1 same");
    // leg_fraction->Draw();


}