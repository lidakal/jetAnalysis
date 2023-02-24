#include <TFile.h>
#include <TH3.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "myPalette.h"
#include "hist_utils.h"

void draw_mB()
{
    // ---- Settings
    float ptMin = 50.;
    float ptMax = 80.;

    float lowNvtxMax = 45.;
    float highNvtxMin = 45.;

    gStyle->SetLegendTextSize(15);

    // Load & draw the nvtx histo
    // TString fdir_nvtx = "./histos/";
    // TString label_nvtx = "nvtx";
    // TString suffix_nvtx = "_truth";
    // TString fname_nvtx = label_nvtx + suffix_nvtx + ".root";
    // TString rootf_nvtx = fdir_nvtx + fname_nvtx;

    // TFile *fin_nvtx = new TFile(rootf_nvtx);
    // TH1I *hNvtx = (TH1I *) fin_nvtx->Get("hNvtx");
    // // normalize_histo(hNvtx);
    // format_histo(hNvtx, kBlue);

    // TLine *line_lowNvtxMax = new TLine(lowNvtxMax, 0., lowNvtxMax, hNvtx->GetMaximum());
    // line_lowNvtxMax->SetLineStyle(2);
    // line_lowNvtxMax->SetLineWidth(2);
    // line_lowNvtxMax->SetLineColor(kBlack);

    // TLine *line_highNvtxMin = new TLine(highNvtxMin, 0., highNvtxMin, hNvtx->GetMaximum());
    // line_highNvtxMin->SetLineStyle(2);
    // line_highNvtxMin->SetLineWidth(2);
    // line_highNvtxMin->SetLineColor(kBlack);

    // TArrow *arrowLeft = new TArrow(lowNvtxMax - 10., 2000., lowNvtxMax, 2000., 0.1, "<|");
    // // arrowLeft->SetArrowSize(10);
    // arrowLeft->SetLineWidth(2);
    // TArrow *arrowRight = new TArrow(highNvtxMin, 2000., highNvtxMin + 10., 2000., 0.1, "|>");
    // arrowRight->SetLineWidth(2);

    // TCanvas *cTest = new TCanvas("cTest", "", 500, 500);
    // hNvtx->Draw("histo");
    // line_lowNvtxMax->Draw("same");
    // arrowLeft->Draw("same");
    // line_highNvtxMin->Draw("same");
    // arrowRight->Draw("same");
    // cTest->Draw();

    // ---- Load the histos
    TString indir = "./histos/";
    // TString fdir = "./histos/";
    // TString label = "mB";
    // TString suffix_truth = "_truth";
    // TString fname_truth = label + suffix_truth + ".root";
    // TString rootf_truth = fdir + fname_truth;

    // TFile *fin_truth = new TFile(rootf_truth);

    // TH3D *hmB_truth_3d = (TH3D *)fin_truth->Get("hmBtag");
    // hmB_truth_3d->SetName(TString(hmB_truth_3d->GetName()) + "_truth");

    // TString suffix_bdt = "_bdt29";
    TString label_aggrTMVA = "aggrTMVA";
    TString fname_aggrTMVA = indir + label_aggrTMVA + "_mB.root";

    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);

    TH3D *hBtag_mB_ratio = (TH3D *)fin_aggrTMVA->Get("hBtag_mB_ratio");
    hBtag_mB_ratio->SetName(TString(hBtag_mB_ratio->GetName()) + "_" + label_aggrTMVA);

    TH2D *hBtag_mB = (TH2D *) fin_aggrTMVA->Get("hBtag_mB");
    TH2D *hBtag_mB_gen = (TH2D *) fin_aggrTMVA->Get("hBtag_mB_gen");


    // ---- Find bins 
    // Int_t iyminLow = 1;
    // Int_t iymaxLow = hmB_truth_3d->GetYaxis()->FindBin(lowNvtxMax) - 1;

    // Int_t iyminHigh = hmB_truth_3d->GetYaxis()->FindBin(highNvtxMin);
    // Int_t iymaxHigh = hmB_truth_3d->GetYaxis()->GetNbins();

    Int_t iyminLow = 1;
    Int_t iymaxLow = hBtag_mB_ratio->GetNbinsY();

    Int_t iyminHigh = 1;
    Int_t iymaxHigh = hBtag_mB_ratio->GetNbinsY();

    Int_t izmin = hBtag_mB_ratio->GetZaxis()->FindBin(ptMin);
    Int_t izmax = hBtag_mB_ratio->GetZaxis()->FindBin(ptMax) - 1;

    Int_t ixmin = 1; // do not include underflow where mB > 0 and mBgen == 0
    Int_t ixmax = hBtag_mB_ratio->GetXaxis()->GetNbins() + 1;

    // ---- Prepare plot
    THStack *hStack_mB_ratio = new THStack("hStack_mB_ratio", "hBtag_mB_ratio");
    hStack_mB_ratio->SetTitle("; m_{pseudo-B, reco} / m_{pseudo-B, gen}; #frac{1}{N_{gen jets}} #frac{dN}{d m_{pseudo-B, reco} / m_{pseudo-B, gen}}");

    TLegend *leg_mB_ratio = new TLegend(0.55, 0.75, 0.85, 0.9);
    leg_mB_ratio->SetFillStyle(0);
    leg_mB_ratio->SetBorderSize(1);
    leg_mB_ratio->SetMargin(0.15);

    THStack *hStack_mB = new THStack("hStack_mB", "hBtag_mB and hBtag_mB_gen");
    hStack_mB->SetTitle("; m_{pseudo-B}; #frac{1}{N_{gen jets}} #frac{dN}{d m_{pseudo-B}}");

    TLegend *leg_mB = new TLegend(0.55, 0.75, 0.85, 0.9);
    leg_mB->SetFillStyle(0);
    leg_mB->SetBorderSize(1);
    leg_mB->SetMargin(0.15);

    // ---- Make projections 
    // TH1D *hmB_truth_lowNvtx = (TH1D *) hmB_truth_3d->ProjectionX("hmB_truth_lowNvtx", iyminLow, iymaxLow, izmin, izmax);
    // normalize_histo(hmB_truth_lowNvtx, ixmin, ixmax);
    // format_histo(hmB_truth_lowNvtx, mykRed);
    // hStack_mB_ratio->Add(hmB_truth_lowNvtx);
    // leg_mB_ratio->AddEntry(hmB_truth_lowNvtx, Form("truth, nvtx < %.0f", lowNvtxMax), "l");
    // leg_mB_ratio->AddEntry(hmB_truth_lowNvtx, "truth", "l");

    // TH1D *hmB_truth_highNvtx = (TH1D *) hmB_truth_3d->ProjectionX("hmB_truth_highNvtx", iyminHigh, iymaxHigh, izmin, izmax);
    // normalize_histo(hmB_truth_highNvtx, ixmin, ixmax);
    // format_histo(hmB_truth_highNvtx, mykRedLight);
    // hStack_mB_ratio->Add(hmB_truth_highNvtx);
    // leg_mB_ratio->AddEntry(hmB_truth_highNvtx, Form("truth, nvtx > %.0f", highNvtxMin), "l");

    TH1D *hBtag_mB_ratio_lowNvtx = (TH1D *) hBtag_mB_ratio->ProjectionX("hBtag_mB_ratio_lowNvtx", iyminLow, iymaxLow, izmin, izmax);
    normalize_histo(hBtag_mB_ratio_lowNvtx, ixmin, ixmax);
    format_histo(hBtag_mB_ratio_lowNvtx, mykBlue);
    hStack_mB_ratio->Add(hBtag_mB_ratio_lowNvtx);
    // leg_mB_ratio->AddEntry(hBtag_mB_ratio_lowNvtx, Form("bdt, nvtx < %.0f", lowNvtxMax), "l");
    leg_mB_ratio->AddEntry(hBtag_mB_ratio_lowNvtx, "Detector / particle level, aggregated with TMVA", "l");

    TH1D *hBtag_mB_ratio_highNvtx = (TH1D *) hBtag_mB_ratio->ProjectionX("hBtag_mB_ratio_highNvtx", iyminHigh, iymaxHigh, izmin, izmax);
    normalize_histo(hBtag_mB_ratio_highNvtx, ixmin, ixmax);
    format_histo(hBtag_mB_ratio_highNvtx, mykBlueLight);
    // hStack_mB_ratio->Add(hmB_bdt_highNvtx);
    // leg_mB_ratio->AddEntry(hmB_bdt_highNvtx, Form("bdt, nvtx > %.0f", highNvtxMin), "l");

    TH1D *hBtag_mB_gen_1d = (TH1D *) hBtag_mB_gen->ProjectionX("hBtag_mB_gen_1d", izmin, izmax);
    format_histo(hBtag_mB_gen_1d, mykRed);
    leg_mB->AddEntry(hBtag_mB_gen_1d, "Particle level", "l");
    hStack_mB->Add(hBtag_mB_gen_1d);

    TH1D *hBtag_mB_1d = (TH1D *) hBtag_mB->ProjectionX("hBtag_mB_1d", izmin, izmax);
    format_histo(hBtag_mB_1d, mykBlue);
    leg_mB->AddEntry(hBtag_mB_1d, "Detector level, aggregated with TMVA", "l");
    hStack_mB->Add(hBtag_mB_1d);

    // ---- Draw the histos
    TCanvas *c_mB_ratio = new TCanvas("c_mB_ratio", "", 1400, 800);

    c_mB_ratio->cd();
    hStack_mB_ratio->Draw("histo nostack");
    leg_mB_ratio->Draw();
    c_mB_ratio->Draw();

    TString cname_mB_ratio = Form("./plots/combined_mB_ratio.png");
    c_mB_ratio->Print(cname_mB_ratio);

    TCanvas *c_mB = new TCanvas("c_mB", "", 1400, 800);   
    c_mB->cd();
    hStack_mB->Draw("histo nostack");
    leg_mB->Draw();
    c_mB->Draw();

    TString cname_mB = "./plots/combined_mB.png";
    c_mB->Print(cname_mB);
    
}