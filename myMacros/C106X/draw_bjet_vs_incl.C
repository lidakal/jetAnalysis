#include "draw_utils.h"
#include "colorblind_palette.h"

void draw_bjet_vs_incl(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N_{2-prong jets} dN/d" + xlabel;

    // ---- Setup 
    gStyle->SetCanvasPreferGL(kTRUE);
    gStyle->SetErrorX(0.5);
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab unfolded data
    TString fin_data_XXT_name = "unfolding/histos/pythia_PF40_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root";
    std::cout << "Reading b jet data from : " << fin_data_XXT_name << std::endl;
    TFile *fin_data_XXT = new TFile(fin_data_XXT_name);
    TH2D *h_data_XXT = (TH2D *) fin_data_XXT->Get("h_data_unfolded")->Clone("h_data_XXT");

    TString fin_data_inclusive_name = "unfolding/histos/dijet_PF40_aggrTMVA_inclusive_unfolded_histograms_"+observable+"_jer_nom_jec_nom.root";
    std::cout << "Reading b jet data from : " << fin_data_inclusive_name << std::endl;
    TFile *fin_data_inclusive = new TFile(fin_data_inclusive_name);
    TH2D *h_data_inclusive = (TH2D *) fin_data_inclusive->Get("h_data_unfolded")->Clone("h_data_inclusive");

    // Grab truth MC 
    TString fin_pythia_XXT_name = "unfolding/histos/pythia_PF40_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_pythia_XXT_name << std::endl;
    TFile *fin_pythia_XXT = new TFile(fin_pythia_XXT_name);
    TH2D *h_pythia_XXT = (TH2D *) fin_pythia_XXT->Get("h_full_efficiency_denominator_"+observable+"pt");

    TString fin_pythia_inclusive_name = "unfolding/histos/dijet_PF40_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_pythia_inclusive_name << std::endl;
    TFile *fin_pythia_inclusive = new TFile(fin_pythia_inclusive_name);
    TH2D *h_pythia_inclusive = (TH2D *) fin_pythia_inclusive->Get("h_full_efficiency_denominator_"+observable+"pt");

    TString fin_herwig_XXT_name = "unfolding/histos/herwig_official_PF40_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_herwig_XXT_name << std::endl;
    TFile *fin_herwig_XXT = new TFile(fin_herwig_XXT_name);
    TH2D *h_herwig_XXT = (TH2D *) fin_herwig_XXT->Get("h_full_efficiency_denominator_"+observable+"pt");

    TString fin_herwig_inclusive_name = "unfolding/histos/herwig_dijet_official_PF40_aggrTMVA_inclusive_response_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_herwig_inclusive_name << std::endl;
    TFile *fin_herwig_inclusive = new TFile(fin_herwig_inclusive_name);
    TH2D *h_herwig_inclusive = (TH2D *) fin_herwig_inclusive->Get("h_full_efficiency_denominator_"+observable+"pt");

    // Make projections 
    int ibin_pt = 2;
    double min_pt = h_data_XXT->GetYaxis()->GetBinLowEdge(ibin_pt);
    double max_pt = h_data_XXT->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_data_XXT_1d = (TH1D *) h_data_XXT->ProjectionX("h_data_XXT_1d", ibin_pt, ibin_pt);
    h_data_XXT_1d->SetMarkerStyle(kFullCircle);
    h_data_XXT_1d->SetMarkerSize(1);
    h_data_XXT_1d->SetMarkerColor(kBlue-3); 
    h_data_XXT_1d->SetLineColor(kBlue-3);
    h_data_XXT_1d->SetLineWidth(1);

    TH1D *h_data_inclusive_1d = (TH1D *) h_data_inclusive->ProjectionX("h_data_inclusive_1d", ibin_pt, ibin_pt);
    h_data_inclusive_1d->SetMarkerStyle(kOpenSquare);
    h_data_inclusive_1d->SetMarkerColor(kRed-3); 
    h_data_inclusive_1d->SetLineColor(kRed-3);
    h_data_inclusive_1d->SetLineWidth(1);

    TH1D *h_pythia_XXT_1d = (TH1D *) h_pythia_XXT->ProjectionX("h_pythia_XXT_1d", ibin_pt, ibin_pt);
    h_pythia_XXT_1d->SetMarkerStyle(1);
    h_pythia_XXT_1d->SetMarkerColor(kBlue-3); 
    h_pythia_XXT_1d->SetLineColor(kBlue-3);
    h_pythia_XXT_1d->SetLineStyle(2);
    h_pythia_XXT_1d->SetLineWidth(2);

    TH1D *h_pythia_inclusive_1d = (TH1D *) h_pythia_inclusive->ProjectionX("h_pythia_inclusive_1d", ibin_pt, ibin_pt);
    h_pythia_inclusive_1d->SetMarkerStyle(1);
    h_pythia_inclusive_1d->SetMarkerColor(kRed-3); 
    h_pythia_inclusive_1d->SetLineColor(kRed-3);
    h_pythia_inclusive_1d->SetLineStyle(6);
    h_pythia_inclusive_1d->SetLineWidth(2);

    TH1D *h_herwig_XXT_1d = (TH1D *) h_herwig_XXT->ProjectionX("h_herwig_XXT_1d", ibin_pt, ibin_pt);
    h_herwig_XXT_1d->SetMarkerStyle(1);
    h_herwig_XXT_1d->SetMarkerColor(kBlue-3); 
    h_herwig_XXT_1d->SetLineColor(kBlue-3);
    h_herwig_XXT_1d->SetLineStyle(2);
    h_herwig_XXT_1d->SetLineWidth(2);

    TH1D *h_herwig_inclusive_1d = (TH1D *) h_herwig_inclusive->ProjectionX("h_herwig_inclusive_1d", ibin_pt, ibin_pt);
    h_herwig_inclusive_1d->SetMarkerStyle(1);
    h_herwig_inclusive_1d->SetMarkerColor(kRed-3); 
    h_herwig_inclusive_1d->SetLineColor(kRed-3);
    h_herwig_inclusive_1d->SetLineStyle(6);
    h_herwig_inclusive_1d->SetLineWidth(2);

    // Normalize histograms 
    int ibin_x_min = 1;
    int ibin_x_max = h_data_XXT_1d->GetNbinsX();
    if (observable != "zpt") ibin_x_min = 2;
    if (observable == "rg") ibin_x_max = h_data_XXT_1d->GetNbinsX() - 1;

    double labelOffset = h_data_XXT_1d->GetXaxis()->GetLabelOffset();
    std::cout << labelOffset << std::endl;
    for (auto h : {
        h_data_XXT_1d, h_data_inclusive_1d,
        h_pythia_XXT_1d, h_pythia_inclusive_1d,
        h_herwig_XXT_1d, h_herwig_inclusive_1d
    }) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        if (observable=="zg") h->GetYaxis()->SetRangeUser(0, 6.);
        else if (observable=="rg") h->GetYaxis()->SetRangeUser(0, 1.15);
        else if (observable=="zpt") h->GetYaxis()->SetRangeUser(0, 4.);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetLabelOffset(10);
    }

    // Make ratios 
    TH1D *h_data_ratio_1d = (TH1D *) h_data_XXT_1d->Clone("h_data_ratio_1d");
    h_data_ratio_1d->Divide(h_data_inclusive_1d);

    TH1D *h_pythia_ratio_1d = (TH1D *) h_pythia_XXT_1d->Clone("h_pythia_ratio_1d");
    h_pythia_ratio_1d->Divide(h_pythia_inclusive_1d);

    TH1D *h_herwig_ratio_1d = (TH1D *) h_herwig_XXT_1d->Clone("h_herwig_ratio_1d");
    h_herwig_ratio_1d->Divide(h_herwig_inclusive_1d);

    // Grab sys errors 
    TString fin_unc_XXT_name = "uncertanties/histos/total_unc_XXT_"+observable+".root";
    std::cout << "Reading b jet unc from : " << fin_unc_XXT_name << std::endl;
    TFile *fin_unc_XXT = new TFile(fin_unc_XXT_name);
    TH1D *h_syst_unc_rel_XXT_up = (TH1D *) fin_unc_XXT->Get(Form("h_syst_unc_up_%d", ibin_pt))->Clone("h_syst_unc_rel_XXT_up");
    TH1D *h_syst_unc_rel_XXT_down = (TH1D *) fin_unc_XXT->Get(Form("h_syst_unc_down_%d", ibin_pt))->Clone("h_syst_unc_rel_XXT_down");

    TString fin_unc_inclusive_name = "uncertanties/histos/total_unc_incl_"+observable+".root";
    std::cout << "Reading b jet unc from : " << fin_unc_inclusive_name << std::endl;
    TFile *fin_unc_inclusive = new TFile(fin_unc_inclusive_name);
    TH1D *h_syst_unc_rel_inclusive_up = (TH1D *) fin_unc_inclusive->Get(Form("h_syst_unc_up_%d", ibin_pt))->Clone("h_syst_unc_rel_inclusive_up");
    TH1D *h_syst_unc_rel_inclusive_down = (TH1D *) fin_unc_inclusive->Get(Form("h_syst_unc_down_%d", ibin_pt))->Clone("h_syst_unc_rel_inclusive_down");

    TString fin_ratio_unc_name = "uncertanties/histos/ratio_unc_"+observable+".root";
    std::cout << "Reading ratio unc from : " << fin_ratio_unc_name << std::endl;
    TFile *fin_ratio_unc = new TFile(fin_ratio_unc_name);
    TH1D *h_ratio_unc_up = (TH1D *) fin_ratio_unc->Get(Form("h_ratio_unc"))->Clone("h_ratio_unc_up");

    // Make TGraphAsymErrors
    int nbins_x = h_data_XXT_1d->GetNbinsX();
    Double_t points_x_XXT[20];
    Double_t points_y_XXT[20];
    Double_t unc_up_XXT[20];
    Double_t unc_down_XXT[20];
    Double_t unc_left_XXT[20];
    Double_t unc_right_XXT[20];

    Double_t points_x_inclusive[20];
    Double_t points_y_inclusive[20];
    Double_t unc_up_inclusive[20];
    Double_t unc_down_inclusive[20];
    Double_t unc_left_inclusive[20];
    Double_t unc_right_inclusive[20];

    Double_t points_x_ratio[20];
    Double_t points_y_ratio[20];
    Double_t unc_up_ratio[20];
    Double_t unc_down_ratio[20];
    Double_t unc_left_ratio[20];
    Double_t unc_right_ratio[20];

    int index = 0;
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        double x_right = h_data_XXT_1d->GetXaxis()->GetBinUpEdge(ibin_x);
        double x_left = h_data_XXT_1d->GetXaxis()->GetBinLowEdge(ibin_x);
        double x_middle = x_left + ((x_right - x_left) / 2);

        double y_XXT = h_data_XXT_1d->GetBinContent(ibin_x);
        double dy_XXT_up = std::abs(h_syst_unc_rel_XXT_up->GetBinContent(ibin_x)) * y_XXT;
        double dy_XXT_down = std::abs(h_syst_unc_rel_XXT_down->GetBinContent(ibin_x)) * y_XXT;

        points_x_XXT[index] = x_middle;
        points_y_XXT[index] = y_XXT;
        unc_up_XXT[index] = dy_XXT_up;
        unc_down_XXT[index] = dy_XXT_down;
        unc_left_XXT[index] = x_middle - x_left;
        unc_right_XXT[index] = x_right - x_middle;

        double y_inclusive = h_data_inclusive_1d->GetBinContent(ibin_x);
        double dy_inclusive_up = std::abs(h_syst_unc_rel_inclusive_up->GetBinContent(ibin_x)) * y_inclusive;
        double dy_inclusive_down = std::abs(h_syst_unc_rel_inclusive_down->GetBinContent(ibin_x)) * y_inclusive;

        points_x_inclusive[index] = x_middle;
        points_y_inclusive[index] = y_inclusive;
        unc_up_inclusive[index] = dy_inclusive_up;
        unc_down_inclusive[index] = dy_inclusive_down;
        unc_left_inclusive[index] = x_middle - x_left;
        unc_right_inclusive[index] = x_right - x_middle;

        double y_ratio = y_XXT / y_inclusive;
        // double dy_ratio_up = std::sqrt(y_ratio*y_ratio*((dy_XXT_up/y_XXT)*(dy_XXT_up/y_XXT)+(dy_inclusive_up/y_inclusive)*(dy_inclusive_up/y_inclusive)));
        // double dy_ratio_down = std::sqrt(y_ratio*y_ratio*((dy_XXT_down/y_XXT)*(dy_XXT_down/y_XXT)+(dy_inclusive_down/y_inclusive)*(dy_inclusive_down/y_inclusive)));

        double dy_ratio_up = y_ratio*h_ratio_unc_up->GetBinContent(ibin_x);
        double dy_ratio_down = dy_ratio_up;
        std::cout << dy_ratio_up << std::endl;

        points_x_ratio[index] = x_middle;
        points_y_ratio[index] = y_ratio;
        unc_up_ratio[index] = dy_ratio_up;
        unc_down_ratio[index] = dy_ratio_down;
        unc_left_ratio[index] = x_middle - x_left;
        unc_right_ratio[index] = x_right - x_middle;

        index++;
    }

    TGraphAsymmErrors *gr_unc_XXT = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_XXT, points_y_XXT, unc_left_XXT, unc_right_XXT, unc_down_XXT, unc_up_XXT);
    gr_unc_XXT->SetFillStyle(1001);
    gr_unc_XXT->SetFillColorAlpha(kAzure-3, 0.1);
    gr_unc_XXT->SetMarkerSize(0);
    gr_unc_XXT->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_inclusive = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_inclusive, points_y_inclusive, unc_left_inclusive, unc_right_inclusive, unc_down_inclusive, unc_up_inclusive);
    gr_unc_inclusive->SetFillStyle(1001);
    gr_unc_inclusive->SetFillColorAlpha(kRed-3, 0.1);
    gr_unc_inclusive->SetMarkerSize(0);
    gr_unc_inclusive->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_ratio, points_y_ratio, unc_left_ratio, unc_right_ratio, unc_down_ratio, unc_up_ratio);
    gr_unc_ratio->SetFillStyle(1001);
    gr_unc_ratio->SetFillColorAlpha(kAzure-3, 0.1);
    gr_unc_ratio->SetMarkerSize(0);
    gr_unc_ratio->SetLineWidth(0);

    TLine *line = new TLine(h_data_XXT_1d->GetXaxis()->GetBinLowEdge(ibin_x_min), 1, h_data_XXT_1d->GetXaxis()->GetBinUpEdge(ibin_x_max), 1);
    line->SetLineStyle(kSolid);
    line->SetLineColor(kBlack);

    // Make a clone of the data histograms for the legend entry
    TH1D *h_data_inclusive_legend = (TH1D *) h_data_inclusive_1d->Clone("h_data_inclusive_legend");
    h_data_inclusive_legend->SetFillStyle(1001);
    h_data_inclusive_legend->SetFillColorAlpha(kRed-3, 0.1);
    // h_data_inclusive_legend->SetLineWidth(1);

    TH1D *h_data_XXT_legend = (TH1D *) h_data_XXT_1d->Clone("h_data_XXT_legend");
    h_data_XXT_legend->SetFillStyle(1001);
    h_data_XXT_legend->SetFillColorAlpha(kAzure-3, 0.1);

    // Make a legend
    TLegend *leg = new TLegend(0.6, 0.6, 0.8, 0.8);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetMargin(0.15);

    leg->AddEntry(h_data_inclusive_legend, "Inclusive jets", "pe1fl");
    leg->AddEntry(h_data_XXT_legend, "Single b jets", "pe1fl");
    // leg->AddEntry(h_pythia_inclusive_1d, "PYTHIA8 CP5 inclusive jets", "l");
    // leg->AddEntry(h_pythia_XXT_1d, "PYTHIA8 CP5 single b jets", "l");

    for (auto h : {
        h_data_ratio_1d, 
        h_pythia_ratio_1d, 
    }) {
        h->GetYaxis()->SetRangeUser(0., 1.8);
        h->GetYaxis()->SetNdivisions(10);
        if (observable=="zg") {
            h->GetYaxis()->SetRangeUser(0.4, 1.5);
            h->GetYaxis()->SetNdivisions(8);
        }
        h->GetYaxis()->SetTitle("Single b / inclusive");
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetXaxis()->SetLabelOffset(labelOffset);
        h->GetXaxis()->SetTitleOffset(3.5);
    }

    // Draw 
    TCanvas *c_result = new TCanvas("c_result", "", 800, 600);
    std::cout << c_result->UseGL() << std::endl;
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.33);

    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.3);

    // top_pad->SetGridy();
    // bottom_pad->SetGridy();

    top_pad->cd();
    
    h_data_XXT_1d->Draw("pe1 same");
    gr_unc_XXT->Draw("e2 same");
    gr_unc_inclusive->Draw("e2 same");
    h_data_XXT_1d->Draw("pe1 same");
    // h_pythia_inclusive_1d->Draw("hist same");
    // h_pythia_XXT_1d->Draw("hist same");
    h_data_inclusive_1d->Draw("pe1 same");
    
    leg->Draw();
    drawHeader();

    // Jets text
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(20);
     jet_info->DrawLatex(0.2, 0.28, "anti-k_{T}, R=0.4 jets");
    jet_info->DrawLatex(0.2, 0.2, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.2, 0.12, "k_{T} > 1 GeV");
    jet_info->Draw();

    bottom_pad->cd();
    h_data_ratio_1d->Draw("pe1 same");
    gr_unc_ratio->Draw("e2 same");
    line->Draw();
    // h_pythia_ratio_1d->Draw("pe1 same");
    h_data_ratio_1d->Draw("pe1 same");


    c_result->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    c_result->Draw();
    c_result->Print("plots_an/bjet_vs_incl_"+observable+".pdf");
    c_result->Print("plots_an/bjet_vs_incl_"+observable+".png");

    // New canvas only with ratio 
    TH1D * h_data_ratio_1d_clone = (TH1D *) h_data_ratio_1d->Clone("h_data_ratio_1d_clone");
    h_data_ratio_1d_clone->SetMarkerStyle(kFullCircle);
    h_data_ratio_1d_clone->SetMarkerSize(1);
    h_data_ratio_1d_clone->SetMarkerColor(kBlack); 
    h_data_ratio_1d_clone->SetLineColor(kBlack);
    h_data_ratio_1d_clone->SetLineWidth(1);
    h_data_ratio_1d_clone->GetXaxis()->SetTitle(xlabel);
    h_data_ratio_1d_clone->GetXaxis()->SetTitleOffset(1.5);

    TH1D *h_data_ratio_legend = (TH1D *) h_data_ratio_1d_clone->Clone("h_data_ratio_legend");
    h_data_ratio_legend->SetFillStyle(1001);
    h_data_ratio_legend->SetFillColorAlpha(kBlack, 0.1);

    TGraphAsymmErrors *gr_unc_ratio_clone = (TGraphAsymmErrors *) gr_unc_ratio->Clone("gr_unc_ratio_clone");
    gr_unc_ratio_clone->SetFillStyle(1001);
    gr_unc_ratio_clone->SetFillColorAlpha(kBlack, 0.1);
    gr_unc_ratio_clone->SetMarkerSize(0);
    gr_unc_ratio_clone->SetLineWidth(0);

    TH1D *h_pythia_ratio_1d_clone = (TH1D *) h_pythia_ratio_1d->Clone("h_pythia_ratio_1d_clone");
    h_pythia_ratio_1d_clone->SetMarkerStyle(1);
    h_pythia_ratio_1d_clone->SetMarkerColor(kOrange-3); 
    h_pythia_ratio_1d_clone->SetLineColor(kOrange-3);
    h_pythia_ratio_1d_clone->SetLineStyle(9);
    h_pythia_ratio_1d_clone->SetLineWidth(2);

    TH1D *h_herwig_ratio_1d_clone = (TH1D *) h_herwig_ratio_1d->Clone("h_herwig_ratio_1d_clone");
    h_herwig_ratio_1d_clone->SetMarkerStyle(1);
    h_herwig_ratio_1d_clone->SetMarkerColor(kAzure-3); 
    h_herwig_ratio_1d_clone->SetLineColor(kAzure-3);
    h_herwig_ratio_1d_clone->SetLineStyle(7);
    h_herwig_ratio_1d_clone->SetLineWidth(2);

    TLegend *leg_ratio = new TLegend(0.6, 0.7, 0.8, 0.85);
    leg_ratio->SetBorderSize(0);
    leg_ratio->SetFillStyle(0);
    leg_ratio->SetMargin(0.15);

    leg_ratio->AddEntry(h_data_ratio_legend, "Data", "pe1fl");
    leg_ratio->AddEntry(h_pythia_ratio_1d_clone, "PYTHIA8 CP5", "l");
    leg_ratio->AddEntry(h_herwig_ratio_1d_clone, "HERWIG7 CH3", "l");

    
    TCanvas *c_ratio = new TCanvas("c_ratio", "", 800, 600);
    h_data_ratio_1d_clone->Draw("pe1 same");
    gr_unc_ratio_clone->Draw("e2 same");
    line->Draw();
    h_data_ratio_1d_clone->Draw("pe1 same");
    h_pythia_ratio_1d_clone->Draw("hist same");
    h_herwig_ratio_1d_clone->Draw("hist same");
    leg_ratio->Draw();
    drawHeader();

    TLatex *jet_info2 = new TLatex;
    jet_info2->SetNDC();
    jet_info2->SetTextSize(20);
    jet_info2->DrawLatex(0.22, 0.3, "anti-k_{T}, R=0.4 jets");
    jet_info2->DrawLatex(0.22, 0.25, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info2->DrawLatex(0.22, 0.2, "k_{T} > 1 GeV");
    jet_info2->Draw();

    c_ratio->Draw();
    c_ratio->Print("plots_an/bjet_vs_incl_"+observable+"_ratio.pdf");
    c_ratio->Print("plots_an/bjet_vs_incl_"+observable+"_ratio.png");

}