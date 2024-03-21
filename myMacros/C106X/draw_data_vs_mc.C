#include "draw_utils.h"

void draw_data_vs_mc(TString observable="zpt")
{
    TString sample = "dijet";
    TString label = "aggrTMVA_inclusive";

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}#equivp_{T}^{B,ch}/p_{T}^{jet,ch}";
    TString ylabel = "1/N dN/dz^{ch}";
    if (observable!="zpt") ylabel = "1/N_{2-prong jets} dN/d" + xlabel;

    // ---- Setup 
    gStyle->SetErrorX(0.5);
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Grab unfolded data
    TString fin_data_name = "unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom_withSF.root";
    if (label.Contains("inclusive")) fin_data_name = "unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom.root";
    std::cout << "Reading b jet data from : " << fin_data_name << std::endl;
    TFile *fin_data = new TFile(fin_data_name);
    TH2D *h_data = (TH2D *) fin_data->Get("h_data_unfolded")->Clone("h_data");

    // Grab truth MC 
    TString fin_pythia_name = "unfolding/histos/"+sample+"_aggrTMVA_inclusive_response_full_jer_nom_jec_nom.root";
    std::cout << "Reading pythia from : " << fin_pythia_name << std::endl;
    TFile *fin_pythia = new TFile(fin_pythia_name);
    TH2D *h_pythia = (TH2D *) fin_pythia->Get("h_sig_training_true_"+observable+"pt");
    if (label.Contains("inclusive")) h_pythia = (TH2D *) fin_pythia->Get("h_training_true_"+observable+"pt");

    TString fin_herwig_name = "unfolding/histos/herwig_"+sample+"_aggrTMVA_inclusive_response_full_jer_nom_jec_nom.root";
    std::cout << "Reading herwig from : " << fin_herwig_name << std::endl;
    TFile *fin_herwig = new TFile(fin_herwig_name);
    TH2D *h_herwig = (TH2D *) fin_herwig->Get("h_sig_training_true_"+observable+"pt");
    if (label.Contains("inclusive")) h_herwig = (TH2D *) fin_herwig->Get("h_training_true_"+observable+"pt");

    // Make projections 
    int ibin_pt = 2;
    double min_pt = h_data->GetYaxis()->GetBinLowEdge(ibin_pt);
    double max_pt = h_data->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_data_1d = (TH1D *) h_data->ProjectionX("h_data_1d", ibin_pt, ibin_pt);
    h_data_1d->SetMarkerStyle(kFullCircle);
    h_data_1d->SetMarkerColor(kBlack); 
    h_data_1d->SetLineColor(kBlack);

    TH1D *h_pythia_1d = (TH1D *) h_pythia->ProjectionX("h_pythia_1d", ibin_pt, ibin_pt);
    h_pythia_1d->SetMarkerStyle(kFullCross);
    h_pythia_1d->SetMarkerColor(kRed); 
    h_pythia_1d->SetLineColor(kRed);

    TH1D *h_herwig_1d = (TH1D *) h_herwig->ProjectionX("h_herwig_1d", ibin_pt, ibin_pt);
    h_herwig_1d->SetMarkerStyle(kFullCrossX);
    h_herwig_1d->SetMarkerColor(kBlue); 
    h_herwig_1d->SetLineColor(kBlue);

    // Normalize histograms 
    int ibin_x_min = 1;
    int ibin_x_max = h_data_1d->GetNbinsX();
    if (observable != "zpt") ibin_x_min = 2;
    if (observable == "rg") ibin_x_max = h_data_1d->GetNbinsX() - 1;

    double labelOffset = h_data_1d->GetXaxis()->GetLabelOffset();
    std::cout << labelOffset << std::endl;
    for (auto h : {
        h_data_1d,
        h_pythia_1d,
        h_herwig_1d
    }) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        if (observable=="zg") h->GetYaxis()->SetRangeUser(0, 6.);
        else if (observable=="rg") h->GetYaxis()->SetRangeUser(0, 1.5);
        else if (observable=="zpt") h->GetYaxis()->SetRangeUser(0, 4.);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetYaxis()->SetTitleOffset(1.5);
        h->GetXaxis()->SetLabelOffset(10);
    }

    // Make ratios 
    TH1D *h_pythia_ratio_1d = (TH1D *) h_pythia_1d->Clone("h_pythia_ratio_1d");
    h_pythia_ratio_1d->Divide(h_data_1d);

    TH1D *h_herwig_ratio_1d = (TH1D *) h_herwig_1d->Clone("h_herwig_ratio_1d");
    h_herwig_ratio_1d->Divide(h_data_1d);

    // Grab sys errors 
    TString fin_unc_name = "uncertanties/histos/total_unc_incl_"+observable+".root";
    if (label.Contains("XXT")) fin_unc_name = "uncertanties/histos/total_unc_XXT_"+observable+".root";
    std::cout << "Reading b jet unc from : " << fin_unc_name << std::endl;
    TFile *fin_unc = new TFile(fin_unc_name);
    TH1D *h_syst_unc_rel_up = (TH1D *) fin_unc->Get(Form("h_syst_unc_rel_up_%d", ibin_pt))->Clone("h_syst_unc_rel_up");
    TH1D *h_syst_unc_rel_down = (TH1D *) fin_unc->Get(Form("h_syst_unc_rel_down_%d", ibin_pt))->Clone("h_syst_unc_rel_down");

    // Make TGraphAsymErrors
    int nbins_x = h_data_1d->GetNbinsX();
    Double_t points_x[20];
    Double_t points_y[20];
    Double_t unc_up[20];
    Double_t unc_down[20];
    Double_t unc_left[20];
    Double_t unc_right[20];

    Double_t points_x_pythia_ratio[20];
    Double_t points_y_pythia_ratio[20];
    Double_t unc_up_pythia_ratio[20];
    Double_t unc_down_pythia_ratio[20];
    Double_t unc_left_pythia_ratio[20];
    Double_t unc_right_pythia_ratio[20];

    Double_t points_x_herwig_ratio[20];
    Double_t points_y_herwig_ratio[20];
    Double_t unc_up_herwig_ratio[20];
    Double_t unc_down_herwig_ratio[20];
    Double_t unc_left_herwig_ratio[20];
    Double_t unc_right_herwig_ratio[20];

    int index = 0;
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        double x_right = h_data_1d->GetXaxis()->GetBinUpEdge(ibin_x);
        double x_left = h_data_1d->GetXaxis()->GetBinLowEdge(ibin_x);
        double x_middle = x_left + ((x_right - x_left) / 2);

        double y = h_data_1d->GetBinContent(ibin_x);
        double dy_up = std::abs(h_syst_unc_rel_up->GetBinContent(ibin_x)) * y;
        double dy_down = std::abs(h_syst_unc_rel_down->GetBinContent(ibin_x)) * y;

        points_x[index] = x_middle;
        points_y[index] = y;
        unc_up[index] = dy_up;
        unc_down[index] = dy_down;
        unc_left[index] = x_middle - x_left;
        unc_right[index] = x_right - x_middle;

        double y_pythia_ratio = 1.;//h_pythia_ratio_1d->GetBinContent(ibin_x);
        double dy_pythia_ratio_up = dy_up / y;//y_pythia_ratio * dy_up / y;
        double dy_pythia_ratio_down = dy_down / y;//y_pythia_ratio * dy_down / y;

        points_x_pythia_ratio[index] = x_middle;
        points_y_pythia_ratio[index] = y_pythia_ratio;
        unc_up_pythia_ratio[index] = dy_pythia_ratio_up;
        unc_down_pythia_ratio[index] = dy_pythia_ratio_down;
        unc_left_pythia_ratio[index] = x_middle - x_left;
        unc_right_pythia_ratio[index] = x_right - x_middle;

        double y_herwig_ratio = 1.;//h_herwig_ratio_1d->GetBinContent(ibin_x);
        double dy_herwig_ratio_up = y_herwig_ratio * dy_up / y;
        double dy_herwig_ratio_down = y_herwig_ratio * dy_down / y;

        points_x_herwig_ratio[index] = x_middle;
        points_y_herwig_ratio[index] = y_herwig_ratio;
        unc_up_herwig_ratio[index] = dy_herwig_ratio_up;
        unc_down_herwig_ratio[index] = dy_herwig_ratio_down;
        unc_left_herwig_ratio[index] = x_middle - x_left;
        unc_right_herwig_ratio[index] = x_right - x_middle;

        index++;
    }

    TGraphAsymmErrors *gr_unc = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x, points_y, unc_left, unc_right, unc_down, unc_up);
    gr_unc->SetFillStyle(3001);
    gr_unc->SetFillColor(kGray);
    gr_unc->SetMarkerSize(0);
    gr_unc->SetLineWidth(0);

    TGraphAsymmErrors *gr_unc_pythia_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_pythia_ratio, points_y_pythia_ratio, unc_left_pythia_ratio, unc_right_pythia_ratio, unc_down_pythia_ratio, unc_up_pythia_ratio);
    gr_unc_pythia_ratio->SetFillStyle(3001);
    gr_unc_pythia_ratio->SetFillColor(kGray);
    gr_unc_pythia_ratio->SetMarkerSize(0);
    gr_unc_pythia_ratio->SetLineWidth(0);

    // TGraphAsymmErrors *gr_unc_herwig_ratio = new TGraphAsymmErrors(ibin_x_max-ibin_x_min+1, points_x_herwig_ratio, points_y_herwig_ratio, unc_left_herwig_ratio, unc_right_herwig_ratio, unc_down_herwig_ratio, unc_up_herwig_ratio);
    // gr_unc_herwig_ratio->SetFillStyle(3004);
    // gr_unc_herwig_ratio->SetFillColor(kBlue);
    // gr_unc_herwig_ratio->SetMarkerSize(0);

    TLine *line = new TLine(h_data_1d->GetXaxis()->GetBinLowEdge(ibin_x_min), 1, h_data_1d->GetXaxis()->GetBinUpEdge(ibin_x_max), 1);
    line->SetLineStyle(kSolid);
    line->SetLineColor(kBlack);

    // Make a legend
    TLegend *leg;
    if (observable=="zpt") leg = new TLegend(0.2, 0.5, 0.5, 0.9);
    else leg = new TLegend(0.5, 0.5, 0.8, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", min_pt, max_pt));
    leg->AddEntry(h_data_1d, "single b jets", "pe1");
    leg->AddEntry(h_pythia_1d, "PYTHIA8 single b jets", "pe1");
    leg->AddEntry(h_herwig_1d, "HERWIG7 single b jets", "pe1");
    leg->AddEntry(gr_unc, "systematic uncertainty", "f");

    for (auto h : {
        h_pythia_ratio_1d, 
        h_herwig_ratio_1d
    }) {
        h->GetYaxis()->SetRangeUser(0.25, 1.75);
        h->GetYaxis()->SetNdivisions(5);
        h->GetYaxis()->SetTitle("mc / data");
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

    top_pad->SetGridy();
    bottom_pad->SetGridy();

    top_pad->cd();
    
    
    h_data_1d->Draw("pe1 same");
    gr_unc->Draw("5 same");
    h_data_1d->Draw("pe1 same");
    h_pythia_1d->Draw("pe1 same");
    h_herwig_1d->Draw("pe1 same");
    
    leg->Draw();
    drawHeader();

    bottom_pad->cd();
    h_pythia_ratio_1d->Draw("pe1 same");
    gr_unc_pythia_ratio->Draw("5 same");
    // gr_unc_herwig_ratio->Draw("5 same");
    line->Draw();
    h_pythia_ratio_1d->Draw("pe1 same");
    h_herwig_ratio_1d->Draw("pe1 same");

    c_result->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    c_result->Draw();
    c_result->Print("plots_an/"+label+"_data_vs_mc_"+observable+".png");
}