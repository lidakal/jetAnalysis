#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../myPalette.h"

void draw_track_stats() 
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    float ptMin = 30.;
    float ptMax = 700.;

    TString indir = "./histos/";
    TString label = "HiForestMiniAOD_HighPU_100000events_conmatch";
    TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "track_stats.root";

    TFile *finPtr = new TFile(fin);

    TH2F *hIp3dSig = (TH2F *) finPtr->Get("hIp3dSig");
    TH2F *hIp3dSig_bjet = (TH2F *) finPtr->Get("hIp3dSig_bjet");
    TH2F *hPt = (TH2F *) finPtr->Get("hPt");
    TH2F *hPt_bjet = (TH2F *) finPtr->Get("hPt_bjet");

    // Get total nb of track to normalize 
    double totalTracks = hIp3dSig->GetEntries();
    double totalTracks_bjet = hIp3dSig_bjet->GetEntries();

    // Prepare stack histos + legends
    THStack *hStack = new THStack();
    hStack->SetTitle("; 3D IP / #sigma; 1 / N_{total tracks} dN / d(3D IP / #sigma)");
    THStack *hStackPt = new THStack();
    hStackPt->SetTitle("; p_{T}^{trk}; 1 / N_{total tracks} dN / dp_{T}^{trk}");

    THStack *hStack_bjet = new THStack();
    hStack_bjet->SetTitle("; 3D IP / #sigma; 1 / N_{total tracks in b-jets} dN / d(3D IP / #sigma)");
    THStack *hStackPt_bjet = new THStack();
    hStackPt_bjet->SetTitle("; p_{T}^{trk}; 1 / N_{total tracks in b-jets} dN / dp_{T}^{trk}");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    TLegend *leg_bjet = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg_bjet->SetFillStyle(0);
    leg_bjet->SetBorderSize(1);
    leg_bjet->SetMargin(0.15);

    // Plot by class

    TH1F *hPU = (TH1F *) hIp3dSig->ProjectionX("hPU", 1, 1);
    // Normalize histo
    hPU->Scale(1. / (totalTracks * hPU->GetBinWidth(1)));
    // Format histo
    hPU->SetLineColor(mykGreen);
    hPU->SetLineWidth(2);
    hPU->SetLineStyle(1);
    leg->AddEntry(hPU, "No match", "l");
    hStack->Add(hPU);

    TH1F *hPU_pt = (TH1F *) hPt->ProjectionX("hPU_pt", 1, 1);
    // Normalize histo
    hPU_pt->GetXaxis()->SetRange(1, hPU_pt->GetNbinsX() + 1);
    hPU_pt->Scale(1. / (totalTracks * hPU_pt->GetBinWidth(1)));
    // Format histo
    hPU_pt->SetLineColor(mykGreen);
    hPU_pt->SetLineWidth(2);
    hPU_pt->SetLineStyle(1);
    // leg->AddEntry(hPU_pt, "No match", "l");
    hStackPt->Add(hPU_pt);

    TH1F *hBkg = (TH1F *) hIp3dSig->ProjectionX("hBkg", 2, 2);
    // Normalize histo
    hBkg->Scale(1. / (totalTracks * hBkg->GetBinWidth(1)));
    // Format histo
    hBkg->SetLineColor(mykRed);
    hBkg->SetLineWidth(2);
    hBkg->SetLineStyle(1);
    leg->AddEntry(hBkg, "Match to bkg", "l");
    hStack->Add(hBkg);

    TH1F *hBkg_pt = (TH1F *) hPt->ProjectionX("hBkg_pt", 2, 2);
    // Normalize histo
    hBkg_pt->GetXaxis()->SetRange(1, hBkg_pt->GetNbinsX() + 1);
    hBkg_pt->Scale(1. / (totalTracks * hBkg_pt->GetBinWidth(1)));
    // Format histo
    hBkg_pt->SetLineColor(mykRed);
    hBkg_pt->SetLineWidth(2);
    hBkg_pt->SetLineStyle(1);
    // leg->AddEntry(hBkg_pt, "Match to bkg", "l");
    hStackPt->Add(hBkg_pt);


    TH1F *hSig = (TH1F *) hIp3dSig->ProjectionX("hSig", 3, 3);
    // Normalize histo
    hSig->Scale(1. / (totalTracks * hSig->GetBinWidth(1)));
    // Format histo
    hSig->SetLineColor(mykBlue);
    hSig->SetLineWidth(2);
    hSig->SetLineStyle(1);
    leg->AddEntry(hSig, "Match to sig", "l");
    hStack->Add(hSig);

    TH1F *hSig_pt = (TH1F *) hPt->ProjectionX("hSig_pt", 3, 3);
    // Normalize histo
    hSig_pt->GetXaxis()->SetRange(1, hSig_pt->GetNbinsX() + 1);
    hSig_pt->Scale(1. / (totalTracks * hSig_pt->GetBinWidth(1)));
    // Format histo
    hSig_pt->SetLineColor(mykBlue);
    hSig_pt->SetLineWidth(2);
    hSig_pt->SetLineStyle(1);
    // leg->AddEntry(hSig_pt, "Match to sig", "l");
    hStackPt->Add(hSig_pt);

    TH1F *hPU_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hPU_bjet", 1, 1);
    // Normalize histo
    hPU_bjet->Scale(1. / (totalTracks_bjet * hPU_bjet->GetBinWidth(1)));
    // Format histo
    hPU_bjet->SetLineColor(mykGreen);
    hPU_bjet->SetLineWidth(2);
    hPU_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hPU_bjet, "No match", "l");
    hStack_bjet->Add(hPU_bjet);

    TH1F *hPU_bjet_pt = (TH1F *) hPt_bjet->ProjectionX("hPU_bjet_pt", 1, 1);
    // Normalize histo
    hPU_bjet_pt->GetXaxis()->SetRange(1, hPU_bjet_pt->GetNbinsX() + 1);
    hPU_bjet_pt->Scale(1. / (totalTracks_bjet * hPU_bjet_pt->GetBinWidth(1)));
    // Format histo
    hPU_bjet_pt->SetLineColor(mykGreen);
    hPU_bjet_pt->SetLineWidth(2);
    hPU_bjet_pt->SetLineStyle(1);
    // leg_bjet->AddEntry(hPU_bjet_pt, "No match", "l");
    hStackPt_bjet->Add(hPU_bjet_pt);

    TH1F *hBkg_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hBkg_bjet", 2, 2);
    // Normalize histo
    hBkg_bjet->Scale(1. / (totalTracks_bjet * hBkg_bjet->GetBinWidth(1)));
    // Format histo
    hBkg_bjet->SetLineColor(mykRed);
    hBkg_bjet->SetLineWidth(2);
    hBkg_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hBkg_bjet, "Match to bkg", "l");
    hStack_bjet->Add(hBkg_bjet);

    TH1F *hBkg_bjet_pt = (TH1F *) hPt_bjet->ProjectionX("hBkg_bjet_pt", 2, 2);
    // Normalize histo
    hBkg_bjet_pt->GetXaxis()->SetRange(1, hBkg_bjet_pt->GetNbinsX() + 1);
    hBkg_bjet_pt->Scale(1. / (totalTracks_bjet * hBkg_bjet_pt->GetBinWidth(1)));
    // Format histo
    hBkg_bjet_pt->SetLineColor(mykRed);
    hBkg_bjet_pt->SetLineWidth(2);
    hBkg_bjet_pt->SetLineStyle(1);
    // leg_bjet->AddEntry(hBkg_bjet_pt, "Match to bkg", "l");
    hStackPt_bjet->Add(hBkg_bjet_pt);

    TH1F *hSig_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hSig_bjet", 3, 3);
    // Normalize histo
    hSig_bjet->Scale(1. / (totalTracks_bjet * hSig_bjet->GetBinWidth(1)));
    // Format histo
    hSig_bjet->SetLineColor(mykBlue);
    hSig_bjet->SetLineWidth(2);
    hSig_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hSig_bjet, "Match to sig", "l");
    hStack_bjet->Add(hSig_bjet);

    TH1F *hSig_bjet_pt = (TH1F *) hPt_bjet->ProjectionX("hSig_bjet_pt", 3, 3);
    // Normalize histo
    hSig_bjet_pt->GetXaxis()->SetRange(1, hSig_bjet_pt->GetNbinsX() + 1);
    hSig_bjet_pt->Scale(1. / (totalTracks_bjet * hSig_bjet_pt->GetBinWidth(1)));
    // Format histo
    hSig_bjet_pt->SetLineColor(mykBlue);
    hSig_bjet_pt->SetLineWidth(2);
    hSig_bjet_pt->SetLineStyle(1);
    // leg_bjet->AddEntry(hSig_bjet_pt, "Match to sig", "l");
    hStackPt_bjet->Add(hSig_bjet_pt);

    // Draw histos 
    TString odir = "./plots/";
    TString pngLabel = odir + label + "_" + ptMin + "_pt_" + ptMax + "track_stats.png";
    TCanvas *c = new TCanvas("c", "", 1200, 800);
    hStack->Draw("histo nostack");
    leg->Draw();
    c->Print(pngLabel);

    TCanvas *c_bjet = new TCanvas("c_bjet", "", 1200, 800);
    TString pngLabel_bjet = odir + label + "_" + ptMin + "_pt_" + ptMax + "track_stats_bjet.png";
    hStack_bjet->Draw("histo nostack");
    leg_bjet->Draw();
    c_bjet->Print(pngLabel_bjet);

    TCanvas *cPt = new TCanvas("cPt", "", 1200, 800);
    TString pngLabelPt = odir + label + "_" + ptMin + "_pt_" + ptMax + "_trkPt.png";
    hStackPt->Draw("histo nostack");
    hStackPt->GetXaxis()->SetRange(1,51);
    leg->Draw();
    cPt->Print(pngLabelPt);

    TCanvas *cPt_bjet = new TCanvas("cPt_bjet", "", 1200, 800);
    TString pngLabelPt_bjet = odir + label + "_" + ptMin + "_pt_" + ptMax + "_trkPt_bjet.png";
    hStackPt_bjet->Draw("histo nostack");
    hStackPt_bjet->GetXaxis()->SetRange(1,51);
    leg_bjet->Draw();
    cPt_bjet->Print(pngLabelPt_bjet);

}