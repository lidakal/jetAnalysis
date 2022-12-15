#include <TFile.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "myPalette.h"

void draw_disagreement_bjets()
{
    // Load the low PU histos
    TString fdir = "./histos/";
    TString label_lowPU = "HiForestMiniAOD_LowPU_10000events";
    TString fname_lowPU = label_lowPU + "_histos_disagreement.root";
    TString rootf_lowPU = fdir + fname_lowPU;

    TFile *fin_lowPU = new TFile(rootf_lowPU);

    TH1F *hdr_lowPU = (TH1F *)fin_lowPU->Get("hdr_bjets");
    hdr_lowPU->SetName(TString(hdr_lowPU->GetName()) + "_lowPU");

    TH1F *hpt_lowPU = (TH1F *)fin_lowPU->Get("hpt_bjets");
    hpt_lowPU->SetName(TString(hpt_lowPU->GetName()) + "_lowPU");

    // Load the high PU histos
    TString label_highPU = "HiForestMiniAOD_HighPU_10000events";
    TString fname_highPU = label_highPU + "_histos_disagreement.root";
    TString rootf_highPU = fdir + fname_highPU;

    TFile *fin_highPU = new TFile(rootf_highPU);

    TH1F *hdr_highPU = (TH1F *)fin_highPU->Get("hdr_bjets");
    hdr_highPU->SetName(TString(hdr_highPU->GetName()) + "_highPU");

    TH1F *hpt_highPU = (TH1F *)fin_highPU->Get("hpt_bjets");
    hpt_highPU->SetName(TString(hpt_highPU->GetName()) + "_highPU");

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
    hdr_lowPU->GetXaxis()->SetRange(0, hdr_lowPU->GetNbinsX() + 1);
    hdr_lowPU->Scale(1. / hdr_lowPU->Integral("width"));
    hdr_lowPU->GetXaxis()->SetRange(1, hdr_lowPU->GetNbinsX());
    // Format histo
    hdr_lowPU->SetLineColor(mykBlue);
    hdr_lowPU->SetLineWidth(2);
    hdr_lowPU->SetLineStyle(1);
    legdr->AddEntry(hdr_lowPU, "Low PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hdrStack->Add(hdr_lowPU);

    // Normalize histo
    hdr_highPU->GetXaxis()->SetRange(0, hdr_highPU->GetNbinsX() + 1);
    hdr_highPU->Scale(1. / hdr_highPU->Integral("width"));
    hdr_highPU->GetXaxis()->SetRange(1, hdr_highPU->GetNbinsX());
    // Format histo
    hdr_highPU->SetLineColor(mykBlueLight);
    hdr_highPU->SetLineWidth(2);
    hdr_highPU->SetLineStyle(1);
    legdr->AddEntry(hdr_highPU, "High PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hdrStack->Add(hdr_highPU);

   cdr->cd();
    hdrStack->Draw("histo nostack");
    legdr->Draw();
    // info->Draw();
    cdr->Draw();
    // cdr->Print(TString("./plots/" + label + "_dr.png"));
    cdr->Print(TString("./plots/combined_dr_disagreement_bjets.png"));

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
    legdr_ratio->AddEntry(hdr_ratio, "Geometrical match, p_{T, gen} > 500 MeV", "l");
    hdrStack_ratio->Add(hdr_ratio);

    TLine *line = new TLine(0., 1., 0.04, 1.);
    line->SetLineWidth(2);
    line->SetLineColorAlpha(kBlack, 0.2);

    cdr_ratio->cd();
    hdrStack_ratio->Draw("histo nostack");
    hdrStack_ratio->SetMaximum(2.);
    line->Draw();
    legdr_ratio->Draw();
    cdr_ratio->Draw();
    cdr_ratio->Print("./plots/combined_dr_ratio_disagreement_bjets.png");

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
    hpt_lowPU->GetXaxis()->SetRange(0, hpt_lowPU->GetNbinsX() + 1);
    hpt_lowPU->Scale(1. / hpt_lowPU->Integral("width"));
    hpt_lowPU->GetXaxis()->SetRange(1, hpt_lowPU->GetNbinsX());
    // Format histo
    hpt_lowPU->SetLineColor(mykBlue);
    hpt_lowPU->SetLineWidth(2);
    hpt_lowPU->SetLineStyle(1);
    legrelpt->AddEntry(hpt_lowPU, "Low PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hptStack->Add(hpt_lowPU);

    // Normalize histo
    hpt_highPU->GetXaxis()->SetRange(0, hpt_highPU->GetNbinsX() + 1);
    hpt_highPU->Scale(1. / hpt_highPU->Integral("width"));
    hpt_highPU->GetXaxis()->SetRange(1, hpt_highPU->GetNbinsX());
    // Format histo
    hpt_highPU->SetLineColor(mykBlueLight);
    hpt_highPU->SetLineWidth(2);
    hpt_highPU->SetLineStyle(1);
    legrelpt->AddEntry(hpt_highPU, "High PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hptStack->Add(hpt_highPU);

    cpt->cd();
    hptStack->Draw("histo nostack");
    legrelpt->Draw();
    // info->Draw();
    cpt->Draw();
    // cpt->Print(TString("./plots/" + label + "_relpt.png"));
    cpt->Print(TString("./plots/combined_relpt_disagreement_bjets.png"));

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
    legpt_ratio->AddEntry(hpt_ratio, "Geometrical match, p_{T, gen} > 500 MeV", "l");
    hptStack_ratio->Add(hpt_ratio);

    TLine *line_pt = new TLine(0.6, 1., 1.4, 1.);
    line_pt->SetLineWidth(2);
    line_pt->SetLineColorAlpha(kBlack, 0.2);

    cpt_ratio->cd();
    hptStack_ratio->Draw("histo nostack");
    hptStack_ratio->SetMaximum(3.);

    legpt_ratio->Draw();
    line_pt->Draw();
    cpt_ratio->Draw();
    cpt_ratio->Print("./plots/combined_relpt_ratio_disagreement_bjets.png");
}