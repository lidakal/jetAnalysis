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

    // float lowNvtxMax = 15.;
    // float highNvtxMin = 35.;

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
    TString fdir = "./histos/";
    // TString label = "HiForestMiniAOD_HighPU_10000events_conmatch";
    TString label = "mB";
    TString suffix_truth = "_truth";
    TString fname_truth = label + suffix_truth + ".root";
    TString rootf_truth = fdir + fname_truth;

    TFile *fin_truth = new TFile(rootf_truth);

    TH3D *hmB_truth_3d = (TH3D *)fin_truth->Get("hmBtag");
    hmB_truth_3d->SetName(TString(hmB_truth_3d->GetName()) + "_truth");

    TString suffix_bdt = "_bdt";
    TString fname_bdt = label + suffix_bdt + ".root";
    TString rootf_bdt = fdir + fname_bdt;

    TFile *fin_bdt = new TFile(rootf_bdt);

    TH3D *hmB_bdt_3d = (TH3D *)fin_bdt->Get("hmBtag");
    hmB_bdt_3d->SetName(TString(hmB_bdt_3d->GetName()) + "_bdt");

    // ---- Find bins 
    // Int_t iyminLow = 1;
    // Int_t iymaxLow = hmB_truth_3d->GetYaxis()->FindBin(lowNvtxMax) - 1;

    // Int_t iyminHigh = hmB_truth_3d->GetYaxis()->FindBin(highNvtxMin);
    // Int_t iymaxHigh = hmB_truth_3d->GetYaxis()->GetNbins();

    Int_t iyminLow = 1;
    Int_t iymaxLow = hmB_truth_3d->GetNbinsY();

    Int_t iyminHigh = 1;
    Int_t iymaxHigh = hmB_truth_3d->GetNbinsY();

    Int_t izmin = hmB_truth_3d->GetZaxis()->FindBin(ptMin);
    Int_t izmax = hmB_truth_3d->GetZaxis()->FindBin(ptMax) - 1;

    Int_t ixmin = 1; // do not include underflow where mB > 0 and mBgen == 0
    Int_t ixmax = hmB_truth_3d->GetXaxis()->GetNbins() + 1;

    // ---- Prepare plot
    THStack *hmBStack = new THStack("hmBStack", "hmB");
    hmBStack->SetTitle("; m_{pseudo-B, reco} / m_{pseudo-B, gen}; #frac{1}{N_{gen jets}} #frac{dN}{d m_{pseudo-B, reco} / m_{pseudo-B, gen}}");

    TLegend *legmB = new TLegend(0.55, 0.6, 0.85, 0.9);
    legmB->SetFillStyle(0);
    legmB->SetBorderSize(1);
    legmB->SetMargin(0.15);

    // ---- Make projections 
    TH1D *hmB_truth_lowNvtx = (TH1D *) hmB_truth_3d->ProjectionX("hmB_truth_lowNvtx", iyminLow, iymaxLow, izmin, izmax);
    normalize_histo(hmB_truth_lowNvtx, ixmin, ixmax);
    format_histo(hmB_truth_lowNvtx, mykRed);
    hmBStack->Add(hmB_truth_lowNvtx);
    // legmB->AddEntry(hmB_truth_lowNvtx, Form("truth, nvtx < %.0f", lowNvtxMax), "l");
    legmB->AddEntry(hmB_truth_lowNvtx, "truth", "l");

    TH1D *hmB_truth_highNvtx = (TH1D *) hmB_truth_3d->ProjectionX("hmB_truth_highNvtx", iyminHigh, iymaxHigh, izmin, izmax);
    normalize_histo(hmB_truth_highNvtx, ixmin, ixmax);
    format_histo(hmB_truth_highNvtx, mykRedLight);
    // hmBStack->Add(hmB_truth_highNvtx);
    // legmB->AddEntry(hmB_truth_highNvtx, Form("truth, nvtx > %.0f", highNvtxMin), "l");

    TH1D *hmB_bdt_lowNvtx = (TH1D *) hmB_bdt_3d->ProjectionX("hmB_bdt_lowNvtx", iyminLow, iymaxLow, izmin, izmax);
    normalize_histo(hmB_bdt_lowNvtx, ixmin, ixmax);
    format_histo(hmB_bdt_lowNvtx, mykBlue);
    hmBStack->Add(hmB_bdt_lowNvtx);
    // legmB->AddEntry(hmB_bdt_lowNvtx, Form("bdt, nvtx < %.0f", lowNvtxMax), "l");
    legmB->AddEntry(hmB_bdt_lowNvtx, "bdt", "l");

    TH1D *hmB_bdt_highNvtx = (TH1D *) hmB_bdt_3d->ProjectionX("hmB_bdt_highNvtx", iyminHigh, iymaxHigh, izmin, izmax);
    normalize_histo(hmB_bdt_highNvtx, ixmin, ixmax);
    format_histo(hmB_bdt_highNvtx, mykBlueLight);
    // hmBStack->Add(hmB_bdt_highNvtx);
    // legmB->AddEntry(hmB_bdt_highNvtx, Form("bdt, nvtx > %.0f", highNvtxMin), "l");

    // ---- Draw the histos
    TCanvas *cmB = new TCanvas("cmB", "", 1400, 800);

    cmB->cd();
    hmBStack->Draw("histo nostack");
    legmB->Draw();
    cmB->Draw();

    // TString cname = Form("./plots/combined_mB_%.0f_nvtx_%.0f.png", lowNvtxMax, highNvtxMin);
    TString cname = "./plots/combined_mB_no_nvtx_cut" + suffix_bdt + ".png";
    cmB->Print(cname);
}