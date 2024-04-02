#include "draw_utils.h"

void draw_bjet_vs_incl(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    TString ylabel = "1/N_{2-prong jets} dN/d" + xlabel;

    // ---- Setup 
    gStyle->SetErrorX(0.5);
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab unfolded data
    TString fin_data_XXT_name = "unfolding/histos/bjet_aggrTMVA_XXT_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root";
    std::cout << "Reading b jet data from : " << fin_data_XXT_name << std::endl;
    TFile *fin_data_XXT = new TFile(fin_data_XXT_name);
    TH2D *h_data_XXT = (TH2D *) fin_data_XXT->Get("h_data_unfolded")->Clone("h_data_XXT");

    TString fin_data_inclusive_name = "unfolding/histos/dijet_aggrTMVA_inclusive_unfolded_histograms_"+observable+"_jer_nom_jec_nom.root";
    std::cout << "Reading b jet data from : " << fin_data_inclusive_name << std::endl;
    TFile *fin_data_inclusive = new TFile(fin_data_inclusive_name);
    TH2D *h_data_inclusive = (TH2D *) fin_data_inclusive->Get("h_data_unfolded")->Clone("h_data_inclusive");

    // Grab truth MC 
    TString fin_mc_XXT_name = "unfolding/histos/bjet_aggrTMVA_inclusive_response_full_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_mc_XXT_name << std::endl;
    TFile *fin_mc_XXT = new TFile(fin_mc_XXT_name);
    TH2D *h_mc_XXT = (TH2D *) fin_mc_XXT->Get("h_sig_training_true_"+observable+"pt");

    TString fin_mc_inclusive_name = "unfolding/histos/dijet_aggrTMVA_inclusive_response_full_jer_nom_jec_nom.root";
    std::cout << "Reading mc from : " << fin_mc_inclusive_name << std::endl;
    TFile *fin_mc_inclusive = new TFile(fin_mc_inclusive_name);
    TH2D *h_mc_inclusive = (TH2D *) fin_mc_inclusive->Get("h_training_true_"+observable+"pt");

    // Make projections 
    int ibin_pt = 2;
    double min_pt = h_data_XXT->GetYaxis()->GetBinLowEdge(ibin_pt);
    double max_pt = h_data_XXT->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_data_XXT_1d = (TH1D *) h_data_XXT->ProjectionX("h_data_XXT_1d", ibin_pt, ibin_pt);
    h_data_XXT_1d->SetMarkerStyle(kFullCircle);
    h_data_XXT_1d->SetMarkerColor(kAzure-3); 
    h_data_XXT_1d->SetLineColor(kAzure-3);

    TH1D *h_data_inclusive_1d = (TH1D *) h_data_inclusive->ProjectionX("h_data_inclusive_1d", ibin_pt, ibin_pt);
    h_data_inclusive_1d->SetMarkerStyle(kOpenSquare);
    h_data_inclusive_1d->SetMarkerColor(kRed-3); 
    h_data_inclusive_1d->SetLineColor(kRed-3);

    TH1D *h_mc_XXT_1d = (TH1D *) h_mc_XXT->ProjectionX("h_mc_XXT_1d", ibin_pt, ibin_pt);
    h_mc_XXT_1d->SetMarkerStyle(1);
    h_mc_XXT_1d->SetMarkerColor(kAzure-3); 
    h_mc_XXT_1d->SetLineColor(kAzure-3);
    h_mc_XXT_1d->SetLineStyle(10);
    h_mc_XXT_1d->SetLineWidth(2);

    TH1D *h_mc_inclusive_1d = (TH1D *) h_mc_inclusive->ProjectionX("h_mc_inclusive_1d", ibin_pt, ibin_pt);
    h_mc_inclusive_1d->SetMarkerStyle(1);
    h_mc_inclusive_1d->SetMarkerColor(kRed-3); 
    h_mc_inclusive_1d->SetLineColor(kRed-3);
    h_mc_inclusive_1d->SetLineStyle(9);
    h_mc_inclusive_1d->SetLineWidth(2);

    // Normalize histograms 
    int ibin_x_min = 1;
    int ibin_x_max = h_data_XXT_1d->GetNbinsX();
    if (observable != "zpt") ibin_x_min = 2;
    if (observable == "rg") ibin_x_max = h_data_XXT_1d->GetNbinsX() - 1;

    double labelOffset = h_data_XXT_1d->GetXaxis()->GetLabelOffset();
    std::cout << labelOffset << std::endl;
    for (auto h : {
        h_data_XXT_1d, h_data_inclusive_1d,
        h_mc_XXT_1d, h_mc_inclusive_1d
    }) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        if (observable=="zg") h->GetYaxis()->SetRangeUser(0, 6.);
        else if (observable=="rg") h->GetYaxis()->SetRangeUser(0, 1.1);
        else if (observable=="zpt") h->GetYaxis()->SetRangeUser(0, 4.);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetLabelOffset(10);
    }

    // Make ratios 
    TH1D *h_data_ratio_1d = (TH1D *) h_data_XXT_1d->Clone("h_data_ratio_1d");
    h_data_ratio_1d->Divide(h_data_inclusive_1d);

    TH1D *h_mc_ratio_1d = (TH1D *) h_mc_XXT_1d->Clone("h_mc_ratio_1d");
    h_mc_ratio_1d->Divide(h_mc_inclusive_1d);

    // Grab sys errors 
    TString fin_unc_XXT_name = "uncertanties/histos/total_unc_XXT_"+observable+".root";
    std::cout << "Reading b jet unc from : " << fin_unc_XXT_name << std::endl;
    TFile *fin_unc_XXT = new TFile(fin_unc_XXT_name);
    TH1D *h_syst_unc_rel_XXT_up = (TH1D *) fin_unc_XXT->Get(Form("h_syst_unc_rel_up_%d", ibin_pt))->Clone("h_syst_unc_rel_XXT_up");
    TH1D *h_syst_unc_rel_XXT_down = (TH1D *) fin_unc_XXT->Get(Form("h_syst_unc_rel_down_%d", ibin_pt))->Clone("h_syst_unc_rel_XXT_down");

    TString fin_unc_inclusive_name = "uncertanties/histos/total_unc_incl_"+observable+".root";
    std::cout << "Reading b jet unc from : " << fin_unc_inclusive_name << std::endl;
    TFile *fin_unc_inclusive = new TFile(fin_unc_inclusive_name);
    TH1D *h_syst_unc_rel_inclusive_up = (TH1D *) fin_unc_inclusive->Get(Form("h_syst_unc_rel_up_%d", ibin_pt))->Clone("h_syst_unc_rel_inclusive_up");
    TH1D *h_syst_unc_rel_inclusive_down = (TH1D *) fin_unc_inclusive->Get(Form("h_syst_unc_rel_down_%d", ibin_pt))->Clone("h_syst_unc_rel_inclusive_down");

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
        double dy_ratio_up = std::sqrt(y_ratio*y_ratio*((dy_XXT_up/y_XXT)*(dy_XXT_up/y_XXT)+(dy_inclusive_up/y_inclusive)*(dy_inclusive_up/y_inclusive)));
        double dy_ratio_down = std::sqrt(y_ratio*y_ratio*((dy_XXT_down/y_XXT)*(dy_XXT_down/y_XXT)+(dy_inclusive_down/y_inclusive)*(dy_inclusive_down/y_inclusive)));

        points_x_ratio[index] = x_middle;
        points_y_ratio[index] = y_ratio;
        unc_up_ratio[index] = dy_ratio_up;
        unc_down_ratio[index] = dy_ratio_down;
        unc_left_ratio[index] = x_middle - x_left;
        unc_right_ratio[index] = x_right - x_middle;

        index++;
    }

    TGraphAsymmErrors *gr_unc_XXT = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_XXT, points_y_XXT, unc_left_XXT, unc_right_XXT, unc_down_XXT, unc_up_XXT);
    gr_unc_XXT->SetFillStyle(3001);
    gr_unc_XXT->SetFillColorAlpha(kAzure-3, 0.8);
    gr_unc_XXT->SetMarkerSize(0);
    gr_unc_XXT->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_inclusive = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_inclusive, points_y_inclusive, unc_left_inclusive, unc_right_inclusive, unc_down_inclusive, unc_up_inclusive);
    gr_unc_inclusive->SetFillStyle(3001);
    gr_unc_inclusive->SetFillColorAlpha(kRed-3, 0.8);
    gr_unc_inclusive->SetMarkerSize(0);
    gr_unc_inclusive->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_ratio, points_y_ratio, unc_left_ratio, unc_right_ratio, unc_down_ratio, unc_up_ratio);
    gr_unc_ratio->SetFillStyle(3001);
    gr_unc_ratio->SetFillColorAlpha(kAzure-3, 1);
    gr_unc_ratio->SetMarkerSize(0);
    gr_unc_ratio->SetLineWidth(0);

    TLine *line = new TLine(h_data_XXT_1d->GetXaxis()->GetBinLowEdge(ibin_x_min), 1, h_data_XXT_1d->GetXaxis()->GetBinUpEdge(ibin_x_max), 1);
    line->SetLineStyle(kSolid);
    line->SetLineColor(kBlack);

    // Make a legend
    TLegend *leg = new TLegend(0.4, 0.6, 0.9, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    // leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", min_pt, max_pt));
    leg->AddEntry(h_data_inclusive_1d, "inclusive jets", "pe1");
    leg->AddEntry(h_data_XXT_1d, "single b jets", "pe1");
    leg->AddEntry(h_mc_inclusive_1d, "PYTHIA8 CP5 inclusive jets", "l");
    leg->AddEntry(h_mc_XXT_1d, "PYTHIA8 CP5 single b jets", "l");
    // leg->AddEntry(gr_unc_XXT, "systematic uncertainty", "f");

    for (auto h : {
        h_data_ratio_1d, 
        h_mc_ratio_1d, 
    }) {
        h->GetYaxis()->SetRangeUser(0.2, 1.6);
        h->GetYaxis()->SetNdivisions(10);
        h->GetYaxis()->SetTitle("single b / inclusive");
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetXaxis()->SetLabelOffset(labelOffset);
        h->GetXaxis()->SetTitleOffset(3.5);
    }

    // Draw 
    TCanvas *c_result = new TCanvas("c_result", "", 800, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.33);

    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.3);

    // top_pad->SetGridy();
    // bottom_pad->SetGridy();

    top_pad->cd();
    
    
    h_data_XXT_1d->Draw("pe1 same");
    gr_unc_XXT->Draw("5 same");
    gr_unc_inclusive->Draw("5 same");
    h_data_XXT_1d->Draw("pe1 same");
    h_mc_inclusive_1d->Draw("hist same");
    h_mc_XXT_1d->Draw("hist same");
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
    h_mc_ratio_1d->Draw("pe1 same");
    gr_unc_ratio->Draw("5 same");
    line->Draw();
    h_mc_ratio_1d->Draw("pe1 same");
    h_data_ratio_1d->Draw("pe1 same");


    c_result->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    c_result->Draw();
    c_result->Print("plots_an/bjet_vs_incl_"+observable+".pdf");
}