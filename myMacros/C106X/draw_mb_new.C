#include "draw_utils.h"

void draw_mb_new()
{
    // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // Load histograms
    TString sample = "dijet";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "histos/"+sample+"_"+label+"_mb_jer_nom_jec_nom.root";
    std::cout << "Reading mc from: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TH1D *hSingleB_mb = (TH1D *) fin->Get("hSingleB_mb");
    TH1D *hBB_mb = (TH1D *) fin->Get("hBB_mb");
    TH1D *hC_mb = (TH1D *) fin->Get("hC_mb");
    TH1D *hL_mb = (TH1D *) fin->Get("hL_mb");

    // Combine C+L while they still have the correct proportions
    TH1D *hCL_mb = (TH1D *) hC_mb->Clone("hCL_mb");
    hCL_mb->Add(hL_mb);

    // Normalize histograms
    for (auto h : {hSingleB_mb, hBB_mb, hCL_mb}) {
        h->Scale(1/h->Integral(), "width");
        h->SetLineWidth(4);
        h->SetMarkerStyle(1);
        h->GetXaxis()->SetTitle("m_{B}^{ch}");
        h->GetYaxis()->SetTitle("1/N dN/dm_{B}^{ch}");
        h->GetYaxis()->SetRangeUser(0, 0.55);
    } 

    // Format histograms
    hSingleB_mb->SetLineStyle(1);
    hSingleB_mb->SetLineColor(kBlack);
    hSingleB_mb->SetTitle("single b jets");

    hBB_mb->SetLineStyle(7);
    hBB_mb->SetLineColor(kBlue-3);
    hBB_mb->SetTitle("bb jets");

    hCL_mb->SetLineStyle(9);
    hCL_mb->SetLineColor(kRed-3);
    hCL_mb->SetTitle("gudsc jets");

    // Build legend
    TLegend *leg_mb = new TLegend(0.5, 0.6, 0.8, 0.8);
    leg_mb->SetBorderSize(0);
    leg_mb->SetFillStyle(0);
    leg_mb->AddEntry(hSingleB_mb, "single b jets", "l");
    leg_mb->AddEntry(hBB_mb, "bb jets", "l");
    leg_mb->AddEntry(hCL_mb, "gudsc jets", "l");

    // Draw histograms 
    TCanvas *c_mb = new TCanvas("c_mb", "", 800, 600);

    hSingleB_mb->Draw("hist same");
    hBB_mb->Draw("hist same");
    hCL_mb->Draw("hist same");
    leg_mb->Draw();
    drawHeaderSimulation();

    c_mb->Draw();
    c_mb->Print("plots_an/mb_templates.pdf");

}