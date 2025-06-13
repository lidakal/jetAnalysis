#include "../cms_palette.h"


void draw_fit_mB(TString observable="rg") 
{
    TString label = "aggrTMVA_XXT";

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
    TString fin_name = "histos/fitted_parameters_RooFit_data_" + label + "_" + observable + "_glued_jer_nom_jec_nom.root";
    std::cout << "Reading file: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get("h_data"); // y=mB, z=jtpt

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

    // Combine dijet and bjet histogram, training and testing
    TH3D *h_b = (TH3D *) h_sig_training_dijet->Clone("h_b");
    h_b->Add(h_sig_testing_dijet);
    h_b->Add(h_sig_training_bjet);
    h_b->Add(h_sig_testing_bjet);

    TH3D *h_bb = (TH3D *) h_bkg_bb_training_dijet->Clone("h_bkg_bb");
    h_bb->Add(h_bkg_bb_testing_dijet);
    h_bb->Add(h_bkg_bb_training_bjet);
    h_bb->Add(h_bkg_bb_testing_bjet);

    TH3D *h_cl = (TH3D *) h_bkg_rest_training_dijet->Clone("h_cl");
    h_cl->Add(h_bkg_rest_testing_dijet);
    h_cl->Add(h_bkg_rest_training_bjet);
    h_cl->Add(h_bkg_rest_testing_bjet);

    // Get fractions
    TH2D *h_b_f = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_bb_f = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_cl_f = (TH2D *) fin->Get("h_bkg_rest_fraction");

    TH2D *h_b_f_mc = (TH2D *) fin->Get("h_sig_fraction_true_training");
    TH2D *h_bb_f_mc = (TH2D *) fin->Get("h_bkg_bb_fraction_true_training");
    TH2D *h_cl_f_mc = (TH2D *) fin->Get("h_bkg_rest_fraction_true_training");

    double x1 = h_b_f->GetXaxis()->GetBinLowEdge(h_b_f->GetNbinsX());
    double x2 = h_b_f->GetXaxis()->GetBinUpEdge(h_b_f->GetNbinsX());
    double y1 = h_b_f->GetYaxis()->GetBinLowEdge(1);
    double y2 = h_b_f->GetYaxis()->GetBinUpEdge(h_b_f->GetNbinsY());

    // Loop over jet pt+observable bins
    int nbins_x = h_data->GetNbinsX();
    int nbins_mb = h_data->GetNbinsY();
    int nbins_pt = h_data->GetNbinsZ();

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;

        double pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

        // Draw fbincl + purity from mc
        TCanvas *c_purity = new TCanvas("c_purity", "", 700, 600);
        c_purity->SetRightMargin(0.05);
        c_purity->SetLeftMargin(0.12);

        TH1D *h_b_f_1d = (TH1D *) h_b_f->ProjectionX(Form("h_b_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_b_f_1d->SetMarkerStyle(kFullCrossX);
        h_b_f_1d->SetMarkerSize(2);
        h_b_f_1d->SetMarkerColor(cmsBlue);
        h_b_f_1d->SetLineColor(cmsBlue);

        TH1D *h_b_f_mc_1d = (TH1D *) h_b_f_mc->ProjectionX(Form("h_b_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_b_f_mc_1d->SetLineColorAlpha(cmsBlue,0.5);
        h_b_f_mc_1d->SetLineWidth(4);

        TH1D *h_bb_f_1d = (TH1D *) h_bb_f->ProjectionX(Form("h_bb_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bb_f_1d->SetMarkerStyle(kFullTriangleDown);
        h_bb_f_1d->SetMarkerSize(2);
        h_bb_f_1d->SetMarkerColor(cmsViolet);
        h_bb_f_1d->SetLineColor(cmsViolet);

        TH1D *h_bb_f_mc_1d = (TH1D *) h_bb_f_mc->ProjectionX(Form("h_bb_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_bb_f_mc_1d->SetLineColorAlpha(cmsViolet,0.5);
        h_bb_f_mc_1d->SetLineWidth(4);

        TH1D *h_cl_f_1d = (TH1D *) h_cl_f->ProjectionX(Form("h_cl_f_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_cl_f_1d->SetMarkerStyle(kFullTriangleUp);
        h_cl_f_1d->SetMarkerSize(2);
        h_cl_f_1d->SetMarkerColor(cmsOrange);
        h_cl_f_1d->SetLineColor(cmsOrange);

        TH1D *h_cl_f_mc_1d = (TH1D *) h_cl_f_mc->ProjectionX(Form("h_cl_f_mc_1d_%d", ibin_pt), ibin_pt, ibin_pt);
        h_cl_f_mc_1d->SetLineColorAlpha(cmsOrange,0.5);
        h_cl_f_mc_1d->SetLineWidth(4);

        double ymin = std::min({h_b_f_1d->GetMinimum(), h_b_f_mc_1d->GetMinimum()});
        double ymax = std::max({h_b_f_1d->GetMaximum(), h_b_f_mc_1d->GetMaximum()});

        ymin *= 0.9;
        ymax *= 1.1;

        ymin = -0.05;
        ymax = 1.6;

        h_cl_f_mc_1d->GetYaxis()->SetRangeUser(ymin, ymax);
        h_cl_f_mc_1d->GetYaxis()->SetTitle("Fraction");
        h_cl_f_mc_1d->GetYaxis()->SetTitleOffset(1.2);
        h_cl_f_mc_1d->GetXaxis()->SetTitle(xlabel);
        h_cl_f_mc_1d->GetXaxis()->SetTitleOffset(1.2);

        TLegend *leg_purity = new TLegend(0.27, 0.62, 0.45, 0.87);
        leg_purity->SetFillStyle(0);
        // leg_purity->SetBorderSize(1);
        leg_purity->SetNColumns(2);
        leg_purity->SetMargin(0.7);
        leg_purity->SetHeader("Data   MC");

        leg_purity->AddEntry(h_cl_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_cl_f_mc_1d, " ", "l");
        leg_purity->AddEntry(h_b_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_b_f_mc_1d, " ", "l");
        leg_purity->AddEntry(h_bb_f_1d, " ", "pe1");
        leg_purity->AddEntry(h_bb_f_mc_1d, " ", "l");


        h_cl_f_mc_1d->Draw("hist same");
        h_bb_f_mc_1d->Draw("hist same");
        h_b_f_mc_1d->Draw("hist same");

        // std::cout << "h_bbb_f->GetBinContent(8,1) = " << h_bbb_f->GetBinContent(8,1) << std::endl;
        // std::cout << "h_bbb_f->GetBinError(8,1) = " << h_bbb_f->GetBinError(8,1) << std::endl;

        // std::cout << "h_bbb_f_1d->GetBinContent(8) = " << h_bbb_f_1d->GetBinContent(8) << std::endl;
        // std::cout << "h_bbb_f_1d->GetBinError(8) = " << h_bbb_f_1d->GetBinError(8) << std::endl;

        h_cl_f_1d->Draw("pe1 same");
        h_bb_f_1d->Draw("pe1 same");
        h_b_f_1d->Draw("pe1 same");

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
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(32);
        jet_info->DrawLatex(0.92, 0.83, "anti-k_{T}, R = 0.4 b-tagged jets");
        jet_info->DrawLatex(0.92, 0.77, Form("%.0f < p_{T}^{jet,reco} < %.0f GeV/c, |#eta^{jet}| < 2",pt_min, pt_max));
        if (observable!="zpt") jet_info->DrawLatex(0.92, 0.71, "Soft drop (charged particles)");
        if (observable!="zpt") jet_info->DrawLatex(0.92, 0.65, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");

        jet_info->DrawLatex(0.27, 0.77, "Light+c");
        jet_info->DrawLatex(0.27, 0.71, "Single-b");
        jet_info->DrawLatex(0.27, 0.65, "Double-b");

        if (observable!="zpt") {
            TPaveText *untagged_text = new TPaveText(0.09, 0.02, 0.37, 0.137, "NDC");
            untagged_text->SetFillColor(0);
            untagged_text->SetBorderSize(0);
            untagged_text->SetTextAlign(22);
            untagged_text->SetTextSize(text_size-4);
            untagged_text->AddText("SD-untagged");
            untagged_text->AddText("or k_{T} < 1 GeV/c");
            untagged_text->Draw();
        }
        
        c_purity->Print("../plots_thesis/"+observable+"_fit_mB_fractions_"+label+".pdf");

        // Create canvas + pads for data+fit
        TCanvas *c_mb = new TCanvas(Form("c_mb_%d", ibin_pt), "", 1200, 1000);

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

            // Make projections 
            TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_b_1d = (TH1D *) h_b->ProjectionY(Form("h_b_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bb_1d = (TH1D *) h_bb->ProjectionY(Form("h_bb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_cl_1d = (TH1D *) h_cl->ProjectionY(Form("h_cl_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            for (auto h : {h_data_1d, h_b_1d, h_bb_1d, h_cl_1d}) {
                h->GetXaxis()->SetRange(1, h->GetNbinsX());
            }
            // Get fractions
            double b_f = h_b_f->GetBinContent(ibin_x, ibin_pt);
            double bb_f = h_bb_f->GetBinContent(ibin_x, ibin_pt);
            double cl_f = h_cl_f->GetBinContent(ibin_x, ibin_pt);

            // Normalize + multiply by fraction + stack 
            double n = 150.;
            h_data_1d->Scale(n/h_data_1d->Integral(1, nbins_mb));

            h_b_1d->Scale(n*b_f/h_b_1d->Integral(1, nbins_mb));
            h_b_1d->SetFillColorAlpha(cmsViolet,0.5);
            h_b_1d->SetLineColor(cmsViolet);
            h_b_1d->SetFillStyle(1001);
            h_b_1d->GetXaxis()->SetTitle("m_{b,ch} (GeV/c^{2})");

            h_bb_1d->Scale(n*bb_f/h_bb_1d->Integral(1, nbins_mb));
            h_bb_1d->SetFillColorAlpha(cmsBlue,0.5);
            h_bb_1d->SetLineColor(cmsBlue);
            h_bb_1d->SetFillStyle(1001);
            h_bb_1d->GetXaxis()->SetTitle("m_{b,ch} (GeV/c^{2})");

            if (h_cl_1d->Integral(1, nbins_mb) > 0) h_cl_1d->Scale(n*cl_f/h_cl_1d->Integral(1, nbins_mb));
            h_cl_1d->SetFillColorAlpha(cmsOrange,0.5);
            h_cl_1d->SetLineColor(cmsOrange);
            h_cl_1d->SetFillStyle(1001);
            h_cl_1d->GetXaxis()->SetTitle("m_{b,ch} (GeV/c^{2})");

            h_data_1d->SetMarkerStyle(kFullCircle);
            h_data_1d->GetXaxis()->SetTitle("m_{b,ch} (GeV/c^{2})");

            THStack *h_fit = new THStack(Form("h_fit_%d_%d", ibin_pt, ibin_x), "");
            h_fit->Add(h_cl_1d, "hist"); // no b
            h_fit->Add(h_b_1d, "hist"); // 1 b
            h_fit->Add(h_bb_1d, "hist"); // 2 b

            TH1D *h_fit_mb = (TH1D *) h_b_1d->Clone(Form("h_fit_mb_%d_%d", ibin_pt, ibin_x));
            h_fit_mb->Add(h_bb_1d);
            h_fit_mb->Add(h_cl_1d);

            Double_t xs[50];
            Double_t ys[50];
            Double_t dxs[50];
            Double_t dys[50];
            for (int ibin_x=1; ibin_x<=h_fit_mb->GetNbinsX(); ibin_x++) {
                int index = ibin_x - 1;

                double x = h_fit_mb->GetXaxis()->GetBinCenter(ibin_x);
                double dx = h_fit_mb->GetXaxis()->GetBinUpEdge(ibin_x) - x;

                double y = h_fit_mb->GetBinContent(ibin_x);
                double dy = h_fit_mb->GetBinError(ibin_x);

                xs[index] = x;
                ys[index] = y;
                dxs[index] = dx;
                dys[index] = dy;
                // std::cout << y << ", " << std::endl;
            }

            TGraphErrors *gr_err = new TGraphErrors(h_fit_mb->GetNbinsX(), xs, ys, dxs, dys);
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
            // current_pad->SetLogy();
            
            h_data_1d->GetXaxis()->SetTitle("m_{b,ch} (GeV/c^{2})");
            h_data_1d->GetXaxis()->SetTitleOffset(2.3);
            h_data_1d->GetYaxis()->SetTitle("a.u.");
            h_data_1d->GetYaxis()->SetTitleOffset(2.6);
            h_data_1d->GetYaxis()->SetLabelOffset(0.015);
            h_data_1d->GetYaxis()->SetRangeUser(0.001,34.999);
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

            TLegend *leg_mb = new TLegend(0.12, 0.2, 0.52, 0.5);
            leg_mb->SetFillStyle(0);
            leg_mb->SetBorderSize(0);
            leg_mb->SetMargin(0.2);
            leg_mb->AddEntry(h_data_1d, "Data", "pe1");
            leg_mb->AddEntry(h_cl_1d, "Light+c", "f");
            leg_mb->AddEntry(h_b_1d, "Single-b", "f");
            leg_mb->AddEntry(h_bb_1d, "Double-b", "f");

            TGaxis *axis1 = new TGaxis(0.015,1.01,0.925,1.01,0.,7.,510,"");
            // if (tagged) axis1 = new TGaxis(0.015,1.01,0.925,1.01,0,2.45,510,"");
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
                leg_mb->Draw();
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
                leg_mb->Draw();
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
                leg_mb->Draw();
            }

            c_mb->cd();
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
        TString jet_text = "anti_{}-_{}k_{T}, R = 0.4, b-tagged jets";
        jet_info2->DrawLatex(0.12, 0.75, jet_text.Data());
        jet_info2->DrawLatex(0.12, 0.65, Form("%.0f <^{}_{} p_{T}^{jet,reco} < %.0f GeV/c", pt_min, pt_max));
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

        c_mb->cd();
        pad13->Draw();
        pad31->Draw();
        pad11->Draw();
        pad12->Draw();
        pad23->Draw();
        // std::cout << "c_mb->GetWindowHeight()=" << c_mb->GetWindowHeight() << std::endl;
        // c_mb->SetWindowSize(c_mb->GetWindowWidth(),700);

        c_mb->Print("../plots_thesis/"+observable+"_"+label+"_mb_fits.pdf");
    } // pt bins    
}