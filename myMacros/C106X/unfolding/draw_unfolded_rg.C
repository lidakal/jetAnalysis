// #include "../../../../RooUnfoldOld/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfoldOld/src/RooUnfoldInvert.h"
#include "../hist_utils.h"

void draw_unfolded_rg()
{
    // ---- Setup 
    gSystem->Load("libRooUnfold.so");

    Double_t ptMin = 100.;
    Double_t ptMax = 120.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TString sample = "bjet";
    TString label = "aggrTMVA_withNb";
    TString fname = "./histos/qcd_" + sample + "_" + label + "_unfold_2d.root";

    TFile *fin = new TFile(fname);
    TH2D *h_data_reco_rgpt = (TH2D *) fin->Get("h_data_reco_rgpt");
    TH2D *h_data_true_rgpt = (TH2D *) fin->Get("h_data_true_rgpt");
    TH2D *h_mc_true_rgpt = (TH2D *) fin->Get("h_mc_true_rgpt");
    TH2D *h_mc_reco_rgpt = (TH2D *) fin->Get("h_mc_reco_rgpt");
    TH2D *h_mc_fake_rgpt = (TH2D *) fin->Get("h_mc_fake_rgpt");
    RooUnfoldResponse *response_rgpt = (RooUnfoldResponse *) fin->Get("response_rgpt");

    // ---- Condition number
    TDecompSVD *svd= new TDecompSVD(response_rgpt->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    std::cout << "Largest value = " << singular_values.Max() 
              << "\nSmallest value = " << singular_values.Min()
              << "\nConditional number = " << cond_number
              << std::endl;

    // ---- Unfold
    Int_t niter = 5;
    RooUnfoldBayes unfold(response_rgpt, h_data_reco_rgpt, niter);
    // RooUnfoldInvert unfold(response_rgpt, h_data_reco_rgpt);
    // TH2D *h_data_reco_rgpt_unfolded = (TH2D *) unfold.Hunfold();
    TH2D *h_data_reco_rgpt_unfolded = (TH2D *) unfold.Hreco();

    // ---- Covariance
    auto covariance = unfold.GetMeasuredCov(); // works

    // ---- Fold back
    // std::cout << "Refolding" << std::endl;
    TH2D *h_data_reco_rgpt_refolded = (TH2D *) response_rgpt->ApplyToTruth(h_data_reco_rgpt_unfolded, "h_data_reco_rgpt_refolded");

    // ---- Make projections
    Int_t iptmin = h_data_reco_rgpt->GetYaxis()->FindBin(ptMin);
    Int_t iptmax = h_data_reco_rgpt->GetYaxis()->FindBin(ptMax) - 1;
    Double_t ptMin_real = h_data_reco_rgpt->GetYaxis()->GetBinLowEdge(iptmin);
    Double_t ptMax_real = h_data_reco_rgpt->GetYaxis()->GetBinUpEdge(iptmax);

    TH1D *h_data_reco_rg = (TH1D *) h_data_reco_rgpt->ProjectionX("h_data_reco_rg", iptmin, iptmax);
    TH1D *h_data_true_rg = (TH1D *) h_data_true_rgpt->ProjectionX("h_data_true_rg", iptmin, iptmax);
    TH1D *h_data_reco_rg_unfolded = (TH1D *) h_data_reco_rgpt_unfolded->ProjectionX("h_data_reco_rg_unfolded", iptmin, iptmax);
    TH1D *h_data_reco_rg_refolded = (TH1D *) h_data_reco_rgpt_refolded->ProjectionX("h_data_reco_rg_refolded", iptmin, iptmax);
    TH1D *h_mc_true_rg = (TH1D *) h_mc_true_rgpt->ProjectionX("h_mc_true_rg", iptmin, iptmax);
    TH1D *h_mc_reco_rg = (TH1D *) h_mc_reco_rgpt->ProjectionX("h_mc_reco_rg", iptmin, iptmax);
    TH1D *h_mc_fake_rg = (TH1D *) h_mc_fake_rgpt->ProjectionX("h_mc_fake_rg", iptmin, iptmax);    

    // reco = Au (refolded) + fake
    h_data_reco_rg_refolded->Add(h_mc_fake_rg);

    // ---- Normalize
    Int_t nbins = h_data_reco_rg->GetNbinsX();
    Int_t ibin_min = 1; // underflow already included in the histo
    Int_t ibin_max = nbins; // overflow already included in the histo
    for (auto h : {h_data_reco_rg, h_data_true_rg,
                   h_data_reco_rg_unfolded, h_data_reco_rg_refolded,
                   h_mc_true_rg, h_mc_reco_rg}) {
                    normalize_histo(h, ibin_min, ibin_max);
                    // h->GetXaxis()->SetRange(2, nbins - 1);
                    h->SetBinContent(1, h->GetBinContent(1) / 10);
                   }


    // ---- Format histos
    THStack *hStack_data_rg = new THStack("hStack_data_rg", "");
    hStack_data_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    THStack *hStack_mc_rg = new THStack("hStack_mc_rg", "");
    hStack_mc_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    TLegend *leg_rg = new TLegend(0.4, 0.7, 0.8, 0.9);
    leg_rg->SetFillStyle(0);
    leg_rg->SetBorderSize(0);
    leg_rg->SetMargin(0.15);
    leg_rg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real));

    format_histo(h_mc_true_rg, mykRed);
    hStack_mc_rg->Add(h_mc_true_rg);
    leg_rg->AddEntry(h_mc_true_rg, "MC true", "l");

    format_histo(h_mc_reco_rg, mykRedLight);
    hStack_mc_rg->Add(h_mc_reco_rg);
    leg_rg->AddEntry(h_mc_reco_rg, "MC reco", "l");

    // format_histo(h_data_reco_rg, mykBlueLight);
    h_data_reco_rg->SetMarkerColor(mykBlueLight);
    h_data_reco_rg->SetMarkerStyle(20);
    h_data_reco_rg->SetMarkerSize(3);
    hStack_data_rg->Add(h_data_reco_rg);
    leg_rg->AddEntry(h_data_reco_rg, "pseudo data", "p");

    h_data_true_rg->SetMarkerColor(mykBlueLight);
    h_data_true_rg->SetMarkerStyle(kCircle);
    h_data_true_rg->SetMarkerSize(3);
    hStack_data_rg->Add(h_data_true_rg);
    leg_rg->AddEntry(h_data_true_rg, "pseudo data - true", "p");

    // format_histo(h_data_rg, mykBlue);
    h_data_reco_rg_unfolded->SetMarkerColor(mykBlue);
    h_data_reco_rg_unfolded->SetMarkerStyle(33);
    h_data_reco_rg_unfolded->SetMarkerSize(4);
    hStack_data_rg->Add(h_data_reco_rg_unfolded);
    leg_rg->AddEntry(h_data_reco_rg_unfolded, "pseudo data unfolded", "p");

    h_data_reco_rg_refolded->SetMarkerColor(mykGreen);
    h_data_reco_rg_refolded->SetMarkerStyle(30);
    h_data_reco_rg_refolded->SetMarkerSize(4);
    hStack_data_rg->Add(h_data_reco_rg_refolded);
    leg_rg->AddEntry(h_data_reco_rg_refolded, "pseudo data re-folded", "p");

    TPaveText *underflow_text = new TPaveText(-0.2, 0.28, 0.2, 0.32);
    underflow_text->SetTextSize(text_size);
    underflow_text->SetFillStyle(0);
    underflow_text->SetBorderSize(0);
    underflow_text->AddText("underflow / 10");

    TPaveText *overflow_text = new TPaveText(2.2, 0.02, 2.4, 0.07);
    overflow_text->SetTextSize(text_size);
    overflow_text->SetFillStyle(0);
    overflow_text->SetBorderSize(0);
    overflow_text->AddText("overflow");

    TCanvas *c_rg = new TCanvas("c_rg", "", 1200, 1000);
    hStack_mc_rg->Draw("nostack hist");
    // h_mc_true_rg->Draw("noclear hist");
    hStack_data_rg->Draw("p same nostack");
    underflow_text->Draw();
    overflow_text->Draw();
    // hStack_mc_rg->SetMinimum(0.1);
    // hStack_data_rg->SetMinimum(0.1);
    double maximum = 0.4;
    hStack_mc_rg->SetMaximum(maximum);
    hStack_data_rg->SetMaximum(maximum);
    leg_rg->Draw();
    c_rg->Draw();
}