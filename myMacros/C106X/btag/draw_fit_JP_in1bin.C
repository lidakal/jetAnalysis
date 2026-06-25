#include "../draw_utils.h"

void draw_fit_JP_in1bin(TString observable="rg", int ibin_pt=1) 
{
    TString sample = "dijet";
    TString label = "aggrTMVA_inclusive";

    TString xlabel;
    if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";

    // Setup plot
    Float_t text_size = 18.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab histograms 
    TString fin_name = "histos/"+observable+"_fit_result_JP_muonJets_" + label + ".root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data_inclusive = (TH3D *) fin->Get("h_data");
    TH3D *h_bbb_inclusive = (TH3D *) fin->Get("h_bbb");
    TH3D *h_ccc_inclusive = (TH3D *) fin->Get("h_ccc");
    TH3D *h_l_inclusive = (TH3D *) fin->Get("h_l");

    TH2D *h_bbb_f_inclusive = (TH2D *) fin->Get("h_bbb_f");
    TH2D *h_ccc_f_inclusive = (TH2D *) fin->Get("h_ccc_f");
    TH2D *h_l_f_inclusive = (TH2D *) fin->Get("h_l_f"); 

    TH2D *h_bbb_f_mc_inclusive = (TH2D *) fin->Get("h_bbb_f_mc");
    TH2D *h_ccc_f_mc_inclusive = (TH2D *) fin->Get("h_ccc_f_mc");
    TH2D *h_l_f_mc_inclusive = (TH2D *) fin->Get("h_l_f_mc");

    TH3D *h_data_tagged = (TH3D *) fin->Get("h_data_tagged");
    TH3D *h_bbb_tagged = (TH3D *) fin->Get("h_bbb_tagged");
    TH3D *h_ccc_tagged = (TH3D *) fin->Get("h_ccc_tagged");
    TH3D *h_l_tagged = (TH3D *) fin->Get("h_l_tagged");

    TH2D *h_bbb_f_tagged = (TH2D *) fin->Get("h_bbb_f_tagged");
    TH2D *h_ccc_f_tagged = (TH2D *) fin->Get("h_ccc_f_tagged");
    TH2D *h_l_f_tagged = (TH2D *) fin->Get("h_l_f_tagged"); 

    TH2D *h_bbb_f_mc_tagged = (TH2D *) fin->Get("h_bbb_f_mc_tagged");
    TH2D *h_ccc_f_mc_tagged = (TH2D *) fin->Get("h_ccc_f_mc_tagged");
    TH2D *h_l_f_mc_tagged = (TH2D *) fin->Get("h_l_f_mc_tagged");

    // Loop over observable bins
    int nbins_x = h_data_inclusive->GetNbinsX();
    int nbins_jp = h_data_inclusive->GetNbinsY();
    int nbins_pt = h_data_inclusive->GetNbinsZ();

    double pt_min = h_data_inclusive->GetZaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_data_inclusive->GetZaxis()->GetBinUpEdge(ibin_pt);


    std::vector<TH3D *> data_points = {h_data_inclusive, h_data_tagged};
    std::vector<TH3D *> b_templates = {h_bbb_inclusive, h_bbb_tagged};
    std::vector<TH3D *> c_templates = {h_ccc_inclusive, h_ccc_tagged};
    std::vector<TH3D *> l_templates = {h_l_inclusive, h_l_tagged};

    std::vector<TH2D *> b_fractions = {h_bbb_f_inclusive, h_bbb_f_tagged};
    std::vector<TH2D *> c_fractions = {h_ccc_f_inclusive, h_ccc_f_tagged};
    std::vector<TH2D *> l_fractions = {h_l_f_inclusive, h_l_f_tagged};

    std::vector<TH2D *> b_fractions_mc = {h_bbb_f_mc_inclusive, h_bbb_f_mc_tagged};
    std::vector<TH2D *> c_fractions_mc = {h_ccc_f_mc_inclusive, h_ccc_f_mc_tagged};
    std::vector<TH2D *> l_fractions_mc = {h_l_f_mc_inclusive, h_l_f_mc_tagged};
    std::vector<TString> types = {"inclusive", "tagged"};

    for (int i=0; i<2; i++) {
        TString type = types[i];
        TH2D *h_bbb_f = b_fractions[i];
        TH2D *h_ccc_f = c_fractions[i];
        TH2D *h_l_f = l_fractions[i];
        TH2D *h_bbb_f_mc = b_fractions_mc[i];
        TH2D *h_ccc_f_mc = c_fractions_mc[i];
        TH2D *h_l_f_mc = l_fractions_mc[i];

        if (i==1) label += "_tagged";

        TLegend *leg_fractions = new TLegend(0.2, 0.4, 0.5, 0.7);
        leg_fractions->SetFillStyle(0);
        leg_fractions->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));
        leg_fractions->SetNColumns(2);

        TH1D *h_bbb_f_1d = (TH1D *) h_bbb_f->ProjectionX(Form("h_bbb_f_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_bbb_f_1d->SetMarkerStyle(kFullCircle);
        h_bbb_f_1d->SetMarkerColor(kRed);
        h_bbb_f_1d->SetLineColor(kRed);
        h_bbb_f_1d->SetMinimum(0);
        h_bbb_f_1d->SetMaximum(1);
        h_bbb_f_1d->GetXaxis()->SetTitle(xlabel);
        h_bbb_f_1d->GetYaxis()->SetTitle("fraction");
        leg_fractions->AddEntry(h_bbb_f_1d, "b fraction in data", "pe1");

        TH1D *h_bbb_f_mc_1d = (TH1D *) h_bbb_f_mc->ProjectionX(Form("h_bbb_f_mc_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_bbb_f_mc_1d->SetMarkerStyle(1);
        h_bbb_f_mc_1d->SetMarkerColor(kRed);
        h_bbb_f_mc_1d->SetLineColor(kRed);
        h_bbb_f_mc_1d->SetLineStyle(kDashed);
        leg_fractions->AddEntry(h_bbb_f_mc_1d, "in mc", "l e1");

        TH1D *h_ccc_f_1d = (TH1D *) h_ccc_f->ProjectionX(Form("h_ccc_f_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_ccc_f_1d->SetMarkerStyle(kFullTriangleUp);
        h_ccc_f_1d->SetMarkerColor(kGreen);
        h_ccc_f_1d->SetLineColor(kGreen);
        h_ccc_f_1d->SetMinimum(0);
        h_ccc_f_1d->SetMaximum(1);
        h_ccc_f_1d->GetXaxis()->SetTitle(xlabel);
        h_ccc_f_1d->GetYaxis()->SetTitle("fraction");
        leg_fractions->AddEntry(h_ccc_f_1d, "c fraction in data", "pe1");

        TH1D *h_ccc_f_mc_1d = (TH1D *) h_ccc_f_mc->ProjectionX(Form("h_ccc_f_mc_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_ccc_f_mc_1d->SetMarkerStyle(1);
        h_ccc_f_mc_1d->SetMarkerColor(kGreen);
        h_ccc_f_mc_1d->SetLineColor(kGreen);
        h_ccc_f_mc_1d->SetLineStyle(kDashed);
        leg_fractions->AddEntry(h_ccc_f_mc_1d, "in mc", "l e1");

        TH1D *h_l_f_1d = (TH1D *) h_l_f->ProjectionX(Form("h_l_f_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_l_f_1d->SetMarkerStyle(kFullCross);
        h_l_f_1d->SetMarkerColor(kBlue);
        h_l_f_1d->SetLineColor(kBlue);
        h_l_f_1d->SetMinimum(0);
        h_l_f_1d->SetMaximum(1);
        h_l_f_1d->GetXaxis()->SetTitle(xlabel);
        h_l_f_1d->GetYaxis()->SetTitle("fraction");
        leg_fractions->AddEntry(h_l_f_1d, "guds fraction in data", "pe1");

        TH1D *h_l_f_mc_1d = (TH1D *) h_l_f_mc->ProjectionX(Form("h_l_f_mc_1d_%d_%s", ibin_pt, type.Data()), ibin_pt, ibin_pt);
        h_l_f_mc_1d->SetMarkerStyle(1);
        h_l_f_mc_1d->SetMarkerColor(kBlue);
        h_l_f_mc_1d->SetLineColor(kBlue);
        h_l_f_mc_1d->SetLineStyle(kDashed);
        leg_fractions->AddEntry(h_l_f_mc_1d, "in mc", "l e1");

        // Draw fbincl + purity from mc
        TCanvas *c_fractions = new TCanvas(Form("c_fractions_%d_%s", ibin_pt, type.Data()), "", 800, 600);
        h_bbb_f_1d->Draw("pe1 same");
        h_bbb_f_mc_1d->Draw("hist e1 same");
        h_ccc_f_1d->Draw("pe1 same");
        h_ccc_f_mc_1d->Draw("hist e1 same");
        h_l_f_1d->Draw("pe1 same");
        h_l_f_mc_1d->Draw("hist e1 same");
        leg_fractions->Draw();
        drawHeader();

        c_fractions->Draw();
        c_fractions->Print("plots_an/"+sample+"_"+label+"_jp_fit_fractions_"+observable+"_"+ibin_pt+".png");

        /// DRAW THE ACTUAL DISTRIBUTIONS
        TH3D *h_data = data_points[i];
        TH3D *h_bbb = b_templates[i];
        TH3D *h_ccc = c_templates[i];
        TH3D *h_l = l_templates[i];

        TCanvas *c_jp = new TCanvas(Form("c_jp_%d_%s", ibin_pt, type.Data()), "", 1200, 1000);

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
            TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d_%s", ibin_pt, ibin_x, type.Data()), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_1d = (TH1D *) h_bbb->ProjectionY(Form("h_bbb_1d_%d_%d_%s", ibin_pt, ibin_x, type.Data()), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_ccc_1d = (TH1D *) h_ccc->ProjectionY(Form("h_ccc_1d_%d_%d_%s", ibin_pt, ibin_x, type.Data()), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_l_1d = (TH1D *) h_l->ProjectionY(Form("h_l_1d_%d_%d_%s", ibin_pt, ibin_x, type.Data()), ibin_x, ibin_x, ibin_pt, ibin_pt);

            for (auto h : {h_data_1d, h_bbb_1d, h_ccc_1d, h_l_1d}) {
                h->GetXaxis()->SetRange(1, h->GetNbinsX());
            }
            // Get fractions
            double bbb_f = h_bbb_f->GetBinContent(ibin_x, ibin_pt);
            double c_f = h_ccc_f->GetBinContent(ibin_x, ibin_pt);
            double l_f = h_l_f->GetBinContent(ibin_x, ibin_pt);

            double bbb_f_mc = h_bbb_f_mc->GetBinContent(ibin_x, ibin_pt);
            double c_f_mc = h_ccc_f_mc->GetBinContent(ibin_x, ibin_pt);
            double l_f_mc = h_l_f_mc->GetBinContent(ibin_x, ibin_pt);
            // std::cout << h_ccc_1d->Integral() << std::endl;

            // Normalize + multiply by fraction + stack 
            double ndata = h_data_1d->Integral(1, nbins_jp);
            THStack *h_fit = new THStack(Form("h_fit_%d_%d_%s", ibin_pt, ibin_x, type.Data()), "");
            // h_fit->SetTitle(";;entries");

            TLegend *leg_jp = new TLegend(0.3, 0.25, 0.8, 1.);
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

            if (h_ccc_1d->Integral(1, nbins_jp)>0) h_ccc_1d->Scale(ndata*c_f/h_ccc_1d->Integral(1, nbins_jp));
            h_ccc_1d->SetFillColor(kGreen);
            h_ccc_1d->SetFillStyle(1001);
            h_ccc_1d->SetMarkerStyle(1);
            h_ccc_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_ccc_1d, "hist e1");
            leg_jp->AddEntry(h_ccc_1d, "c jets", "f");

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
            if (i==0) current_pad->SetLogy();

            // DRAW ALSO TEMPLATES WITH FRACTIONS FROM MC
            TH1D *h_bbb_1d_mc = (TH1D *) h_bbb_1d->Clone("h_bbb_1d_mc");
            h_bbb_1d_mc->Scale(ndata*bbb_f_mc/h_bbb_1d_mc->Integral(1, nbins_jp));

            TH1D *h_ccc_1d_mc = (TH1D *) h_ccc_1d->Clone("h_ccc_1d_mc");
            if (h_ccc_1d_mc->Integral(1, nbins_jp)>0) h_ccc_1d_mc->Scale(ndata*c_f_mc/h_ccc_1d_mc->Integral(1, nbins_jp));

            TH1D *h_l_1d_mc = (TH1D *) h_l_1d->Clone("h_l_1d_mc");
            if (h_l_1d_mc->Integral(1, nbins_jp)>0) h_l_1d_mc->Scale(ndata*l_f_mc/h_l_1d_mc->Integral(1, nbins_jp));

            TH1D *h_mc_1d = (TH1D *) h_bbb_1d_mc->Clone("h_mc_1d");
            h_mc_1d->Add(h_ccc_1d_mc);
            h_mc_1d->Add(h_l_1d_mc);
            h_mc_1d->SetLineStyle(9);
            h_mc_1d->SetLineColor(kBlack);
            h_mc_1d->SetFillStyle(0);
            h_mc_1d->SetLineWidth(2);
            leg_jp->AddEntry(h_mc_1d, "MC with original fractions", "l");

            h_data_1d->Draw("pe1");
            h_fit->Draw("same");
            h_mc_1d->Draw("hist same");
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
                lumi->DrawLatex(0.45, 0.92, "#bf{301 fb^{-1} (pp 5.02 TeV)}");
                lumi->Draw();
            }

            // RATIO - data/fit
            TH1D *h_fit_sum = (TH1D *) h_bbb_1d->Clone(Form("h_fit_%d", ibin_x));
            h_fit_sum->Add(h_ccc_1d);
            h_fit_sum->Add(h_l_1d);

            TH1D *h_ratio = (TH1D *) h_data_1d->Clone(Form("h_ratio_%d", ibin_x));
            h_ratio->Divide(h_fit_sum);
            h_ratio->GetYaxis()->SetTitle("data/mc");
            h_ratio->SetTitle("fit fractions");
            h_ratio->GetYaxis()->SetNdivisions(6);
            h_ratio->GetXaxis()->SetTitleOffset(6.);
            // h_ratio->SetMarkerStyle(kFullCross);
            // h_ratio->GetYaxis()->SetTitleOffset();

            TH1D *h_ratio_mc = (TH1D *) h_data_1d->Clone(Form("h_ratio_mc_%d", ibin_x));
            h_ratio_mc->Divide(h_mc_1d);
            h_ratio_mc->GetYaxis()->SetTitle("data/mc");
            h_ratio_mc->GetYaxis()->SetNdivisions(6);
            h_ratio_mc->SetTitle("mc fractions");
            h_ratio_mc->GetXaxis()->SetTitleOffset(6.);
            h_ratio_mc->SetLineStyle(h_mc_1d->GetLineStyle());
            h_ratio_mc->SetLineColor(h_mc_1d->GetLineColor());
            h_ratio_mc->SetLineWidth(h_mc_1d->GetLineWidth());

            double ymin = std::min({h_ratio->GetMinimum(), h_ratio_mc->GetMinimum()});
            double ymax = std::max({h_ratio->GetMaximum(), h_ratio_mc->GetMaximum()});

            h_ratio->GetYaxis()->SetRangeUser(ymin-0.1, ymax+0.1);
            h_ratio_mc->GetYaxis()->SetRangeUser(ymin-0.1, ymax+0.1);

            TPad *ratio_pad = ratio_pads[ibin_x-1];
            ratio_pad->SetTopMargin(0.);
            ratio_pad->SetBottomMargin(0.4);
            ratio_pad->SetLeftMargin(0.2);
            ratio_pad->SetRightMargin(0.05);
            ratio_pad->cd();
            h_ratio->Draw("pe1");
            h_ratio_mc->Draw("hist same");

            TLegend *leg_ratio = new TLegend(0.3, 0., 0.8, 0.2);
            leg_ratio->SetFillStyle(0);
            leg_ratio->SetBorderSize(0);
            // leg_ratio->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));

            leg_ratio->AddEntry(h_ratio, "ratio with fit fraction", "pe1");
            leg_ratio->AddEntry(h_ratio_mc, "ratio with mc fraction", "l");

            TLine *line = new TLine(0., 1., 7., 1.);
            line->SetLineColor(kGray);
            line->SetLineStyle(kDashed);
            line->Draw();

            pad33->cd();
            leg_jp->Draw();
            leg_ratio->Draw();

            c_jp->cd();
            current_pad->Draw();
            ratio_pad->Draw();
            pad33->Draw();

            // h_fit->GetYaxis()->SetTitleOffset(5.);
        } // observable bins

        c_jp->Draw();
        c_jp->Print("plots_an/"+sample+"_"+label+"_jp_fits_"+observable+"_"+ibin_pt+".png");
    }
}