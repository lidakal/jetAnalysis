#include <TFile.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "myPalette.h"

void draw_mB()
{
    // Load the low PU histos
    TString fdir = "./histos/";
    TString label_lowPU_geo = "HiForestMiniAOD_LowPU_10000events_geomatch";
    TString fname_lowPU_geo = label_lowPU_geo + "_histos.root";
    TString rootf_lowPU_geo = fdir + fname_lowPU_geo;

    TFile *fin_lowPU_geo = new TFile(rootf_lowPU_geo);

    TH1F *hmB_lowPU_geo = (TH1F *)fin_lowPU_geo->Get("hmB");
    hmB_lowPU_geo->SetName(TString(hmB_lowPU_geo->GetName()) + "_lowPU_geo");

    TString label_lowPU_con = "HiForestMiniAOD_LowPU_10000events_conmatch";
    TString fname_lowPU_con = label_lowPU_con + "_histos.root";
    TString rootf_lowPU_con = fdir + fname_lowPU_con;

    TFile *fin_lowPU_con = new TFile(rootf_lowPU_con);

    TH1F *hmB_lowPU_con = (TH1F *)fin_lowPU_con->Get("hmB");
    hmB_lowPU_con->SetName(TString(hmB_lowPU_con->GetName()) + "_lowPU_con");

    // Load the high PU histos
    TString label_highPU_geo = "HiForestMiniAOD_HighPU_10000events_geomatch";
    TString fname_highPU_geo = label_highPU_geo + "_histos.root";
    TString rootf_highPU_geo = fdir + fname_highPU_geo;

    TFile *fin_highPU_geo = new TFile(rootf_highPU_geo);

    TH1F *hmB_highPU_geo = (TH1F *)fin_highPU_geo->Get("hmB");
    hmB_highPU_geo->SetName(TString(hmB_highPU_geo->GetName()) + "_highPU_geo");

    TString label_highPU_con = "HiForestMiniAOD_HighPU_10000events_conmatch";
    TString fname_highPU_con = label_highPU_con + "_histos.root";
    TString rootf_highPU_con = fdir + fname_highPU_con;

    TFile *fin_highPU_con = new TFile(rootf_highPU_con);

    TH1F *hmB_highPU_con = (TH1F *)fin_highPU_con->Get("hmB");
    hmB_highPU_con->SetName(TString(hmB_highPU_con->GetName()) + "_highPU_con");

    // ---- Draw the histos
    TCanvas *cmB = new TCanvas("cmB", "", 1400, 800);

    THStack *hmBStack = new THStack("hmBStack", "hmB with and without cuts, for low and high PU");
    hmBStack->SetTitle("; m_{pseudo-B, reco} / m_{pseudo-B, gen}; #frac{1}{N_{tracks}} #frac{dN}{d m_{pseudo-B, reco} / m_{pseudo-B, gen}}");

    TLegend *legmB = new TLegend(0.55, 0.6, 0.85, 0.9);
    legmB->SetFillStyle(0);
    legmB->SetBorderSize(1);
    legmB->SetMargin(0.15);
    gStyle->SetLegendTextSize(15);

    // Normalize histo
    hmB_lowPU_geo->GetXaxis()->SetRange(0, hmB_lowPU_geo->GetNbinsX() + 1);
    hmB_lowPU_geo->Scale(1. / hmB_lowPU_geo->Integral("width"));
    hmB_lowPU_geo->GetXaxis()->SetRange(1, hmB_lowPU_geo->GetNbinsX());
    // Format histo
    hmB_lowPU_geo->SetLineColor(mykBlue);
    hmB_lowPU_geo->SetLineWidth(2);
    hmB_lowPU_geo->SetLineStyle(1);
    legmB->AddEntry(hmB_lowPU_geo, "Low PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hmBStack->Add(hmB_lowPU_geo);

    // Normalize histo
    hmB_highPU_geo->GetXaxis()->SetRange(0, hmB_highPU_geo->GetNbinsX() + 1);
    hmB_highPU_geo->Scale(1. / hmB_highPU_geo->Integral("width"));
    hmB_highPU_geo->GetXaxis()->SetRange(1, hmB_highPU_geo->GetNbinsX());
    // Format histo
    hmB_highPU_geo->SetLineColor(mykBlueLight);
    hmB_highPU_geo->SetLineWidth(2);
    hmB_highPU_geo->SetLineStyle(1);
    legmB->AddEntry(hmB_highPU_geo, "High PU: Geometrical match, p_{T, gen} > 500 MeV", "l");
    hmBStack->Add(hmB_highPU_geo);

    // Normalize histo
    hmB_lowPU_con->GetXaxis()->SetRange(0, hmB_lowPU_con->GetNbinsX()+1);
    hmB_lowPU_con->Scale(1. / hmB_lowPU_con->Integral("width"));
    hmB_lowPU_con->GetXaxis()->SetRange(1, hmB_lowPU_con->GetNbinsX());
    // Format histo
    hmB_lowPU_con->SetLineColor(mykRed);
    hmB_lowPU_con->SetLineWidth(2);
    hmB_lowPU_con->SetLineStyle(1);
    legmB->AddEntry(hmB_lowPU_con, Form("Low PU: #DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2"), "l");
    hmBStack->Add(hmB_lowPU_con);

    // Normalize histo
    hmB_highPU_con->GetXaxis()->SetRange(0, hmB_highPU_con->GetNbinsX()+1);
    hmB_highPU_con->Scale(1. / hmB_highPU_con->Integral("width"));
    hmB_highPU_con->GetXaxis()->SetRange(1, hmB_highPU_con->GetNbinsX());
    // Format histo
    hmB_highPU_con->SetLineColor(mykRedLight);
    hmB_highPU_con->SetLineWidth(2);
    hmB_highPU_con->SetLineStyle(1);
    legmB->AddEntry(hmB_highPU_con, Form("High PU: #DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2"), "l");
    hmBStack->Add(hmB_highPU_con);

    cmB->cd();
    hmBStack->Draw("histo nostack");
    legmB->Draw();
    cmB->Draw();
    cmB->Print(TString("./plots/combined_mB.png"));

    // Ratio histos
    TCanvas *cmB_ratio = new TCanvas("cmB_ratio", "", 1200, 800);

    THStack *hmBStack_ratio = new THStack("hmBStack_ratio", "high/low PU of hmB histos");
    hmBStack_ratio->SetTitle("; m_{pseudo-B, reco} / m_{pseudo-B, gen}; High / Low PU");

    TLegend *legmB_ratio = new TLegend(0.6, 0.77, 0.85, 0.9);
    legmB_ratio->SetFillStyle(0);
    legmB_ratio->SetBorderSize(1);
    legmB_ratio->SetMargin(0.15);

    TH1F *hmB_ratio = (TH1F *)hmB_highPU_geo->Clone();
    hmB_ratio->SetName("hmB_ratio");
    hmB_ratio->Divide(hmB_lowPU_geo);
    hmB_ratio->SetLineColor(mykBlue);
    hmB_ratio->SetLineWidth(2);
    hmB_ratio->SetLineStyle(1);
    legmB_ratio->AddEntry(hmB_ratio, "Geometrical match, p_{T, gen} > 500 MeV", "l");
    hmBStack_ratio->Add(hmB_ratio);

    TH1F *hmBCut_ratio = (TH1F *)hmB_highPU_con->Clone();
    hmBCut_ratio->SetName("hmBCut_ratio");
    hmBCut_ratio->Divide(hmB_lowPU_con);
    hmBCut_ratio->SetLineColor(mykRed);
    hmBCut_ratio->SetLineWidth(2);
    hmBCut_ratio->SetLineStyle(1);
    legmB_ratio->AddEntry(hmBCut_ratio, "#DeltaR_{track-gen} < 0.02 && 0.8 < #frac{p_{T, track}}{p_{T, gen}} < 1.2", "l");
    hmBStack_ratio->Add(hmBCut_ratio);

    TLine *line = new TLine(0., 1., 0.04, 1.);
    line->SetLineWidth(2);
    line->SetLineColorAlpha(kBlack, 0.2);

    cmB_ratio->cd();
    hmBStack_ratio->Draw("histo nostack");
    hmBStack_ratio->SetMaximum(4.5);
    line->Draw();
    legmB_ratio->Draw();
    cmB_ratio->Draw();
    cmB_ratio->Print("./plots/combined_mB_ratio.png");
}