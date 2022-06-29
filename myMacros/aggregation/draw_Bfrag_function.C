#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TStyle.h"

#include "../pt2dscan/myPalette.h"

void draw_Bfrag_function()
{
    std::string fname = "~/rootFiles/bFF.root";
    TFile *fin = new TFile(fname.c_str());

    TCanvas *c = new TCanvas("c", "c", 1000, 600);
    c->SetGrid(1);

    THStack *hs = new THStack();

    TLegend *leg = new TLegend(0.17, 0.7, 0.4, 0.9);
    leg->SetBorderSize(1);
    leg->SetFillStyle(1000);
    leg->SetFillColor(0);
    leg->SetHeader("Track aggregation criteria:");
    leg->SetMargin(0.1);
    gStyle->SetLegendTextSize(15);

    TLegendEntry *header = (TLegendEntry*) leg->GetListOfPrimitives()->First();
    header->SetTextAlign(22);
        //header->SetTextColor(2);
    header->SetTextSize(17);

    Float_t ptmin = 50.;
    Float_t ptmax = 70.;

    TH2D *hBtag_reco_truth = (TH2D *) fin->Get("hBtag_reco_truth");
    hBtag_reco_truth->GetYaxis()->SetRangeUser(ptmin, ptmax);
    TH1D *h_truthInfo = (TH1D *) hBtag_reco_truth->ProjectionX();
    h_truthInfo->Rebin(2);
    h_truthInfo->GetXaxis()->SetRange(0, h_truthInfo->GetNbinsX() + 1);
    h_truthInfo->Scale(1 / h_truthInfo->Integral("width"));
    h_truthInfo->GetXaxis()->SetRange(1, h_truthInfo->GetNbinsX());
    h_truthInfo->SetLineColor(mykRed);
    h_truthInfo->SetLineWidth(2);
    hs->Add(h_truthInfo);
    leg->AddEntry(h_truthInfo, "Truth level information", "l");

    TH2D *hBtag_reco_highEff = (TH2D *) fin->Get("hBtag_reco_highEff");
    hBtag_reco_highEff->GetYaxis()->SetRangeUser(ptmin, ptmax);
    TH1D *h_highEff = (TH1D *) hBtag_reco_highEff->ProjectionX();
    h_highEff->Rebin(2);
    h_highEff->GetXaxis()->SetRange(0, h_highEff->GetNbinsX() + 1);
    h_highEff->Scale(1 / h_highEff->Integral("width"));
    h_highEff->GetXaxis()->SetRange(1, h_highEff->GetNbinsX());
    h_highEff->SetLineColor(mykBlue);
    h_highEff->SetLineWidth(2);
    hs->Add(h_highEff);
    leg->AddEntry(h_highEff, "inSV OR |3D IP/#sigma| > 3", "l");

    std::string xtitle = "#it{p_{T}^{pseudo-B}} / #it{p_{T}^{charged jet}}";
    std::string ytitle = "1 / N_{#it{b}-tagged #it{b}-jets} dN / d(" + xtitle + ")";
    hs->SetTitle(Form("; %s; %s", xtitle.c_str(), ytitle.c_str()));

    TPaveText *info = new TPaveText(0.2, 0.45, 0.45, 0.65, "ndc");
    //TPaveText *info = new TPaveText(3., 2., 4.5, 4.5);
    info->AddText("Reconstructed #it{b}-tagged #it{b}-jets");
    info->AddText(Form("%.0f < #it{p_{T}^{jet}} < %.0f (GeV)", ptmin, ptmax));
    info->AddText("-2 < #it{#eta^{jet}} < 2");
    info->SetFillColor(0);
    info->SetFillStyle(0);
    info->SetBorderSize(0);
    info->SetTextSize(15);

    TPaveText *mcinfo = new TPaveText(0.17, 0.9, 0.47, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");

    c->cd();
    hs->Draw("nostack hist");
    mcinfo->Draw();
    leg->Draw();
    info->Draw();

    c->Draw();
    std::string savename = "bFF.png";
    c->Print(savename.c_str());

}