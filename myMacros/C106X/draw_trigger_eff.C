#include "draw_utils.h"

void draw_trigger_eff()
{
    // PF PRESCALES
    double prescale_pf60 = 6.3534438;
    double prescale_pf40 = 33.917210;
    double prescale_pf30 = 153.12145;

    // CALOJET PRESCALES
    // double prescale_calo40 = 29.1544;
    // double prescale_calo30 = 152.29134;

    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPaintTextFormat(".2f");

    // Load LowEGJet
    TFile *fin_LowEGJet_w30 = new TFile("histos/LowEGJet_PF30and40and60_aggrTMVA_inclusive_trg_eff.root");
    TH1D *h_low_w30 = (TH1D *) fin_LowEGJet_w30->Get("h_low")->Clone("h_low_w30"); // PFJet60 and PFJet40
    TH1D *h_lower_w30 = (TH1D *) fin_LowEGJet_w30->Get("h_lower")->Clone("h_lower_w30"); // PFJet30

    TFile *fin_LowEGJet = new TFile("histos/LowEGJet_PF40and60_aggrTMVA_inclusive_trg_eff.root");
    TH1D *h_low = (TH1D *) fin_LowEGJet->Get("h_low"); // PFJet60
    TH1D *h_lower = (TH1D *) fin_LowEGJet->Get("h_lower"); // PFJet40

    // Load HighEGJet
    TFile *fin_HighEGJet = new TFile("histos/HighEGJet_PF80and100_aggrTMVA_inclusive_trg_eff.root");
    TH1D *h_high = (TH1D *) fin_HighEGJet->Get("h_high"); // PFJet80 and PFJet100

    for (auto h : {
        h_low_w30,
        h_lower_w30,
        h_low,
        h_lower,
        h_high
    }) {
        h->GetXaxis()->SetRangeUser(30,300);
        h->Rebin(2);
    }

    // Combine 40+60+80+100
    h_lower->SetFillStyle(1001);
    h_lower->SetFillColor(kAzure-4);

    TH1D *h_lower_withPrescale = (TH1D *) h_lower->Clone("h_lowerwith_Prescale");
    h_lower_withPrescale->Scale(prescale_pf40);

    h_low->SetFillStyle(1001);
    h_low->SetFillColor(kMagenta-9);

    TH1D *h40and60and80and100 = (TH1D *) h_high->Clone("h40and60and80and100");
    h40and60and80and100->Add(h_low);
    h40and60and80and100->Add(h_lower_withPrescale); 

    // h40and60and80and100->GetXaxis()->SetRangeUser(100,120);
    // std::cout << h40and60and80and100->Integral() << std::endl;

    h_high->SetFillStyle(1001);
    h_high->SetFillColor(kGreen-3);

    THStack *h40and60and80and100_stack = new THStack();
    h40and60and80and100_stack->SetTitle("; p_{T}^{jet}; # jets");
    h40and60and80and100_stack->Add(h_lower_withPrescale);
    h40and60and80and100_stack->Add(h_low);
    h40and60and80and100_stack->Add(h_high);

    // Combine 30 and 40 and 60 and 80 and 100
    h_lower_w30->SetFillStyle(1001);
    h_lower_w30->SetFillColor(kAzure-4);

    TH1D *h_lower_w30_withPrescale = (TH1D *) h_lower_w30->Clone("h_lower_w30_withPrescale");
    h_lower_w30_withPrescale->Scale(prescale_pf30);

    TH1D *h30and40and60and80and100 = (TH1D *) h_high->Clone("h30and40and60and80and100");
    h30and40and60and80and100->Add(h_low_w30);
    h30and40and60and80and100->Add(h_lower_w30_withPrescale);

    // h30and40and60and80and100->GetXaxis()->SetRangeUser(100,120);
    // std::cout << h30and40and60and80and100->Integral() << std::endl;


    h_low_w30->SetFillStyle(1001);
    h_low_w30->SetFillColor(kMagenta-9);

    THStack *h30and40and60and80and100_stack = new THStack();
    h30and40and60and80and100_stack->SetTitle("; p_{T}^{jet}; # jets");
    h30and40and60and80and100_stack->Add(h_lower_w30_withPrescale);
    h30and40and60and80and100_stack->Add(h_low_w30);
    h30and40and60and80and100_stack->Add(h_high);

    // // Calculate trigger efficiency
    // h40and60and80and100->Rebin(2);
    // h30and40and60and80and100->Rebin(2);

    TH1D *h_eff = (TH1D *) h40and60and80and100->Clone("h_eff");
    h_eff->Divide(h30and40and60and80and100);
    h_eff->GetXaxis()->SetTitle("p_{T}^{jet}");
    h_eff->GetYaxis()->SetTitle("PFJe460 efficiency");
    h_eff->SetLineWidth(2);
    h_eff->SetLineColor(kBlue-3);

    h_eff->GetYaxis()->SetRangeUser(0., 1.1);
    h_eff->SetMarkerSize(700);

    // // Make legends
    TLegend *leg_40and60and80and100 = new TLegend(0.35, 0.7, 0.85, 0.85);
    leg_40and60and80and100->SetFillStyle(0);
    leg_40and60and80and100->SetMargin(0.1);
    leg_40and60and80and100->AddEntry(h40and60and80and100, "Combined", "pe1");
    leg_40and60and80and100->AddEntry(h_high, "80 OR 100", "f");
    leg_40and60and80and100->AddEntry(h_low, "60 AND !80 AND !100", "f");
    leg_40and60and80and100->AddEntry(h_lower, "40 AND !60 AND !80 AND !100", "f");

    TLegend *leg_30and40and60and80and100 = new TLegend(0.3, 0.7, 0.85, 0.85);
    leg_30and40and60and80and100->SetFillStyle(0);
    leg_30and40and60and80and100->SetMargin(0.1);
    leg_30and40and60and80and100->AddEntry(h30and40and60and80and100, "Combined", "pe1");
    leg_30and40and60and80and100->AddEntry(h_high, "80 OR 100", "f");
    leg_30and40and60and80and100->AddEntry(h_low_w30, "(40 OR 60) AND !80 AND !100", "f");
    leg_30and40and60and80and100->AddEntry(h_lower_w30_withPrescale, "30 AND !40 AND !60 AND !80 AND !100", "f");
    
    // Draw 
    TCanvas *c_40and60and80and100 = new TCanvas("c_40and60and80and100", "Combination 40,60,80 and 100", 800, 600);
    h40and60and80and100->GetXaxis()->SetRangeUser(30, 300);
    h40and60and80and100->Draw("pe1 same");
    h40and60and80and100_stack->Draw("hist same");
    h40and60and80and100->Draw("pe1 same");
    leg_40and60and80and100->Draw();
    c_40and60and80and100->SetLogy();
    drawHeader();
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->DrawLatex(0.5, 0.58, "anti-k_{T}, R=0.4 inclusive jets");
    // jet_info->DrawLatex(0.5, 0.52, "|#eta^{jet}| < 2");
    // jet_info->Draw();
    c_40and60and80and100->Draw();
    h40and60and80and100_stack->GetXaxis()->SetRangeUser(30,300);
    c_40and60and80and100->Print("plots_an/jtpt_spectrum_pf40to100.png");

    TCanvas *c_30and40and60and80and100 = new TCanvas("c_30and40and60and80and100", "Combination 30, 40, 60, 80 and 100", 800, 600);
    h30and40and60and80and100->GetXaxis()->SetRangeUser(30,300);
    h30and40and60and80and100->Draw("pe1 same");
    h30and40and60and80and100_stack->Draw("hist same");
    h30and40and60and80and100->Draw("pe1 same");
    leg_30and40and60and80and100->Draw();
    c_30and40and60and80and100->SetLogy();
    drawHeader();
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->DrawLatex(0.5, 0.58, "anti-k_{T}, R=0.4 inclusive jets");
    // jet_info->DrawLatex(0.5, 0.52, "|#eta^{jet}| < 2");
    // jet_info->Draw();
    c_30and40and60and80and100->Draw();
    h30and40and60and80and100_stack->GetXaxis()->SetRangeUser(30,300);
    c_30and40and60and80and100->Print("plots_an/jtpt_spectrum_pf30to100.png");

    TCanvas *c_eff = new TCanvas("c_eff", "Trigger efficiency of PFJet60", 800, 600);
    h_eff->GetXaxis()->SetRangeUser(30,300);
    h_eff->Draw("hist text");

    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->DrawLatex(0.5, 0.34, "anti-k_{T}, R=0.4 inclusive jets");
    // jet_info->DrawLatex(0.5, 0.28, "|#eta^{jet}| < 2");
    // jet_info->Draw();

    TLine *line = new TLine(80., 0., 80., 1.1);
    line->Draw();
    c_eff->SetGridy();
    drawHeader();
    c_eff->Draw();
    c_eff->Print("plots_an/pfjet40_efficiency.png");
}