#include "../draw_utils.h"

void draw_fractions_in1bin(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Load histograms
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    TString option = "_glued";
    TString sample = "herwig_bjet";
    TString label = "aggrTMVA_XXT";
    TString label_in = "data";
    if (sample.Contains("herwig")) label_in += "_herwig_" + label;
    else label_in += "_" + label;
    TString method = "RooFit";
    TString fin_name = "histos/fitted_parameters_" + method + "_" + label_in + "_" + observable + option + suffix + ".root";
    std::cout << "fin = " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);

    // Load true fractions based on pseudo data
    TH2D *h_sig_fraction_true_training = (TH2D *) fin->Get("h_sig_fraction_true_training");
    TH2D *h_bkg_bb_fraction_true_training = (TH2D *) fin->Get("h_bkg_bb_fraction_true_training");
    TH2D *h_bkg_rest_fraction_true_training = (TH2D *) fin->Get("h_bkg_rest_fraction_true_training");

    // Load fractions estimated from data
    TH2D *h_sig_fraction = (TH2D *) fin->Get("h_sig_fraction");
    TH2D *h_bkg_bb_fraction = (TH2D *) fin->Get("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) fin->Get("h_bkg_rest_fraction");

    Int_t nbins_x = h_sig_fraction->GetNbinsX();
    Int_t nbins_pt = h_sig_fraction->GetNbinsZ();

    TCanvas *c_fraction = new TCanvas("c_fraction", "", 800, 600);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);

    int ibin_pt = 2;
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    Double_t pt_min = h_sig_fraction->GetYaxis()->GetBinLowEdge(ibin_pt);
    Double_t pt_max = h_sig_fraction->GetYaxis()->GetBinUpEdge(ibin_pt);

    // Make projections 
    TH1D *h_sig_fraction_true_per_pt = (TH1D *) h_sig_fraction_true_training->ProjectionX(Form("h_sig_fraction_true_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);
    TH1D *h_sig_fraction_per_pt = (TH1D *) h_sig_fraction->ProjectionX(Form("h_sig_fraction_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);

    TH1D *h_bkg_bb_fraction_true_per_pt = (TH1D *) h_bkg_bb_fraction_true_training->ProjectionX(Form("h_bkg_bb_fraction_true_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);
    TH1D *h_bkg_bb_fraction_per_pt = (TH1D *) h_bkg_bb_fraction->ProjectionX(Form("h_bkg_bb_fraction_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);

    TH1D *h_bkg_rest_fraction_true_per_pt = (TH1D *) h_bkg_rest_fraction_true_training->ProjectionX(Form("h_bkg_rest_fraction_true_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);
    TH1D *h_bkg_rest_fraction_per_pt = (TH1D *) h_bkg_rest_fraction->ProjectionX(Form("h_bkg_rest_fraction_per_pt_%d", ibin_pt), ibin_pt, ibin_pt);

    THStack *h_fractions = new THStack(Form("h_fractions_%d", ibin_pt), "");
    h_fractions->SetTitle("; " + xlabel + "; fraction");

    TLegend *leg_fractions = new TLegend(0.25, 0.38, 0.65, 0.63);
    leg_fractions->SetFillStyle(0);
    leg_fractions->SetBorderSize(0);
    leg_fractions->SetMargin(0.15);
    leg_fractions->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f", pt_min, pt_max));
    leg_fractions->SetNColumns(2);

    h_sig_fraction_per_pt->SetMarkerStyle(kFullCircle);
    h_sig_fraction_per_pt->SetMarkerSize(1);
    h_sig_fraction_per_pt->SetMarkerColor(kBlack);
    h_fractions->Add(h_sig_fraction_per_pt, "pe1");
    leg_fractions->AddEntry(h_sig_fraction_per_pt, TString("signal fraction"), "pe1");

    h_sig_fraction_true_per_pt->SetLineStyle(kDashed);
    h_sig_fraction_true_per_pt->SetLineWidth(1);
    h_sig_fraction_true_per_pt->SetLineColor(kBlack);
    h_sig_fraction_true_per_pt->SetMarkerStyle(1);
    h_fractions->Add(h_sig_fraction_true_per_pt, "hist e1");
    leg_fractions->AddEntry(h_sig_fraction_true_per_pt, "true", "l");

    h_bkg_bb_fraction_per_pt->SetMarkerStyle(kFullTriangleUp);
    h_bkg_bb_fraction_per_pt->SetMarkerSize(1);
    h_bkg_bb_fraction_per_pt->SetMarkerColor(kBlue);
    h_bkg_bb_fraction_per_pt->SetLineColor(kBlue);
    h_fractions->Add(h_bkg_bb_fraction_per_pt, "pe1");
    leg_fractions->AddEntry(h_bkg_bb_fraction_per_pt, TString("bb fraction"), "pe1");

    h_bkg_bb_fraction_true_per_pt->SetLineStyle(kDashed);
    h_bkg_bb_fraction_true_per_pt->SetLineWidth(1);
    h_bkg_bb_fraction_true_per_pt->SetLineColor(kBlue);
    h_bkg_bb_fraction_true_per_pt->SetMarkerStyle(1);
    h_fractions->Add(h_bkg_bb_fraction_true_per_pt, "hist e1");
    leg_fractions->AddEntry(h_bkg_bb_fraction_true_per_pt, "true", "l");

    h_bkg_rest_fraction_per_pt->SetMarkerStyle(kFullCross);
    h_bkg_rest_fraction_per_pt->SetMarkerSize(1);
    h_bkg_rest_fraction_per_pt->SetMarkerColor(kGreen);
    h_bkg_rest_fraction_per_pt->SetLineColor(kGreen);
    h_fractions->Add(h_bkg_rest_fraction_per_pt, "pe1");
    leg_fractions->AddEntry(h_bkg_rest_fraction_per_pt, TString("light+c fraction"), "pe1");

    h_bkg_rest_fraction_true_per_pt->SetLineStyle(kDashed);
    h_bkg_rest_fraction_true_per_pt->SetLineWidth(1);
    h_bkg_rest_fraction_true_per_pt->SetLineColor(kGreen);
    h_bkg_rest_fraction_true_per_pt->SetMarkerStyle(1);
    h_fractions->Add(h_bkg_rest_fraction_true_per_pt, "hist e1");
    leg_fractions->AddEntry(h_bkg_rest_fraction_true_per_pt, "true", "l");

    pad2->SetBottomMargin(0.01);
    // pad2->SetLogy();
    pad2->cd();
    h_fractions->Draw("nostack");
    leg_fractions->Draw();
    drawHeader();

    // Draw ratio 
    THStack *h_fraction_ratio = new THStack("h_fraction_ratio", "");
    h_fraction_ratio->SetTitle("; " + xlabel + "; fit/true");

    TH1D *h_sig_fraction_ratio = (TH1D *) h_sig_fraction_per_pt->Clone(Form("h_fraction_ratio_%d", ibin_pt));
    h_sig_fraction_ratio->Divide(h_sig_fraction_true_per_pt);
    h_sig_fraction_ratio->GetXaxis()->SetTitle(xlabel);
    h_sig_fraction_ratio->GetXaxis()->SetTitleOffset(2.5);
    h_sig_fraction_ratio->GetYaxis()->SetNdivisions(6);
    h_sig_fraction_ratio->GetYaxis()->SetTitle("fit fraction/true");
    h_sig_fraction_ratio->SetMinimum(0);
    h_sig_fraction_ratio->SetMaximum(2);
    h_fraction_ratio->Add(h_sig_fraction_ratio, "pe1");

    TH1D *h_bkg_bb_fraction_ratio = (TH1D *) h_bkg_bb_fraction_per_pt->Clone(Form("h_fraction_ratio_%d", ibin_pt));
    h_bkg_bb_fraction_ratio->Divide(h_bkg_bb_fraction_true_per_pt);
    h_bkg_bb_fraction_ratio->GetXaxis()->SetTitleOffset(2.5);
    h_bkg_bb_fraction_ratio->GetYaxis()->SetNdivisions(6);
    h_fraction_ratio->Add(h_bkg_bb_fraction_ratio, "pe1");

    TH1D *h_bkg_rest_fraction_ratio = (TH1D *) h_bkg_rest_fraction_per_pt->Clone(Form("h_fraction_ratio_%d", ibin_pt));
    h_bkg_rest_fraction_ratio->Divide(h_bkg_rest_fraction_true_per_pt);
    h_bkg_rest_fraction_ratio->GetXaxis()->SetTitleOffset(2.5);
    h_bkg_rest_fraction_ratio->GetYaxis()->SetNdivisions(6);
    h_fraction_ratio->Add(h_bkg_rest_fraction_ratio, "pe1");

    pad1->SetTopMargin(0.05);
    pad1->SetBottomMargin(0.3);
    pad1->cd();
    h_sig_fraction_ratio->Draw("pe1");
    h_bkg_bb_fraction_ratio->Draw("pe1 same");
    h_bkg_rest_fraction_ratio->Draw("pe1 same");

    TLine *line = new TLine(h_sig_fraction_per_pt->GetXaxis()->GetBinLowEdge(1), 1., h_sig_fraction_per_pt->GetXaxis()->GetBinUpEdge(h_sig_fraction_per_pt->GetNbinsX()), 1);
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);
    line->Draw();

    c_fraction->cd();
    pad1->Draw();
    pad2->Draw();


    c_fraction->Update();
    c_fraction->Print("plots_an/"+sample+"_"+label+"_fit_fraction_"+observable+".png");
    // Make decorations
    TPaveText *info_top_left = new TPaveText(-0.4, 0.865, 0.85, 0.94, "nb ndc");
    info_top_left->SetTextSize(text_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

    TPaveText *info_top_right = new TPaveText(0.2, 0.87, 1.6, 0.95, "nb ndc");
    info_top_right->SetTextSize(text_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");
        
}