// #include "draw_utils.h"
#include "cms_palette.h"

void drawHeader(void) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.15, 0.965, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.965, "pp 301^{} pb^{-1} (5.02 TeV)");
}

void draw_trigger_eff()
{
    // PF PRESCALES
    double prescale_pf60 = 6.3534438;
    double prescale_pf40 = 33.917210;
    double prescale_pf30 = 153.12145;

    // CALOJET PRESCALES
    // double prescale_calo40 = 29.1544;
    // double prescale_calo30 = 152.29134;

    std::vector<int> colors = {cmsOrange, cmsBlue, cmsViolet};

    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
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
        std::cout << h->GetName() << ": " << h->GetNbinsX() << std::endl;
        h->Rebin(2);
    }

    TH1D *h_lower_withPrescale = (TH1D *) h_lower->Clone("h_lower_withPrescale");
    h_lower_withPrescale->Scale(prescale_pf40);

    TH1D *h40and60and80and100 = (TH1D *) h_high->Clone("h40and60and80and100");
    h40and60and80and100->Add(h_low);
    h40and60and80and100->Add(h_lower_withPrescale); 
    h40and60and80and100->GetXaxis()->SetTitleOffset(1.2);
    h40and60and80and100->GetXaxis()->SetTitle("p_{T}^{leading jet} (GeV/c)");
    h40and60and80and100->GetYaxis()->SetTitleOffset(1.7);
    h40and60and80and100->GetYaxis()->SetTitle("Number of events");


    int i = 0;
    for (auto h : {h_lower_withPrescale, h_low, h_high}) {
        h->SetFillStyle(1001);
        h->SetLineColorAlpha(colors[i],1.0);
        h->SetFillColorAlpha(colors[i],0.5);
        i++;
    }

    THStack *h40and60and80and100_stack = new THStack();
    h40and60and80and100_stack->SetTitle("; p_{T}^{jet}; # jets");
    h40and60and80and100_stack->Add(h_lower_withPrescale);
    h40and60and80and100_stack->Add(h_low);
    h40and60and80and100_stack->Add(h_high);
    h40and60and80and100->SetLineColorAlpha(kBlack, 1.0);

    TH1D *h_lower_w30_withPrescale = (TH1D *) h_lower_w30->Clone("h_lower_w30_withPrescale");
    h_lower_w30_withPrescale->Scale(prescale_pf30);

    TH1D *h30and40and60and80and100 = (TH1D *) h_high->Clone("h30and40and60and80and100");
    h30and40and60and80and100->Add(h_low_w30);
    h30and40and60and80and100->Add(h_lower_w30_withPrescale);
    h30and40and60and80and100->GetXaxis()->SetTitleOffset(1.2);
    h30and40and60and80and100->GetXaxis()->SetTitle("p_{T}^{leading jet} (GeV/c)");
    h30and40and60and80and100->GetYaxis()->SetTitleOffset(1.7);
    h30and40and60and80and100->GetYaxis()->SetTitle("Number of events");
    h30and40and60and80and100->SetLineColorAlpha(kBlack, 1.0);

    i = 0;
    for (auto h : {h_lower_w30_withPrescale, h_low_w30, h_high}) {
        h->SetFillStyle(1001);
        h->SetLineColorAlpha(colors[i],1.0);
        h->SetFillColorAlpha(colors[i],0.5);
        i++;
    }

    THStack *h30and40and60and80and100_stack = new THStack();
    h30and40and60and80and100_stack->SetTitle("; p_{T}^{leading jet}; # jets");
    h30and40and60and80and100_stack->Add(h_lower_w30_withPrescale);
    h30and40and60and80and100_stack->Add(h_low_w30);
    h30and40and60and80and100_stack->Add(h_high);

    // // Calculate trigger efficiency
    // h40and60and80and100->Rebin(2);
    // h30and40and60and80and100->Rebin(2);

    TH1D *h_eff = (TH1D *) h40and60and80and100->Clone("h_eff");
    h_eff->Divide(h30and40and60and80and100);
    h_eff->GetXaxis()->SetTitle("p_{T}^{leading jet} (GeV/c)");
    h_eff->GetYaxis()->SetTitle("Trigger efficiency");
    h_eff->SetLineWidth(3);
    h_eff->SetLineColor(cmsBlue);

    h_eff->GetYaxis()->SetRangeUser(0., 1.1);
    h_eff->SetMarkerSize(700);

    // // Make legends
    TLegend *leg_40and60and80and100 = new TLegend(0.27, 0.73, 0.92, 0.9);
    leg_40and60and80and100->SetFillStyle(0);
    leg_40and60and80and100->SetMargin(0.07);
    leg_40and60and80and100->AddEntry(h40and60and80and100, "Combined", "pe1");
    leg_40and60and80and100->AddEntry(h_high, "80 OR 100", "f");
    leg_40and60and80and100->AddEntry(h_low, "60 AND !80 AND !100", "f");
    leg_40and60and80and100->AddEntry(h_lower_withPrescale, "40 AND !60 AND !80 AND !100", "f");

    TLegend *leg_30and40and60and80and100 = new TLegend(0.27, 0.73, 0.92, 0.9);
    leg_30and40and60and80and100->SetFillStyle(0);
    leg_30and40and60and80and100->SetMargin(0.07);
    leg_30and40and60and80and100->AddEntry(h30and40and60and80and100, "Combined", "pe1");
    leg_30and40and60and80and100->AddEntry(h_high, "80 OR 100", "f");
    leg_30and40and60and80and100->AddEntry(h_low_w30, "(40 OR 60) AND !80 AND !100", "f");
    leg_30and40and60and80and100->AddEntry(h_lower_w30_withPrescale, "30 AND !40 AND !60 AND !80 AND !100", "f");
    
    // Draw 
    TCanvas *c_40and60and80and100 = new TCanvas("c_40and60and80and100", "Combination 40,60,80 and 100", 700, 600);
    c_40and60and80and100->SetLeftMargin(0.15);
    c_40and60and80and100->SetRightMargin(0.05);
    c_40and60and80and100->SetTopMargin(0.07);
    c_40and60and80and100->SetBottomMargin(0.15);
    h40and60and80and100->GetXaxis()->SetRangeUser(30, 300);
    h40and60and80and100->GetYaxis()->SetRangeUser(1e4,1e10);
    h40and60and80and100->Draw("pe1 same");
    h40and60and80and100_stack->Draw("hist same");
    h40and60and80and100->Draw("pe1 same");
    leg_40and60and80and100->Draw();
    c_40and60and80and100->SetLogy();
    c_40and60and80and100->RedrawAxis();
    drawHeader();
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.9, 0.56, "anti-k_{T}, R=0.4 inclusive jets");
    jet_info->DrawLatex(0.9, 0.5, "|#eta^{jet}| < 2");
    jet_info->Draw();
    c_40and60and80and100->Draw();
    h40and60and80and100_stack->GetXaxis()->SetRangeUser(30,300);
    // c_40and60and80and100->Print("plots_an/jtpt_spectrum_pf40to100.png");
    // c_40and60and80and100->Print("plots_thesis/jtpt_spectrum_pf40to100.png");
    c_40and60and80and100->Print("plots_thesis/jtpt_spectrum_pf40to100.pdf");

    TCanvas *c_30and40and60and80and100 = new TCanvas("c_30and40and60and80and100", "Combination 30, 40, 60, 80 and 100", 700, 600);
    c_30and40and60and80and100->SetLeftMargin(0.15);
    c_30and40and60and80and100->SetRightMargin(0.05);
    c_30and40and60and80and100->SetTopMargin(0.07);
    c_30and40and60and80and100->SetBottomMargin(0.15);
    h30and40and60and80and100->GetXaxis()->SetRangeUser(30,300);
    h30and40and60and80and100->GetYaxis()->SetRangeUser(1e4,1e10);
    h30and40and60and80and100->Draw("pe1 same");
    h30and40and60and80and100_stack->Draw("hist same");
    h30and40and60and80and100->Draw("pe1 same");
    leg_30and40and60and80and100->Draw();
    c_30and40and60and80and100->SetLogy();
    c_30and40and60and80and100->RedrawAxis();
    drawHeader();
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->SetTextSize(text_size-4);
    // jet_info->SetTextAlign(32);
    jet_info->DrawLatex(0.9, 0.56, "anti-k_{T}, R=0.4 inclusive jets");
    jet_info->DrawLatex(0.9, 0.5, "|#eta^{jet}| < 2");
    jet_info->Draw();
    c_30and40and60and80and100->Draw();
    h30and40and60and80and100_stack->GetXaxis()->SetRangeUser(30,300);
    // c_30and40and60and80and100->Print("plots_an/jtpt_spectrum_pf30to100.png");
    // c_30and40and60and80and100->Print("plots_thesis/jtpt_spectrum_pf30to100.png");
    c_30and40and60and80and100->Print("plots_thesis/jtpt_spectrum_pf30to100.pdf");

    TCanvas *c_eff = new TCanvas("c_eff", "Trigger efficiency of PFJet60", 700, 600);
    c_eff->SetLeftMargin(0.15);
    c_eff->SetRightMargin(0.05);
    c_eff->SetTopMargin(0.07);
    c_eff->SetBottomMargin(0.15);
    h_eff->GetXaxis()->SetRangeUser(30,300);
    h_eff->Draw("hist text");

    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->DrawLatex(0.5, 0.34, "anti-k_{T}, R=0.4 inclusive jets");
    // jet_info->DrawLatex(0.5, 0.28, "|#eta^{jet}| < 2");
    // jet_info->Draw();

    TLine *line = new TLine(80., 0., 80., 1.1);
    line->Draw();
    // c_eff->SetGridy();
    drawHeader();
    jet_info->DrawLatex(0.9, 0.56, "anti-k_{T}, R=0.4 inclusive jets");
    jet_info->DrawLatex(0.9, 0.5, "|#eta^{jet}| < 2");
    c_eff->Draw();
    // c_eff->Print("plots_an/pfjet40_efficiency.png");
    // c_eff->Print("plots_thesis/pfjet40_efficiency.png");
    c_eff->Print("plots_thesis/pfjet40_efficiency.pdf");
}