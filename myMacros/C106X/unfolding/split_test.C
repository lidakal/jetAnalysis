// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../hist_utils.h"

void split_test()
{
    // Dummy test: apply the unfolding to the MC used to fill the matrix
    //             result should be 1

    // ---- Setup 
    gSystem->Load("libRooUnfold.so");
    gStyle->SetErrorX(0.5);

    Double_t ptMin = 80.;
    Double_t ptMax = 100.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab bjet histos
    TString sample = "bjet";
    TString label = "aggrTMVA_withHLT";
    TString fname = "./histos/" + sample + "_" + label + "_response.root";

    TFile *fin = new TFile(fname);
    TH2D *h_sig_training_true_rgpt = (TH2D *) fin->Get("h_sig_training_true_rgpt");
    TH2D *h_sig_training_reco_rgpt = (TH2D *) fin->Get("h_sig_training_reco_rgpt");
    TH2D *h_sig_training_purity_nominator_rgpt = (TH2D *) fin->Get("h_sig_training_purity_nominator_rgpt");
    TH2D *h_sig_training_efficiency_nominator_rgpt = (TH2D *) fin->Get("h_sig_training_efficiency_nominator_rgpt");
    RooUnfoldResponse *response_rgpt = (RooUnfoldResponse *) fin->Get("response_rgpt");

    // TH2D *h_sig_testing_true_rgpt = (TH2D *) fin->Get("h_sig_testing_true_rgpt");
    // TH2D *h_sig_testing_reco_rgpt = (TH2D *) fin->Get("h_sig_testing_reco_rgpt");

    // ---- Grab testing from dijet histos
    TString fname_dijet = "./histos/dijet_aggrTMVA_withHLT_v2_response.root";

    TFile *fin_dijet = new TFile(fname_dijet);
    TH2D *h_sig_testing_true_rgpt = (TH2D *) fin_dijet->Get("h_sig_testing_true_rgpt");
    TH2D *h_sig_testing_reco_rgpt = (TH2D *) fin_dijet->Get("h_sig_testing_reco_rgpt");
    TH2D *h_bkg_bb_testing_true_rgpt = (TH2D *) fin_dijet->Get("h_bkg_bb_testing_true_rgpt");
    TH2D *h_bkg_bb_testing_reco_rgpt = (TH2D *) fin_dijet->Get("h_bkg_bb_testing_reco_rgpt");
    TH2D *h_bkg_rest_testing_true_rgpt = (TH2D *) fin_dijet->Get("h_bkg_rest_testing_true_rgpt");
    TH2D *h_bkg_rest_testing_reco_rgpt = (TH2D *) fin_dijet->Get("h_bkg_rest_testing_reco_rgpt");

    // Merge testing 
    TH2D *h_testing_true_rgpt = (TH2D *) h_sig_testing_true_rgpt->Clone("h_testing_true_rgpt");
    // h_testing_true_rgpt->Add(h_bkg_bb_testing_true_rgpt);
    // h_testing_true_rgpt->Add(h_bkg_rest_testing_true_rgpt);

    TH2D *h_testing_reco_rgpt = (TH2D *) h_sig_testing_reco_rgpt->Clone("h_testing_reco_rgpt");
    h_testing_reco_rgpt->Add(h_bkg_bb_testing_reco_rgpt);
    h_testing_reco_rgpt->Add(h_bkg_rest_testing_reco_rgpt);

    // Grab signal fraction from template fit
    TString fname_fit = "../template_fit/histos/fitted_parameters_RooFit_pseudo_data.root";
    TFile *fin_fit = new TFile(fname_fit);
    TH2D *h_sig_fraction = (TH2D *) fin_fit->Get("h_sig_fraction");    

    // Multiply histograms by signal fraction
    TH2D *h_testing_reco_fit_rgpt = (TH2D *) h_testing_reco_rgpt->Clone();
    h_testing_reco_fit_rgpt->Multiply(h_sig_fraction);

    // Note: Result = unfold(raw * purity) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Purity correction 
    TH2D *h_purity = (TH2D *) h_sig_training_purity_nominator_rgpt->Clone("h_purity");
    h_purity->Divide(h_sig_training_reco_rgpt);

    TH2D *h_testing_reco_fit_purity_corrected_rgpt = (TH2D *) h_testing_reco_fit_rgpt->Clone("h_testing_reco_fit_purity_corrected_rgpt");
    h_testing_reco_fit_purity_corrected_rgpt->Multiply(h_purity);

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    // Int_t niter = 5;
    // RooUnfoldBayes unfold(response_rgpt, h_sig_testing_reco_rgpt, niter);
    RooUnfoldInvert unfold(response_rgpt, h_testing_reco_fit_purity_corrected_rgpt);
    TH2D *h_testing_reco_fit_unfolded_rgpt = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_testing_reco_fit_refolded_rgpt = (TH2D *) response_rgpt->ApplyToTruth(h_testing_reco_fit_unfolded_rgpt, "h_testing_reco_fit_refolded_rgpt"); // to be compared to purity corrected testing fit

    // ---- Efficiency correction
    TH2D *h_efficiency = (TH2D *) h_sig_training_efficiency_nominator_rgpt->Clone("h_efficiency");
    h_efficiency->Divide(h_sig_training_true_rgpt);

    TH2D *h_testing_reco_fit_efficiency_corrected_rgpt = (TH2D *) h_testing_reco_fit_unfolded_rgpt->Clone("h_testing_reco_fit_efficiency_corrected_rgpt");
    h_testing_reco_fit_efficiency_corrected_rgpt->Divide(h_efficiency);

    // ---- Make projections
    Int_t iptmin = h_sig_testing_reco_rgpt->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_sig_testing_reco_rgpt->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_sig_testing_reco_rgpt->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_sig_testing_reco_rgpt->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_testing_reco_fit_rg = (TH1D *) h_testing_reco_fit_rgpt->ProjectionX("h_testing_reco_fit_rg", iptmin, iptmax);
    TH1D *h_testing_true_rg = (TH1D *) h_testing_true_rgpt->ProjectionX("h_testing_true_rg", iptmin, iptmax);
    TH1D *h_testing_reco_fit_purity_corrected_rg = (TH1D *) h_testing_reco_fit_purity_corrected_rgpt->ProjectionX("h_testing_reco_fit_purity_corrected_rg", iptmin, iptmax);
    TH1D *h_testing_reco_fit_unfolded_rg = (TH1D *) h_testing_reco_fit_unfolded_rgpt->ProjectionX("h_testing_reco_fit_unfolded_rg", iptmin, iptmax);
    TH1D *h_testing_reco_fit_refolded_rg = (TH1D *) h_testing_reco_fit_refolded_rgpt->ProjectionX("h_testing_reco_fit_refolded_rg", iptmin, iptmax);
    TH1D *h_testing_reco_fit_efficiency_corrected_rg = (TH1D *) h_testing_reco_fit_efficiency_corrected_rgpt->ProjectionX("h_testing_reco_fit_efficiency_corrected_rg", iptmin, iptmax);
    TH1D *h_sig_training_true_rg = (TH1D *) h_sig_training_true_rgpt->ProjectionX("h_sig_training_true_rg", iptmin, iptmax);
    TH1D *h_sig_training_reco_rg = (TH1D *) h_sig_training_reco_rgpt->ProjectionX("h_sig_training_reco_rg", iptmin, iptmax);

    // ---- Normalize
    Int_t nbins = h_testing_reco_fit_rg->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    for (auto h : {h_testing_reco_fit_rg, h_testing_true_rg,
                   h_testing_reco_fit_purity_corrected_rg, 
                   h_testing_reco_fit_unfolded_rg,
                   h_testing_reco_fit_refolded_rg,
                   h_testing_reco_fit_efficiency_corrected_rg,
                   h_sig_training_true_rg, h_sig_training_reco_rg}) {
                    normalize_histo(h, ibin_min, ibin_max);
                    h->Sumw2();
                    h->SetBinContent(1, h->GetBinContent(1) / 10);
                   }


    // ---- Format histos
    THStack *hStack_testing_rg = new THStack("hStack_testing_rg", "");
    hStack_testing_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    THStack *hStack_sig_training_rg = new THStack("hStack_sig_training_rg", "");
    hStack_sig_training_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    TLegend *leg_rg = new TLegend(0.4, 0.65, 0.8, 0.85);
    leg_rg->SetFillStyle(0);
    leg_rg->SetBorderSize(0);
    leg_rg->SetMargin(0.15);
    leg_rg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real));

    format_histo(h_sig_training_true_rg, mykRed);
    h_sig_training_true_rg->SetLineStyle(kDashed);
    hStack_sig_training_rg->Add(h_sig_training_true_rg);
    // leg_rg->AddEntry(h_sig_training_true_rg, "sig_training true", "l");

    format_histo(h_sig_training_reco_rg, mykRedLight);
    h_sig_training_reco_rg->SetLineStyle(kDashed);
    hStack_sig_training_rg->Add(h_sig_training_reco_rg);
    // leg_rg->AddEntry(h_sig_training_reco_rg, "sig_training reco", "l");

    h_testing_reco_fit_rg->SetMarkerColor(kBlack);
    h_testing_reco_fit_rg->SetMarkerStyle(kOpenCircle);
    h_testing_reco_fit_rg->SetMarkerSize(2);
    // h_testing_reco_fit_rg->SetLineWidth(0);
    hStack_testing_rg->Add(h_testing_reco_fit_rg, "pe1");
    leg_rg->AddEntry(h_testing_reco_fit_rg, "Detector level pseudo data", "p");

    h_testing_true_rg->SetMarkerColor(kBlue);
    h_testing_true_rg->SetLineColor(kBlue);
    h_testing_true_rg->SetMarkerStyle(kOpenDiamond);
    h_testing_true_rg->SetMarkerSize(3);
    // h_testing_true_rg->SetLineWidth(0);
    hStack_testing_rg->Add(h_testing_true_rg, "pe1");
    leg_rg->AddEntry(h_testing_true_rg, "Particle level pseudo data", "p");

    h_testing_reco_fit_efficiency_corrected_rg->SetMarkerColor(kRed);
    h_testing_reco_fit_efficiency_corrected_rg->SetLineColor(kRed);
    h_testing_reco_fit_efficiency_corrected_rg->SetMarkerStyle(kOpenTriangleUp);
    h_testing_reco_fit_efficiency_corrected_rg->SetMarkerSize(2);
    // h_testing_reco_fit_efficiency_corrected_rg->SetLineWidth(0);
    hStack_testing_rg->Add(h_testing_reco_fit_efficiency_corrected_rg, "pe1");
    leg_rg->AddEntry(h_testing_reco_fit_efficiency_corrected_rg, "Unfolded pseudo data", "p");

    // std::cout << "refolded bin 2 = " << h_sig_testing_reco_refolded_rg->GetBinContent(2) << std::endl;
    h_testing_reco_fit_refolded_rg->SetMarkerColor(kGreen);
    h_testing_reco_fit_refolded_rg->SetMarkerStyle(kOpenTriangleDown);
    h_testing_reco_fit_refolded_rg->SetMarkerSize(2);
    // h_testing_reco_fit_refolded_rg->SetLineWidth(0g);
    hStack_testing_rg->Add(h_testing_reco_fit_refolded_rg, "pe1");
    leg_rg->AddEntry(h_testing_reco_fit_refolded_rg, "Refolded pseudo data", "p");

    TPaveText *underflow_text = new TPaveText(-0.4, 0.0, 0., 0.02);
    underflow_text->SetTextSize(text_size);
    underflow_text->SetFillStyle(0);
    underflow_text->SetBorderSize(0);
    underflow_text->AddText("1-prong/10");

    // TPaveText *overflow_text = new TPaveText(1.6, 0.02, 2., 0.07);
    // overflow_text->SetTextSize(text_size);
    // overflow_text->SetFillStyle(0);
    // overflow_text->SetBorderSize(0);
    // overflow_text->AddText("overflow");

    // Make decorations
    TPaveText *info_top_left = new TPaveText(-0.4, 0.27, 0.3, 0.285, "br ndc");
    info_top_left->SetTextSize(text_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->SetBorderSize(0);
    info_top_left->AddText("#bf{CMS} #it{Internal Simulation}");

    TPaveText *info_top_right = new TPaveText(0.8, 0.27, 1.6, 0.285, "br ndc");
    info_top_right->SetTextSize(text_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->SetBorderSize(0);
    info_top_right->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    TPaveText *test_info_text = new TPaveText(-0.2, 0.05, 0.2, 0.1, "br ndc");
    test_info_text->SetTextSize(text_size);
    test_info_text->SetFillStyle(0);
    test_info_text->SetBorderSize(0);
    test_info_text->AddText("SPLIT TEST");

    // ------- RATIO PLOTS

    TH1D *h_efficiency_corrected_true_ratio = (TH1D *) h_testing_reco_fit_efficiency_corrected_rg->Clone("h_efficiency_corrected_true_ratio");
    h_efficiency_corrected_true_ratio->Divide(h_testing_true_rg);
    h_efficiency_corrected_true_ratio->SetMarkerStyle(kFullCircle);
    h_efficiency_corrected_true_ratio->SetMarkerColor(kBlack);
    h_efficiency_corrected_true_ratio->SetLineColor(kBlack);
    h_efficiency_corrected_true_ratio->SetMarkerSize(2);
    h_efficiency_corrected_true_ratio->GetXaxis()->SetTitle("ln(0.4/R_{g})");
    h_efficiency_corrected_true_ratio->GetYaxis()->SetTitle("unfolded / true");
    h_efficiency_corrected_true_ratio->GetXaxis()->SetTitleOffset(4.5);

    TLine *line = new TLine(-0.4, 1., 2., 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);
    
    // ---- Draw canvas

    TCanvas *c_rg = new TCanvas("c_rg", "", 1200, 1000);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);

    pad1->SetTopMargin(0.);
    pad1->SetBottomMargin(0.3);
    pad2->SetBottomMargin(0.);

    pad1->cd();
    h_efficiency_corrected_true_ratio->Draw("pe1");
    line->Draw();

    pad2->cd();
    hStack_testing_rg->Draw("nostack");
    info_top_left->Draw();
    info_top_right->Draw();
    // underflow_text->Draw();
    // overflow_text->Draw();
    // hStack_mc_rg->SetMinimum(0.1);
    // hStack_testing_rg->SetMinimum(0.1);
    double maximum = 0.27;
    // hStack_sig_training_rg->SetMaximum(maximum);
    hStack_testing_rg->SetMaximum(maximum);
    leg_rg->Draw();
    test_info_text->Draw();
    info_top_left->Draw();
    info_top_right->Draw();

    c_rg->cd();
    pad1->Draw();
    pad2->Draw();
    c_rg->Draw();
    c_rg->Print("plots/split_test_rg.png");

    
    // TPaveText *info_top_left_ratio = new TPaveText(-0.4, 0.4, 0.42, 0.42);
    // info_top_left_ratio->SetTextSize(text_size);
    // info_top_left_ratio->SetFillStyle(0);
    // info_top_left_ratio->SetLineWidth(0);
    // info_top_left_ratio->SetBorderSize(0);
    // info_top_left_ratio->AddText("#bf{CMS} #it{Internal Simulation}");

    // TPaveText *info_top_right_ratio = new TPaveText(0.7, 0.4, 2., 0.42);
    // info_top_right_ratio->SetTextSize(text_size);
    // info_top_right_ratio->SetFillStyle(0);
    // info_top_right_ratio->SetLineWidth(0);
    // info_top_right_ratio->SetBorderSize(0);
    // info_top_right_ratio->AddText("PYTHIA8 #sqrt{s} = 5.02 TeV #it{pp}");

    // THStack *h_ratio = new THStack("h_ratio", "");
    // h_ratio->SetTitle("; ln(0.4/R_{g}); ratio");

    // TLegend *leg_ratio = new TLegend(0.2, 0.2, 0.8, 0.4);
    // leg_ratio->SetFillStyle(0);
    // leg_ratio->SetBorderSize(1);
    // // leg_ratio->SetMargin(0.15);
    // leg_ratio->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real));

    

    // TH1D *h_efficiency_corrected_true_ratio = (TH1D *) h_sig_testing_reco_efficiency_corrected_rg->Clone("h_efficiency_corrected_true_ratio");
    // h_efficiency_corrected_true_ratio->Divide(h_sig_testing_true_rg);
    // h_efficiency_corrected_true_ratio->SetMarkerStyle(33);
    // h_efficiency_corrected_true_ratio->SetMarkerColor(mykBlue);
    // h_efficiency_corrected_true_ratio->SetMarkerSize(2);
    // h_ratio->Add(h_efficiency_corrected_true_ratio);
    // leg_ratio->AddEntry(h_efficiency_corrected_true_ratio, "unfolded+corrected / true", "p");

    // TH1D *h_refolded_reco_purity_corrected_ratio = (TH1D *) h_sig_testing_reco_refolded_rg->Clone("h_refolded_reco_purity_corrected_ratio");
    // h_refolded_reco_purity_corrected_ratio->Divide(h_sig_testing_reco_purity_corrected_rg);
    // h_refolded_reco_purity_corrected_ratio->SetMarkerStyle(30);
    // h_refolded_reco_purity_corrected_ratio->SetMarkerColor(mykGreen);
    // h_refolded_reco_purity_corrected_ratio->SetMarkerSize(2);
    // h_ratio->Add(h_refolded_reco_purity_corrected_ratio);
    // leg_ratio->AddEntry(h_refolded_reco_purity_corrected_ratio, "refolded / reco+purity corrected", "p");

    // TPaveText *test_info_text_ratio = new TPaveText(0., 1.3, 1., 1.4);
    // test_info_text_ratio->SetTextSize(text_size);
    // test_info_text_ratio->SetFillStyle(0);
    // test_info_text_ratio->SetBorderSize(0);
    // test_info_text_ratio->AddText("TRIVIAL TEST");

    // TCanvas *c_ratio = new TCanvas("c_ratio", "", 1000, 800);
    // h_ratio->Draw("nostack p");
    // h_ratio->SetMaximum(1.5);
    // h_ratio->SetMinimum(0.5);
    // line->Draw();
    // leg_ratio->Draw();
    // test_info_text_ratio->Draw();
    // info_top_left_ratio->Draw();
    // info_top_right_ratio->Draw();
    // c_ratio->Draw();
    // TString c_ratio_name = "plots/trivial_test_ratio.png";
    // c_ratio->Print(c_ratio_name);
}