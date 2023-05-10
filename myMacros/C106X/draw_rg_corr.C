#include "myPalette.h"

void draw_rg_corr()
{
    double ptMin = 100.;
    double ptMax = 120.;

    // ---- Grab histos ----
    TString indir = "./histos/qcd_bjet_";
    TString label = "aggrTMVA_lateKt";
    TString fname = indir + label + "_rgkt.root";
    TFile *fin = new TFile(fname);

    // gen for correction
    TH3D *hB_rgkt_gen = (TH3D *) fin->Get("hB_rgkt_gen");
    TH3D *hBtag_rgkt_gen = (TH3D *) fin->Get("hBtag_rgkt_gen");

    // reco tagged
    TH3D *hBtag_rgkt = (TH3D *) fin->Get("hBtag_rgkt");
    TH3D *hLtag_rgkt = (TH3D *) fin->Get("hLtag_rgkt");
    TH3D *hCtag_rgkt = (TH3D *) fin->Get("hCtag_rgkt");
    
    // Find pt bins
    Int_t izmin = hB_rgkt_gen->GetZaxis()->FindBin(ptMin);
    Int_t izmax = hB_rgkt_gen->GetZaxis()->FindBin(ptMax) - 1;

    // Find kt bins 
    double ktMin = 0.;
    Int_t iymin = hB_rgkt_gen->GetYaxis()->FindBin(std::log(ktMin));
    Int_t iymax = hB_rgkt_gen->GetNbinsY() + 1;

    // Make rg projections 
    THStack *hStack = new THStack ("hStack", "");
    hStack->SetTitle("; ln(0.4/R_{g}); 1/N dN/d(ln(0.4/R_{g}))");

    TLegend *leg = new TLegend(0.5, 0.7, 0.8, 0.85);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > %.0f GeV", ptMin, ptMax, ktMin));

    TH1F *hB_gen = (TH1F *) hB_rgkt_gen->ProjectionX("hB_gen", iymin, iymax, izmin, izmax);
    hB_gen->Scale(1/hB_gen->Integral("width"));
    hB_gen->SetFillStyle(1);
    hB_gen->SetLineColor(mykBlue);
    hB_gen->SetLineWidth(2);
    hStack->Add(hB_gen);
    leg->AddEntry(hB_gen, "gen b jets", "l");

    TH1F *hBtag_gen = (TH1F *) hBtag_rgkt_gen->ProjectionX("hBtag_gen", iymin, iymax, izmin, izmax);
    hBtag_gen->Scale(1/hBtag_gen->Integral("width"));
    hBtag_gen->SetFillStyle(1);
    hBtag_gen->SetLineColor(mykRed);
    hBtag_gen->SetLineWidth(2);
    hStack->Add(hBtag_gen);
    leg->AddEntry(hBtag_gen, "tagged gen b jets", "l");

    TH1F *hBtag_reco = (TH1F *) hBtag_rgkt->ProjectionX("hBtag_reco", iymin, iymax, izmin, izmax);
    TH1F *hLtag_reco = (TH1F *) hLtag_rgkt->ProjectionX("hLtag_reco", iymin, iymax, izmin, izmax);
    TH1F *hCtag_reco = (TH1F *) hCtag_rgkt->ProjectionX("hCtag_reco", iymin, iymax, izmin, izmax);

    TH1F *hXtag_reco = (TH1F *) hBtag_reco->Clone();
    hXtag_reco->SetName("hXtag_reco");
    hXtag_reco->Add(hLtag_reco);
    hXtag_reco->Add(hCtag_reco);
    hXtag_reco->Scale(1/hXtag_reco->Integral("width"));
    hXtag_reco->SetLineColor(mykRedLight);
    hXtag_reco->SetLineWidth(2);
    hXtag_reco->SetLineStyle(9);
    

    hBtag_reco->Scale(1/hBtag_reco->Integral("width"));
    hBtag_reco->SetLineColor(mykGreenLight);
    hBtag_reco->SetLineWidth(2);
    hBtag_reco->SetLineStyle(9);
    hStack->Add(hBtag_reco);
    leg->AddEntry(hBtag_reco, "tagged reco b jets", "l");

    hStack->Add(hXtag_reco);
    leg->AddEntry(hXtag_reco, "tagged reco jets", "l");

    // Grab & apply corrections
    // true tag = x * true all
    // x = true tag / true all 
    // reco tag = x * reco all
    // reco all = 1/x * reco tag
    THStack *hStack_corrected = new THStack("hStack_corrected", "");
    hStack_corrected->SetTitle("; ln(0.4/R_{g}); 1/N dN/d(ln(0.4/R_{g}))");

    TLegend *leg_corrected = new TLegend(0.5, 0.7, 0.8, 0.85);
    leg_corrected->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > %.0f GeV", ptMin, ptMax, ktMin));
    
    TH1F *hBtag_reco_corrected = (TH1F *) hBtag_reco->Clone();
    hBtag_reco_corrected->SetName("hBtag_reco_corrected");
    hBtag_reco_corrected->SetLineColor(mykRed);
    hBtag_reco_corrected->SetLineWidth(2);
    hBtag_reco_corrected->SetLineStyle(1);

    TH1F *hXtag_reco_corrected = (TH1F *) hXtag_reco->Clone();
    hXtag_reco_corrected->SetName("hXtag_reco_corrected");
    hXtag_reco_corrected->SetLineColor(mykGreen);
    hXtag_reco_corrected->SetLineWidth(2);
    hXtag_reco_corrected->SetLineStyle(1);

    TH1F *hBtag_gen_ratio = (TH1F *) hBtag_gen->Clone();
    hBtag_gen_ratio->SetName("hBtag_ratio"); // tagged gen / gen
    hBtag_gen_ratio->Divide(hB_gen);
    hBtag_gen_ratio->SetLineColor(mykBlue);
    hBtag_gen_ratio->SetLineWidth(2);
    hBtag_gen_ratio->GetXaxis()->SetTitle("ln(0.4/R_{g})");
    hBtag_gen_ratio->GetYaxis()->SetTitle("tagged gen b jets / gen b jets");

    Int_t nbins = hB_gen->GetNbinsX();
    for (Int_t ibin = 1; ibin <= nbins; ibin++) {
        Float_t bin_center = hB_gen->GetBinCenter(ibin);
        Float_t bin_corr = hBtag_gen->GetBinContent(ibin) / hB_gen->GetBinContent(ibin);

        Float_t reco_content = hBtag_reco_corrected->GetBinContent(ibin);
        hBtag_reco_corrected->SetBinContent(ibin, (1 / bin_corr) * reco_content);

        Float_t reco_content_X = hXtag_reco_corrected->GetBinContent(ibin);
        hXtag_reco_corrected->SetBinContent(ibin, (1 / bin_corr) * reco_content_X);

        std::cout << "bin_center=" << bin_center <<", bin_corr=" << bin_corr << std::endl;
        std::cout << "\tgen=" << hB_gen->GetBinContent(ibin) <<", tag gen=" << hBtag_gen->GetBinContent(ibin) << std::endl;
    }

    hStack_corrected->Add(hB_gen);
    leg_corrected->AddEntry(hB_gen, "gen b jets", "l");

    hStack_corrected->Add(hBtag_reco_corrected);
    leg_corrected->AddEntry(hBtag_reco_corrected, "corrected tagged reco b jets", "l");

    hStack_corrected->Add(hXtag_reco_corrected);
    leg_corrected->AddEntry(hXtag_reco_corrected, "corrected tagged reco jets", "l");

    TCanvas *c = new TCanvas("c", "", 1200, 1000);
    // gPad->SetLogy();
    // hStack->SetMinimum(1);
    // hStack->SetMaximum(10000);
    hStack->Draw("no stack histo");
    hStack->SetMaximum(1.2);
    leg->Draw();
    c->Draw();

    TCanvas *c_corrected = new TCanvas("c_corrected", "", 1200, 1000);
    // gPad->SetLogy();
    // hStack->SetMinimum(1);
    // hStack->SetMaximum(10000);
    hStack_corrected->Draw("no stack histo");
    hStack_corrected->SetMaximum(1.2);
    leg_corrected->Draw();
    c_corrected->Draw();

    TCanvas *c_ratio = new TCanvas("c_ratio", "", 1200, 1000);
    // gStyle->SetPaintTextFormat("g");
    gStyle->SetPaintTextFormat("4.1f m");
    hBtag_gen_ratio->Draw("hist text0 same");
    c_ratio->Draw();
}