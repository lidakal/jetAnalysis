#include "../cms_palette.h"


void draw_fit_JP(TString observable="rg", bool tagged=false) 
{
    TString label = "aggrTMVA_inclusive";
    // label += (tagged ? "_tagged" : "");
    TString tagged_name = tagged ? "_tagged" : "";

    TString xlabel;
    if (observable=="rg") xlabel = "^{}ln^{}(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";

    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TLatex *bin_info = new TLatex;
    bin_info->SetNDC();
    bin_info->SetTextSize(text_size-4);
    bin_info->SetTextAlign(32);

    // Grab histograms 
    TString fin_name = "histos/"+observable+"_fit_result_JP_" + label + ".root";
    std::cout << "Reading file: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get("h_data"+tagged_name);
    TH3D *h_bbb = (TH3D *) fin->Get("h_bbb"+tagged_name);
    TH3D *h_c = (TH3D *) fin->Get("h_ccc"+tagged_name);
    TH3D *h_l = (TH3D *) fin->Get("h_l"+tagged_name);

    TH2D *h_bbb_f = (TH2D *) fin->Get("h_bbb_f"+tagged_name);
    TH2D *h_c_f = (TH2D *) fin->Get("h_ccc_f"+tagged_name);
    TH2D *h_l_f = (TH2D *) fin->Get("h_l_f"+tagged_name); 

    std::cout << h_data->GetNbinsY() << std::endl;

    for (auto h : {h_data, h_bbb, h_c, h_l}) {
        // h->RebinX(h->GetNbinsX()); // rebin only in x
        // if (tagged) h->RebinY(14);
    }

    TH2D *h_bbb_f_mc = (TH2D *) fin->Get("h_bbb_f_mc"+tagged_name);
    TH2D *h_ccc_f_mc = (TH2D *) fin->Get("h_ccc_f_mc"+tagged_name);
    TH2D *h_l_f_mc = (TH2D *) fin->Get("h_l_f_mc"+tagged_name);

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
    // if (observable=="rg") overflow->Draw();

    // Loop over jet pt+observable bins
    int nbins_x = h_bbb->GetNbinsX();
    int nbins_jp = h_bbb->GetNbinsY();
    int nbins_pt = h_bbb->GetNbinsZ();

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        // if (ibin_pt!=3) continue;

        double pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

        // Draw fbincl + purity from mc
        TCanvas *c_purity = new TCanvas("c_purity", "", 700, 600);
        c_purity->SetRightMargin(0.05);
        c_purity->SetLeftMargin(0.12);

        TH1D *h_bbb_f_1d = (TH1D *) h_bbb_f->ProjectionX(Form("h_bbb_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bbb_f_1d->SetMarkerStyle(kFullCrossX);
        h_bbb_f_1d->SetMarkerSize(2);
        h_bbb_f_1d->SetMarkerColor(cmsOrange);
        h_bbb_f_1d->SetLineColor(cmsOrange);

        TH1D *h_bbb_f_mc_1d = (TH1D *) h_bbb_f_mc->ProjectionX(Form("h_bbb_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bbb_f_mc_1d->SetLineColorAlpha(cmsOrange,0.5);
        h_bbb_f_mc_1d->SetLineWidth(4);

        TH1D *h_ccc_f_1d = (TH1D *) h_c_f->ProjectionX(Form("h_ccc_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_ccc_f_1d->SetMarkerStyle(kFullTriangleDown);
        h_ccc_f_1d->SetMarkerSize(2);
        h_ccc_f_1d->SetMarkerColor(cmsBlue);
        h_ccc_f_1d->SetLineColor(cmsBlue);

        TH1D *h_ccc_f_mc_1d = (TH1D *) h_ccc_f_mc->ProjectionX(Form("h_ccc_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_ccc_f_mc_1d->SetLineColorAlpha(cmsBlue,0.5);
        h_ccc_f_mc_1d->SetLineWidth(4);

        TH1D *h_l_f_1d = (TH1D *) h_l_f->ProjectionX(Form("h_l_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_l_f_1d->SetMarkerStyle(kFullTriangleUp);
        h_l_f_1d->SetMarkerSize(2);
        h_l_f_1d->SetMarkerColor(cmsViolet);
        h_l_f_1d->SetLineColor(cmsViolet);


        TH1D *h_l_f_mc_1d = (TH1D *) h_l_f_mc->ProjectionX(Form("h_l_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_l_f_mc_1d->SetLineColorAlpha(cmsViolet,0.5);
        h_l_f_mc_1d->SetLineWidth(4);

        double ymin = std::min({h_bbb_f_1d->GetMinimum(), h_bbb_f_mc_1d->GetMinimum()});
        double ymax = std::max({h_bbb_f_1d->GetMaximum(), h_bbb_f_mc_1d->GetMaximum()});

        ymin *= 0.9;
        ymax *= 1.1;

        ymin = -0.05;
        ymax = 1.05;

        h_l_f_mc_1d->GetYaxis()->SetRangeUser(ymin, ymax);
        h_l_f_mc_1d->GetYaxis()->SetTitle("Fraction");
        h_l_f_mc_1d->GetYaxis()->SetTitleOffset(1.2);
        h_l_f_mc_1d->GetXaxis()->SetTitle(xlabel);
        h_l_f_mc_1d->GetXaxis()->SetTitleOffset(1.2);

        TLegend *leg_purity = new TLegend(0.7, 0.3, 0.9, 0.55);
        if (observable=="zpt"&&!tagged) leg_purity = new TLegend(0.65, 0.6, 0.85, 0.85);
        leg_purity->SetFillStyle(0);
        // leg_purity->SetBorderSize(1);
        leg_purity->SetNColumns(2);
        leg_purity->SetMargin(0.7);
        leg_purity->SetHeader("Data    MC");

        leg_purity->AddEntry(h_l_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_l_f_mc_1d, " ", "l");
        leg_purity->AddEntry(h_ccc_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_ccc_f_mc_1d, " ", "l");
        leg_purity->AddEntry(h_bbb_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_bbb_f_mc_1d, " ", "l");

        h_l_f_mc_1d->Draw("hist same");
        h_ccc_f_mc_1d->Draw("hist same");
        h_bbb_f_mc_1d->Draw("hist same");

        std::cout << "h_bbb_f->GetBinContent(8,1) = " << h_bbb_f->GetBinContent(8,1) << std::endl;
        std::cout << "h_bbb_f->GetBinError(8,1) = " << h_bbb_f->GetBinError(8,1) << std::endl;

        std::cout << "h_bbb_f_1d->GetBinContent(8) = " << h_bbb_f_1d->GetBinContent(8) << std::endl;
        std::cout << "h_bbb_f_1d->GetBinError(8) = " << h_bbb_f_1d->GetBinError(8) << std::endl;

        h_l_f_1d->Draw("pe1 same");
        h_ccc_f_1d->Draw("pe1 same");
        h_bbb_f_1d->Draw("pe1 same");

        TLatex *prelim_pur = new TLatex;
        prelim_pur->SetNDC();
        prelim_pur->SetTextSize(28);
        prelim_pur->SetTextAlign(12);
        prelim_pur->DrawLatex(0.12, 0.93, "#bf{CMS} #it{Private work}");

        TLatex *lumi_pur = new TLatex;
        lumi_pur->SetNDC();
        lumi_pur->SetTextSize(28);
        lumi_pur->SetTextAlign(32);
        lumi_pur->DrawLatex(0.95, 0.93, "pp 301^{} pb^{-1} (5.02 TeV)");

        leg_purity->Draw();

        // Jets text
        if (observable=="zpt"&&!tagged) {
            TLatex *jet_info = new TLatex;
            jet_info->SetNDC();
            jet_info->SetTextSize(text_size-4);
            jet_info->SetTextAlign(12);
            if (tagged) jet_info->DrawLatex(0.17, 0.61, "anti-k_{T}, R = 0.4 b-tagged jets");
            else jet_info->DrawLatex(0.17, 0.55, "anti-k_{T}, R = 0.4 inclusive jets");
            jet_info->DrawLatex(0.17, 0.49, "80 < p_{T}^{jet,reco} < 140 GeV/c, |#eta^{jet}| < 2");
            jet_info->DrawLatex(0.17, 0.43, "Soft drop (charged particles)");
            jet_info->DrawLatex(0.17, 0.37, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

            TLatex *jet_info_v2 = new TLatex;
            jet_info_v2->SetNDC();
            jet_info_v2->SetTextSize(text_size-4);
            jet_info_v2->SetTextAlign(32);
            jet_info_v2->DrawLatex(0.65, 0.76, "guds");
            jet_info_v2->DrawLatex(0.65, 0.7, "c");
            jet_info_v2->DrawLatex(0.65, 0.64, "b");
        } else {
            TLatex *jet_info = new TLatex;
            jet_info->SetNDC();
            jet_info->SetTextSize(text_size-4);
            jet_info->SetTextAlign(32);
            if (tagged) jet_info->DrawLatex(0.9, 0.75, "anti-k_{T}, R = 0.4 b-tagged jets");
            else jet_info->DrawLatex(0.9, 0.75, "anti-k_{T}, R = 0.4 inclusive jets");
            jet_info->DrawLatex(0.9, 0.69, "80 < p_{T}^{jet,reco} < 140 GeV/c, |#eta^{jet}| < 2");
            jet_info->DrawLatex(0.9, 0.63, "Soft drop (charged particles)");
            jet_info->DrawLatex(0.9, 0.57, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

            jet_info->DrawLatex(0.69, 0.46, "guds");
            jet_info->DrawLatex(0.69, 0.4, "c");
            jet_info->DrawLatex(0.69, 0.34, "b");
        }

        c_purity->Print("../plots_thesis/"+observable+"_fit_JP_fractions_"+label+tagged_name+".pdf");

        // Create canvas + pads for data+fit
        TCanvas *c_jp = new TCanvas(Form("c_jp_%d", ibin_pt), "", 1200, 1000);

        double x=0.3;
        double xl=0.08, xr=1-(3*x)-xl;
        double x1=xl+x, x2=x1+x;

        double y=0.3;
        double yb=0.06, yt=1-(3*y)-yb;
        double y1=yb+y, y2=y1+y;

        double gapx=0.0045, gapy=0.0045;

        TPad *pad11 = new TPad(Form("pad11_%d", ibin_pt),"",0.,0.,x1,y1);
        TPad *pad12 = new TPad(Form("pad12_%d", ibin_pt),"",x1,0.,x2,y1);
        TPad *pad13 = new TPad(Form("pad13_%d", ibin_pt),"",x2,0.,1.,y1);
        TPad *pad21 = new TPad(Form("pad21_%d", ibin_pt),"",0.,y1,x1,y2);
        TPad *pad22 = new TPad(Form("pad22_%d", ibin_pt),"",x1,y1,x2,y2);
        TPad *pad23 = new TPad(Form("pad23_%d", ibin_pt),"",x2,y1,1.,y2);
        TPad *pad31 = new TPad(Form("pad31_%d", ibin_pt),"",0.,y2,x1,1.);
        TPad *pad32 = new TPad(Form("pad32_%d", ibin_pt),"",x1,y2,x2,1.);
        TPad *pad33 = new TPad(Form("pad33_%d", ibin_pt),"",x2,y2,1.,1.);

        for (auto pad : {pad31, pad32, pad33}) {
            pad->SetBottomMargin(gapy/(1-y2));
            pad->SetTopMargin((yt+gapy)/(1-y2));
        }
        for (auto pad : {pad21, pad22, pad23}) {
            pad->SetBottomMargin(gapy/y);
            pad->SetTopMargin(gapy/y);
        }
        for (auto pad : {pad11, pad12, pad13}) {
            pad->SetBottomMargin((yb+gapy)/y1);
            pad->SetTopMargin(gapy/y1);
        }
        for (auto pad : {pad31, pad21, pad11}) {
            pad->SetLeftMargin((xl+gapx)/x1);
            pad->SetRightMargin(gapx/x1);
        }
        for (auto pad : {pad32, pad22, pad12}) {
            pad->SetLeftMargin(gapx/x);
            pad->SetRightMargin(gapx/x);
        }
        for (auto pad : {pad33, pad23, pad13}) {
            pad->SetLeftMargin(gapx/(1-x2));
            pad->SetRightMargin((xr+gapx)/(1-x2));
        }

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

            // debug
            // double bbb_f = h_bbb_f_mc->GetBinContent(ibin_x, ibin_pt);
            // double c_f = h_ccc_f_mc->GetBinContent(ibin_x, ibin_pt);
            // double l_f = h_l_f_mc->GetBinContent(ibin_x, ibin_pt);

            // std::cout << "\tbbb_f = " << bbb_f << std::endl;
            // std::cout << "\tc_f = " << c_f << std::endl;
            // std::cout << "\tl_f = " << l_f << std::endl;

            // Normalize + multiply by fraction + stack 
            double ndata = h_data_1d->Integral(1, nbins_jp);
            THStack *h_fit = new THStack(Form("h_fit_%d_%d", ibin_pt, ibin_x), "");

            // std::cout << bbb_f << std::endl;
            h_bbb_1d->Scale(ndata*bbb_f/h_bbb_1d->Integral(1, nbins_jp));
            h_bbb_1d->SetFillColorAlpha(cmsOrange,0.5);
            h_bbb_1d->SetLineColor(cmsOrange);
            h_bbb_1d->SetFillStyle(1001);
            h_bbb_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_bbb_1d, "hist");

            h_c_1d->Scale(ndata*c_f/h_c_1d->Integral(1, nbins_jp));
            h_c_1d->SetFillColorAlpha(cmsBlue,0.5);
            h_c_1d->SetLineColor(cmsBlue);
            h_c_1d->SetFillStyle(1001);
            h_c_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_c_1d, "hist");

            if (h_l_1d->Integral(1, nbins_jp) > 0) h_l_1d->Scale(ndata*l_f/h_l_1d->Integral(1, nbins_jp));
            h_l_1d->SetFillColorAlpha(cmsViolet,0.5);
            h_l_1d->SetLineColor(cmsViolet);
            h_l_1d->SetFillStyle(1001);
            h_l_1d->GetXaxis()->SetTitle("JP discr");
            h_fit->Add(h_l_1d, "hist");

            h_data_1d->SetMarkerStyle(kFullCircle);
            h_data_1d->GetXaxis()->SetTitle("JP discr");

            TH1D *h_fit_jp = (TH1D *) h_bbb_1d->Clone(Form("h_fit_jp_%d_%d", ibin_pt, ibin_x));
            h_fit_jp->Add(h_c_1d);
            h_fit_jp->Add(h_l_1d);

            Double_t xs[50];
            Double_t ys[50];
            Double_t dxs[50];
            Double_t dys[50];
            for (int ibin_x=1; ibin_x<=h_fit_jp->GetNbinsX(); ibin_x++) {
                int index = ibin_x - 1;

                double x = h_fit_jp->GetXaxis()->GetBinCenter(ibin_x);
                double dx = h_fit_jp->GetXaxis()->GetBinUpEdge(ibin_x) - x;

                double y = h_fit_jp->GetBinContent(ibin_x);
                double dy = h_fit_jp->GetBinError(ibin_x);

                xs[index] = x;
                ys[index] = y;
                dxs[index] = dx;
                dys[index] = dy;
                // std::cout << y << ", " << std::endl;
            }

            TGraphErrors *gr_err = new TGraphErrors(h_fit_jp->GetNbinsX(), xs, ys, dxs, dys);
            gr_err->SetFillStyle(3354);
            gr_err->SetFillColorAlpha(kBlack, 0.8);
            gr_err->SetLineWidth(10);

            TPad *current_pad = pads[ibin_x-1];
            std::cout << "pad id=" << ibin_x-1 << std::endl;
            // std::cout << "h_data_1d->Integral()=" << h_data_1d->Integral() << std::endl;
            // current_pad->SetRightMargin(0.1);
            // current_pad->SetLeftMargin(0.12);
            // current_pad->SetTopMargin(0.05);
            // current_pad->SetBottomMargin(0.1);

            current_pad->cd();
            if (!tagged) current_pad->SetLogy();
            current_pad->SetLogy();
            
            h_data_1d->GetXaxis()->SetTitle("JP score");
            h_data_1d->GetXaxis()->SetTitleOffset(2.3);
            h_data_1d->GetYaxis()->SetTitle("a.u.");
            h_data_1d->GetYaxis()->SetTitleOffset(2.5);
            if (tagged) h_data_1d->GetYaxis()->SetRangeUser(2e-1,2e9);
            else h_data_1d->GetYaxis()->SetRangeUser(1e3,2e8);
            h_data_1d->Draw("pe1");
            h_fit->Draw("same");
            h_data_1d->Draw("pe1 same");
            gr_err->Draw("e2 same");
            // leg_jp->Draw();            

            double w=current_pad->GetWNDC(); 
            double h=current_pad->GetHNDC();
            double xtext=0.25, ytext=0.25; // out of x,y=0.3
            // std::cout << "current_pad->GetLeftMargin()=" << current_pad->GetLeftMargin() << std::endl;
            if (observable!="zpt") { 
                if (ibin_x==1) {
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(ytext/h), "SD-untagged");
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(0.85*ytext/h), "or k_{T} < 1 GeV/c");
                } else if (ibin_x==nbins_x&&observable=="rg") {
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(ytext/h), Form("%.2f^{} <^{} %s", x_min, xlabel.Data()));
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(0.85*ytext/h), "k_{T} > 1 GeV/c");
                } else {
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(ytext/h), Form("%.2f^{} <^{} %s < %.2f", x_min, xlabel.Data(), x_max));
                    bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(0.85*ytext/h), "k_{T} > 1 GeV/c");
                }
            } else {
                bin_info->DrawLatex(current_pad->GetLeftMargin()+(xtext/w), current_pad->GetBottomMargin()+(ytext/h), Form("%.2f^{} <^{} %s < %.2f", x_min, xlabel.Data(), x_max));
            }

            TLegend *leg_jp = new TLegend(0.12, 0.2, 0.52, 0.5);
            leg_jp->SetFillStyle(0);
            leg_jp->SetBorderSize(0);
            leg_jp->SetMargin(0.2);
            leg_jp->AddEntry(h_data_1d, "Data", "pe1");
            leg_jp->AddEntry(h_l_1d, "guds", "f");
            leg_jp->AddEntry(h_c_1d, "c", "f");
            leg_jp->AddEntry(h_bbb_1d, "b", "f");

            TGaxis *axis1 = new TGaxis(0.015,1.01,0.925,1.01,0,2.1,510,"");
            if (tagged) axis1 = new TGaxis(0.015,1.01,0.925,1.01,0,2.45,510,"");
            axis1->SetName("axis1");
            // std::cout << h_data_1d->GetXaxis()->GetTitle()->GetTextFont() << std::endl;
            axis1->SetLabelFont(h_data_1d->GetXaxis()->GetLabelFont());
            axis1->SetLabelSize(h_data_1d->GetXaxis()->GetLabelSize());
            axis1->SetLabelOffset(h_data_1d->GetXaxis()->GetLabelOffset());
            axis1->SetTitleFont(h_data_1d->GetXaxis()->GetTitleFont());
            axis1->SetTitleSize(h_data_1d->GetXaxis()->GetTitleSize());
            axis1->SetTitle(h_data_1d->GetXaxis()->GetTitle());
            axis1->SetTitleOffset(h_data_1d->GetXaxis()->GetTitleOffset());
            // gStyle->SetStripDecimals(0); // to make all axis labels have 1 decimal
            if (observable=="rg"&&ibin_x==1) {
                pad13->cd();
                pad13->Range(0.,0.,1.,1);
                axis1->Draw();
                leg_jp->Draw();
            } else if (observable=="zg"&&ibin_x==1) {
                std::cout << "Drawing axis for zg" << std::endl;
                pad11->cd();
                pad11->Range(-0.25,0.,0.935,1);
                axis1->Draw();

                pad12->cd();
                pad12->Range(0.,0.,0.94,1);
                axis1->Draw();

                pad23->cd();
                pad23->Range(0.,0.,1.,1.);
                auto axis2 = (TGaxis *) axis1->Clone("axis2");
                axis2->SetTitleOffset(h_data_1d->GetXaxis()->GetTitleOffset()*1.2);
                axis2->Draw();
                leg_jp->Draw();
            } else if (observable=="zpt"&&ibin_x==1) {
                std::cout << "Drawing axis for zg" << std::endl;
                pad11->cd();
                pad11->Range(-0.25,0.,0.935,1);
                axis1->Draw();

                pad12->cd();
                pad12->Range(0.,0.,0.94,1);
                axis1->Draw();

                pad13->cd();
                pad13->Range(0.,0.,1.,1);
                // auto axis2 = (TGaxis *) axis1->Clone("axis2");
                // axis2->SetTitleOffset(h_data_1d->GetXaxis()->GetTitleOffset()*1.2);
                axis1->Draw();
                leg_jp->Draw();
            }

            c_jp->cd();
            current_pad->Draw();
            current_pad->RedrawAxis();
            // if (observable=="zg"&&ibin_x==1) {
            //     pad11->Draw();
            // } else if (observable=="rg"&&ibin_x==1) {
            //     pad13->Draw();
            // }

            
        } // observable bins

        if (observable=="zg") pad23->cd();
        else pad13->cd();
        TLatex *jet_info2 = new TLatex;
        jet_info2->SetNDC();
        jet_info2->SetTextSize(text_size-4);
        jet_info2->SetTextAlign(11);
        TString jet_text = "anti_{}-_{}k_{T}, R = 0.4";
        if (tagged) jet_text += ", b-tagged jets";
        else jet_text += ", inclusive jets";
        jet_info2->DrawLatex(0.12, 0.75, jet_text.Data());
        jet_info2->DrawLatex(0.12, 0.65, "80 <^{}_{} p_{T}^{jet,reco} < 140 GeV/c");
        jet_info2->DrawLatex(0.12, 0.55, "|#eta^{jet}| < 2");

        pad31->cd();
        TLatex *prelim = new TLatex;
        prelim->SetNDC();
        prelim->SetTextSize(28);
        prelim->SetTextAlign(12);
        prelim->DrawLatex(0.22, 0.93, "#bf{CMS} #it{Private work}");

        pad33->cd();
        TLatex *lumi = new TLatex;
        lumi->SetNDC();
        lumi->SetTextSize(28);
        lumi->SetTextAlign(32);
        lumi->DrawLatex(0.93, 0.93, "pp 301^{} pb^{-1} (5.02 TeV)");

        c_jp->cd();
        pad13->Draw();
        pad31->Draw();
        pad11->Draw();
        pad12->Draw();
        pad23->Draw();
        // std::cout << "c_jp->GetWindowHeight()=" << c_jp->GetWindowHeight() << std::endl;
        // c_jp->SetWindowSize(c_jp->GetWindowWidth(),700);

        c_jp->Print("../plots_thesis/"+observable+"_"+label+"_jp_fits"+tagged_name+".pdf");
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