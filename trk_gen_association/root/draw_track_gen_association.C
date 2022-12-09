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
    // Load the low PU histos
    TString fdir = "./histos/";
    TString label_lowPU = "HiForestMiniAOD_LowPU_10000events";
    TString fname_lowPU = label_lowPU + "_histos.root";
    TString rootf_lowPU = fdir + fname_lowPU;

    TFile *fin_lowPU = new TFile(rootf_lowPU);

    TH1F *hdr_lowPU = (TH1F *)fin_lowPU->Get("hdr");
    hdr_lowPU->SetName(TString(hdr_lowPU->GetName()) + "_lowPU");
    TH1F *hdrCut_lowPU = (TH1F *)fin_lowPU->Get("hdrCut");
    hdrCut_lowPU->SetName(TString(hdrCut_lowPU->GetName()) + "_lowPU");

    TH1F *hpt_lowPU = (TH1F *)fin_lowPU->Get("hpt");
    hpt_lowPU->SetName(TString(hpt_lowPU->GetName()) + "_lowPU");
    TH1F *hptCut_lowPU = (TH1F *)fin_lowPU->Get("hptCut");
    hptCut_lowPU->SetName(TString(hptCut_lowPU->GetName()) + "_lowPU");

    // Load the high PU histos
    TString label_highPU = "HiForestMiniAOD_HighPU_10000events";
    TString fname_highPU = label_highPU + "_histos.root";
    TString rootf_highPU = fdir + fname_highPU;

    TFile *fin_highPU = new TFile(rootf_highPU);

    TH1F *hdr_highPU = (TH1F *)fin_highPU->Get("hdr");
    hdr_highPU->SetName(TString(hdr_highPU->GetName()) + "_highPU");
    TH1F *hdrCut_highPU = (TH1F *)fin_highPU->Get("hdrCut");
    hdrCut_highPU->SetName(TString(hdrCut_highPU->GetName()) + "_highPU");

    TH1F *hpt_highPU = (TH1F *)fin_highPU->Get("hpt");
    hpt_highPU->SetName(TString(hpt_highPU->GetName()) + "_highPU");
    TH1F *hptCut_highPU = (TH1F *)fin_highPU->Get("hptCut");
    hptCut_highPU->SetName(TString(hptCut_highPU->GetName()) + "_highPU");
    
    // Count untagged 
    Float_t totalTracks_lowPU = hdrCut_lowPU->GetEntries();
    Float_t taggedTracks_lowPU = hdrCut_lowPU->Integral();
    Float_t untaggedPerc_lowPU = (1 - (taggedTracks_lowPU / totalTracks_lowPU)) * 100;

    Float_t totalTracks_highPU = hdrCut_highPU->GetEntries();
    Float_t taggedTracks_highPU = hdrCut_highPU->Integral();
    Float_t untaggedPerc_highPU = (1 - (taggedTracks_highPU / totalTracks_highPU)) * 100;

    // Create the plot label 
    // TPaveText *info = new TPaveText(0.55, 0.7, 0.85, 0.75, "ndc");
    // info->SetFillStyle(0);
    // info->SetBorderSize(1);
    // info->SetTextSize(15);
    // info->AddText(label);
    
    // ---- Draw the DR histos
    TCanvas *cdr = new TCanvas("cdr", "", 1400, 800);

    THStack *hdrStack = new THStack("hdrStack", "hdr with and without cuts, for low and high PU");
    hdrStack->SetTitle("; #DeltaR_{track-gen}; #frac{1}{N_{tracks}} #frac{dN}{d #DeltaR_{track-gen}}");

    TLegend *legdr = new TLegend(0.55, 0.6, 0.85, 0.9);
    legdr->SetFillStyle(0);
    legdr->SetBorderSize(1);
    legdr->SetMargin(0.15);
    gStyle->SetLegendTextSize(15);

    // [DEBUG]
    // Float_t nentries = hdr->GetEntries();
    // Float_t integral = hdr->Integral();
    // hdr->GetXaxis()->SetRange(0, hdr->GetNbinsX()+1);
    // Float_t integral_full_range = hdr->Integral();
    // std::cout << "Entries: " << nentries
    //           << "\nIntegral: " << integral
    //           << "\nIntegral after SetRange: " << integral_full_range
    //           << std::endl;

    // Normalize histo
    hdr_lowPU->GetXaxis()->SetRange(0, hdr_lowPU->GetNbinsX()+1);
    hdr_lowPU->Scale(1. / hdr_lowPU->Integral("width"));
    hdr_lowPU->GetXaxis()->SetRange(1, hdr_lowPU->GetNbinsX());
    // Format histo
    hdr_lowPU->SetLineColor(mykBlue);
    hdr_lowPU->SetLineWidth(2);
    hdr_lowPU->SetLineStyle(1);
    legdr->AddEntry(hdr_lowPU, "Low PU: No matching cuts", "l");
    hdrStack->Add(hdr_lowPU);

    // Normalize histo
    hdr_highPU->GetXaxis()->SetRange(0, hdr_highPU->GetNbinsX()+1);
    hdr_highPU->Scale(1. / hdr_highPU->Integral("width"));
    hdr_highPU->GetXaxis()->SetRange(1, hdr_highPU->GetNbinsX());
    // Format histo
    hdr_highPU->SetLineColor(mykBlueLight);
    hdr_highPU->SetLineWidth(2);
    hdr_highPU->SetLineStyle(1);
    legdr->AddEntry(hdr_highPU, "High PU: No matching cuts", "l");
    hdrStack->Add(hdr_highPU);

    // Normalize histo
    // Note: Normalize only by tagged tracks
    // hdrCut_highPU->GetXaxis()->SetRange(0, hdrCut_highPU->GetNbinsX()+1);
    hdrCut_highPU->Scale(1. / hdrCut_highPU->Integral("width"));
    // hdrCut_highPU->GetXaxis()->SetRange(1, hdrCut_highPU->GetNbinsX());
    // Format histo
    hdrCut_highPU->SetLineColor(mykRedLight);
    hdrCut_highPU->SetLineWidth(2);
    hdrCut_highPU->SetLineStyle(1);
    legdr->AddEntry(hdrCut_highPU, Form("High PU: #splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc_highPU), "l");
    hdrStack->Add(hdrCut_highPU);

    // Normalize histo
    // Note: Normalize only by tagged tracks
    // hdrCut_lowPU->GetXaxis()->SetRange(0, hdrCut_lowPU->GetNbinsX()+1);
    hdrCut_lowPU->Scale(1. / hdrCut_lowPU->Integral("width"));
    // hdrCut_lowPU->GetXaxis()->SetRange(1, hdrCut_lowPU->GetNbinsX());
    // Format histo
    hdrCut_lowPU->SetLineColor(mykRed);
    hdrCut_lowPU->SetLineWidth(2);
    hdrCut_lowPU->SetLineStyle(1);
    legdr->AddEntry(hdrCut_lowPU, Form("Low PU: #splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc_lowPU), "l");
    hdrStack->Add(hdrCut_lowPU);

    cdr->cd();
    hdrStack->Draw("histo nostack");
    legdr->Draw();
    // info->Draw();
    cdr->Draw();
    // cdr->Print(TString("./plots/" + label + "_dr.png"));
    cdr->Print(TString("./plots/combined_dr.png"));

    // Ratio histos
    TCanvas *cdr_ratio = new TCanvas("cdr_ratio", "", 1200, 800);

    THStack *hdrStack_ratio = new THStack("hdrStack_ratio", "high/low PU of hdr histos");
    hdrStack_ratio->SetTitle("; #DeltaR_{track-gen}; High / Low PU");

    TLegend *legdr_ratio = new TLegend(0.6, 0.77, 0.85, 0.9);
    legdr_ratio->SetFillStyle(0);
    legdr_ratio->SetBorderSize(1);
    legdr_ratio->SetMargin(0.15);    

    TH1F *hdr_ratio = (TH1F *)hdr_highPU->Clone();
    hdr_ratio->SetName("hdr_ratio");
    hdr_ratio->Divide(hdr_lowPU);
    hdr_ratio->SetLineColor(mykBlue);
    hdr_ratio->SetLineWidth(2);
    hdr_ratio->SetLineStyle(1);
    legdr_ratio->AddEntry(hdr_ratio, "No matching cuts", "l");
    hdrStack_ratio->Add(hdr_ratio);

    TH1F *hdrCut_ratio = (TH1F *)hdrCut_highPU->Clone();
    hdrCut_ratio->SetName("hdrCut_ratio");
    hdrCut_ratio->Divide(hdrCut_lowPU);
    hdrCut_ratio->SetLineColor(mykRed);
    hdrCut_ratio->SetLineWidth(2);
    hdrCut_ratio->SetLineStyle(1);
    legdr_ratio->AddEntry(hdrCut_ratio, "#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2", "l");
    hdrStack_ratio->Add(hdrCut_ratio);

    TLine *line = new TLine(0., 1., 0.04, 1.);
    line->SetLineWidth(2);
    line->SetLineColorAlpha(kBlack, 0.2);

    cdr_ratio->cd();
    hdrStack_ratio->Draw("histo nostack");
    hdrStack_ratio->SetMaximum(2.);
    line->Draw();
    legdr_ratio->Draw();
    cdr_ratio->Draw();
    cdr_ratio->Print("./plots/combined_dr_ratio.png");

    // ---- Draw the RelPt histos
    TCanvas *cpt = new TCanvas("cpt", "", 1400, 800);

    THStack *hptStack = new THStack("hptStack", "relpt with and without cuts");
    hptStack->SetTitle("; p_{T, track} / p_{T, gen}; #frac{1}{N_{tracks}} #frac{dN}{d(p_{T, track} / p_{T, gen})}");

    TLegend *legrelpt = new TLegend(0.55, 0.6, 0.85, 0.9);
    legrelpt->SetFillStyle(0);
    legrelpt->SetBorderSize(1);
    legrelpt->SetMargin(0.15);
    gStyle->SetLegendTextSize(15);

    // Normalize histo
    hpt_lowPU->GetXaxis()->SetRange(0, hpt_lowPU->GetNbinsX()+1);
    hpt_lowPU->Scale(1. / hpt_lowPU->Integral("width"));
    hpt_lowPU->GetXaxis()->SetRange(1, hpt_lowPU->GetNbinsX());
    // Format histo
    hpt_lowPU->SetLineColor(mykBlue);
    hpt_lowPU->SetLineWidth(2);
    hpt_lowPU->SetLineStyle(1);
    legrelpt->AddEntry(hpt_lowPU, "Low PU: No matching cuts", "l");
    hptStack->Add(hpt_lowPU);

    // Normalize histo
    hpt_highPU->GetXaxis()->SetRange(0, hpt_highPU->GetNbinsX()+1);
    hpt_highPU->Scale(1. / hpt_highPU->Integral("width"));
    hpt_highPU->GetXaxis()->SetRange(1, hpt_highPU->GetNbinsX());
    // Format histo
    hpt_highPU->SetLineColor(mykBlueLight);
    hpt_highPU->SetLineWidth(2);
    hpt_highPU->SetLineStyle(1);
    legrelpt->AddEntry(hpt_highPU, "High PU: No matching cuts", "l");
    hptStack->Add(hpt_highPU);
    
    // Normalize histo
    // Note: Normalize only by tagged tracks
    // hptCut_lowPU->GetXaxis()->SetRange(0, hptCut_lowPU->GetNbinsX()+1);
    hptCut_lowPU->Scale(1. / hptCut_lowPU->Integral("width"));
    // hptCut_lowPU->GetXaxis()->SetRange(1, hptCut_lowPU->GetNbinsX());
    // Format histo
    hptCut_lowPU->SetLineColor(mykRed);
    hptCut_lowPU->SetLineWidth(2);
    hptCut_lowPU->SetLineStyle(1);
    legrelpt->AddEntry(hptCut_lowPU, Form("Low PU: #splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc_lowPU), "l");
    hptStack->Add(hptCut_lowPU);

    // Normalize histo
    // Note: Normalize only by tagged tracks
    // hptCut_highPU->GetXaxis()->SetRange(0, hptCut_highPU->GetNbinsX()+1);
    hptCut_highPU->Scale(1. / hptCut_highPU->Integral("width"));
    // hptCut_highPU->GetXaxis()->SetRange(1, hptCut_highPU->GetNbinsX());
    // Format histo
    hptCut_highPU->SetLineColor(mykRedLight);
    hptCut_highPU->SetLineWidth(2);
    hptCut_highPU->SetLineStyle(1);
    legrelpt->AddEntry(hptCut_highPU, Form("High PU: #splitline{#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2}{Untagged tracks: %.2f%%}", untaggedPerc_highPU), "l");
    hptStack->Add(hptCut_highPU);

    cpt->cd();
    hptStack->Draw("histo nostack");
    legrelpt->Draw();
    // info->Draw();
    cpt->Draw();
    // cpt->Print(TString("./plots/" + label + "_relpt.png"));
    cpt->Print(TString("./plots/combined_relpt.png"));

    // Ratio histos
    TCanvas *cpt_ratio = new TCanvas("cpt_ratio", "", 1200, 800);

    THStack *hptStack_ratio = new THStack("hptStack_ratio", "high/low PU of hpt histos");
    hptStack_ratio->SetTitle("; p_{T, track} / p_{T, gen}; High / Low PU");

    TLegend *legpt_ratio = new TLegend(0.6, 0.77, 0.85, 0.9);
    legpt_ratio->SetFillStyle(0);
    legpt_ratio->SetBorderSize(1);
    legpt_ratio->SetMargin(0.15);    

    TH1F *hpt_ratio = (TH1F *)hpt_highPU->Clone();
    hpt_ratio->SetName("hpt_ratio");
    hpt_ratio->Divide(hpt_lowPU);
    hpt_ratio->SetLineColor(mykBlue);
    hpt_ratio->SetLineWidth(2);
    hpt_ratio->SetLineStyle(1);
    legpt_ratio->AddEntry(hpt_ratio, "No matching cuts", "l");
    hptStack_ratio->Add(hpt_ratio);

    TH1F *hptCut_ratio = (TH1F *)hptCut_highPU->Clone();
    hptCut_ratio->SetName("hptCut_ratio");
    hptCut_ratio->Divide(hptCut_lowPU);
    hptCut_ratio->SetLineColor(mykRed);
    hptCut_ratio->SetLineWidth(2);
    hptCut_ratio->SetLineStyle(1);
    legpt_ratio->AddEntry(hptCut_ratio, "#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2", "l");
    hptStack_ratio->Add(hptCut_ratio);

    TLine *line_pt = new TLine(0.6, 1., 1.4, 1.);
    line_pt->SetLineWidth(2);
    line_pt->SetLineColorAlpha(kBlack, 0.2);

    cpt_ratio->cd();
    hptStack_ratio->Draw("histo nostack");
    hptStack_ratio->SetMaximum(3.);
    
    legpt_ratio->Draw();
    line_pt->Draw();
    cpt_ratio->Draw();
    cpt_ratio->Print("./plots/combined_relpt_ratio.png");
}