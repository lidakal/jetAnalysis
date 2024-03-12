// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../draw_utils.h"

void bottomline_test(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // ---- Setup 
    gSystem->Load("libRooUnfold.so");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    Double_t ptMin = 100.;
    Double_t ptMax = 120.;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab data (b tagged) 
    TString label = "aggrTMVA_XXT";
    TString fname_data = "../template_fit/histos/data_"+label+"_histograms.root";
    std::cout << "Getting data from " << fname_data << std::endl;
    TFile *fin_data = new TFile(fname_data);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_" + observable);
    h_data->GetYaxis()->SetRange(0, h_data->GetNbinsY() + 1); // all mb bins

    int nbins_x = h_data->GetNbinsX();
    int nbins_pt = h_data->GetNbinsZ();
    
    TH2D *h_raw = (TH2D *) h_data->Project3D("zx");
    h_raw->GetXaxis()->SetRange(1, h_raw->GetNbinsX());
    h_raw->GetYaxis()->SetRange(1, h_raw->GetNbinsY());

    // Grab signal fraction from template fit
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    TString option = "_glued";
    TString fname_fit = "../template_fit/histos/fitted_parameters_RooFit_data_"+label+"_" + observable + option + suffix + ".root";
    std::cout << "Getting signal fraction from " << fname_fit << std::endl;
    TFile *fin_fit = new TFile(fname_fit);
    TH2D *h_sig_fraction = (TH2D *) fin_fit->Get("h_sig_fraction");    

    // Multiply histograms by signal fraction
    std::cout << "\t---->Multiplying by signal fraction" << std::endl;
    TH2D *h_raw_fit = (TH2D *) h_raw->Clone("h_raw_fit");
    h_raw_fit->Multiply(h_sig_fraction);

    // Note: Result = unfold(raw * purity) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Grab response matrix + corrections
    TString sample = "bjet"; // sample is used only for unfolding
    TString fname_response_XXT = "./histos/"+sample+"_"+label+"_response_full" + suffix + ".root";
    std::cout << "Getting response from : " << fname_response_XXT << std::endl;
    TFile *fin_response_XXT = new TFile(fname_response_XXT);
    // TH2D *h_sig_training_true = (TH2D *) fin_response_XXT->Get("h_sig_training_true_"+observable+"pt");
    TH2D *h_sig_training_reco = (TH2D *) fin_response_XXT->Get("h_sig_training_reco_"+observable+"pt");
    TH2D *h_sig_training_purity_nominator = (TH2D *) fin_response_XXT->Get("h_sig_training_purity_nominator_"+observable+"pt");
    TH2D *h_sig_training_efficiency_nominator = (TH2D *) fin_response_XXT->Get("h_sig_training_efficiency_nominator_"+observable+"pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin_response_XXT->Get("response_"+observable+"pt");

    // ---- Condition number
    TDecompSVD *svd= new TDecompSVD(response->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    // std::cout << "\nCondition number = " << cond_number
    //           << std::endl;

    // ---- Grab the truth level MC from non b tagged jets ---- 
    TString fname_response_inclusive = "./histos/"+sample+"_aggrTMVA_inclusive_response_full" + suffix + ".root";
    std::cout << "Getting truth from : " << fname_response_inclusive << std::endl;
    TFile *fin_response_inclusive = new TFile(fname_response_inclusive);
    TH2D *h_sig_training_true = (TH2D *) fin_response_inclusive->Get("h_sig_training_true_"+observable+"pt");

    
    // ---- Purity correction
    TH2D *h_sig_training_purity = (TH2D *) fin_response_XXT->Get("h_sig_training_purity_"+observable+"pt");

    std::cout << "\t---->Multiplying data by purity" << std::endl;
    TH2D *h_raw_fit_purity_corrected = (TH2D *) h_raw_fit->Clone("h_raw_fit_purity_corrected");
    h_raw_fit_purity_corrected->Multiply(h_sig_training_purity);

    std::cout << "\t---->Multiplying MC reco by purity" << std::endl;
    TH2D *h_sig_training_reco_purity_corrected = (TH2D *) h_sig_training_reco->Clone("h_sig_training_reco_purity_corrected");
    h_sig_training_reco_purity_corrected->Multiply(h_sig_training_purity);

    // ---- Unfold
    std::cout << "\t---->Unfolding" << std::endl;
    // Int_t niter =  10;
    // RooUnfoldBayes unfold(response, h_raw_fit_purity_corrected, niter);
    RooUnfoldInvert unfold(response, h_raw_fit_purity_corrected);
    // TH2D *h_raw_fit_purity_corrected_unfolded = (TH2D *) h_raw_fit_purity_corrected->Clone();
    TH2D *h_raw_fit_purity_corrected_unfolded = (TH2D *) unfold.Hreco();
    h_raw_fit_purity_corrected_unfolded->SetName("h_raw_fit_purity_corrected_unfolded");

    // ---- Fold back
    std::cout << "\t---->Refolding" << std::endl;
    TH2D *h_raw_fit_purity_corrected_refolded = (TH2D *) response->ApplyToTruth(h_raw_fit_purity_corrected_unfolded, "h_raw_refolded");
    // h_raw_refolded->Divide(h_purity);

    // ---- Efficiency correction 
    std::cout << "\t---->Dividing by recostruction efficiency" << std::endl;
    TH2D *h_sig_training_efficiency = (TH2D *) fin_response_XXT->Get("h_sig_training_efficiency_"+observable+"pt");

    TH2D *h_raw_fit_purity_corrected_unfolded_efficiency_corrected = (TH2D *) h_raw_fit_purity_corrected_unfolded->Clone("h_raw_fit_purity_corrected_unfolded_efficiency_corrected");
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected->Divide(h_sig_training_efficiency);

    // // Grab b tagging efficiency correction 
    TString fname_b_tag_eff = "../btag/histos/"+label+"_" +observable + "_efficiency" + suffix + ".root";
    std::cout << "Getting b tagging efficiency from: " << fname_b_tag_eff << std::endl;
    TFile *fin_b_tag_eff = new TFile(fname_b_tag_eff);
    TH2D *h_eff = (TH2D *) fin_b_tag_eff->Get("h_eff");
    TH2D *h_eff_withSF = (TH2D *) fin_b_tag_eff->Get("h_eff_withSF");
    std::cout << "\t---->Dividing by b tagging efficiency (with SF)" << std::endl;
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected->Divide(h_eff_withSF);

    // REFOLDING TEST FOR SFS SANITY 
    std::cout << "\t---->SF refolding test" << std::endl;
    // undo the efficiency correction but leave the SF
    TH2D *h_unfolded_with_sf = (TH2D *) h_raw_fit_purity_corrected_unfolded_efficiency_corrected->Clone("h_unfolded_with_sf");
    h_unfolded_with_sf->Multiply(h_eff);
    h_unfolded_with_sf->Multiply(h_sig_training_efficiency);
    // and refold
    TH2D *h_refolded_with_sf = (TH2D *) response->ApplyToTruth(h_unfolded_with_sf, "h_refolded_with_sf");
    // fix the unc 
    for (int ibin_x=1; ibin_x<=nbins_x; ibin_x++) {
        for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
            double stat_unc = h_raw_fit_purity_corrected->GetBinError(ibin_x, ibin_pt);
            // std::cout << stat_unc << std::endl;
            h_raw_fit_purity_corrected_refolded->SetBinError(ibin_x, ibin_pt, stat_unc);
            h_refolded_with_sf->SetBinError(ibin_x, ibin_pt, stat_unc);
        }
    }  
    // compare to other refolded => ratio should be the reco SF
    TH2D *h_refolded_ratio = (TH2D *) h_raw_fit_purity_corrected_refolded->Clone("h_refolded_ratio");
    h_refolded_ratio->Divide(h_refolded_with_sf);
    // h_refolded_ratio->Divide(h_b_tag_sf);  

    TH1D *h_refolded_without_sf_1d = (TH1D*) h_raw_fit_purity_corrected_refolded->ProjectionX("h_refolded_without_sf_1d", 2,2);
    h_refolded_without_sf_1d->SetTitle("Refolded without SF");
    // h_refolded_without_sf_1d->Scale(1/h_refolded_without_sf_1d->Integral(), "width");
    h_refolded_without_sf_1d->SetMarkerStyle(kFullCross);

    TH1D *h_refolded_with_sf_1d = (TH1D*) h_refolded_with_sf->ProjectionX("h_refolded_with_sf_1d", 2,2);
    h_refolded_with_sf_1d->SetTitle("Refolded with SF");
    h_refolded_with_sf_1d->SetMarkerColor(kRed);
    h_refolded_with_sf_1d->SetLineColor(kRed);
    // h_refolded_with_sf_1d->Scale(1/h_refolded_with_sf_1d->Integral(), "width");

    // std::cout << h_refolded_without_sf_1d->GetBinContent(1) << std::endl;
    // std::cout << h_refolded_with_sf_1d->GetBinContent(1) << std::endl;

    TH1D *h_refolded_ratio_1d = (TH1D *) h_refolded_ratio->ProjectionX("h_refolded_ratio_1d", 2, 2);
    h_refolded_ratio_1d->SetTitle("SF from refolding");

    TString fname_sfs = "../btag/histos/aggrTMVA_inclusive_"+observable+"_sfs.root";
    std::cout << "Getting reco sfs from: " << fname_sfs << std::endl;
    TFile *fin_sfs = new TFile(fname_sfs);
    TH2D *h_sf = (TH2D *) fin_sfs->Get("h_eff_sf")->Clone("h_sf");
    TH1D *h_sf_1d = (TH1D *) h_sf->ProjectionX("h_sf_1d", 2, 2);
    h_sf_1d->SetMarkerColor(kRed);
    h_sf_1d->SetLineColor(kRed);
    h_sf_1d->SetTitle("SF reco");
    h_sf_1d->GetXaxis()->SetTitle(xlabel);
    h_sf_1d->GetYaxis()->SetTitle("b tagging efficiency SF");
    h_sf_1d->SetMinimum(0.85);
    h_sf_1d->SetMaximum(1.5);

    // ---- Make projections
    Int_t iptmin = h_raw->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_raw->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_raw->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_raw->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_sig_training_true_1d = (TH1D *) h_sig_training_true->ProjectionX("h_sig_training_true_1d", iptmin, iptmax);
    TH1D *h_sig_training_reco_1d = (TH1D *) h_sig_training_reco->ProjectionX("h_sig_training_reco_1d", iptmin, iptmax);
    TH1D *h_sig_training_reco_purity_corrected_1d = (TH1D *) h_sig_training_reco_purity_corrected->ProjectionX("h_sig_training_reco_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_raw_fit_1d = (TH1D *) h_raw_fit->ProjectionX("h_raw_fit_1d", iptmin, iptmax);
    TH1D *h_raw_fit_purity_corrected_1d = (TH1D *) h_raw_fit_purity_corrected->ProjectionX("h_raw_fit_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_raw_fit_purity_corrected_unfolded_1d = (TH1D *) h_raw_fit_purity_corrected_unfolded->ProjectionX("h_raw_fit_purity_corrected_unfolded_1d", iptmin, iptmax);
    TH1D *h_raw_fit_purity_corrected_refolded_1d = (TH1D *) h_raw_fit_purity_corrected_refolded->ProjectionX("h_raw_fit_purity_corrected_refolded_1d", iptmin, iptmax);
    TH1D *h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d = (TH1D *) h_raw_fit_purity_corrected_unfolded_efficiency_corrected->ProjectionX("h_raw_purity_corrected_unfolded_efficiency_corrected_1d", iptmin, iptmax);
    
    // ---- Normalize
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins_x; 

    double x_min = h_raw_fit_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double x_max = h_raw_fit_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {
                   h_sig_training_true_1d, h_sig_training_reco_1d,
                   h_sig_training_reco_purity_corrected_1d,
                   h_raw_fit_1d,
                   h_raw_fit_purity_corrected_1d,
                   h_raw_fit_purity_corrected_unfolded_1d,
                   h_raw_fit_purity_corrected_refolded_1d,
                   h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d
                   }) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(ibin_min, ibin_max), "width");
                   }


    // ---- Format histos
    THStack *hStack_data = new THStack("hStack_data", "");
    hStack_data->SetTitle("; "+xlabel+"; 1/N_{jets} dN/d("+xlabel+")");

    THStack *hStack_mc = new THStack("hStack_mc", "");
    hStack_mc->SetTitle("; "+xlabel+"; 1/N_{jets} dN/d("+xlabel+")");

    TLegend *leg = new TLegend(0.4, 0.45, 0.9, 0.75);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin_real, ptMax_real));

    h_raw_fit_purity_corrected_1d->SetMarkerColor(kBlack);
    h_raw_fit_purity_corrected_1d->SetMarkerStyle(kFullCircle);
    h_raw_fit_purity_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_fit_purity_corrected_1d, "pe1");
    leg->AddEntry(h_raw_fit_purity_corrected_1d, "Detector level data (tagged single b jets)", "pe1");

    h_sig_training_reco_purity_corrected_1d->SetMarkerColor(kRed);
    h_sig_training_reco_purity_corrected_1d->SetLineColor(kRed);
    h_sig_training_reco_purity_corrected_1d->SetMarkerStyle(kFullTriangleUp);
    h_sig_training_reco_purity_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_sig_training_reco_purity_corrected_1d, "pe1");
    leg->AddEntry(h_sig_training_reco_purity_corrected_1d, "Detector level MC (tagged single b jets)", "pe1");

    h_sig_training_true_1d->SetMarkerColor(kRed);
    h_sig_training_true_1d->SetLineColor(kRed);
    h_sig_training_true_1d->SetMarkerStyle(kOpenTriangleUp);
    h_sig_training_true_1d->SetMarkerSize(1);
    hStack_data->Add(h_sig_training_true_1d, "pe1");
    leg->AddEntry(h_sig_training_true_1d, "Particle level MC (single b jets)", "pe1");

    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerColor(kBlue);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d->SetLineColor(kBlue);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerStyle(kOpenCross);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d, "pe1");
    leg->AddEntry(h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d, "Unfolded data (single b jets)", "pe1");

    h_raw_fit_purity_corrected_refolded_1d->SetMarkerColor(kBlue);
    h_raw_fit_purity_corrected_refolded_1d->SetLineColor(kBlue);
    h_raw_fit_purity_corrected_refolded_1d->SetMarkerStyle(kFullCross);
    h_raw_fit_purity_corrected_refolded_1d->SetMarkerSize(1);
    hStack_data->Add(h_raw_fit_purity_corrected_refolded_1d, "pe1");
    leg->AddEntry(h_raw_fit_purity_corrected_refolded_1d, "Refolded data", "pe1");

    TCanvas *c_unfold = new TCanvas("c_unfold", "", 800, 600);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);
    pad1->SetTopMargin(0.01);
    pad1->SetBottomMargin(0.3);
    pad2->SetBottomMargin(0.01);

    pad2->cd();
    hStack_data->Draw("nostack");
    leg->Draw();
    TLatex *test_info_text = new TLatex;
    test_info_text->SetNDC();
    test_info_text->SetTextSize(text_size);
    test_info_text->DrawLatex(0.5, 0.8, "BOTTOMLINE TEST");
    test_info_text->Draw();
    drawHeader();    

    // ---- RATIO PLOT
    THStack *h_ratio = new THStack("h_ratio", "");
    h_ratio->SetTitle("; "+xlabel+"; ratio");

    TLine *line = new TLine(x_min, 1., x_max, 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);

    TLegend *leg_ratio = new TLegend(0.5, 0.3, 0.85, 0.5);
    leg_ratio->SetBorderSize(1);

    TH1D *h_raw_fit_purity_corrected_refolded_smeared_ratio = (TH1D *) h_raw_fit_purity_corrected_refolded_1d->Clone("h_raw_fit_purity_corrected_refolded_smeared_ratio");
    h_raw_fit_purity_corrected_refolded_smeared_ratio->Divide(h_sig_training_reco_purity_corrected_1d);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetMarkerStyle(kFullCircle);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetMarkerColor(kBlack);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetLineColor(kBlack);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetMarkerSize(1);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetMinimum(0.);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->SetMaximum(2.);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->GetYaxis()->SetTitle("ratio");
    h_raw_fit_purity_corrected_refolded_smeared_ratio->GetXaxis()->SetTitle(xlabel);
    h_raw_fit_purity_corrected_refolded_smeared_ratio->GetXaxis()->SetTitleOffset(3.5);
    h_ratio->Add(h_raw_fit_purity_corrected_refolded_smeared_ratio, "pe1");
    leg_ratio->AddEntry(h_raw_fit_purity_corrected_refolded_smeared_ratio, "reco data / reco mc", "pe1");

    TH1D *h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio = (TH1D *) h_raw_fit_purity_corrected_unfolded_efficiency_corrected_1d->Clone("h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio");
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->Divide(h_sig_training_true_1d);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerStyle(kOpenCross);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerColor(kBlue);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->SetLineColor(kBlue);
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->SetMarkerSize(1);
    // h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->SetMaximum(2.);
    h_ratio->Add(h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio, "pe1");
    leg_ratio->AddEntry(h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio, "unfolded data / true mc", "pe1");
    // std::cout << h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->GetBinContent(2) << std::endl;

    pad1->cd();
    h_raw_fit_purity_corrected_refolded_smeared_ratio->Draw("pe1");
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected_ratio->Draw("pe1 same");
    leg_ratio->Draw();
    line->Draw();

    c_unfold->cd();
    pad1->Draw();
    pad2->Draw();
    c_unfold->Draw();
    c_unfold->Print("plots_an/"+sample+"_"+label+"_bottomline_test_"+observable+".png");

    // ---- SF PLOT 
    TCanvas *c_sf_closure = new TCanvas("c_sf_closure", "", 800, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bot_pad = new TPad("bot_pad", "", 0., 0., 1., 0.33);
    bot_pad->SetTopMargin(0.01);
    bot_pad->SetBottomMargin(0.3);
    top_pad->SetBottomMargin(0.01);

    top_pad->cd();
    h_sf_1d->Draw("pe1 same");
    h_refolded_ratio_1d->Draw("pe1 same");
    auto leg_sf = top_pad->BuildLegend();
    leg_sf->SetHeader("100 < p_{T}^{jet} < 120 (GeV)");
    drawHeader();

    bot_pad->cd();
    TH1D *h_sf_ratio_1d = (TH1D *) h_refolded_ratio_1d->Clone("h_sf_ratio_1d");
    h_sf_ratio_1d->Divide(h_sf_1d);
    h_sf_ratio_1d->GetYaxis()->SetTitle("SF refolded / SF reco");
    h_sf_ratio_1d->GetYaxis()->SetNdivisions(6);
    h_sf_ratio_1d->GetXaxis()->SetTitle(xlabel);
    h_sf_ratio_1d->GetXaxis()->SetTitleOffset(4.);
    h_sf_ratio_1d->Draw("pe1");
    line->Draw();

    c_sf_closure->cd();
    top_pad->Draw();
    bot_pad->Draw();

    c_sf_closure->Draw();
    c_sf_closure->Print("plots_an/SF_closure_test_"+observable+".png");

    // ---- Covariance after unfolding
    TMatrixD covariance_matrix = unfold.Ereco();
    TH2D *covariance_histogram = new TH2D(covariance_matrix);
    covariance_histogram->GetXaxis()->SetTitle("Detector level " + xlabel + " * p_{T}^{jet} bins");
    covariance_histogram->GetYaxis()->SetTitle("Particle level " + xlabel + " * p_{T}^{jet} bins");
    TCanvas *c_cov = new TCanvas("c_cov", "covariance", 800, 600);
    covariance_histogram->Draw("colz");

    // ---- Correlation after unfolding
    TH2D *correlation_histogram = (TH2D *) covariance_histogram->Clone("correlation_histogram");
    for (int ibinx=0; ibinx <= correlation_histogram->GetNbinsX()+1; ibinx++) {
        for (int ibiny=0; ibiny <= correlation_histogram->GetNbinsY()+1; ibiny++) {
            double covxy = covariance_histogram->GetBinContent(ibinx, ibiny);
            double covxx = covariance_histogram->GetBinContent(ibinx, ibinx);
            double covyy = covariance_histogram->GetBinContent(ibiny, ibiny);
            correlation_histogram->SetBinContent(ibinx, ibiny, covxy/std::sqrt(covxx*covyy));
        }
    }
    correlation_histogram->GetXaxis()->SetTitle("Detector level " + xlabel + " * p_{T}^{jet} bins");
    correlation_histogram->GetYaxis()->SetTitle("Particle level " + xlabel + " * p_{T}^{jet} bins");
    correlation_histogram->GetZaxis()->SetRangeUser(-1.,1.);
    TCanvas *c_cor = new TCanvas("c_cor", "correlation", 800, 600);
    correlation_histogram->Draw("colz");

    for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
        double y = ibin_pt*nbins_x;
        // std::cout << nbins_pt*nbins_x << std::endl;
        TLine *line_cory = new TLine(0, y, nbins_pt*nbins_x, y);
        TLine *line_corx = new TLine(y, 0, y, nbins_pt*nbins_x);

        c_cov->cd();
        line_cory->Draw();
        line_corx->Draw();

        c_cor->cd();
        line_cory->Draw();
        line_corx->Draw();
    }

    if (sample.Contains("herwig")) drawHeaderHerwig();
    else drawHeaderSimulation();
    c_cor->Draw();
    c_cor->Print("plots_an/"+sample+"_"+label+"_correlation_"+observable+".png");


    TString fout_name = "./histos/aggrTMVA_XXT_unfolded_histograms_"+observable+suffix+"_withSF.root";
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_raw_fit_purity_corrected_unfolded_efficiency_corrected->SetName("h_data_unfolded");
    h_raw_fit_purity_corrected_unfolded_efficiency_corrected->Write();

    h_raw_fit->SetName("h_data_singleb");
    h_raw_fit->Write();

    h_sig_training_reco->SetName("h_mc_singleb");
    h_sig_training_reco->Write();

    fout->Close();
    delete fout;
}