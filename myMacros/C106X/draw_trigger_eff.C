#include "draw_utils.h"

void draw_trigger_eff()
{
    const double prescale60 = 6.32142; // From HighEGJet, to be applied to LowEGJet
    const double prescale40 = 29.1544; // From HighEGJet, to be applied to LowEGJet

    // Load 40 and 60 from LowEGJet
    TFile *fin_LowEGJet = new TFile("histos/LowEGJet_aggrTMVA_inclusive_trg_eff.root");
    TH1D *h40 = (TH1D *) fin_LowEGJet->Get("h40");
    TH1D *h60 = (TH1D *) fin_LowEGJet->Get("h60");

    // Load 80 from HighEGJet
    TFile *fin_HighEGJet = new TFile("histos/HighEGJet_aggrTMVA_inclusive_trg_eff.root");
    TH1D *h80 = (TH1D *) fin_HighEGJet->Get("h80");

    // Combine 60 and 80
    h60->SetFillStyle(1001);
    h60->SetFillColor(kAzure-4);

    TH1D *h60withPrescale = (TH1D *) h60->Clone("h60withPrescale");
    h60withPrescale->Scale(prescale60);

    TH1D *h60and80 = (TH1D *) h80->Clone("h60and80");
    h60and80->Add(h60withPrescale);

    h80->SetFillStyle(1001);
    h80->SetFillColor(kRed-3);

    THStack *h60and80_stack = new THStack();
    h60and80_stack->SetTitle("; p_{T}^{jet}; # jets");
    h60and80_stack->Add(h60withPrescale);
    h60and80_stack->Add(h80);

    // Combine 40 and 60 and 80
    TH1D *h40withPrescale = (TH1D *) h40->Clone("h40withPrescale");
    h40withPrescale->Scale(prescale40);
    h40withPrescale->SetFillStyle(1001);
    h40withPrescale->SetFillColor(kGreen-7);

    TH1D *h40and60and80 = (TH1D *) h80->Clone("h40and60and80");
    h40and60and80->Add(h60);
    h40and60and80->Add(h40withPrescale);

    THStack *h40and60and80_stack = new THStack();
    h40and60and80_stack->SetTitle("; p_{T}^{jet}; # jets");
    h40and60and80_stack->Add(h40withPrescale);
    h40and60and80_stack->Add(h60);
    h40and60and80_stack->Add(h80);

    // Calculate trigger efficiency
    TH1D *h60eff = (TH1D *) h60and80->Clone("h60eff");
    h60eff->Divide(h40and60and80);
    h60eff->GetXaxis()->SetTitle("p_{T}^{jet}");
    h60eff->GetYaxis()->SetTitle("CaloJet60 efficiency");
    h60eff->SetLineWidth(2);
    h60eff->SetLineColor(kBlue-3);

    // Make legends
    TLegend *leg_60and80 = new TLegend(0.5, 0.5, 0.7, 0.7);
    leg_60and80->AddEntry(h80, "pass80", "f");
    leg_60and80->AddEntry(h60withPrescale, "pass60not80", "f");

    TLegend *leg_40and60and80 = new TLegend(0.5, 0.5, 0.7, 0.7);
    leg_40and60and80->AddEntry(h80, "pass80", "f");
    leg_40and60and80->AddEntry(h60, "pass60not80", "f");
    leg_40and60and80->AddEntry(h40withPrescale, "pass40not60not80", "f");
    
    // Draw 
    TCanvas *c_60and80 = new TCanvas("c_60and80", "Combination 60 and 80", 800, 600);
    h60and80_stack->Draw("hist");
    leg_60and80->Draw();
    c_60and80->SetLogy();
    drawHeader();
    c_60and80->Draw();
    c_60and80->Print("plots_an/jtpt_spectrum_60and80.png");

    TCanvas *c_40and60and80 = new TCanvas("c_40and60and80", "Combination 40 and 60 and 80", 800, 600);
    h40and60and80_stack->Draw("hist");
    leg_40and60and80->Draw();
    c_40and60and80->SetLogy();
    drawHeader();
    c_40and60and80->Draw();
    c_40and60and80->Print("plots_an/jtpt_spectrum_40and60and80.png");

    TCanvas *c_60eff = new TCanvas("c_60eff", "Trigger efficiency of CaloJet60", 800, 600);
    h60eff->Draw("hist");
    drawHeader();
    c_60eff->Draw();
    c_60eff->Print("plots_an/calojet60_efficiency.png");
}