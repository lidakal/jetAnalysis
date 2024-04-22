#include "../draw_utils.h"

void draw_fit_in1bin(TString observable = "rg", TString jer_opt="nom", TString jec_opt="nom")
{
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString option = "_glued";
    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString label_in = "data";
    if (sample.Contains("herwig")) label_in += "_herwig_" + label;
    else label_in += "_" + label;
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + "_" + observable + option + suffix + ".root";
    std::cout << "fin = " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    TH3D *h_data = (TH3D *) fin->Get(TString("h_data"));
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

    // Note: combine after making the projections

    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");

    // Make projections 

    Int_t nbins_x = h_data->GetNbinsX();
    Int_t nbins_pt = h_data->GetNbinsZ();

    Int_t ibin_pt = 2;
    Int_t ibin_x = 2;
    if (observable=="zpt") ibin_x = nbins_x;

    Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

    Double_t x_min = h_data->GetXaxis()->GetBinLowEdge(ibin_x);
    Double_t x_max = h_data->GetXaxis()->GetBinUpEdge(ibin_x);

    TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
    TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

    // Compile dijet + bjet training and testing - no need to add them all together
    TH1D *h_sig_mb = (TH1D *) h_sig_training_dijet_mb->Clone(Form("h_sig_mb_%d_%d", ibin_pt, ibin_x));
    h_sig_mb->Add(h_sig_testing_dijet_mb);
    h_sig_mb->Add(h_sig_training_bjet_mb);
    h_sig_mb->Add(h_sig_testing_bjet_mb);

    TH1D *h_bkg_bb_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone(Form("h_bkg_bb_mb_%d_%d", ibin_pt, ibin_x));
    h_bkg_bb_mb->Add(h_bkg_bb_testing_dijet_mb);
    h_bkg_bb_mb->Add(h_bkg_bb_training_bjet_mb);
    h_bkg_bb_mb->Add(h_bkg_bb_testing_bjet_mb);

    TH1D *h_bkg_rest_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone(Form("h_bkg_rest_mb_%d_%d", ibin_pt, ibin_x));
    h_bkg_rest_mb->Add(h_bkg_rest_testing_dijet_mb);
    h_bkg_rest_mb->Add(h_bkg_rest_training_bjet_mb);
    h_bkg_rest_mb->Add(h_bkg_rest_testing_bjet_mb);

    Int_t nbins_mb = h_data_mb->GetNbinsX();
    Double_t ndata = h_data_mb->Integral(1, nbins_mb);
    Double_t sig_fraction = h_sig_fraction->GetBinContent(ibin_x, ibin_pt);
    Double_t bkg_bb_fraction = h_bkg_bb_fraction->GetBinContent(ibin_x, ibin_pt);
    Double_t bkg_rest_fraction = h_bkg_rest_fraction->GetBinContent(ibin_x, ibin_pt);

    THStack *h_stack_mb = new THStack(Form("h_stack_mb_%d_%d", ibin_pt, ibin_x), "");
    // h_stack_mb->SetTitle("; m_{B}^{ch}; entries");

    TLegend *leg_mb = new TLegend(0.65, 0.25, 0.8, 0.52);
    leg_mb->SetFillStyle(0);
    leg_mb->SetBorderSize(0);
    leg_mb->SetMargin(0.15);
    leg_mb->SetTextSize(text_size);
    // leg_mb->SetHeader(Form("b tagged jets, k_{T} > 1 GeV"));

    TString range_text;
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";

    if (observable!="zpt" && ibin_x==1) range_text = "fail SD or k_{T} < 1 GeV";
    else if (observable=="rg" && ibin_x==nbins_x) range_text = Form("%.2f < %s", x_min, xlabel.Data());
    else range_text = Form("%.2f < %s < %.2f", x_min, xlabel.Data(), x_max);

    // Format histos
    // for (auto h : {h_data_mb, h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb}) {
    //     h->Sumw2();
    // }
    // h_data_mb->Sumw2();
    h_data_mb->SetMarkerStyle(kFullCircle);
    h_data_mb->SetMarkerColor(kBlack);
    h_data_mb->SetMarkerSize(1);
    h_data_mb->GetYaxis()->SetTitleOffset(1.5);
    h_data_mb->GetYaxis()->SetTitle("entries");
    leg_mb->AddEntry(h_data_mb, "data", "pe1");

    h_sig_mb->Scale(ndata*sig_fraction/h_sig_mb->Integral(1, nbins_mb));
    h_sig_mb->SetFillStyle(1001);
    h_sig_mb->SetFillColor(kRed-7);
    h_sig_mb->SetMarkerStyle(1);
    h_stack_mb->Add(h_sig_mb);
    leg_mb->AddEntry(h_sig_mb, "single b jets", "f");

    h_bkg_bb_mb->Scale(ndata*bkg_bb_fraction/h_bkg_bb_mb->Integral(1, nbins_mb));
    h_bkg_bb_mb->SetFillStyle(1001);
    h_bkg_bb_mb->SetFillColor(kBlue-3);
    h_bkg_bb_mb->SetMarkerStyle(1);
    h_stack_mb->Add(h_bkg_bb_mb);
    leg_mb->AddEntry(h_bkg_bb_mb, "bb jets", "f");

    h_bkg_rest_mb->Scale(ndata*bkg_rest_fraction/h_bkg_rest_mb->Integral(1, nbins_mb));
    h_bkg_rest_mb->SetFillStyle(1001);
    h_bkg_rest_mb->SetFillColor(kGreen-6);
    h_bkg_rest_mb->SetMarkerStyle(1);
    h_stack_mb->Add(h_bkg_rest_mb);
    leg_mb->AddEntry(h_bkg_rest_mb, "gudsc jets", "f");

    TH1D *h_fit_mb = (TH1D *) h_sig_mb->Clone(Form("h_fit_mb_%d_%d", ibin_pt, ibin_x));
    h_fit_mb->Add(h_bkg_bb_mb);
    h_fit_mb->Add(h_bkg_rest_mb);
    h_fit_mb->Sumw2();

    TH1D *h_ratio = (TH1D *) h_data_mb->Clone(Form("h_ratio_%d_%d", ibin_pt, ibin_x));
    h_ratio->Divide(h_fit_mb);
    h_ratio->GetYaxis()->SetNdivisions(8);

    double ymax = std::max({h_data_mb->GetMaximum(), h_fit_mb->GetMaximum()});
    ymax = ymax * 1.1;
    h_data_mb->GetYaxis()->SetRangeUser(0.0001,ymax);

    TCanvas *c_mb = new TCanvas("c_mb","",800,600);
    TPad *top_pad = new TPad("top_pad","",0.,0.33,1.,1.);
    TPad *bottom_pad = new TPad("bottom_pad","",0.,0.,1.,0.33);

    top_pad->SetBottomMargin(0.01);
    bottom_pad->SetTopMargin(0.01);
    bottom_pad->SetBottomMargin(0.25);

    top_pad->cd();
    h_data_mb->Draw("pe1"); // draw once to have the correct scale
    h_stack_mb->Draw("hist e1 same");
    h_data_mb->Draw("pe1 same");
    drawHeader();
    leg_mb->Draw();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->DrawLatex(0.525, 0.8, "anti-k_{T}, R=0.4 b-tagged jets");
    jet_info->DrawLatex(0.5, 0.72, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    if (observable=="rg") jet_info->DrawLatex(0.585, 0.64, range_text);
    else if (observable=="zg") jet_info->DrawLatex(0.65, 0.64, range_text);
    else if (observable=="zpt") jet_info->DrawLatex(0.645, 0.64, range_text);
    if (observable!="zpt") jet_info->DrawLatex(0.695, 0.56, "k_{T} > 1 GeV");
    jet_info->Draw();

    // Plot ratio
    bottom_pad->cd();
    TLine *line = new TLine(0., 1., 7., 1.);
    line->SetLineColor(kGray);
    line->SetLineStyle(kDashed);

    h_ratio->GetXaxis()->SetTitle("m_{B}^{ch}");
    h_ratio->GetYaxis()->SetTitle("data / fit");
    h_ratio->GetXaxis()->SetTitleOffset(3.);
    h_ratio->GetYaxis()->SetTitleOffset(1.5);

    h_ratio->Draw("pe1");
    line->Draw();
    h_ratio->Draw("pe1 same");

    c_mb->cd();
    top_pad->Draw();
    bottom_pad->Draw();

    h_stack_mb->GetYaxis()->SetTitleOffset(1.5);
    
    c_mb->Print("plots_an/template_fit_"+observable+"_in1bin.pdf");
    c_mb->Print("plots_an/template_fit_"+observable+"_in1bin.png");
}