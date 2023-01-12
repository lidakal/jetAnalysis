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

    // Get total nb of track to normalize 
    double totalTracks = hIp3dSig->GetEntries();
    double totalTracks_bjet = hIp3dSig_bjet->GetEntries();

    // Prepare stack histos + legends
    THStack *hStack = new THStack();
    hStack->SetTitle("; 3D IP / #sigma; 1 / N_{total tracks} dN / d(3D IP / #sigma)");

    THStack *hStack_bjet = new THStack();
    hStack_bjet->SetTitle("; 3D IP / #sigma; 1 / N_{total tracks} dN / d(3D IP / #sigma)");

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

    TH1F *hBkg = (TH1F *) hIp3dSig->ProjectionX("hBkg", 2, 2);
    // Normalize histo
    hBkg->Scale(1. / (totalTracks * hBkg->GetBinWidth(1)));
    // Format histo
    hBkg->SetLineColor(mykRed);
    hBkg->SetLineWidth(2);
    hBkg->SetLineStyle(1);
    leg->AddEntry(hBkg, "Match to bkg", "l");
    hStack->Add(hBkg);

    TH1F *hSig = (TH1F *) hIp3dSig->ProjectionX("hSig", 3, 3);
    // Normalize histo
    hSig->Scale(1. / (totalTracks * hSig->GetBinWidth(1)));
    // Format histo
    hSig->SetLineColor(mykBlue);
    hSig->SetLineWidth(2);
    hSig->SetLineStyle(1);
    leg->AddEntry(hSig, "Match to sig", "l");
    hStack->Add(hSig);

    TH1F *hPU_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hPU_bjet", 1, 1);
    // Normalize histo
    hPU_bjet->Scale(1. / (totalTracks_bjet * hPU_bjet->GetBinWidth(1)));
    // Format histo
    hPU_bjet->SetLineColor(mykGreen);
    hPU_bjet->SetLineWidth(2);
    hPU_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hPU_bjet, "No match", "l");
    hStack_bjet->Add(hPU_bjet);

    TH1F *hBkg_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hBkg_bjet", 2, 2);
    // Normalize histo
    hBkg_bjet->Scale(1. / (totalTracks_bjet * hBkg_bjet->GetBinWidth(1)));
    // Format histo
    hBkg_bjet->SetLineColor(mykRed);
    hBkg_bjet->SetLineWidth(2);
    hBkg_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hBkg_bjet, "Match to bkg", "l");
    hStack_bjet->Add(hBkg_bjet);

    TH1F *hSig_bjet = (TH1F *) hIp3dSig_bjet->ProjectionX("hSig_bjet", 3, 3);
    // Normalize histo
    hSig_bjet->Scale(1. / (totalTracks_bjet * hSig_bjet->GetBinWidth(1)));
    // Format histo
    hSig_bjet->SetLineColor(mykBlue);
    hSig_bjet->SetLineWidth(2);
    hSig_bjet->SetLineStyle(1);
    leg_bjet->AddEntry(hSig_bjet, "Match to sig", "l");
    hStack_bjet->Add(hSig_bjet);

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

}