#include <TFile.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "myPalette.h"

void draw_track_gen_association()
{
    // Load the histos
    TString fdir = "./histos/";
    TString label = "HiForestMiniAOD_HighPU_10000events";
    TString fname = label + "_histos.root";
    TString rootf = fdir + fname;

    TFile *fin = new TFile(rootf);

    TH1F *hdr = (TH1F *)fin->Get("hdr");
    TH1F *hdrCut = (TH1F *)fin->Get("hdrCut");

    TH1F *hpt = (TH1F *)fin->Get("hpt");
    TH1F *hptCut = (TH1F *)fin->Get("hptCut");

    // Count untagged
    Float_t totalTracks = hdrCut->GetEntries();
    Float_t taggedTracks = hdrCut->Integral();
    Float_t untaggedPerc = (1 - (taggedTracks / totalTracks)) * 100;

    // Create the plot label 
    TPaveText *info = new TPaveText(0.55, 0.7, 0.85, 0.75, "ndc");
    info->SetFillStyle(0);
    info->SetBorderSize(1);
    info->SetTextSize(15);
    info->AddText(label);
    
    // Draw the DR histos
    TCanvas *cdr = new TCanvas("cdr", "", 1000, 800);

    THStack *hdrStack = new THStack("hdrStack", "hdr with and without cuts");
    hdrStack->SetTitle("; #DeltaR_{track-gen}; #frac{1}{N_{tracks}} #frac{dN}{d #DeltaR_{track-gen}}");

    TLegend *legdr = new TLegend(0.55, 0.75, 0.85, 0.9);
    legdr->SetFillStyle(0);
    legdr->SetBorderSize(1);
    legdr->SetMargin(0.15);
    gStyle->SetLegendTextSize(15);

    hdr->Scale(1. / hdr->Integral("width")); // Normalize histo
    hdr->SetLineColor(mykBlue);
    hdr->SetLineWidth(2);
    hdr->SetLineStyle(1);
    legdr->AddEntry(hdr, "No matching cuts", "l");
    hdrStack->Add(hdr);
    
    hdrCut->Scale(1. / hdrCut->Integral("width")); // Normalize histo
    hdrCut->SetLineColor(mykRed);
    hdrCut->SetLineWidth(2);
    hdrCut->SetLineStyle(1);
    legdr->AddEntry(hdrCut, Form("#splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc), "l");
    hdrStack->Add(hdrCut);

    cdr->cd();
    hdrStack->Draw("histo nostack");
    legdr->Draw();
    info->Draw();
    cdr->Draw();
    cdr->Print(TString("./plots/" + label + "_dr.png"));

    // Draw the RelPt histos
    TCanvas *crelpt = new TCanvas("crelpt", "", 1000, 800);

    THStack *hptStack = new THStack("hptStack", "relpt with and without cuts");
    hptStack->SetTitle("; p_{T, track} / p_{T, gen}; #frac{1}{N_{tracks}} #frac{dN}{d(p_{T, track} / p_{T, gen})}");

    TLegend *legrelpt = new TLegend(0.55, 0.75, 0.85, 0.9);
    legrelpt->SetFillStyle(0);
    legrelpt->SetBorderSize(1);
    legrelpt->SetMargin(0.15);
    gStyle->SetLegendTextSize(15);

    hpt->Scale(1. / hpt->Integral("width")); // Normalize histo
    hpt->SetLineColor(mykBlue);
    hpt->SetLineWidth(2);
    hpt->SetLineStyle(1);
    legrelpt->AddEntry(hpt, "No matching cuts", "l");
    hptStack->Add(hpt);
    
    hptCut->Scale(1. / hptCut->Integral("width")); // Normalize histo
    hptCut->SetLineColor(mykRed);
    hptCut->SetLineWidth(2);
    hptCut->SetLineStyle(1);
    legrelpt->AddEntry(hptCut, Form("#splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc), "l");
    hptStack->Add(hptCut);

    crelpt->cd();
    hptStack->Draw("histo nostack");
    legrelpt->Draw();
    info->Draw();
    crelpt->Draw();
    crelpt->Print(TString("./plots/" + label + "_relpt.png"));
}