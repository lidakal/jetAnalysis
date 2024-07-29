#include "draw_utils.h"
#include "cms_palette.h"

void draw_data_vs_mc(TString observable="rg")
{
    // RUN WITH ROOT 6.30 from /cvmfs/sft.cern.ch/lcg/views/LCG_105/x86_64-centos7-gcc11-opt/setup.sh

    bool inclusive = true;
    TString pythia_sample = inclusive ? "dijet_PF40" : "pythia_PF40";
    TString herwig_sample = inclusive ? "herwig_dijet_official_PF40" : "herwig_official_PF40";
    TString label = inclusive ? "aggrTMVA_inclusive" : "aggrTMVA_XXT";
    TString pythia_var_sample = inclusive ? "inclusive" : "bjet";
    TString pythia_var_h = inclusive ? "all_noUO" : "b_noUO";

    TString xlabel;
    // if (observable=="rg") xlabel = "ln#frac{R_{ }}{ R_{g}}";
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch} #equiv p_{T}^{b,ch}/^{}p_{T}^{jet,ch}";
    // TString ylabel = "#frac{1}{N} #frac{dN}{dz_{b,ch}}";
    TString ylabel = "1/N^{} dN^{}/^{}dz_{b,ch}";
    if (observable!="zpt") {
        // ylabel = "#frac{1}{N} #frac{dN}{" + xlabel + "}";
        ylabel = "1/N^{} dN^{}/^{}d^{}" + xlabel;
        // ylabel=xlabel;
        // xlabel = "Charged " + xlabel;
    }

    // ---- Setup 
    gStyle->SetCanvasPreferGL(kTRUE);
    gStyle->SetErrorX(0.5);
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab nominal unfolded data
    TString fin_data_name = "unfolding/histos/"+pythia_sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom" + TString(inclusive ? "" : "_withSF") +".root";
    std::cout << "Reading data from : " << fin_data_name << std::endl;
    TFile *fin_data = new TFile(fin_data_name);
    TH2D *h_data = (TH2D *) fin_data->Get("h_data_unfolded")->Clone("h_data");

    // Grab truth MC 
    TString fin_pythia_name = "unfolding/histos/"+pythia_sample+"_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading pythia from : " << fin_pythia_name << std::endl;
    TFile *fin_pythia = new TFile(fin_pythia_name);
    TH2D *h_pythia = (TH2D *) fin_pythia->Get("h_full_efficiency_denominator_"+observable+"pt");

    TString fin_herwig_name = "unfolding/histos/"+herwig_sample+"_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading herwig from : " << fin_herwig_name << std::endl;
    TFile *fin_herwig = new TFile(fin_herwig_name);
    TH2D *h_herwig = (TH2D *) fin_herwig->Get("h_full_efficiency_denominator_"+observable+"pt");

    // Grab pythia variations
    TString fin_FSRup_name = "histos/"+pythia_var_sample+"_FSRup_gen_histograms.root";
    std::cout << "Reading FSRup from : " << fin_FSRup_name << std::endl;
    TFile *fin_FSRup = new TFile(fin_FSRup_name);
    TH2D *h_FSRup = (TH2D *) fin_FSRup->Get("h_"+observable+"_"+pythia_var_h);

    TString fin_FSRdown_name = "histos/"+pythia_var_sample+"_FSRdown_gen_histograms.root";
    std::cout << "Reading FSRdown from : " << fin_FSRdown_name << std::endl;
    TFile *fin_FSRdown = new TFile(fin_FSRdown_name);
    TH2D *h_FSRdown = (TH2D *) fin_FSRdown->Get("h_"+observable+"_"+pythia_var_h);

    TString fin_ISRup_name = "histos/"+pythia_var_sample+"_ISRup_gen_histograms.root";
    std::cout << "Reading ISRup from : " << fin_ISRup_name << std::endl;
    TFile *fin_ISRup = new TFile(fin_ISRup_name);
    TH2D *h_ISRup = (TH2D *) fin_ISRup->Get("h_"+observable+"_"+pythia_var_h);

    TString fin_ISRdown_name = "histos/"+pythia_var_sample+"_ISRdown_gen_histograms.root";
    std::cout << "Reading ISRdown from : " << fin_ISRdown_name << std::endl;
    TFile *fin_ISRdown = new TFile(fin_ISRdown_name);
    TH2D *h_ISRdown = (TH2D *) fin_ISRdown->Get("h_"+observable+"_"+pythia_var_h);

    // Make projections 
    int ibin_pt = 2;
    double min_pt = h_data->GetYaxis()->GetBinLowEdge(ibin_pt);
    double max_pt = h_data->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_data_1d = (TH1D *) h_data->ProjectionX("h_data_1d", ibin_pt, ibin_pt);
    h_data_1d->SetMarkerStyle(kFullCircle);
    h_data_1d->SetMarkerColor(kBlack); 
    h_data_1d->SetLineColor(kBlack);
    h_data_1d->SetLineStyle(1);
    h_data_1d->SetLineWidth(1);

    TH1D *h_pythia_1d = (TH1D *) h_pythia->ProjectionX("h_pythia_1d", ibin_pt, ibin_pt);
    // h_pythia_1d->SetMarkerStyle(kFullCross);
    h_pythia_1d->SetMarkerStyle(1);
    h_pythia_1d->SetMarkerColor(cmsRed); // kOrange-3
    h_pythia_1d->SetLineColor(cmsRed);
    h_pythia_1d->SetLineStyle(9);
    h_pythia_1d->SetLineWidth(3);

    TH1D *h_herwig_1d = (TH1D *) h_herwig->ProjectionX("h_herwig_1d", ibin_pt, ibin_pt);
    // h_herwig_1d->SetMarkerStyle(kFullCrossX);
    h_herwig_1d->SetMarkerStyle(1);
    h_herwig_1d->SetMarkerColor(cmsBlue);  // kAzure-3
    h_herwig_1d->SetLineColor(cmsBlue);
    h_herwig_1d->SetLineStyle(8);
    h_herwig_1d->SetLineWidth(3);

    // Find correct pt bins for pythia variations 
    double ptmin = h_data->GetYaxis()->GetBinLowEdge(ibin_pt);
    double ptmax = h_data->GetYaxis()->GetBinUpEdge(ibin_pt);
    int ibin_pt_min_pythia_var = h_FSRup->GetYaxis()->FindBin(ptmin);
    int ibin_pt_max_pythia_var = h_FSRup->GetYaxis()->FindBin(ptmax)-1;

    TH1D *h_FSRup_1d = (TH1D *) h_FSRup->ProjectionX("h_FSRup_1d", ibin_pt_min_pythia_var, ibin_pt_max_pythia_var);
    // h_FSRup_1d->SetMarkerStyle(kFullCross);
    h_FSRup_1d->SetMarkerStyle(1);
    h_FSRup_1d->SetMarkerColor(cmsOrange); 
    h_FSRup_1d->SetLineColor(cmsOrange);
    h_FSRup_1d->SetLineStyle(7);
    h_FSRup_1d->SetLineWidth(3);

    TH1D *h_FSRdown_1d = (TH1D *) h_FSRdown->ProjectionX("h_FSRdown_1d", ibin_pt_min_pythia_var, ibin_pt_max_pythia_var);
    // h_FSRdown_1d->SetMarkerStyle(kFullCross);
    h_FSRdown_1d->SetMarkerStyle(1);
    h_FSRdown_1d->SetMarkerColor(cmsYellow); 
    h_FSRdown_1d->SetLineColor(cmsYellow);
    h_FSRdown_1d->SetLineStyle(6);
    h_FSRdown_1d->SetLineWidth(3);

    TH1D *h_ISRup_1d = (TH1D *) h_ISRup->ProjectionX("h_ISRup_1d", ibin_pt_min_pythia_var, ibin_pt_max_pythia_var);
    // h_ISRup_1d->SetMarkerStyle(kFullCross);
    h_ISRup_1d->SetMarkerStyle(1);
    h_ISRup_1d->SetMarkerColor(cmsOrange);
    h_ISRup_1d->SetLineColor(cmsOrange);
    h_ISRup_1d->SetLineStyle(5);
    h_ISRup_1d->SetLineWidth(3);

    TH1D *h_ISRdown_1d = (TH1D *) h_ISRdown->ProjectionX("h_ISRdown_1d", ibin_pt_min_pythia_var, ibin_pt_max_pythia_var);
    // h_ISRdown_1d->SetMarkerStyle(kFullCross);
    h_ISRdown_1d->SetMarkerStyle(1);
    h_ISRdown_1d->SetMarkerColor(cmsYellow);
    h_ISRdown_1d->SetLineColor(cmsYellow);
    h_ISRdown_1d->SetLineStyle(4);
    h_ISRdown_1d->SetLineWidth(3);

    // Normalize histograms 
    int ibin_x_min = 1;
    int ibin_x_max = h_data_1d->GetNbinsX();
    if (observable != "zpt") ibin_x_min = 2;
    if (observable == "rg") ibin_x_max = h_data_1d->GetNbinsX() - 1;

    double labelOffset = h_data_1d->GetXaxis()->GetLabelOffset();
    // std::cout << labelOffset << std::endl;
    for (auto h : {
        h_data_1d,
        h_pythia_1d,
        h_herwig_1d,
    }) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        if (observable=="zg") {
            h->GetYaxis()->SetRangeUser(0, 5.5);
            if (inclusive) h->GetYaxis()->SetRangeUser(0, 4.5);
        } else if (observable=="rg") {
            h->GetYaxis()->SetRangeUser(0, 1.25);
            if (inclusive) h->GetYaxis()->SetRangeUser(0, 1.);
        }
        else if (observable=="zpt") h->GetYaxis()->SetRangeUser(0, 4.);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetLabelOffset(10);
    }

    // Find correct x bins for pythia variations 
    double xmin = h_data->GetXaxis()->GetBinLowEdge(ibin_x_min);
    double xmax = h_data->GetXaxis()->GetBinUpEdge(ibin_x_max);
    int ibin_x_min_pythia_var = h_FSRup_1d->GetXaxis()->FindBin(xmin);
    int ibin_x_max_pythia_var = h_FSRup_1d->GetXaxis()->FindBin(xmax)-1;

    for (auto h : {
        h_FSRup_1d,
        h_FSRdown_1d,
        h_ISRup_1d,
        h_ISRdown_1d,
    }) {
        h->GetXaxis()->SetRange(ibin_x_min_pythia_var, ibin_x_max_pythia_var);
        h->Scale(1/h->Integral(ibin_x_min_pythia_var, ibin_x_max_pythia_var), "width");
        if (observable=="zg") h->GetYaxis()->SetRangeUser(0, 5.5);
    }

    // Make ratios 
    TH1D *h_pythia_ratio_1d = (TH1D *) h_pythia_1d->Clone("h_pythia_ratio_1d");
    h_pythia_ratio_1d->Divide(h_data_1d);

    TH1D *h_herwig_ratio_1d = (TH1D *) h_herwig_1d->Clone("h_herwig_ratio_1d");
    h_herwig_ratio_1d->Divide(h_data_1d);

    TH1D *h_FSRup_ratio_1d = (TH1D *) h_FSRup_1d->Clone("h_FSRup_ratio_1d");
    h_FSRup_ratio_1d->Divide(h_data_1d);

    TH1D *h_FSRdown_ratio_1d = (TH1D *) h_FSRdown_1d->Clone("h_FSRdown_ratio_1d");
    h_FSRdown_ratio_1d->Divide(h_data_1d);

    TH1D *h_ISRup_ratio_1d = (TH1D *) h_ISRup_1d->Clone("h_ISRup_ratio_1d");
    h_ISRup_ratio_1d->Divide(h_data_1d);

    TH1D *h_ISRdown_ratio_1d = (TH1D *) h_ISRdown_1d->Clone("h_ISRdown_ratio_1d");
    h_ISRdown_ratio_1d->Divide(h_data_1d);

    // Grab sys errors 
    TString fin_unc_name = "uncertanties/histos/total_unc_"+TString(inclusive?"incl":"XXT")+"_"+observable+".root";
    std::cout << "Reading unc from : " << fin_unc_name << std::endl;
    TFile *fin_unc = new TFile(fin_unc_name);
    TH1D *h_syst_unc_rel_up = (TH1D *) fin_unc->Get(Form("h_syst_unc_up_%d", ibin_pt))->Clone("h_syst_unc_rel_up");
    TH1D *h_syst_unc_rel_down = (TH1D *) fin_unc->Get(Form("h_syst_unc_down_%d", ibin_pt))->Clone("h_syst_unc_rel_down");

    // Make TGraphAsymErrors
    int nbins_x = h_data_1d->GetNbinsX();
    Double_t points_x[20];
    Double_t points_y[20];
    Double_t unc_up[20];
    Double_t unc_down[20];
    Double_t unc_left[20];
    Double_t unc_right[20];

    Double_t points_x_ratio[20];
    Double_t points_y_ratio[20];
    Double_t unc_up_ratio[20];
    Double_t unc_down_ratio[20];
    Double_t unc_left_ratio[20];
    Double_t unc_right_ratio[20];

    Double_t total_unc_up_ratio[20];
    Double_t total_unc_down_ratio[20];

    int index = 0;
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        double x_right = h_data_1d->GetXaxis()->GetBinUpEdge(ibin_x);
        double x_left = h_data_1d->GetXaxis()->GetBinLowEdge(ibin_x);
        double x_middle = x_left + ((x_right - x_left) / 2);

        double y = h_data_1d->GetBinContent(ibin_x);
        double dy_up = std::abs(h_syst_unc_rel_up->GetBinContent(ibin_x)) * y;
        double dy_down = std::abs(h_syst_unc_rel_down->GetBinContent(ibin_x)) * y;

        double dy_stat = h_data_1d->GetBinError(ibin_x);
        double dy_total_up = std::sqrt((dy_up*dy_up)+ (dy_stat*dy_stat));
        double dy_total_down = std::sqrt((dy_down*dy_down)+ (dy_stat*dy_stat));

        // uncertainties for upper plot
        points_x[index] = x_middle;
        points_y[index] = y;
        unc_up[index] = dy_up;
        unc_down[index] = dy_down;
        unc_left[index] = x_middle - x_left;
        unc_right[index] = x_right - x_middle;

        // uncertainties for ratio
        double y_ratio = 1.;
        double dy_ratio_up = dy_up / y;
        double dy_ratio_down = dy_down / y;

        points_x_ratio[index] = x_middle;
        points_y_ratio[index] = y_ratio;
        unc_up_ratio[index] = dy_ratio_up;
        unc_down_ratio[index] = dy_ratio_down;
        unc_left_ratio[index] = x_middle - x_left;
        unc_right_ratio[index] = x_right - x_middle;

        total_unc_up_ratio[index] = dy_total_up / y;
        total_unc_down_ratio[index] = dy_total_down / y;

        index++;
    }

    TGraphAsymmErrors *gr_unc = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x, points_y, unc_left, unc_right, unc_down, unc_up);
    gr_unc->SetFillStyle(1001);
    gr_unc->SetFillColorAlpha(kBlack, 0.1);
    gr_unc->SetMarkerSize(0);
    gr_unc->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_ratio, points_y_ratio, unc_left_ratio, unc_right_ratio, unc_down_ratio, unc_up_ratio);
    gr_unc_ratio->SetFillStyle(1001);
    gr_unc_ratio->SetFillColorAlpha(kBlack, 0.1);
    gr_unc_ratio->SetMarkerSize(0);
    gr_unc_ratio->SetLineWidth(0);
    gr_unc_ratio->SetLineColorAlpha(kBlack, 0.);

    TGraphAsymmErrors *gr_total_unc_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_ratio, points_y_ratio, unc_left_ratio, unc_right_ratio, total_unc_down_ratio, total_unc_up_ratio);
    gr_total_unc_ratio->SetFillStyle(1001);
    gr_total_unc_ratio->SetFillColorAlpha(kBlack, 0.1);
    gr_total_unc_ratio->SetMarkerSize(0);
    gr_total_unc_ratio->SetLineWidth(0);
    gr_total_unc_ratio->SetLineColorAlpha(kBlack, 0.);

    TLine *line = new TLine(h_data_1d->GetXaxis()->GetBinLowEdge(ibin_x_min), 1, h_data_1d->GetXaxis()->GetBinUpEdge(ibin_x_max), 1);
    line->SetLineStyle(kSolid);
    line->SetLineColor(kBlack);

    // Clone data histogram for legend 
    TH1D *h_data_legend = (TH1D *) h_data_1d->Clone("h_data_legend");
    h_data_legend->SetFillColor(kGray);
    h_data_legend->SetFillStyle(3001);    
    // h_data_legend->SetLineWidth(0);

    // Make a legend
    TLegend *leg;
    if (observable=="zpt") leg = new TLegend(0.22, 0.3, 0.54, 0.55);
    else leg = new TLegend(0.18, 0.05, 0.5, 0.3);
    // if (inclusive) leg = new TLegend(0.4, 0.63, 0.8, 0.87);
    if (inclusive) leg = new TLegend(0.2, 0.1, 0.6, 0.35);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetMargin(0.2);
    leg->AddEntry(h_data_legend, "Data", "lepf");
    leg->AddEntry(h_pythia_1d, "Pythia8 CP5", "l");
    leg->AddEntry(h_FSRup_1d, "Pythia8 CP5 FSR up", "l");
    leg->AddEntry(h_FSRdown_1d, "Pythia8 CP5 FSR down", "l");
    // leg->AddEntry(h_ISRup_1d, "Pythia8 CP5 ISR up", "l");
    // leg->AddEntry(h_ISRdown_1d, "Pythia8 CP5 ISR down", "l");
    leg->AddEntry(h_herwig_1d, "Herwig7 CH3", "l");
    // leg->AddEntry(gr_unc, "Systematic uncertainty", "f");

    // legend for uncertainties in ratio plot
    TLegend *leg_ratio; // keep height=0.09, width=0.19
    if (observable!="zpt") leg_ratio = new TLegend(0.18, 0.81, 0.37, 0.9);
    else leg_ratio = new TLegend(0.7, 0.81, 0.89, 0.9);
    if (observable=="zg"&&inclusive) {
        // std::cout << "here" << std::endl;
        leg_ratio = new TLegend(0.4, 0.81, 0.59, 0.9);
    }
    leg_ratio->SetBorderSize(0);
    leg_ratio->SetFillStyle(0);
    leg_ratio->SetMargin(0.2);
    // leg_ratio->AddEntry(gr_unc_ratio, "Systematic unc.", "fl");
    leg_ratio->AddEntry(gr_total_unc_ratio, "Syst.#oplusStat.", "fl");

    for (auto h : {
        h_pythia_ratio_1d, 
        h_herwig_ratio_1d
    }) {
        h->GetYaxis()->SetRangeUser(0., 2.);
        if (inclusive) h->GetYaxis()->SetRangeUser(0.8, 1.4);
        if (observable=="zg") {
            h->GetYaxis()->SetRangeUser(0.4, 1.6);
            if (inclusive) h->GetYaxis()->SetRangeUser(0.85, 1.15);
        }
        if (observable=="zpt") h->GetYaxis()->SetRangeUser(0., 2.2);
        h->GetYaxis()->SetNdivisions(505);
        h->GetYaxis()->SetTitle("Ratio to data");
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetXaxis()->SetLabelOffset(labelOffset);
        h->GetXaxis()->SetTitleOffset(1.1);
    }    

    // Draw 
    TCanvas *c_result = new TCanvas("c_result", "", 750, 900);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.4, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.4);

    top_pad->SetLeftMargin(0.15);
    top_pad->SetRightMargin(0.05);
    bottom_pad->SetLeftMargin(0.15);
    bottom_pad->SetRightMargin(0.05);
    
    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.45);

    // top_pad->SetGridy();
    // bottom_pad->SetGridy();

    top_pad->cd();
    
    
    h_data_1d->Draw("pe1 same");
    gr_unc->Draw("e2 same");
    h_pythia_1d->Draw("hist same");
    h_herwig_1d->Draw("hist same");
    h_FSRup_1d->Draw("hist same");
    h_FSRdown_1d->Draw("hist same");
    // h_ISRup_1d->Draw("hist same");
    // h_ISRdown_1d->Draw("hist same");
    h_data_1d->Draw("pe1 same");
    h_data_legend->Draw("SAME_E1_][P0");

    leg->Draw();
    drawHeader();
    // Jets text
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    if (observable=="zpt") {
        jet_info->SetTextAlign(10); 
        jet_info->DrawLatex(0.22, 0.8, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.22, 0.73, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
        jet_info->Draw();
    } else if (!inclusive){
        jet_info->SetTextAlign(30); // works with PDF
        jet_info->DrawLatex(0.9, 0.80, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.9, 0.73, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
        jet_info->DrawLatex(0.9, 0.64, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.57, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
    }
    if (inclusive) {
        // jet_info->SetTextAlign(10);
        // jet_info->DrawLatex(0.185, 0.26, "anti-k_{T}, R = 0.4 inclusive jets");
        // jet_info->DrawLatex(0.185, 0.19, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
        // jet_info->DrawLatex(0.185, 0.12, "Soft drop (charged particles)");
        // jet_info->DrawLatex(0.185, 0.05, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
        jet_info->SetTextAlign(30); // works with PDF
        jet_info->DrawLatex(0.91, 0.81, "anti-k_{T}, R = 0.4 inclusive jets");
        jet_info->DrawLatex(0.91, 0.74, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
        jet_info->DrawLatex(0.91, 0.67, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.91, 0.6, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
    }  

    bottom_pad->cd();
    h_pythia_ratio_1d->GetYaxis()->SetTitleOffset(1.5);
    h_pythia_ratio_1d->Draw("hist same");
    // gr_unc_ratio->Draw("e2 same");
    gr_total_unc_ratio->Draw("e2 same");
    line->Draw();
    h_pythia_ratio_1d->Draw("hist same");
    h_herwig_ratio_1d->Draw("hist same");
    h_FSRup_ratio_1d->Draw("hist same");
    h_FSRdown_ratio_1d->Draw("hist same");
    // h_ISRup_ratio_1d->Draw("hist same");
    // h_ISRdown_ratio_1d->Draw("hist same");
    leg_ratio->Draw();

    if (observable=="rg") {
        auto axis5 = new TGaxis(2.1, -0.8, 0. ,-0.8, 0.048982571, 0.4, 2,"NIGS-");
        if (inclusive) axis5 = new TGaxis(2.1, 0.55, 0. ,0.55, 0.048982571, 0.4, 2,"NIGS-");
        axis5->SetTitle("R_{g}");
        axis5->CenterTitle();
        axis5->SetTitleFont(43);
        axis5->SetTitleSize(text_size);
        axis5->SetTitleOffset(1.2);
        axis5->SetLabelFont(43);
        axis5->SetLabelSize(text_size);
        axis5->SetLabelOffset(0.12);
        axis5->SetTickSize(0.05);
        axis5->SetMoreLogLabels(); // add the secondary tick labels
        axis5->SetNoExponent();
        // axis5->ChangeLabel(2, -1, 0.);
        axis5->Draw("same");
    }

    c_result->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    c_result->Draw();
    c_result->Print("plots_an/"+label+"_data_vs_mc_"+observable+".pdf");
    c_result->Print("plots_an/"+label+"_data_vs_mc_"+observable+".png");
}