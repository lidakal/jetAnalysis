// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../draw_utils.h"

void split_test(TString observable="rg")
{
    // Dummy test: apply the unfolding to the MC used to fill the matrix
    //             result should be 1

    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

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
    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString fname = "./histos/" + sample + "_" + label + "_response_split_jer_nom_jec_nom.root";
    std::cout << "File in : " << fname << std::endl;

    TFile *fin = new TFile(fname);
    TH2D *h_sig_training_true = (TH2D *) fin->Get("h_sig_training_true_" + observable + "pt");
    TH2D *h_sig_training_reco = (TH2D *) fin->Get("h_sig_training_reco_" + observable + "pt");
    TH2D *h_sig_testing_true = (TH2D *) fin->Get("h_sig_testing_true_" + observable + "pt");
    TH2D *h_sig_testing_reco = (TH2D *) fin->Get("h_sig_testing_reco_" + observable + "pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_" + observable + "pt");

    // Note: Result =  unfold(raw * purity * signal yield) * 1 / (efficiency)
    //       fakes are negligible

    // ---- Purity correction 
    TH2D *h_sig_training_purity = (TH2D *) fin->Get("h_sig_training_purity_" + observable + "pt");
    TH2D *h_sig_testing_reco_purity_corrected = (TH2D *) h_sig_testing_reco->Clone("h_sig_testing_reco_purity_corrected");
    h_sig_testing_reco_purity_corrected->Multiply(h_sig_training_purity);

    TH2D *h_sig_training_reco_purity_corrected = (TH2D *) h_sig_training_reco->Clone("h_sig_training_reco_purity_corrected");
    h_sig_training_reco_purity_corrected->Multiply(h_sig_training_purity);

    // ---- Unfolding
    RooUnfold::ErrorTreatment errorTreatment = RooUnfold::kCovariance;
    // Int_t niter = 1;
    // RooUnfoldBayes unfold(response, h_sig_testing_reco, niter);
    RooUnfoldInvert unfold(response, h_sig_testing_reco_purity_corrected);
    TH2D *h_sig_testing_reco_unfolded = (TH2D *) unfold.Hreco(errorTreatment);
    TH2D *h_sig_testing_reco_refolded = (TH2D *) response->ApplyToTruth(h_sig_testing_reco_unfolded, "h_sig_testing_reco_refolded"); // to be compared to purity corrected sig_testing

    // ---- Efficiency correction
    TH2D *h_sig_training_efficiency = (TH2D *) fin->Get("h_sig_training_efficiency_"+observable+"pt");

    TH2D *h_sig_testing_reco_efficiency_corrected = (TH2D *) h_sig_testing_reco_unfolded->Clone("h_sig_testing_reco_efficiency_corrected");
    h_sig_testing_reco_efficiency_corrected->Divide(h_sig_training_efficiency);

    // ---- Make projections
    Int_t iptmin = h_sig_testing_reco->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_sig_testing_reco->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_sig_testing_reco->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_sig_testing_reco->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_sig_testing_reco_1d = (TH1D *) h_sig_testing_reco->ProjectionX("h_sig_testing_reco_1d", iptmin, iptmax);
    TH1D *h_sig_testing_true_1d = (TH1D *) h_sig_testing_true->ProjectionX("h_sig_testing_true_1d", iptmin, iptmax);
    TH1D *h_sig_testing_reco_purity_corrected_1d = (TH1D *) h_sig_testing_reco_purity_corrected->ProjectionX("h_sig_testing_reco_purity_corrected_1d", iptmin, iptmax);
    TH1D *h_sig_testing_reco_unfolded_1d = (TH1D *) h_sig_testing_reco_unfolded->ProjectionX("h_sig_testing_reco_unfolded_1d", iptmin, iptmax);
    TH1D *h_sig_testing_reco_refolded_1d = (TH1D *) h_sig_testing_reco_refolded->ProjectionX("h_sig_testing_reco_refolded_1d", iptmin, iptmax);
    TH1D *h_sig_testing_reco_efficiency_corrected_1d = (TH1D *) h_sig_testing_reco_efficiency_corrected->ProjectionX("h_sig_testing_reco_efficiency_corrected_1d", iptmin, iptmax);

    TH1D *h_sig_training_true_1d = (TH1D *) h_sig_training_true->ProjectionX("h_sig_training_true_1d", iptmin, iptmax);
    TH1D *h_sig_training_reco_1d = (TH1D *) h_sig_training_reco->ProjectionX("h_sig_training_reco_1d", iptmin, iptmax);
    TH1D *h_sig_training_reco_purity_corrected_1d = (TH1D *) h_sig_training_reco_purity_corrected->ProjectionX("h_sig_training_reco_purity_corrected_1d", iptmin, iptmax);

    // ---- Normalize
    Int_t nbins = h_sig_testing_reco_1d->GetNbinsX();
    Int_t ibin_min = 1; 
    Int_t ibin_max = nbins; 
    // if (observable=="rg") ibin_max = nbins-1;
    double x_min = h_sig_testing_reco_1d->GetXaxis()->GetBinLowEdge(ibin_min);
    double rg_max = h_sig_testing_reco_1d->GetXaxis()->GetBinUpEdge(ibin_max);
    for (auto h : {
                   h_sig_testing_reco_1d, h_sig_testing_true_1d,
                   h_sig_testing_reco_purity_corrected_1d, 
                   h_sig_testing_reco_unfolded_1d,
                   h_sig_testing_reco_refolded_1d,
                   h_sig_testing_reco_efficiency_corrected_1d,
                   h_sig_training_true_1d, h_sig_training_reco_1d,
                   h_sig_training_reco_purity_corrected_1d,
                  }) {
                    h->GetXaxis()->SetRange(ibin_min, ibin_max);
                    h->Scale(1/h->Integral(), "width");
                    h->Sumw2();
                   }


    // ---- Format histos
    THStack *hStack = new THStack("hStack", "");
    hStack->SetTitle("; "+xlabel+"; 1/N_{jets} dN/d("+xlabel+")");

    TLegend *leg = new TLegend(0.5, 0.45, 0.8, 0.75);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin_real, ptMax_real));

    h_sig_testing_reco_1d->SetMarkerColor(kBlack);
    h_sig_testing_reco_1d->SetMarkerStyle(kFullCircle);
    h_sig_testing_reco_1d->SetMarkerSize(1);
    // h_sig_testing_reco_1d->SetLineWidth(0);
    hStack->Add(h_sig_testing_reco_1d, "pe1");
    leg->AddEntry(h_sig_testing_reco_1d, "Detector level pseudo data", "pe1");

    h_sig_testing_true_1d->SetMarkerColor(kBlack);
    h_sig_testing_true_1d->SetLineColor(kBlack);
    h_sig_testing_true_1d->SetMarkerStyle(kOpenCircle);
    h_sig_testing_true_1d->SetMarkerSize(1);
    // h_sig_testing_true_1d->SetLineWidth(0);
    hStack->Add(h_sig_testing_true_1d, "pe1");
    leg->AddEntry(h_sig_testing_true_1d, "Particle level pseudo data", "pe1");

    h_sig_training_reco_1d->SetMarkerColor(kRed);
    h_sig_training_reco_1d->SetLineColor(kRed);
    h_sig_training_reco_1d->SetMarkerStyle(kFullTriangleUp);
    h_sig_training_reco_1d->SetMarkerSize(1);
    // h_sig_training_reco_1d->SetLineWidth(0);
    hStack->Add(h_sig_training_reco_1d, "pe1");
    leg->AddEntry(h_sig_training_reco_1d, "Detector level MC", "pe1");

    h_sig_training_true_1d->SetMarkerColor(kRed);
    h_sig_training_true_1d->SetLineColor(kRed);
    h_sig_training_true_1d->SetMarkerStyle(kOpenTriangleUp);
    h_sig_training_true_1d->SetMarkerSize(1);
    // h_sig_training_true_1d->SetLineWidth(0);
    hStack->Add(h_sig_training_true_1d, "pe1");
    leg->AddEntry(h_sig_training_true_1d, "Particle level MC", "pe1");    

    h_sig_testing_reco_efficiency_corrected_1d->SetMarkerColor(kBlue);
    h_sig_testing_reco_efficiency_corrected_1d->SetLineColor(kBlue);
    h_sig_testing_reco_efficiency_corrected_1d->SetMarkerStyle(kOpenCross);
    h_sig_testing_reco_efficiency_corrected_1d->SetMarkerSize(1);
    // h_sig_testing_reco_efficiency_corrected_1d->SetLineWidth(0);
    hStack->Add(h_sig_testing_reco_efficiency_corrected_1d, "pe1");
    leg->AddEntry(h_sig_testing_reco_efficiency_corrected_1d, "Unfolded pseudo data", "pe1");

    h_sig_testing_reco_refolded_1d->SetMarkerColor(kBlue);
    h_sig_testing_reco_refolded_1d->SetLineColor(kBlue);
    h_sig_testing_reco_refolded_1d->SetMarkerStyle(kFullCross);
    h_sig_testing_reco_refolded_1d->SetMarkerSize(1);
    // h_sig_testing_reco_refolded_1d->SetLineWidth(0g);
    hStack->Add(h_sig_testing_reco_refolded_1d, "pe1");
    leg->AddEntry(h_sig_testing_reco_refolded_1d, "Refolded pseudo data", "pe1");

    // ------- RATIO PLOTS

    THStack *hStack_ratio = new THStack("hStack_ratio", "");
    hStack_ratio->SetTitle("; " + xlabel + "; ratio");

    TLegend *leg_ratio = new TLegend(0.4, 0.3, 0.85, 0.5);
    leg_ratio->SetBorderSize(1);

    TH1D *h_smeared_smeared_ratio = (TH1D *) h_sig_testing_reco_1d->Clone("h_smeared_smeared_ratio");
    h_smeared_smeared_ratio->Divide(h_sig_training_reco_1d);
    h_smeared_smeared_ratio->SetMarkerStyle(kFullCircle);
    h_smeared_smeared_ratio->SetMarkerColor(kBlack);
    h_smeared_smeared_ratio->SetLineColor(kBlack);
    h_smeared_smeared_ratio->SetMarkerSize(1);
    h_smeared_smeared_ratio->SetMinimum(0.);
    h_smeared_smeared_ratio->SetMaximum(2.5);
    h_smeared_smeared_ratio->GetYaxis()->SetTitle("ratio");
    h_smeared_smeared_ratio->GetXaxis()->SetTitle(xlabel);
    h_smeared_smeared_ratio->GetXaxis()->SetTitleOffset(3.5);
    hStack_ratio->Add(h_smeared_smeared_ratio, "pe1");
    leg_ratio->AddEntry(h_smeared_smeared_ratio, "reco pseudo data / reco mc", "pe1");

    TH1D *h_efficiency_corrected_true_ratio = (TH1D *) h_sig_testing_reco_efficiency_corrected_1d->Clone("h_efficiency_corrected_true_ratio");
    // h_efficiency_corrected_true_ratio->Divide(h_sig_training_true_1d);
    h_efficiency_corrected_true_ratio->Divide(h_sig_testing_true_1d);
    h_efficiency_corrected_true_ratio->SetMarkerStyle(kOpenCross);
    h_efficiency_corrected_true_ratio->SetMarkerColor(kBlue);
    h_efficiency_corrected_true_ratio->SetLineColor(kBlue);
    h_efficiency_corrected_true_ratio->SetMarkerSize(1);
    hStack_ratio->Add(h_efficiency_corrected_true_ratio, "pe1");
    leg_ratio->AddEntry(h_efficiency_corrected_true_ratio, "unfolded pseudo data / true pseudo data", "pe1");

    TLine *line = new TLine(x_min, 1., rg_max, 1.);
    line->SetLineWidth(2.); 
    line->SetLineStyle(kDashed);
    line->SetLineColor(kGray);
    
    // ---- Draw canvas

    TCanvas *c_split = new TCanvas("c_split", "", 800, 600);
    TPad *pad1 = new TPad("pad1", "", 0., 0., 1., 0.3);
    TPad *pad2 = new TPad("pad2", "", 0., 0.3, 1., 1.);

    pad1->SetTopMargin(0.);
    pad1->SetBottomMargin(0.3);
    pad2->SetBottomMargin(0.);

    pad1->cd();
    h_smeared_smeared_ratio->Draw("pe1 same");
    h_efficiency_corrected_true_ratio->Draw("pe1 same");
    leg_ratio->Draw();
    line->Draw();

    pad2->cd();
    hStack->Draw("nostack");
    leg->Draw();
    TLatex *test_info_text = new TLatex;
    test_info_text->SetNDC();
    test_info_text->SetTextSize(text_size);
    test_info_text->DrawLatex(0.6, 0.8, "SPLIT TEST");
    test_info_text->Draw();
    if (sample.Contains("herwig")) drawHeaderHerwig();
    else drawHeaderSimulation();

    c_split->cd();
    pad1->Draw();
    pad2->Draw();

    c_split->Draw();    
    c_split->Print("plots_an/"+sample+"_"+label+"_split_test_"+observable+".png");
    
}