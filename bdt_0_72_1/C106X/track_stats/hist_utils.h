#include "THStack.h"
#include "TH2.h"
#include "TLegend.h"

#include <iostream>

void format_histo(TH1F *h, int color, int lineWidth=2, int lineStyle=1) {
    h->SetLineColor(color);
    h->SetLineWidth(lineWidth);
    h->SetLineStyle(lineStyle);
}

void draw_histos_by_class(TH2F *h, THStack *hStack, bool normByTotalTracks=true, TLegend *leg=nullptr) {
    double totalTracks = h->GetEntries();

    TString label = hStack->GetName();
    // std::cout << "DEBUG: "
    //           << "hStack->GetName() = " << label
    //           << std::endl;

    // ---- PU tracks ----
    TH1F *hPU = (TH1F *) h->ProjectionX("hPU_" + label, 1, 1);
    if (normByTotalTracks) {
        hPU->Scale(1. / (totalTracks * hPU->GetBinWidth(1)));
    } else {
        hPU->GetXaxis()->SetRange(0, hPU->GetNbinsX() + 1);
        hPU->Scale(1. / hPU->Integral("width"));
        hPU->GetXaxis()->SetRange(1, hPU->GetNbinsX());
    }
    format_histo(hPU, mykGreen);
    if (leg) leg->AddEntry(hPU, "No match", "l");
    hStack->Add(hPU);

    // ---- Bkg tracks ----
    TH1F *hBkg = (TH1F *) h->ProjectionX("hBkg_" + label, 2, 2);
    if (normByTotalTracks) {
        hBkg->Scale(1. / (totalTracks * hBkg->GetBinWidth(1)));
    } else {
        hBkg->GetXaxis()->SetRange(0, hBkg->GetNbinsX() + 1);
        hBkg->Scale(1. / hBkg->Integral("width"));
        hBkg->GetXaxis()->SetRange(1, hBkg->GetNbinsX());
    }
    hBkg->Scale(0.5);
    format_histo(hBkg, mykRed);
    // if (leg) leg->AddEntry(hBkg, "Match to bkg", "l");
    // hStack->Add(hBkg);

    // ---- Sig tracks ----
    TH1F *hSig = (TH1F *) h->ProjectionX("hSig_" + label, 3, 3);
    if (normByTotalTracks) {
        hSig->Scale(1. / (totalTracks * hSig->GetBinWidth(1)));
    } else {
        hSig->GetXaxis()->SetRange(0, hSig->GetNbinsX() + 1);
        hSig->Scale(1. / hSig->Integral("width"));
        hSig->GetXaxis()->SetRange(1, hSig->GetNbinsX());
    }
    format_histo(hSig, mykBlue);
    if (leg) leg->AddEntry(hSig, "Match to sig", "l");
    hStack->Add(hSig);
}

void draw_canvas(THStack *h, TLegend *leg,
                 float ptMin, float ptMax, 
                 TString label, 
                 TString varName,
                 TString odir="./plots/",
                 TCanvas *c=nullptr) {
    TCanvas *c_local;
    if (c) {
        c_local = c;
    } else {
        c_local = new TCanvas("c_" + label, "", 1200, 800); 
    }

    TString pngLabel = odir + label 
                       + "_" + ptMin + "_pt_" + ptMax 
                       + "_" + varName 
                       + ".png";

    c_local->cd();

    h->Draw("histo nostack");
    leg->Draw();
    
    c_local->Print(pngLabel);
}