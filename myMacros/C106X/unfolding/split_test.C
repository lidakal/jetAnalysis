// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../draw_utils.h"

void split_test(TString observable="rg")
{
    // Dummy test: apply the unfolding to the MC used to fill the matrix
    //             result should be 1

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";
    TString ylabel = "1/N dN/d" + xlabel;

    // ---- Setup 
    gSystem->Load("libRooUnfold.so");
    gStyle->SetErrorX(0.5);

    Double_t ptMin = 100.;
    Double_t ptMax = 120.;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab raw histos
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fname = "./histos/" + sample + "_" + label + "_response_jer_nom_jec_nom.root";
    std::cout << "File in : " << fname << std::endl;
    bool inclusive = sample.Contains("dijet");

    // response, purity, efficiency from half0
    // pseudo data from half1

    TFile *fin = new TFile(fname);
    TH2D *h_half0_efficiency_denominator = (TH2D *) fin->Get("h_half0_efficiency_denominator_" + observable + "pt");
    TH2D *h_half0_purity_denominator = (TH2D *) fin->Get("h_half0_purity_denominator_" + observable + "pt");
    TH2D *h_half1_efficiency_denominator = (TH2D *) fin->Get("h_half1_efficiency_denominator_" + observable + "pt");
    TH2D *h_half1_purity_denominator = (TH2D *) fin->Get("h_half1_purity_denominator_" + observable + "pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_" + observable + "pt_half0");
    TH2D *h_purity = (TH2D *) fin->Get("h_half0_purity_" + observable + "pt");
    TH2D *h_efficiency = (TH2D *) fin->Get("h_half0_efficiency_"+observable+"pt");
    
    // Note: Result =  unfold(raw * purity * signal yield) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Purity correction 
    TH2D *h_purity_corrected = (TH2D *) h_half1_purity_denominator->Clone("h_purity_corrected");
    h_purity_corrected->Multiply(h_purity);

    // TH2D *h_half0_purity_denominator_purity_corrected = (TH2D *) h_half0_purity_denominator->Clone("h_half0_purity_denominator_purity_corrected");
    // h_half0_purity_denominator_purity_corrected->Multiply(h_sig_training_purity);

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    // Int_t niter = 1;
    // RooUnfoldBayes unfold(response, h_half1_purity_denominator, niter);
    RooUnfoldInvert unfold(response, h_purity_corrected);
    TH2D *h_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_refolded = (TH2D *) response->ApplyToTruth(h_unfolded, "h_refolded"); 
    // ---- Efficiency correction
    TH2D *h_efficiency_corrected = (TH2D *) h_unfolded->Clone("h_efficiency_corrected");
    h_efficiency_corrected->Divide(h_efficiency);

    // ---- Make projections
    Int_t iptmin = h_half1_purity_denominator->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_half1_purity_denominator->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_half1_purity_denominator->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_half1_purity_denominator->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_half1_purity_denominator_1d = (TH1D *) h_half1_purity_denominator->ProjectionX("h_half1_purity_denominator_1d", iptmin, iptmax);
    TH1D *h_half1_efficiency_denominator_1d = (TH1D *) h_half1_efficiency_denominator->ProjectionX("h_half1_efficiency_denominator_1d", iptmin, iptmax);
    TH1D *h_purity_corrected_1d = (TH1D *) h_purity_corrected->ProjectionX("h_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_unfolded_1d = (TH1D *) h_unfolded->ProjectionX("h_unfolded_1d", iptmin, iptmax);
    TH1D *h_refolded_1d = (TH1D *) h_refolded->ProjectionX("h_refolded_1d", iptmin, iptmax);
    TH1D *h_efficiency_corrected_1d = (TH1D *) h_efficiency_corrected->ProjectionX("h_efficiency_corrected_1d", iptmin, iptmax);

    // TH1D *h_half0_efficiency_denominator_1d = (TH1D *) h_half0_efficiency_denominator->ProjectionX("h_half0_efficiency_denominator_1d", iptmin, iptmax);
    // TH1D *h_half0_purity_denominator_1d = (TH1D *) h_half0_purity_denominator->ProjectionX("h_half0_purity_denominator_1d", iptmin, iptmax);
    // TH1D *h_half0_purity_denominator_purity_corrected_1d = (TH1D *) h_half0_purity_denominator_purity_corrected->ProjectionX("h_half0_purity_denominator_purity_corrected_1d", iptmin, iptmax);

    // ---- Normalize
    Int_t nbins = h_half1_purity_denominator_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    // if (observable=="rg") ibin_max = nbins-1;
    double x_min = h_half1_purity_denominator_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double rg_max = h_half1_purity_denominator_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {
                   h_half1_purity_denominator_1d, h_half1_efficiency_denominator_1d,
                   h_purity_corrected_1d, 
                   h_unfolded_1d,
                   h_refolded_1d,
                   h_efficiency_corrected_1d,
                  //  h_half0_efficiency_denominator_1d, h_half0_purity_denominator_1d,
                  //  h_half0_purity_denominator_purity_corrected_1d,
                  }) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(), "width");
                    h->Sumw2();
                   }


    // ---- Format histos
    TLegend *leg = new TLegend(0.5, 0.45, 0.8, 0.75);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin_real, ptMax_real));

    h_half1_purity_denominator_1d->SetMarkerColor(kRed);
    h_half1_purity_denominator_1d->SetLineColor(kRed);
    h_half1_purity_denominator_1d->SetMarkerStyle(kFullTriangleUp);
    h_half1_purity_denominator_1d->SetMarkerSize(1);
    // h_half1_purity_denominator_1d->SetLineWidth(0);
    h_half1_purity_denominator_1d->GetYaxis()->SetTitle(ylabel);
    leg->AddEntry(h_half1_purity_denominator_1d, "Detector level pseudo-data", "pe1");
    if (!inclusive) h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(-0.05, 0.7);
    else h_half1_purity_denominator_1d->GetYaxis()->SetRangeUser(0., 0.5);

    h_half1_efficiency_denominator_1d->SetMarkerColor(kBlue);
    h_half1_efficiency_denominator_1d->SetLineColor(kBlue);
    h_half1_efficiency_denominator_1d->SetMarkerStyle(kFullTriangleDown);
    h_half1_efficiency_denominator_1d->SetMarkerSize(1);
    // h_half1_efficiency_denominator_1d->SetLineWidth(0);
    leg->AddEntry(h_half1_efficiency_denominator_1d, "Particle level pseudo-data", "pe1");

    // h_half0_purity_denominator_1d->SetMarkerColor(kRed);
    // h_half0_purity_denominator_1d->SetLineColor(kRed);
    // h_half0_purity_denominator_1d->SetMarkerStyle(kFullTriangleUp);
    // h_half0_purity_denominator_1d->SetMarkerSize(1);
    // // h_half0_purity_denominator_1d->SetLineWidth(0);
    // hStack->Add(h_half0_purity_denominator_1d, "pe1");
    // leg->AddEntry(h_half0_purity_denominator_1d, "Detector level MC", "pe1");

    // h_half0_efficiency_denominator_1d->SetMarkerColor(kRed);
    // h_half0_efficiency_denominator_1d->SetLineColor(kRed);
    // h_half0_efficiency_denominator_1d->SetMarkerStyle(kOpenTriangleUp);
    // h_half0_efficiency_denominator_1d->SetMarkerSize(1);
    // // h_half0_efficiency_denominator_1d->SetLineWidth(0);
    // hStack->Add(h_half0_efficiency_denominator_1d, "pe1");
    // leg->AddEntry(h_half0_efficiency_denominator_1d, "Particle level MC", "pe1");    

    h_efficiency_corrected_1d->SetMarkerColor(kBlack);
    h_efficiency_corrected_1d->SetLineColor(kBlack);
    h_efficiency_corrected_1d->SetMarkerStyle(kFullCircle);
    h_efficiency_corrected_1d->SetMarkerSize(1);
    // h_efficiency_corrected_1d->SetLineWidth(0);
    leg->AddEntry(h_efficiency_corrected_1d, "Unfolded pseudo data", "pe1");

    // h_half1_purity_denominator_refolded_1d->SetMarkerColor(kBlue);
    // h_half1_purity_denominator_refolded_1d->SetLineColor(kBlue);
    // h_half1_purity_denominator_refolded_1d->SetMarkerStyle(kFullCross);
    // h_half1_purity_denominator_refolded_1d->SetMarkerSize(1);
    // // h_half1_purity_denominator_refolded_1d->SetLineWidth(0g);
    // hStack->Add(h_half1_purity_denominator_refolded_1d, "pe1");
    // leg->AddEntry(h_half1_purity_denominator_refolded_1d, "Refolded pseudo data", "pe1");

    // ------- RATIO PLOTS

    // THStack *hStack_ratio = new THStack("hStack_ratio", "");
    // hStack_ratio->SetTitle("; " + xlabel + "; ratio");

    // TLegend *leg_ratio = new TLegend(0.4, 0.3, 0.85, 0.5);
    // leg_ratio->SetBorderSize(1);

    // TH1D *h_smeared_smeared_ratio = (TH1D *) h_half1_purity_denominator_1d->Clone("h_smeared_smeared_ratio");
    // h_smeared_smeared_ratio->Divide(h_half0_purity_denominator_1d);
    // h_smeared_smeared_ratio->SetMarkerStyle(kFullCircle);
    // h_smeared_smeared_ratio->SetMarkerColor(kBlack);
    // h_smeared_smeared_ratio->SetLineColor(kBlack);
    // h_smeared_smeared_ratio->SetMarkerSize(1);
    // h_smeared_smeared_ratio->SetMinimum(0.);
    // h_smeared_smeared_ratio->SetMaximum(2.5);
    // h_smeared_smeared_ratio->GetYaxis()->SetTitle("ratio");
    // h_smeared_smeared_ratio->GetXaxis()->SetTitle(xlabel);
    // h_smeared_smeared_ratio->GetXaxis()->SetTitleOffset(3.5);
    // hStack_ratio->Add(h_smeared_smeared_ratio, "pe1");
    // leg_ratio->AddEntry(h_smeared_smeared_ratio, "reco pseudo data / reco mc", "pe1");

    TH1D *h_true_ratio = (TH1D *) h_efficiency_corrected_1d->Clone("h_true_ratio");
    h_true_ratio->Divide(h_half1_efficiency_denominator_1d);
    h_true_ratio->GetYaxis()->SetTitle("Unfolded / true");
    h_true_ratio->GetXaxis()->SetTitle(xlabel);
    if (!inclusive) h_true_ratio->GetYaxis()->SetRangeUser(0., 2.);
    else h_true_ratio->GetYaxis()->SetRangeUser(0.93, 1.05);
    h_true_ratio->GetXaxis()->SetTitleOffset(3.);
    
    // ---- Draw canvas

    TCanvas *c_split = new TCanvas("c_split", "", 800, 600);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);

    pad1->SetTopMargin(0.01);
    pad1->SetBottomMargin(0.3);
    pad2->SetBottomMargin(0.01);

    pad1->cd();
    h_true_ratio->Draw("pe1 same");

    pad2->cd();
    h_half1_purity_denominator_1d->Draw("pe1");
    h_half1_efficiency_denominator_1d->Draw("pe1 same");
    h_efficiency_corrected_1d->Draw("pe1 same");

    leg->Draw();
    // TLatex *test_info_text = new TLatex;
    // test_info_text->SetNDC();
    // test_info_text->SetTextSize(text_size);
    // test_info_text->DrawLatex(0.6, 0.8, "SPLIT TEST");
    // test_info_text->Draw();
    TLatex *test_info_text = new TLatex;
    test_info_text->SetNDC();
    // test_info_text->SetTextSize(text_size);
    test_info_text->DrawLatex(0.2, 0.2, "SPLIT TEST");
    test_info_text->DrawLatex(0.2, 0.15, sample.Contains("herwig") ? "herwig nominal" : "pythia nominal");
    test_info_text->DrawLatex(0.2, 0.1, inclusive ? "inclusive jets" : "b jets");
    test_info_text->Draw();
    if (sample.Contains("herwig")) drawHeaderHerwig();
    else drawHeaderSimulation();

    c_split->cd();
    pad1->Draw();
    pad2->Draw();

    // c_split->Draw();    
    // c_split->Print("plots_an/"+sample+"_"+label+"_split_test_"+observable+".png");
    
}