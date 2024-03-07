#include "../draw_utils.h"

void draw_fit_JP_in1bin(TString observable="rg", bool tagged=false) 
{
    TString sample = "dijet";
    TString label = "aggrTMVA_inclusive";
    label += (tagged ? "_tagged" : "");

    TString xlabel;
    if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    // Setup plot
    Float_t text_size = 18.;
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

    TH2D *h_bbb_f_mc = (TH2D *) fin->Get("h_bbb_f_mc");
    TH2D *h_c_f_mc = (TH2D *) fin->Get("h_c_f_mc");
    TH2D *h_l_f_mc = (TH2D *) fin->Get("h_l_f_mc");

    // Loop over observable bins
    int nbins_x = h_bbb->GetNbinsX();
    int nbins_jp = h_bbb->GetNbinsY();
    int nbins_pt = h_bbb->GetNbinsZ();

    int ibin_pt = 2;
    double pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

    TLegend *leg_purity = new TLegend(0.2, 0.4, 0.5, 0.7);
    leg_purity->SetFillStyle(0);
    leg_purity->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));
    leg_purity->SetNColumns(2);

    TH1D *h_bbb_f_1d = (TH1D *) h_bbb_f->ProjectionX(Form("h_bbb_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_bbb_f_1d->SetMarkerStyle(kFullCircle);
    h_bbb_f_1d->SetMarkerColor(kRed);
    h_bbb_f_1d->SetLineColor(kRed);
    h_bbb_f_1d->SetMinimum(0);
    h_bbb_f_1d->SetMaximum(1);
    h_bbb_f_1d->GetXaxis()->SetTitle(xlabel);
    h_bbb_f_1d->GetYaxis()->SetTitle("fraction");
    leg_purity->AddEntry(h_bbb_f_1d, "b fraction in data", "pe1");

    TH1D *h_bbb_f_mc_1d = (TH1D *) h_bbb_f_mc->ProjectionX(Form("h_bbb_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_bbb_f_mc_1d->SetMarkerStyle(1);
    h_bbb_f_mc_1d->SetMarkerColor(kRed);
    h_bbb_f_mc_1d->SetLineColor(kRed);
    h_bbb_f_mc_1d->SetLineStyle(kDashed);
    leg_purity->AddEntry(h_bbb_f_mc_1d, "in mc", "l e1");

    TH1D *h_c_f_1d = (TH1D *) h_c_f->ProjectionX(Form("h_c_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_c_f_1d->SetMarkerStyle(kFullTriangleUp);
    h_c_f_1d->SetMarkerColor(kGreen);
    h_c_f_1d->SetLineColor(kGreen);
    h_c_f_1d->SetMinimum(0);
    h_c_f_1d->SetMaximum(1);
    h_c_f_1d->GetXaxis()->SetTitle(xlabel);
    h_c_f_1d->GetYaxis()->SetTitle("fraction");
    leg_purity->AddEntry(h_c_f_1d, "c fraction in data", "pe1");

    TH1D *h_c_f_mc_1d = (TH1D *) h_c_f_mc->ProjectionX(Form("h_c_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_c_f_mc_1d->SetMarkerStyle(1);
    h_c_f_mc_1d->SetMarkerColor(kGreen);
    h_c_f_mc_1d->SetLineColor(kGreen);
    h_c_f_mc_1d->SetLineStyle(kDashed);
    leg_purity->AddEntry(h_c_f_mc_1d, "in mc", "l e1");

    TH1D *h_l_f_1d = (TH1D *) h_l_f->ProjectionX(Form("h_l_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_l_f_1d->SetMarkerStyle(kFullCross);
    h_l_f_1d->SetMarkerColor(kBlue);
    h_l_f_1d->SetLineColor(kBlue);
    h_l_f_1d->SetMinimum(0);
    h_l_f_1d->SetMaximum(1);
    h_l_f_1d->GetXaxis()->SetTitle(xlabel);
    h_l_f_1d->GetYaxis()->SetTitle("fraction");
    leg_purity->AddEntry(h_l_f_1d, "guds fraction in data", "pe1");

    TH1D *h_l_f_mc_1d = (TH1D *) h_l_f_mc->ProjectionX(Form("h_l_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_l_f_mc_1d->SetMarkerStyle(1);
    h_l_f_mc_1d->SetMarkerColor(kBlue);
    h_l_f_mc_1d->SetLineColor(kBlue);
    h_l_f_mc_1d->SetLineStyle(kDashed);
    leg_purity->AddEntry(h_l_f_mc_1d, "in mc", "l e1");

    // Draw fbincl + purity from mc
    TCanvas *c_purity = new TCanvas("c_purity", "", 800, 600);
    h_bbb_f_1d->Draw("pe1 same");
    h_bbb_f_mc_1d->Draw("hist e1 same");
    h_c_f_1d->Draw("pe1 same");
    h_c_f_mc_1d->Draw("hist e1 same");
    h_l_f_1d->Draw("pe1 same");
    h_l_f_mc_1d->Draw("hist e1 same");
    leg_purity->Draw();
    drawHeader();

    c_purity->Draw();
    c_purity->Print("plots_an/"+sample+"_"+label+"_jp_fit_fractions_"+observable+".png");

    ///// DRAW THE ACTUAL DISTRIBUTIONS

    TCanvas *c_jp = new TCanvas(Form("c_jp_%d", ibin_pt), "", 1200, 1000);

    TPad *pad11 = new TPad("pad11", "",0.0,0.77,0.33,0.99);
    TPad *pad12 = new TPad("pad12", "",0.33,0.77,0.66,0.99);
    TPad *pad13 = new TPad("pad13", "",0.66,0.77,0.99,0.99);

    TPad *pad21 = new TPad("pad21", "",0.0,0.44,0.33,0.66);
    TPad *pad22 = new TPad("pad22", "",0.33,0.44,0.66,0.66);
    TPad *pad23 = new TPad("pad23", "",0.66,0.44,0.99,0.66);

    TPad *pad31 = new TPad("pad31", "",0.0,0.11,0.33,0.33);
    TPad *pad32 = new TPad("pad32", "",0.33,0.11,0.66,0.33);
    TPad *pad33 = new TPad("pad33", "",0.66,0.11,0.99,0.33);

    TPad *pad111 = new TPad("pad111", "",0.0,0.66,0.33,0.77);
    TPad *pad121 = new TPad("pad121", "",0.33,0.66,0.66,0.77);
    TPad *pad131 = new TPad("pad131", "",0.66,0.66,0.99,0.77);

    TPad *pad211 = new TPad("pad211", "",0.0,0.33,0.33,0.44);
    TPad *pad221 = new TPad("pad221", "",0.33,0.33,0.66,0.44);
    TPad *pad231 = new TPad("pad231", "",0.66,0.33,0.99,0.44);

    TPad *pad311 = new TPad("pad311", "",0.0,0.,0.33,0.11);
    TPad *pad321 = new TPad("pad321", "",0.33,0.,0.66,0.11);
    TPad *pad331 = new TPad("pad331", "",0.66,0.,0.99,0.11);
    

    std::vector<TPad *> dist_pads = {pad11, pad12, pad13,
                                     pad21, pad22, pad23,
                                     pad31, pad32};

    std::vector<TPad *> ratio_pads = {pad111, pad121, pad131,
                                      pad211, pad221, pad231,
                                      pad311, pad321};

    for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
        // if (ibin_pt != 2) continue;
        // if (ibin_x != 3) continue;

        double x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
        double x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

        TString bin_info = Form("%.2f < %s < %.2f", x_min, xlabel.Data(), x_max);
        if (observable!="zpt" && ibin_x==1) bin_info = "fail SD or k_{T} < 1 GeV";
        if (observable=="rg"&&ibin_x==nbins_x) bin_info = Form("%.2f < %s", x_min, xlabel.Data());

        // Make projections 
        TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
        TH1D *h_bbb_1d = (TH1D *) h_bbb->ProjectionY(Form("h_bbb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
        TH1D *h_c_1d = (TH1D *) h_c->ProjectionY(Form("h_c_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
        TH1D *h_l_1d = (TH1D *) h_l->ProjectionY(Form("h_l_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

        for (auto h : {h_data_1d, h_bbb_1d, h_c_1d, h_l_1d}) {
            h->GetXaxis()->SetRange(2, h->GetNbinsX());
        }
        // Get fractions
        double bbb_f = h_bbb_f->GetBinContent(ibin_x, ibin_pt);
        double c_f = h_c_f->GetBinContent(ibin_x, ibin_pt);
        double l_f = h_l_f->GetBinContent(ibin_x, ibin_pt);
        // std::cout << h_c_1d->Integral() << std::endl;

        // Normalize + multiply by fraction + stack 
        double ndata = h_data_1d->Integral(1, nbins_jp);
        THStack *h_fit = new THStack(Form("h_fit_%d_%d", ibin_pt, ibin_x), "");
        // h_fit->SetTitle(";;entries");

        TLegend *leg_jp = new TLegend(0.4, 0.4, 0.8, 0.95);
        leg_jp->SetFillStyle(0);
        leg_jp->SetBorderSize(0);
        leg_jp->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

        // std::cout << bbb_f << std::endl;
        h_bbb_1d->Scale(ndata*bbb_f/h_bbb_1d->Integral(1, nbins_jp));
        h_bbb_1d->SetFillColor(kRed);
        h_bbb_1d->SetFillStyle(1001);
        h_bbb_1d->SetMarkerStyle(1);
        h_bbb_1d->GetXaxis()->SetTitle("JP discr");
        h_fit->Add(h_bbb_1d, "hist e1");
        leg_jp->AddEntry(h_bbb_1d, "b jets", "f");

        h_c_1d->Scale(ndata*c_f/h_c_1d->Integral(1, nbins_jp));
        h_c_1d->SetFillColor(kGreen);
        h_c_1d->SetFillStyle(1001);
        h_c_1d->SetMarkerStyle(1);
        h_c_1d->GetXaxis()->SetTitle("JP discr");
        h_fit->Add(h_c_1d, "hist e1");
        leg_jp->AddEntry(h_c_1d, "c jets", "f");

        if (h_l_1d->Integral(1, nbins_jp) > 0) h_l_1d->Scale(ndata*l_f/h_l_1d->Integral(1, nbins_jp));
        h_l_1d->SetFillColor(kBlue);
        h_l_1d->SetFillStyle(1001);
        h_l_1d->SetMarkerStyle(1);
        h_l_1d->GetXaxis()->SetTitle("JP discr");
        h_fit->Add(h_l_1d, "hist e1");
        leg_jp->AddEntry(h_l_1d, "guds jets", "f");

        h_data_1d->SetMarkerStyle(kFullCircle);
        h_data_1d->GetXaxis()->SetTitle("JP discr");
        h_data_1d->GetYaxis()->SetTitle("entries");
        h_data_1d->GetYaxis()->SetTitleOffset(4.);
        leg_jp->AddEntry(h_data_1d, "data", "pe1");

        TPad *current_pad = dist_pads[ibin_x-1];
        current_pad->SetRightMargin(0.1);
        current_pad->SetLeftMargin(0.12);
        current_pad->SetTopMargin(0.05);
        current_pad->SetBottomMargin(0.1);

        current_pad->SetTopMargin(0.15);
        current_pad->SetBottomMargin(0.);
        current_pad->SetLeftMargin(0.2);
        current_pad->SetRightMargin(0.05);

        current_pad->cd();
        if (!tagged) current_pad->SetLogy();
        
        h_data_1d->Draw("pe1");
        h_fit->Draw("same");
        h_data_1d->Draw("pe1 same");

        TPaveText *bin_info_text = new TPaveText(0.4, 0.7, 0.9, 0.8, "ndc");
        bin_info_text->SetTextSize(text_size);
        bin_info_text->SetFillStyle(0);
        bin_info_text->SetBorderSize(0);
        bin_info_text->AddText(bin_info);
        bin_info_text->Draw();

        if (ibin_x==1) {
            TLatex *prelim = new TLatex;
            prelim->SetNDC();
            prelim->SetTextSize(20);
            prelim->DrawLatex(0.185, 0.92, "#bf{CMS} #it{Work in Progress}");
            prelim->Draw();
        }

        if (ibin_x==3) {
            TLatex *lumi = new TLatex;
            lumi->SetNDC();
            lumi->SetTextSize(20);
            lumi->DrawLatex(0.45, 0.92, "#bf{302 fb^{-1} (pp 5.02 TeV)}");
            lumi->Draw();
        }

        pad33->cd();
        leg_jp->Draw();

        // RATIO - data/fit
        TH1D *h_fit_sum = (TH1D *) h_bbb_1d->Clone(Form("h_fit_%d", ibin_x));
        h_fit_sum->Add(h_c_1d);
        h_fit_sum->Add(h_l_1d);

        TH1D *h_ratio = (TH1D *) h_data_1d->Clone(Form("h_ratio_%d", ibin_x));
        h_ratio->Divide(h_fit_sum);
        h_ratio->GetYaxis()->SetTitle("data/fit");
        h_ratio->GetYaxis()->SetNdivisions(6);
        h_ratio->GetXaxis()->SetTitleOffset(6.);
        // h_ratio->GetYaxis()->SetTitleOffset();

        TPad *ratio_pad = ratio_pads[ibin_x-1];
        ratio_pad->SetTopMargin(0.);
        ratio_pad->SetBottomMargin(0.4);
        ratio_pad->SetLeftMargin(0.2);
        ratio_pad->SetRightMargin(0.05);
        ratio_pad->cd();
        h_ratio->Draw();

        TLine *line = new TLine(0., 1., 7., 1.);
        line->SetLineColor(kGray);
        line->SetLineStyle(kDashed);
        line->Draw();

        c_jp->cd();
        current_pad->Draw();
        ratio_pad->Draw();
        pad33->Draw();

        // h_fit->GetYaxis()->SetTitleOffset(5.);
    } // observable bins

    c_jp->Draw();
    c_jp->Print("plots_an/"+sample+"_"+label+"_jp_fits_"+observable+".png");
}